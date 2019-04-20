//
// SVFrameOutIOS.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFrameOutIOS.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../basesys/SVStaticData.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTextureIOS.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderCmdOut.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRendererBase.h"
#include "../rendercore/SVRenderTexture.h"
#include "../mtl/SVMtlCore.h"
#include "../basesys/SVConfig.h"
#ifdef SV_IOS

SVFrameOutIOS::SVFrameOutIOS(SVInst *_app)
:SVFrameOutNode(_app){
    ntype = "SVFrameOutIOS";
    m_dataswap = nullptr;
    m_outStreamFbo = nullptr;
    m_pMtl = nullptr;
    m_rsType = RST_DATATRANS;
}

SVFrameOutIOS::~SVFrameOutIOS() {
    m_dataswap = nullptr;
    m_outStreamFbo = nullptr;
    m_pMtl = nullptr;
}

void SVFrameOutIOS::init(SV_OUTSTEAMFORMATE _outformate,s32 _w,s32 _h) {
    //创建输出纹理
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if( t_renderer ) {
        SVTexturePtr t_tex = t_renderer->createSVTex(E_TEX_OUTSTREAM,_w,_h,GL_RGBA); //GL_RGBA
        m_outStreamFbo = MakeSharedPtr<SVRenderTexture>(mApp,t_tex,false,false);
        mApp->getRenderMgr()->pushRCmdCreate(m_outStreamFbo);
    }
    //
    m_dataswap = MakeSharedPtr<SVDataSwap>();
    if (_outformate == SV_OUT_STEAM_RGBA) {
        m_dataswap->resize(4*_w*_h);
    } else if (_outformate == SV_OUT_STEAM_BGRA) {
        m_dataswap->resize(4*_w*_h);
    } else if (_outformate == SV_OUT_STEAM_RGB2YUVNV12) {
        m_dataswap->resize(1.5*_w*_h);
    } else if (_outformate == SV_OUT_STEAM_RGB2YUVNV21) {
        m_dataswap->resize(1.5*_w*_h);
    } else if (_outformate == SV_OUT_STEAM_RGB2YUVYV12) {
        m_dataswap->resize(1.5*_w*_h);
    } else if (_outformate == SV_OUT_STEAM_RGB2YUVI420) {
        m_dataswap->resize(1.5*_w*_h);
    }
    //
    m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "rgba");
}

void SVFrameOutIOS::destroy(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if( t_renderer ) {
        t_renderer->destroySVTex(E_TEX_OUTSTREAM);
    }
    m_outStreamFbo = nullptr;
    m_pMtl = nullptr;
}

void SVFrameOutIOS::setCallback(cb_out_stream _cb) {
    m_pOutStreamCB = _cb;
}

void SVFrameOutIOS::update(f32 _dt) {
    SVNode::update(_dt);
     SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(m_pMtl && t_renderer){
        m_pMtl->update(_dt);
        m_pMtl->setTexture(0,t_renderer->getSVTex(E_TEX_MAIN));    //那第一张纹理
        m_pMtl->setBlendEnable(false);
        bool t_mirror = mApp->getConfig()->mirror;
        if (t_mirror) {
            m_pMtl->setTexcoordFlip(1.0f, -1.0f);
        }else{
            m_pMtl->setTexcoordFlip(-1.0f, -1.0f);
        }
    }
}

void SVFrameOutIOS::render() {
    //输出到目标纹理 然后读取数据
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if( t_renderer && t_rs) {
        SVTexturePtr t_out_tex = t_renderer->getSVTex(E_TEX_OUTSTREAM);
        SVRenderCmdStreamOutIOSPtr t_cmd = MakeSharedPtr<SVRenderCmdStreamOutIOS>(mApp);
        t_cmd->mTag = "SVFrameOutIOS";
        t_cmd->setParam(m_outStreamFbo,t_out_tex,m_dataswap,m_pOutStreamCB);
        t_cmd->setMesh(mApp->getDataMgr()->m_screenMesh);
        t_cmd->setMaterial(m_pMtl);
        t_rs->pushRenderCmd(m_rsType, t_cmd);
    }
}

void SVFrameOutIOS::lockData() {
    
}

void SVFrameOutIOS::unlockData() {
    
}

void* SVFrameOutIOS::getData() {
    return nullptr;
}

#endif //ios 










