//
// SVMtlRGB2YUV.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlRGB2YUV.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderer.h"

SVMtlRGB2YUV::SVMtlRGB2YUV(SVInst *_app, SV_OUTSTEAMFORMATE formate)
:SVMtlCore(_app,"rgb2yuvnv21") {
    if (formate == SV_OUT_STEAM_RGB2YUVNV21) {
        reloadShader("rgb2yuvnv21");
    }
    if (formate == SV_OUT_STEAM_RGB2YUVNV12) {
        reloadShader("rgb2yuvnv12");
    }
    if (formate == SV_OUT_STEAM_RGB2YUVYV12) {
        reloadShader("rgb2yuvyv12");
    }
    if (formate == SV_OUT_STEAM_RGB2YUVI420) {
        reloadShader("rgb2yuvi420");
    }
}

SVMtlRGB2YUV::SVMtlRGB2YUV(SVMtlRGB2YUV *_mtl)
:SVMtlCore(_mtl){
    m_tex_w = _mtl->m_tex_w;
    m_tex_h = _mtl->m_tex_h;
}

SVMtlRGB2YUV::~SVMtlRGB2YUV() {
}

SVMtlCorePtr SVMtlRGB2YUV::clone() {
    return PointerSharedPtr<SVMtlRGB2YUV>(new SVMtlRGB2YUV(this));
}

void SVMtlRGB2YUV::setTex(f32 _w, f32 _h) {
    m_tex_w = _w;
    m_tex_h = _h;
}

void SVMtlRGB2YUV::_submitMtl(SVRendererPtr _render) {
    _render->submitUniformf("width", m_tex_w);
    _render->submitUniformf("height", m_tex_h);
}

