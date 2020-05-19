//
// SVFrameOutTex.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFrameOutTex.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVStaticData.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRendererBase.h"
#include "../rendercore/SVRenderScene.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlCore.h"
#include "../core/SVGeoGen.h"


SVFrameOutTex::SVFrameOutTex(SVInst *_app)
:SVFrameOutNode(_app){
    ntype = "SVFrameOutTex";
    m_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"rgba");
    m_pMesh = mApp->getDataMgr()->m_screenMesh;
    //
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if( t_renderer ) {
#if defined (SV_IOS)
        SVTexturePtr t_tex = t_renderer->createSVTexIOS(E_TEX_OUTSTREAM,
                                                     mApp->m_pGlobalParam->m_inner_width,
                                                     mApp->m_pGlobalParam->m_inner_height,
                                                     GL_RGBA); //GL_RGBA
        m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,t_tex,false,false);
        mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
#else
        SVTexturePtr t_tex = t_renderer->createSVTex(E_TEX_OUTSTREAM,
                                                     mApp->m_pGlobalParam->m_inner_width,
                                                     mApp->m_pGlobalParam->m_inner_height,
                                                     GL_RGBA); //GL_RGBA
        m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,t_tex,false,false);
        mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
#endif
    }
}

SVFrameOutTex::~SVFrameOutTex(){
    m_fbo = nullptr;
    m_pMtl = nullptr;
    m_pMesh= nullptr;
}

void SVFrameOutTex::update(f32 _dt){
    SVNode::update(_dt);
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(m_pMtl){
        m_pMtl->setModelMatrix(m_absolutMat.get());
        m_pMtl->setTexture(0,t_renderer->getSVTex(E_TEX_MAIN));    //那第一张纹理
        m_pMtl->setBlendEnable(false);
        m_pMtl->setTexcoordFlip(1.0f, 1.0f);
        m_pMtl->update(_dt);
    }
}

s32 SVFrameOutTex::getTexId(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVTexturePtr t_out_tex = t_renderer->getSVTex(E_TEX_OUTSTREAM);
    return t_out_tex->getTexID();
}

void SVFrameOutTex::render(){
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVTexturePtr t_out_tex = t_renderer->getSVTex(E_TEX_OUTSTREAM);
    if (t_rs && false  == t_rs->isSuspend() ) {
                SVRenderCmdPassPtr t_cmd = MakeSharedPtr<SVRenderCmdPass>();
                t_cmd->mTag = "SVFrameOutTex";
                t_cmd->setFbo(m_fbo);
                t_cmd->setTexture(t_out_tex);
                t_cmd->setMesh(mApp->getDataMgr()->m_screenMesh);
                t_cmd->setMaterial(m_pMtl);
                t_rs->pushRenderCmd(RST_DATATRANS, t_cmd);
    }
}
