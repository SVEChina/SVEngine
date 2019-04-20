//
// SVMtlAni2D.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlAni2D.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"
#include "../rendercore/SVGL/SVRResGL.h"

SVMtlAni2D::SVMtlAni2D(SVInst *_app)
:SVMtlCore(_app,"normal2dani_c") {
    m_LogicParamDepth.enable = false;
    m_LogicParamBlend.enable = true;
    m_LogicParamBlend.srcParam = GL_ONE;
    m_LogicParamBlend.dstParam = GL_ONE_MINUS_SRC_ALPHA;
}

SVMtlAni2D::SVMtlAni2D(SVMtlAni2D *_mtl)
:SVMtlCore(_mtl){
    m_LogicParamBlendMode.copy(_mtl->m_LogicParamBlendMode);
}

SVMtlAni2D::~SVMtlAni2D() {
    
}

SVMtlCorePtr SVMtlAni2D::clone() {
    return PointerSharedPtr<SVMtlAni2D>(new SVMtlAni2D(this));
}

void SVMtlAni2D::reset() {
    SVMtlCore::reset();
    m_LogicParamBlendMode.reset();
}

void SVMtlAni2D::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    if((m_LogicMtlFlag0&MTL_F0_BLENDMODE)>0){
        _render->submitUniformi(NAME_BLENDMODE, m_LogicParamBlendMode.m_blendmode);
    }
}

void SVMtlAni2D::setBlendMode(SVMTLBLENDMODE _blendMode){
    m_LogicParamBlendMode.m_blendmode = _blendMode;
    m_LogicMtlFlag0 |= MTL_F0_BLENDMODE;
}
