//
// SVRenderCmdOut.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderCmdOut.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVTexture.h"
#include "../base/SVDataSwap.h"
#include "SVRenderMgr.h"
#include "SVRenderTexture.h"
#include "SVRenderer.h"
#include <sys/time.h>
#include "SVGL/SVRResGL.h"
#include "SVFboObject.h"

#ifdef SV_IOS
#include "../mtl/SVTextureIOS.h"
#endif

//读数据
SVRenderCmdStreamOutNor::SVRenderCmdStreamOutNor(SVInst *_app)
: mApp(_app) {
    m_pCB = nullptr;
    m_dataswap = nullptr;
    m_fbo = nullptr;
    m_tex = nullptr;
}

SVRenderCmdStreamOutNor::~SVRenderCmdStreamOutNor() {
    m_dataswap = nullptr;
    m_fbo = nullptr;
    m_tex = nullptr;
}

void SVRenderCmdStreamOutNor::setParam(SVRenderTexturePtr _fbo,
                                       SVTexturePtr _tex,
                                       SVDataSwapPtr _dataswap,
                                       cb_out_stream _cb) {
    m_fbo = _fbo;
    m_tex = _tex;
    m_dataswap = _dataswap;
    m_pCB = _cb;
}

void SVRenderCmdStreamOutNor::render() {
    if(m_fbo && m_tex && m_pMtl && m_pMesh && m_dataswap) {
        m_fbo->setTexture(m_tex);
        m_fbo->bind();
        if (m_pMtl->submitMtl()) {
            m_pMesh->render(m_pRenderer);
            m_pMtl->recoverMtl();
        }
        _fectchdata();
        m_fbo->unbind();
    }
}

void SVRenderCmdStreamOutNor::_fectchdata() {
    m_dataswap->lockData();
    s32 m_formate = SV_OUT_STEAM_RGBA;
    s32 m_width = m_fbo->getWidth();
    s32 m_height = m_fbo->getHeight();
    if (m_formate == SV_OUT_STEAM_RGBA) {
        glReadPixels(0,
                     0,
                     m_width,
                     m_height,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     m_dataswap->getData());
    } else if (m_formate == SV_OUT_STEAM_BGRA) {
        glReadPixels(0,
                     0,
                     m_width,
                     m_height,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     m_dataswap->getData());
    } else if (m_formate == SV_OUT_STEAM_RGB2YUVNV12) {
        s32 t_height = (s32(m_height * 0.375)) <= 0 ? 1 : (s32(m_height * 0.375));
        glReadPixels(0,
                     0,
                     m_width,
                     t_height + 1,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     m_dataswap->getData());
    } else if (m_formate == SV_OUT_STEAM_RGB2YUVNV21) {
        s32 t_height = (s32(m_height * 0.375)) <= 0 ? 1 : (s32(m_height * 0.375));
        glReadPixels(0,
                     0,
                     m_width,
                     t_height + 1,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     m_dataswap->getData());
    } else if (m_formate == SV_OUT_STEAM_RGB2YUVYV12) {
        s32 t_height = (s32(m_height * 0.375)) <= 0 ? 1 : (s32(m_height * 0.375));
        glReadPixels(0,
                     0,
                     m_width,
                     t_height + 1,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     m_dataswap->getData());
    } else if (m_formate == SV_OUT_STEAM_RGB2YUVI420) {
        s32 t_height = (s32(m_height * 0.375)) <= 0 ? 1 : (s32(m_height * 0.375));
        glReadPixels(0,
                     0,
                     m_width,
                     t_height + 1,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     m_dataswap->getData());
    }
    timeval timTagCur;
    gettimeofday(&timTagCur, nullptr);
    long t_timtag = long(timTagCur.tv_sec*1000.0f + timTagCur.tv_usec*0.001f);
    if(m_pCB){
        (*m_pCB)(m_width,m_height,m_formate,m_dataswap->getData(),t_timtag);
    }
    m_dataswap->unlockData();
}

//
SVRenderCmdStreamOutIOS::SVRenderCmdStreamOutIOS(SVInst *_app)
:SVRenderCmdStreamOutNor(_app){
}

SVRenderCmdStreamOutIOS::~SVRenderCmdStreamOutIOS() {
}

void SVRenderCmdStreamOutIOS::_fectchdata() {
#ifdef SV_IOS
    m_dataswap->lockData();
    //
    s32 m_formate = SV_OUT_STEAM_RGBA;
    s32 m_width = m_fbo->getWidth();
    s32 m_height = m_fbo->getHeight();
    //
    SVRendererPtr t_renderer = mApp->getRenderer();
    if( t_renderer ) {
        SVTexturePtr t_tex_main = t_renderer->getSVTex(E_TEX_OUTSTREAM);
        SVTextureIOSPtr t_tex_main_ios = std::dynamic_pointer_cast<SVTextureIOS>(t_tex_main);
        if( t_tex_main_ios){
            t_tex_main_ios->fetchData((u8*)(m_dataswap->getData()), m_width, m_height);
        }
    }
    //
    timeval timTagCur;
    gettimeofday(&timTagCur, nullptr);
    long t_timtag = long(timTagCur.tv_sec*1000.0f + timTagCur.tv_usec*0.001f);
    if (m_pCB) {
        (*m_pCB)(m_width,m_height,m_formate,m_dataswap->getData(),t_timtag);
    }
    m_dataswap->unlockData();
#endif
}

