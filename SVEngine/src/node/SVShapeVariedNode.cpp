//
// SVShapeVariedNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVShapeVariedNode.h"
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

SVShapeVariedNode::SVShapeVariedNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVFaceShapeVariedNode";
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMtl = MakeSharedPtr<SVMtlFaceShapeVaried>(mApp);
    m_pMesh =  mApp->getDataMgr()->m_screenMesh;
}

SVShapeVariedNode::~SVShapeVariedNode(){
    
}

void SVShapeVariedNode::addPass(SVPassPtr _pass) {
    if(_pass){
        m_passPool.append(_pass);
    }
}

void SVShapeVariedNode::clearPass() {
    m_passPool.clear();
}

void SVShapeVariedNode::init(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return ;
    SVTexturePtr t_tex = mApp->getTexMgr()->createUnctrlTexture(mApp->m_pGlobalParam->m_inner_width, mApp->m_pGlobalParam->m_inner_height,GL_RGBA, GL_RGBA);
    SVTexturePtr teximg = mApp->getTexMgr()->getTextureSync("svres/filterimg/mask", true);
    m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,
                                           t_tex,
                                           false,
                                           false);
    mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
    SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl->setTexcoordFlip(1.0f, 1.0f);
    
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->setMtl(m_pMtl);
    m_pMtl->setTexcoordFlip(1.0f, 1.0f);
    t_pass1->setMesh(m_pMesh);
    t_pass1->setInTex(0, t_renderer->getSVTex(E_TEX_MAIN));
    t_pass1->setInTex(1, teximg);
    t_pass1->setOutTex(t_tex);
    addPass(t_pass1);
    
    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    t_pass2->setMtl(t_mtl);
    t_pass2->setInTex(0, t_tex);
    t_pass2->setMesh(mApp->getDataMgr()->m_screenMesh);
    t_pass2->setOutTex(t_renderer->getSVTex(E_TEX_MAIN));
    addPass(t_pass2);
}

void SVShapeVariedNode::destroy(){
    exit();
    clearPass();
}

void SVShapeVariedNode::update(f32 _dt){
    SVNode::update(_dt);
    for(s32 i=0;i<m_passPool.size();i++){
        if(m_passPool[i]->m_pMtl){
            m_passPool[i]->m_pMtl->update(_dt);
        }
    }
}

void SVShapeVariedNode::render(){
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
    SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
    if( t_person && t_person->getExist() ){
        t_person->getFaceData();
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (t_rs && false  == t_rs->isSuspend() ) {
            for(s32 i=0;i<m_passPool.size();i++){
                if(m_passPool[i]->m_pMtl){
                    SVRenderCmdPassPtr t_cmd = MakeSharedPtr<SVRenderCmdPass>();
                    t_cmd->mTag = "SVFaceShapeNode";
                    t_cmd->setFbo(m_fbo);
                    t_cmd->setTexture(m_passPool[i]->m_outTex);
                    t_cmd->setMesh(m_passPool[i]->m_pMesh);
                    t_cmd->setMaterial(m_passPool[i]->m_pMtl->clone());
                    t_rs->pushRenderCmd(RST_FACEMORPH, t_cmd);
                }
            }
        }
    }
}
