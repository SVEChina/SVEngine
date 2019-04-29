//
// SVMultPassNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMultPassNode.h"
#include "SVScene.h"
#include "../mtl/SVMtlCore.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVStaticData.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRendererBase.h"
#include "../mtl/SVTexMgr.h"
#include "../core/SVPass.h"

//
SVMultPassNode::SVMultPassNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVMultPassNode";
    m_rsType = RST_IMGFILTER;
    m_name = "SVMultPassNode";
}

SVMultPassNode::~SVMultPassNode() {
    m_fbo = nullptr;
}

void SVMultPassNode::create(s32 _w,s32 _h) {
    m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,nullptr,false,false);
    mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
}

void SVMultPassNode::destroy() {
    m_passPool.destroy();
    m_fbo = nullptr;
}

void SVMultPassNode::update(f32 dt) {
    SVNode::update(dt);
    for(s32 i=0;i<m_passPool.size();i++){
        if(m_passPool[i]->m_pMtl){
            m_passPool[i]->m_pMtl->update(0.0f);
        }
    }
}

void SVMultPassNode::render() {
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (t_rs && false  == t_rs->isSuspend() && m_visible ) {
        for(s32 i=0;i<m_passPool.size();i++){
            if(m_passPool[i]->m_pMtl){
                SVRenderCmdPassPtr t_cmd = MakeSharedPtr<SVRenderCmdPass>();
                t_cmd->mTag = m_passPool[i]->mTag;
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
                t_rs->pushRenderCmd(m_rsType, t_cmd);
            }
        }
    }
}

void SVMultPassNode::addPass(SVPassPtr _pass) {
    if(_pass){
        m_passPool.append(_pass);
    }
}

SVPassPtr SVMultPassNode::getPass(s32 _index) {
    if(_index>=0 && _index<m_passPool.size())
        return m_passPool[_index];
    return nullptr;
}

void SVMultPassNode::clearPass() {
    m_passPool.destroy();
}
