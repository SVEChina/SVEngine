//
// SVMtlBeauty.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlBeauty.h"
#include "../mtl/SVTexMgr.h"
#include "../rendercore/SVRendererBase.h"

//
SVMtlBeauty::SVMtlBeauty(SVInst *_app)
:SVMtlCore(_app,"beauty") {
    m_lImgW=720;
    m_lImgH=1280;
    m_lSmoothSize=0.5f;
}

SVMtlBeauty::SVMtlBeauty(SVMtlBeauty* _mtl)
:SVMtlCore(_mtl) {
    m_lImgW=_mtl->m_lImgW;
    m_lImgH=_mtl->m_lImgH;
    m_lSmoothSize=_mtl->m_lSmoothSize;
}

SVMtlBeauty::~SVMtlBeauty(){
}

SVMtlCorePtr SVMtlBeauty::clone() {
    return PointerSharedPtr<SVMtlBeauty>(new SVMtlBeauty(this));
}

void SVMtlBeauty::_submitMtl(SVRendererBasePtr _render) {
    SVMtlCore::_submitMtl(_render);
    _render->submitUniformf("softenStrength", m_lSmoothSize);
    _render->submitUniformf2("imageSize", m_lImgW,m_lImgH);
}

void SVMtlBeauty::setSmooth(f32 _smooth) {
    if (_smooth >= 0.0) {
        m_lSmoothSize = clamp(0.0f,_smooth,1.0f);
    }
}

void SVMtlBeauty::setBGSize(s32 _width, s32 _height) {
    m_lImgW = _width;
    m_lImgH = _height;
}

