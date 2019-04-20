//
// SVFrameOutRead.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFrameOutRead.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../base/SVDataSwap.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVStaticData.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRendererBase.h"
#include "../rendercore/SVContextBase.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderCmdOut.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlCore.h"
#include "../core/SVGeoGen.h"


SVFrameOutRead::SVFrameOutRead(SVInst *_app)
:SVFrameOutNode(_app){
    ntype = "SVFrameOutRead";
    m_outFormat = SV_OUT_STEAM_RGBA;
    m_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"rgba");
    m_width = 720;
    m_height = 1280;
    m_pDataSwap = MakeSharedPtr<SVDataSwap>();
}

SVFrameOutRead::~SVFrameOutRead(){
    m_pDataSwap = nullptr;
    m_fbo = nullptr;
    m_pMtl = nullptr;
}

void SVFrameOutRead::create(s32 _width,s32 _height) {
    m_width = _width;
    m_height = _height;
    //创建fbo
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if( t_renderer ) {
        SVTexturePtr t_tex = t_renderer->createSVTex(E_TEX_OUTSTREAM,
                                                     m_width,
                                                     m_height,
                                                     GL_RGBA);
        m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,t_tex,false,false);
        mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
    }
    _resize();
}

void SVFrameOutRead::destroy(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if( t_renderer ) {
        m_fbo = nullptr;
        t_renderer->destroySVTex(E_TEX_OUTSTREAM);
    }
}

void SVFrameOutRead::setFormat(SV_OUTSTEAMFORMATE _fromate) {
    m_outFormat = _fromate;
    if(m_outFormat == SV_OUT_STEAM_RGB2YUVNV12) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"rgb2yuvnv12");
    }else if(m_outFormat == SV_OUT_STEAM_RGBA) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"rgba");
    }else if(m_outFormat == SV_OUT_STEAM_RGB2YUVNV21) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"rgb2yuvnv21");
    }else if(m_outFormat == SV_OUT_STEAM_RGB2YUVYV12) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"rgb2yuvyv12");
    }else if(m_outFormat == SV_OUT_STEAM_RGB2YUVI420) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"rgb2yuvi420");
    }else if(m_outFormat == SV_OUT_STEAM_BGRA) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"bgra");
    }
    _resize();
}

void SVFrameOutRead::_resize() {
    if(m_pDataSwap) {
        s32 t_size = m_width*m_height;
        if(m_outFormat == SV_OUT_STEAM_RGB2YUVNV12) {
            t_size *=1.5;
        }else if(m_outFormat == SV_OUT_STEAM_RGBA) {
            t_size *=4;
        }else if(m_outFormat == SV_OUT_STEAM_RGB2YUVNV21) {
            t_size *=1.5;
        }else if(m_outFormat == SV_OUT_STEAM_RGB2YUVYV12) {
            t_size *=1.5;
        }else if(m_outFormat == SV_OUT_STEAM_RGB2YUVI420) {
            t_size *=1.5;
        }else if(m_outFormat == SV_OUT_STEAM_BGRA) {
            t_size *=4;
        }
        m_pDataSwap->resize(t_size);
    }
}

void SVFrameOutRead::update(f32 _dt){
    SVFrameOutNode::update(_dt);
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(m_pMtl){
        m_pMtl->setModelMatrix(m_absolutMat.get());
        m_pMtl->setTexture(0,E_TEX_MAIN);    //那第一张纹理
        m_pMtl->setBlendEnable(false);
        m_pMtl->setTexcoordFlip(1.0f, -1.0f);
        m_pMtl->update(_dt);
    }
}

void SVFrameOutRead::render(){
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVTexturePtr t_out_tex = t_renderer->getSVTex(E_TEX_OUTSTREAM);
    if (t_out_tex && t_rs && false  == t_rs->isSuspend() ) {
        SVRenderCmdStreamOutNorPtr t_cmd = MakeSharedPtr<SVRenderCmdStreamOutNor>(mApp);
        t_cmd->mTag = "SVFrameOutRead::render";
        t_cmd->setParam(m_fbo,t_out_tex,m_pDataSwap,nullptr);
        t_cmd->setMesh(mApp->getDataMgr()->m_screenMesh);
        t_cmd->setMaterial(m_pMtl->clone());
        t_rs->pushRenderCmd(RST_MAGIC_OUT, t_cmd);
    }
}

void SVFrameOutRead::lockData() {
    if(m_pDataSwap) {
        m_pDataSwap->lockData();
    }
}

void SVFrameOutRead::unlockData() {
    if(m_pDataSwap) {
        m_pDataSwap->unlockData();
    }
}

void* SVFrameOutRead::getData() {
    if(m_pDataSwap) {
        return m_pDataSwap->getData();
    }
    return nullptr;
}
