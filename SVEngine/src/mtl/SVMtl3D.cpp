//
// SVMtl3D.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtl3D.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"
SVMtl3D::SVMtl3D(SVInst *_app)
:SVMtlCore(_app,"normal3d") {
    m_LogicParamDepth.enable = true;
    m_LogicParamBlend.enable = true;
    m_LogicParamBlend.srcParam = GL_ONE;
    m_LogicParamBlend.dstParam = GL_ONE_MINUS_SRC_ALPHA;
    m_ambientStrength = 1.0f;
    m_ambient_color.set(1.0f, 1.0f, 1.0f, 1.0f);
}

SVMtl3D::SVMtl3D(SVMtl3D *_mtl)
:SVMtlCore(_mtl){
    m_LogicParamDepth.enable = _mtl->m_LogicParamDepth.enable;
    m_LogicParamBlend.enable = _mtl->m_LogicParamBlend.enable;
    m_LogicParamBlend.srcParam = _mtl->m_LogicParamBlend.srcParam;
    m_LogicParamBlend.dstParam = _mtl->m_LogicParamBlend.dstParam;
    m_ambientStrength = _mtl->m_ambientStrength;
    m_ambient_color = _mtl->m_ambient_color;
}

SVMtl3D::~SVMtl3D() {
    
}

SVMtlCorePtr SVMtl3D::clone() {
    return PointerSharedPtr<SVMtl3D>(new SVMtl3D(this));
}

void SVMtl3D::reset() {
    SVMtlCore::reset();
}

void SVMtl3D::_submitMtl(SVRendererBasePtr _render){
    //
    _render->submitUniformf("u_ambientStrength", m_ambientStrength);
    _render->submitUniformf4v("u_ambient_color", m_ambient_color.get());
}
