//
// SVDeformImageMove.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVDeformImageMove.h"
#include "../node/SVScene.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../event/SVOpEvent.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVStaticData.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "../core/SVGeoGen.h"
#include "../core/SVPass.h"
#include "../app/SVGlobalMgr.h"
#include "../mtl/SVMtlFaceShape.h"
#include "../mtl/SVMtlShapeVaried.h"
#include "../mtl/SVTexMgr.h"
#include "../base/SVVec2.h"
#include "../node/SVCameraNode.h"

SVDeformImageMove::SVDeformImageMove(SVInst *_app)
:SVGBase(_app){
    m_pMtlBg  = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_pIUMP    = MakeSharedPtr<SVImageUsingMove>();
    m_pMeshBg = nullptr;
    m_tt_w = 0;
    m_tt_h = 0;
    m_dataPoint = nullptr;
    m_fbo = nullptr;
    m_wPointCount = 30;
    m_hPointCont = 80;
    m_flip = false;
    is_swith = true;
}

SVDeformImageMove::~SVDeformImageMove(){
    m_passPool.clear();
    m_pMeshBg = nullptr;
    m_pMtlBg = nullptr;
    m_pIUMP = nullptr;
    m_dataPoint = nullptr;
}

void SVDeformImageMove::init(SVTexturePtr _intex,SVTexturePtr _texout){
    SVRendererBasePtr t_renderer =  mApp->getRenderer();
    if(t_renderer && _intex && _texout){
        m_tt_w = _intex->getwidth();
        m_tt_h = _intex->getheight();
        m_flip=true;
        _initPoint();
        m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,
                                               _texout,
                                               false,
                                               false);
        mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
        //设置该fbo的矩阵关系
        SVCameraNode t_camera(mApp);
        t_camera.resetCamera(m_tt_w, m_tt_h,120.0f);
        m_fbo->setLink(true);
        m_fbo->setProjMat(t_camera.getProjectMatObj());
        m_fbo->setViewMat(t_camera.getViewMatObj());
        //
        SVPassPtr t_pass = MakeSharedPtr<SVPass>();
        t_pass->setMtl(m_pMtlBg);
        t_pass->setMesh(m_pMeshBg);
        t_pass->setInTex(0,_intex);
        t_pass->setOutTex(_texout);
        m_passPool.append(t_pass);
    }
}

void SVDeformImageMove::setPoint(V2 *_data){
    m_dataPoint = _data;
}

void SVDeformImageMove::updatePointMSL(){
    if(m_dataPoint){
        m_pIUMP->clearContrl();
        pointMove(m_dataPoint);
    }
}

void SVDeformImageMove::_initPoint(){
    if(!m_pMeshBg) {
        m_pMeshBg = mApp->getRenderMgr()->createMeshRObj();
    }
    //索引数据
    u16 m_dataIndex[14400];//30*80*2*3
    //
    s32 iWidthPoint = m_wPointCount , iHeightPoint = m_hPointCont;
    s32 iDataCount = iWidthPoint * iHeightPoint;
    s32 iWidthRect  = iWidthPoint - 1;
    s32 iHeightRect = iHeightPoint - 1;
    f32 fWidthDelta =  m_tt_w / iWidthRect;
    f32 fHeightDelta = m_tt_h / iHeightRect;
    s32 iDataIndex = 0;
    memset(m_pointScreen , 0 , sizeof(V2) * iDataCount);
    for (s32 i = 0 ; i < iHeightPoint ; ++i){
        for (s32 j = 0 ; j < iWidthPoint ; ++j ){
            m_pointScreen[iDataIndex].x = fWidthDelta * j;
            m_pointScreen[iDataIndex].y = fHeightDelta * i;
            iDataIndex += 1;
        }
    }
    s32 iRectCount  = iWidthRect * iHeightRect;
    s32 m_iIndexCount = iRectCount * 2 * 3;
    s32 iIndexIndex = 0;
    for (s32 i = 0 ; i < iHeightRect ; ++i){
        for (s32 j = 0; j < iWidthRect ; ++j){
            m_dataIndex[iIndexIndex]     = i * iWidthPoint + j;
            m_dataIndex[iIndexIndex + 1] = i * iWidthPoint + j + 1;
            m_dataIndex[iIndexIndex + 2] = (i + 1) * iWidthPoint + j;
            m_dataIndex[iIndexIndex + 3] = (i + 1) * iWidthPoint + j;
            m_dataIndex[iIndexIndex + 4] = i * iWidthPoint + j + 1;
            m_dataIndex[iIndexIndex + 5] = (i + 1) * iWidthPoint + j + 1;
            iIndexIndex += 6;
        }
    }
    //第一次更新索引数据
    SVDataSwapPtr t_index = MakeSharedPtr<SVDataSwap>();
    t_index->writeData((void  *)m_dataIndex, m_iIndexCount * sizeof(u16));
    m_pMeshBg->setIndexData(t_index, m_iIndexCount);
    //第一次更新顶点数据
    _refreshScreenRectMesh(m_pointScreen,m_pointScreen);
    //创建mesh
    m_pMeshBg->createMesh();
}

void SVDeformImageMove::_refreshScreenRectMesh(V2 *_data,V2 *_targetData){
    if(m_pMeshBg) {
        //渲染数据
        V2_C_T0 pVer[m_wPointCount*m_hPointCont];
        for (s32 i = 0; i < m_wPointCount*m_hPointCont ; ++i){
            f32 x= _targetData[i].x/m_tt_w;
            f32 y= _targetData[i].y/m_tt_h;
            f32 t_x= _data[i].x/m_tt_w;
            f32 t_y= _data[i].y/m_tt_h;
            pVer[i].x = 1.0-x*2.0;
            pVer[i].y = y*2.0-1.0;
            pVer[i].t0x = t_x;
            pVer[i].t0y = t_y;
            pVer[i].r = 255;
            pVer[i].g = 255;
            pVer[i].b = 255;
            pVer[i].a = 255;
        }
        m_pMeshBg->setVertexType(E_VF_V2_C_T0);
        SVDataSwapPtr t_datav = MakeSharedPtr<SVDataSwap>();
        t_datav->writeData(pVer, sizeof(V2_C_T0) * m_wPointCount * m_hPointCont);
        m_pMeshBg->setVertexDataNum( m_wPointCount * m_hPointCont );
        m_pMeshBg->setVertexData(t_datav);
    }
}

void SVDeformImageMove::update(f32 _dt){
    if(!is_swith){
        return;
    }
    if(m_pMtlBg){
        m_pMtlBg->setTexcoordFlip(1.0f, 1.0f);
    }
    //
    for(s32 i=0;i<m_passPool.size();i++){
        if(m_passPool[i]->m_pMtl){
            m_passPool[i]->m_pMtl->update(_dt);
        }
    }
}

void SVDeformImageMove::render(){
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (is_swith && t_rs && false  == t_rs->isSuspend() ) {
        for(s32 i=0;i<m_passPool.size();i++){
            if(m_passPool[i]->m_pMtl){
                SVRenderCmdPassPtr t_cmd = MakeSharedPtr<SVRenderCmdPass>();
                t_cmd->mTag = "SVBackGroundNode";
                t_cmd->setFbo(m_fbo);
                if( m_passPool[i]->m_outTexType == E_TEX_END ) {
                    t_cmd->setTexture(m_passPool[i]->m_outTex);
                }else{
                    SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(m_passPool[i]->m_outTexType);
                    t_cmd->setTexture(t_tex);
                }
                if(m_passPool[i]->m_pMesh){
                    t_cmd->setMesh(m_passPool[i]->m_pMesh);
                }else{
                    t_cmd->setMesh(mApp->getDataMgr()->m_screenMesh);
                }
                t_cmd->setMaterial(m_passPool[i]->m_pMtl);
                t_rs->pushRenderCmd(RST_PREFILTER, t_cmd);//m_rsType
            }
        }
    }
}

s32 SVDeformImageMove::getWidth(){
    return m_tt_w;
}

s32 SVDeformImageMove::getHeight(){
    return m_tt_h;
}

void  SVDeformImageMove::setTagPoint(u32 _postion,V2 _point){
    m_pointMap.append(_postion, _point);
}

V2 SVDeformImageMove::MSL(V2 point){
    if(m_pIUMP) {
        FVec2 t_xy=m_pIUMP->MLS(FVec2(point.x,point.y));
        V2 xy;
        xy.x=t_xy.x;
        xy.y=t_xy.y;
        return xy;
    }
    V2 xy;
    xy.x=0;
    xy.y=0;
    return xy;
}
void SVDeformImageMove::setDeformSwitch(bool _swith){
    is_swith=_swith;
}

void SVDeformImageMove::pointMove(V2 *t_data){
    V2 t_outlinePoints[106];
    for(s32 i  = 0 ; i < 106 ; i++){
        t_outlinePoints[i].x   = t_data[i].x;
        if(m_flip){
            t_outlinePoints[i].y =  t_data[i].y;
        }else{
            t_outlinePoints[i].y = m_tt_h-t_data[i].y;
        }
    }
    FVec2 eyer=FVec2(t_outlinePoints[77].x,t_outlinePoints[77].y);
    FVec2 eyel=FVec2(t_outlinePoints[74].x,t_outlinePoints[74].y);
    
    f32 leng=getDistanceFrom(eyer,eyel);
    f32 _smooth=(leng/240.0);
    f32 t_inversedStandardLength = 1.0 / leng;
    FVec2 t_eyel=eyer-eyel;
    f64 angle = atan2(t_eyel.y, t_eyel.x) * 180.0/PI;
    m_pIUMP->setControl(FVec2(m_tt_w*0.5f,m_tt_h*0.5f));
    m_pIUMP->setTargetControl(FVec2(m_tt_w*0.5f,m_tt_h*0.5f));
    FVec2 t_rangleV2(t_outlinePoints[46].x,t_outlinePoints[46].y);
    SVMap<u32,V2>::Iterator it=m_pointMap.begin();
    while (it!=m_pointMap.end()) {
        u32 t_postion=it->key;
        V2 t_point=it->data;
        m_pIUMP->setControl(FVec2(t_outlinePoints[t_postion].x,t_outlinePoints[t_postion].y));
        FVec2 point_v= FVec2(t_outlinePoints[t_postion].x,t_outlinePoints[t_postion].y);
        point_v=rotateBy(-angle,point_v,t_rangleV2);
        point_v= FVec2(point_v.x+t_point.x*_smooth,point_v.y+t_point.y*_smooth);
        point_v=rotateBy(angle,point_v,t_rangleV2);
        m_pIUMP->setTargetControl(point_v);
        it++;
    }
    //
    V2 t_targetData[m_wPointCount*m_hPointCont];
    for(s32 i=0;i< m_wPointCount*m_hPointCont;i++){
        if(m_pointScreen[i].x == 0.0
           ||m_pointScreen[i].x == m_tt_w
           ||m_pointScreen[i].y == 0.0
           ||m_pointScreen[i].y == m_tt_h){
            t_targetData[i].x = m_pointScreen[i].x;
            t_targetData[i].y = m_pointScreen[i].y;
        }else{
            FVec2 t_xy=m_pIUMP->MLS(FVec2(m_pointScreen[i].x,m_pointScreen[i].y));
            t_targetData[i].x=t_xy.x;
            t_targetData[i].y=t_xy.y;
        }
    }
    //
    _refreshScreenRectMesh(m_pointScreen, t_targetData);
}


//            SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
//            if(m_dataPoint){
//            }else if( t_person && t_person->getExist()){
//                V2 *t_data = (V2*)t_person->getFaceDataOriginal();
//                pointMove(t_data);
//            }
