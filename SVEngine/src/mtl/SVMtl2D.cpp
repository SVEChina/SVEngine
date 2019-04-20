//
// SVMtl2D.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtl2D.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"
SVMtl2D::SVMtl2D(SVInst *_app , cptr8 _shader)
:SVMtlCore(_app, _shader) {
    m_alpha = 1.0f;
    m_LogicParamDepth.enable = false;
    m_LogicParamBlend.enable = true;
    m_LogicParamBlend.srcParam = GL_ONE;
    m_LogicParamBlend.dstParam = GL_ONE_MINUS_SRC_ALPHA;
}

SVMtl2D::SVMtl2D(SVMtl2D *_mtl)
:SVMtlCore(_mtl){
    m_alpha = _mtl->m_alpha;
}

SVMtl2D::~SVMtl2D() {
    
}

SVMtlCorePtr SVMtl2D::clone() {
    return PointerSharedPtr<SVMtl2D>(new SVMtl2D(this));
}

void SVMtl2D::reset() {
    SVMtlCore::reset();
}

void SVMtl2D::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("alpha", m_alpha);
}

void SVMtl2D::setAlpha(f32 _alpha){
    if (m_alpha != _alpha) {
        m_alpha = _alpha;
    }
}
