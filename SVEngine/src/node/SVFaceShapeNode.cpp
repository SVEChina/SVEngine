//
// SVFacePointNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVFaceShapeNode.h"
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
#include "../rendercore/SVRenderer.h"
#include "../core/SVGeoGen.h"
#include "../core/SVPass.h"
#include "../app/SVGlobalMgr.h"
#include "../mtl/SVMtlFaceShape.h"
#include "../mtl/SVTexMgr.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectST.h"


SVFaceShapeNode::SVFaceShapeNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVFaceShapeNode";
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMtl = MakeSharedPtr<SVMtlFaceShape>(mApp);
    m_pMesh = SVGeoGen::createRectMesh(mApp,1.0, 1.0, 36, 64);
}

SVFaceShapeNode::~SVFaceShapeNode(){
    
}

void SVFaceShapeNode::init(){
    startListen();
    SVRendererPtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return ;
    SVTexturePtr t_tex = mApp->getTexMgr()->createUnctrlTexture(mApp->m_pGlobalParam->m_inner_width, mApp->m_pGlobalParam->m_inner_height,GL_RGBA, GL_RGBA);
    m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,
                                           t_tex,
                                           false,
                                           false);
    SVTexturePtr t_texmain = t_renderer->getSVTex(E_TEX_MAIN);
    mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
    SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl->setTexcoordFlip(1.0f, 1.0f);
    
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->setMtl(m_pMtl);
    m_pMtl->setTexcoordFlip(1.0f, -1.0f);
    t_pass1->setMesh(m_pMesh);
    t_pass1->setInTex(0, t_texmain);
    t_pass1->setOutTex(t_tex);
    addPass(t_pass1);
    
    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    t_pass2->setMtl(t_mtl);
    t_pass2->setInTex(0, t_tex);
    t_pass2->setMesh(mApp->getDataMgr()->m_screenMesh);
    t_pass2->setOutTex(t_texmain);
    addPass(t_pass2);
}

void SVFaceShapeNode::addPass(SVPassPtr _pass) {
    if(_pass){
        m_passPool.append(_pass);
    }
}

void SVFaceShapeNode::clearPass() {
    m_passPool.destroy();
}

void SVFaceShapeNode::destroy(){
    stopListen();
    clearPass();
}

void SVFaceShapeNode::update(f32 _dt){
    SVNode::update(_dt);
    for(s32 i=0;i<m_passPool.size();i++){
        if(m_passPool[i]->m_pMtl){
            m_passPool[i]->m_pMtl->update(_dt);
        }
    }
}

void SVFaceShapeNode::render(){
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
    SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
    if( t_person && t_person->getExist() ){
        t_person->getFaceData();
        //t_person->getFaceData()
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (t_rs && false  == t_rs->isSuspend() ) {
            for(s32 i=0;i<m_passPool.size();i++){
                if(m_passPool[i]->m_pMtl){
                    SVRenderCmdPassPtr t_cmd = MakeSharedPtr<SVRenderCmdPass>();
                    t_cmd->mTag = "SVFaceShapeNode";
                    t_cmd->setFbo(m_fbo);
                    t_cmd->setTexture(m_passPool[i]->m_outTex);
                    t_cmd->setMesh(m_passPool[i]->m_pMesh);
                    t_cmd->setMaterial(m_passPool[i]->m_pMtl);
                    t_rs->pushRenderCmd(RST_FACEMORPH, t_cmd);
                }
            }
        }
    }
}

bool SVFaceShapeNode::procEvent(SVEventPtr _event){
    SVFaceShapeEventPtr pEvent = DYN_TO_SHAREPTR(SVFaceShapeEvent,_event);//dynamic_cast<SVFaceShapeEvent*>(_event);
    if (pEvent != NULL){
        m_pMtl->setEyeLarge(pEvent->m_eyeSmooth/100.0, pEvent->m_eyeSmooth/100.0);
        m_pMtl->setChinSlim(1.0-pEvent->m_faceSmooth/1000.0);
        m_pMtl->setFaceSlim(1.0-pEvent->m_faceSmooth/1000.0);
        m_pMtl->setChinUpSlim(1.0-pEvent->m_faceSmooth/1000.0);
        m_pMtl->setChinMiddleSlim(1.0-pEvent->m_faceSmooth/1000.0);
    }
    return true;
}
