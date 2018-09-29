//
// SVFaceImageUsingMoveNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFaceImageUsingMoveNode.h"
#include "SVScene.h"
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
#include "../core/SVImageUsingMove.h"
#include "../base/SVVec2.h"



SVFaceImageUsingMoveNode::SVFaceImageUsingMoveNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVDeformNode";
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMtl       = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_pMesh      = mApp->getRenderMgr()->createMeshRObj();;
    m_iump       = MakeSharedPtr<SVImageUsingMove>();
    m_dataPoint=nullptr;
    m_wPointCount=30;
    m_hPointCont=80;
    m_flip=false;
}

SVFaceImageUsingMoveNode::~SVFaceImageUsingMoveNode(){
    
}

void SVFaceImageUsingMoveNode::addPass(SVPassPtr _pass) {
    if(_pass){
        m_passPool.append(_pass);
    }
}

void SVFaceImageUsingMoveNode::clearPass() {
    m_passPool.clear();
}

void SVFaceImageUsingMoveNode::init(){
    m_width=mApp->m_pGlobalParam->m_inner_width;
    m_height=mApp->m_pGlobalParam->m_inner_height;
    SVRendererBasePtr t_renderer = mApp->getRenderMgr()->getRenderer();
    if(!t_renderer)
        return ;
     m_tex = mApp->getTexMgr()->createUnctrlTexture(m_width, m_height,GL_RGBA, GL_RGBA);
    m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,
                                           m_tex,
                                           false,
                                           false);
    initPoint();
    mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
    SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl->setTexcoordFlip(1.0f, 1.0f);
    
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->setMtl(m_pMtl);
    m_pMtl->setTexcoordFlip(1.0f, 1.0f);
    t_pass1->setMesh(m_pMesh);
    t_pass1->setInTex(0, t_renderer->getSVTex(E_TEX_MAIN));
    t_pass1->setOutTex(m_tex);
    addPass(t_pass1);
    
    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    t_pass2->setMtl(t_mtl);
    t_pass2->setInTex(0, m_tex);
    t_pass2->setMesh(mApp->getDataMgr()->m_screenMesh);
    t_pass2->setOutTex(t_renderer->getSVTex(E_TEX_MAIN));
    addPass(t_pass2);
}

void SVFaceImageUsingMoveNode::setPoint(V2 *_data){
    m_dataPoint=_data;
}

void SVFaceImageUsingMoveNode::init(SVTexturePtr _tex,s32 _w,s32 _h,SVTexturePtr _outTex){
    m_flip=true;
    m_width=_w;
    m_height=_h;
    SVRendererBasePtr t_renderer = mApp->getRenderMgr()->getRenderer();
    if(!t_renderer)
        return ;
    initPoint();
    m_tex = mApp->getTexMgr()->createUnctrlTexture(m_width, m_height,GL_RGBA, GL_RGBA);
    m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,
                                           m_tex,
                                           false,
                                           false);
    
    mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
    SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl->setTexcoordFlip(1.0f, 1.0f);
    
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->setMtl(m_pMtl);
    m_pMtl->setTexcoordFlip(1.0f, 1.0f);
    t_pass1->setMesh(m_pMesh);
    t_pass1->setInTex(0, _tex);
    t_pass1->setOutTex(m_tex);
    addPass(t_pass1);
    
    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
     t_mtl->setTexcoordFlip(1.0f, 1.0f);
    t_pass2->setMtl(t_mtl);
    t_pass2->setInTex(0, m_tex);
    t_pass2->setMesh(mApp->getDataMgr()->m_screenMesh);
    t_pass2->setOutTex(_outTex);
    addPass(t_pass2);
}

void SVFaceImageUsingMoveNode::updatePointMSL(){
    if(m_dataPoint){
              m_iump->clearContrl();
        pointMove(m_dataPoint);}
}

void SVFaceImageUsingMoveNode::initPoint(){
    
    f32 _w=m_width ;
    f32 _h=m_height;
    
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

void SVFaceImageUsingMoveNode::createScreenRectMesh(V2 *t_data,V2 *t_targetData){
    SVDataSwapPtr t_index = MakeSharedPtr<SVDataSwap>();
    t_index->writeData((void  *)m_dataIndex, m_iIndexCount * sizeof(u16));
    m_pMesh->setIndexData(t_index, m_iIndexCount);
    //渲染数据
    V2_C_T0 pVer[m_wPointCount*m_hPointCont];
    
    for (s32 i = 0; i < m_wPointCount*m_hPointCont ; ++i){
        f32 x= t_targetData[i].x/m_width;
        f32 y= t_targetData[i].y/m_height;
        
        f32 t_x= t_data[i].x/m_width;
        f32 t_y= t_data[i].y/m_height;
        
        pVer[i].x = 1.0-x*2.0;
        pVer[i].y = y*2.0-1.0;
        pVer[i].t0x = t_x;
        pVer[i].t0y = t_y;
        pVer[i].r = 255;
        pVer[i].g = 255;
        pVer[i].b = 255;
        pVer[i].a = 255;
    }

    m_pMesh->setVertexType(E_VF_V2_C_T0);
    SVDataSwapPtr t_datav = MakeSharedPtr<SVDataSwap>();
    t_datav->writeData(pVer, sizeof(V2_C_T0) * m_wPointCount*m_hPointCont);
    m_pMesh->setVertexDataNum( m_wPointCount*m_hPointCont);
    m_pMesh->setVertexData(t_datav);
    m_pMesh->createMesh();
}

void SVFaceImageUsingMoveNode::destroy(){
    exit();
    clearPass();
}

void SVFaceImageUsingMoveNode::update(f32 _dt){
    SVNode::update(_dt);
    for(s32 i=0;i<m_passPool.size();i++){
        if(m_passPool[i]->m_pMtl){
            m_passPool[i]->m_pMtl->update(_dt);
            SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
      
            if(m_dataPoint){
              //  pointMove(m_dataPoint);
            }else if( t_person && t_person->getExist()){
                m_iump->clearContrl();
                V2 *t_data = (V2*)t_person->getFaceDataOriginal();
                pointMove(t_data);
            }
        }
    }
}

void  SVFaceImageUsingMoveNode:: setTagPoint(u32 _postion,V2 _point){
    m_pointMap.append(_postion, _point);
}

V2 SVFaceImageUsingMoveNode::MSL(V2 point){
     FVec2 t_xy=m_iump->MLS(FVec2(point.x,point.y));
    V2 xy;
    xy.x=t_xy.x;
    xy.y=t_xy.y;
    return xy;
}

void  SVFaceImageUsingMoveNode::pointMove(V2 *t_data){
    V2 t_outlinePoints[106];
    for(int i  = 0 ; i < 106 ; i++){
        t_outlinePoints[i].x   = t_data[i].x;
        if(m_flip){
            t_outlinePoints[i].y =  t_data[i].y;
        }else{
            t_outlinePoints[i].y = m_height-t_data[i].y;
        }
    }
    FVec2 eyer=FVec2(t_outlinePoints[77].x,t_outlinePoints[77].y);
    FVec2 eyel=FVec2(t_outlinePoints[74].x,t_outlinePoints[74].y);
   
    f32 leng=getDistanceFrom(eyer,eyel);
    
    f32 _smooth=(leng/240.0);
 
    f32 t_inversedStandardLength = 1.0 / leng;
    FVec2 t_eyel=eyer-eyel;
    f64 angle = atan2(t_eyel.y, t_eyel.x) * 180.0/PI;
    m_iump->setControl(FVec2(m_width/2,m_height/2));
    m_iump->setTargetControl(FVec2(m_width/2,m_height/2));
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
           ||m_pointScreen[i].x==m_width
           ||m_pointScreen[i].y==0.0
           ||m_pointScreen[i].y==m_height){
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



void SVFaceImageUsingMoveNode::render(){
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
    SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
    if( t_person && t_person->getExist()||m_dataPoint ){
        t_person->getFaceData();
        //t_person->getFaceData()
        SVRenderScenePtr t_rs = mApp->m_pGlobalParam->m_curScene->getRenderRS();

        m_fbo->setTexture(m_tex);
        SVRenderCmdFboBindPtr t_fbo_bind = MakeSharedPtr<SVRenderCmdFboBind>(m_fbo);
        t_fbo_bind->mTag = "main_frame_bind";
        t_rs->pushRenderCmd(RST_PREFILTER, t_fbo_bind);
        //
        SVRenderCmdClearPtr t_clear = MakeSharedPtr<SVRenderCmdClear>();
        t_clear->mTag = "main_frame_clear";
        t_rs->pushRenderCmd(RST_PREFILTER, t_clear);
        //
        SVRenderCmdFboUnbindPtr t_fbo_unbind = MakeSharedPtr<SVRenderCmdFboUnbind>(m_fbo);
        t_fbo_unbind->mTag = "main_frame_bind";
        t_rs->pushRenderCmd(RST_PREFILTER, t_fbo_unbind);
        
        if (t_rs && false  == t_rs->isSuspend()) {
            for(s32 i=0;i<m_passPool.size();i++){
                if(m_passPool[i]->m_pMtl){
                    SVRenderCmdPassPtr t_cmd = MakeSharedPtr<SVRenderCmdPass>();
                    t_cmd->mTag = "SVFaceShapeNode";
                    t_cmd->setFbo(m_fbo);
                    t_cmd->setTexture(m_passPool[i]->m_outTex);
                    t_cmd->setMesh(m_passPool[i]->m_pMesh);
                    t_cmd->setMaterial(m_passPool[i]->m_pMtl->clone());
                    t_rs->pushRenderCmd(m_rsType, t_cmd);
                }
            }
        }
    }
}
