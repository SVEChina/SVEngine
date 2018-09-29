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
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectST.h"

#include "../base/SVVec2.h"


SVDeformImageMove::SVDeformImageMove(SVInst *_app)
:SVGBase(_app){
    m_pMtlBg       = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_pMeshBg    = mApp->getRenderMgr()->createMeshRObj();
    m_iump       = MakeSharedPtr<SVImageUsingMove>();
    m_tex = nullptr;
    m_dataPoint=nullptr;
    m_fbo=nullptr;
    m_wPointCount=30;
    m_hPointCont=80;
    m_inw=10;
    m_inh=10;
    m_flip=false;
    is_swith=true;
    m_pass1 = MakeSharedPtr<SVPass>();
    m_pass1->setMtl(m_pMtlBg);
    m_pMtlBg->setTexcoordFlip(1.0f, 1.0f);
    m_pass1->setMesh(m_pMeshBg);
//    m_pass2 = MakeSharedPtr<SVPass>();
//    SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
//    t_mtl->setTexcoordFlip(1.0f, 1.0f);
//    m_pass2->setMesh(mApp->getDataMgr()->m_screenMesh);
//    m_pass2->setMtl(t_mtl);
    addPass(m_pass1);
    //addPass(m_pass2);
  
}

SVDeformImageMove::~SVDeformImageMove(){
    destroy();
}

void SVDeformImageMove::addPass(SVPassPtr _pass) {
    if(_pass){
        m_passPool.append(_pass);
    }
}

void SVDeformImageMove::clearPass() {
    m_passPool.clear();
}

void SVDeformImageMove::setPoint(V2 *_data){
    m_dataPoint=_data;
}

void SVDeformImageMove::init(SVTexturePtr _tex,SVTexturePtr _texout){
    
    m_tex = _tex;
    m_flip=true;
    SVRendererBasePtr t_renderer = mApp->getRenderMgr()->getRenderer();
    if(!t_renderer)
        return ;
    initPoint();
    m_pTexBg = mApp->getTexMgr()->createUnctrlTexture(m_tex->getwidth(), m_tex->getheight(),GL_RGBA, GL_RGBA);
    m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,
                                           m_pTexBg,
                                           false,
                                           false);
    mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
    m_pass1->setInTex(0, m_tex);
    m_pass1->setOutTex(_texout);
//    m_pass2->setInTex(0, m_pTexBg);
//    m_pass2->setOutTex(_texout);
}

void SVDeformImageMove::updatePointMSL(){
    if(m_dataPoint){
        m_iump->clearContrl();
        pointMove(m_dataPoint);
    }
}

void SVDeformImageMove::initPoint(){
    
    f32 _w=m_tex->getwidth() ;
    f32 _h=m_tex->getheight();
    
    s32 iWidthPoint = m_wPointCount , iHeightPoint = m_hPointCont;
    s32 iDataCount = iWidthPoint * iHeightPoint;
    
    s32 iWidthRect  = iWidthPoint - 1;
    s32 iHeightRect = iHeightPoint - 1;
    f32 fWidthDelta =  _w / iWidthRect;
    f32 fHeightDelta = _h / iHeightRect;
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
    m_iIndexCount = iRectCount * 2 * 3;
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
}

void SVDeformImageMove::createScreenRectMesh(V2 *t_data,V2 *t_targetData){
    SVDataSwapPtr t_index = MakeSharedPtr<SVDataSwap>();
    t_index->writeData((void  *)m_dataIndex, m_iIndexCount * sizeof(u16));
    m_pMeshBg->setIndexData(t_index, m_iIndexCount);
    //渲染数据
    V2_C_T0 pVer[m_wPointCount*m_hPointCont];
    
    for (s32 i = 0; i < m_wPointCount*m_hPointCont ; ++i){
        f32 x= t_targetData[i].x/m_tex->getwidth();
        f32 y= t_targetData[i].y/m_tex->getheight();
        
        f32 t_x= t_data[i].x/m_tex->getwidth();
        f32 t_y= t_data[i].y/m_tex->getheight();
        
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
    t_datav->writeData(pVer, sizeof(V2_C_T0) * m_wPointCount*m_hPointCont);
    m_pMeshBg->setVertexDataNum( m_wPointCount*m_hPointCont);
    m_pMeshBg->setVertexData(t_datav);
    m_pMeshBg->createMesh();
}

void SVDeformImageMove::destroy(){
    clearPass();
    m_pMeshBg=nullptr;
    m_pMtlBg=nullptr;
    m_pTexBg=nullptr;
    m_iump=nullptr;
}

void SVDeformImageMove::update(f32 _dt){
    if(!is_swith){
        return;
    }
    for(s32 i=0;i<m_passPool.size();i++){
        if(m_passPool[i]->m_pMtl){
            m_passPool[i]->m_pMtl->update(_dt);
            SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
            if(m_dataPoint){
//                 m_iump->clearContrl();
//                 pointMove(m_dataPoint);
            }else if( t_person && t_person->getExist()){
                V2 *t_data = (V2*)t_person->getFaceDataOriginal();
                pointMove(t_data);
            }
        }
    }
}

s32 SVDeformImageMove::getWidth(){
    return m_tex->getwidth();
}

s32 SVDeformImageMove::getHeight(){
    return m_tex->getheight();
}

void  SVDeformImageMove::setTagPoint(u32 _postion,V2 _point){
    m_pointMap.append(_postion, _point);
}

V2 SVDeformImageMove::MSL(V2 point){
    FVec2 t_xy=m_iump->MLS(FVec2(point.x,point.y));
    V2 xy;
    xy.x=t_xy.x;
    xy.y=t_xy.y;
    return xy;
}
void SVDeformImageMove::setDeformSwitch(bool _swith){
    is_swith=_swith;
}

void  SVDeformImageMove::pointMove(V2 *t_data){
    V2 t_outlinePoints[106];
    for(int i  = 0 ; i < 106 ; i++){
        t_outlinePoints[i].x   = t_data[i].x;
        if(m_flip){
            t_outlinePoints[i].y =  t_data[i].y;
        }else{
            t_outlinePoints[i].y = m_tex->getheight()-t_data[i].y;
        }
    }
    FVec2 eyer=FVec2(t_outlinePoints[77].x,t_outlinePoints[77].y);
    FVec2 eyel=FVec2(t_outlinePoints[74].x,t_outlinePoints[74].y);
    
    f32 leng=getDistanceFrom(eyer,eyel);
    
    f32 _smooth=(leng/240.0);
    
    f32 t_inversedStandardLength = 1.0 / leng;
    FVec2 t_eyel=eyer-eyel;
    f64 angle = atan2(t_eyel.y, t_eyel.x) * 180.0/PI;
    m_iump->setControl(FVec2(m_tex->getwidth()/2,m_tex->getheight()/2));
    m_iump->setTargetControl(FVec2(m_tex->getwidth()/2,m_tex->getheight()/2));
    FVec2 t_rangleV2(t_outlinePoints[46].x,t_outlinePoints[46].y);
    SVMap<u32,V2>::Iterator it=m_pointMap.begin();
    while (it!=m_pointMap.end()) {
        u32 t_postion=it->key;
        V2 t_point=it->data;
        m_iump->setControl(FVec2(t_outlinePoints[t_postion].x,t_outlinePoints[t_postion].y));
        FVec2 point_v= FVec2(t_outlinePoints[t_postion].x,t_outlinePoints[t_postion].y);
        point_v=rotateBy(-angle,point_v,t_rangleV2);
        point_v= FVec2(point_v.x+t_point.x*_smooth,point_v.y+t_point.y*_smooth);
        point_v=rotateBy(angle,point_v,t_rangleV2);
        m_iump->setTargetControl(point_v);
        it++;
    }
    
    V2 t_targetData[ m_wPointCount*m_hPointCont];
    
    for(int i=0;i< m_wPointCount*m_hPointCont;i++){
        if(m_pointScreen[i].x==0.0
           ||m_pointScreen[i].x==m_tex->getwidth()
           ||m_pointScreen[i].y==0.0
           ||m_pointScreen[i].y==m_tex->getheight()){
            t_targetData[i].x=m_pointScreen[i].x;
            t_targetData[i].y=m_pointScreen[i].y;
        }else{
            FVec2 t_xy=m_iump->MLS(FVec2(m_pointScreen[i].x,m_pointScreen[i].y));
            t_targetData[i].x=t_xy.x;
            t_targetData[i].y=t_xy.y;
        }
    }
    createScreenRectMesh(m_pointScreen, t_targetData);
}

void SVDeformImageMove::render(){
    if(!is_swith){
        return;
    }
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
    SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
    if( t_person && t_person->getExist()||m_dataPoint ){
        t_person->getFaceData();
        //t_person->getFaceData()
        
        SVRenderScenePtr t_rs = mApp->m_pGlobalParam->m_curScene->getRenderRS();
        if (t_rs && false  == t_rs->isSuspend()) {
            for(s32 i=0;i<m_passPool.size();i++){
                if(m_passPool[i]->m_pMtl){
                    SVRenderCmdPassPtr t_cmd = MakeSharedPtr<SVRenderCmdPass>();
                    t_cmd->mTag = "SVBackGroundNode";
                    t_cmd->setFbo(m_fbo);
                    t_cmd->setTexture(m_passPool[i]->m_outTex);
                    t_cmd->setMesh(m_passPool[i]->m_pMesh);
                    t_cmd->setMaterial(m_passPool[i]->m_pMtl->clone());
                    t_rs->pushRenderCmd(RST_PREFILTER, t_cmd);
                }
            }
        }
    }
}
