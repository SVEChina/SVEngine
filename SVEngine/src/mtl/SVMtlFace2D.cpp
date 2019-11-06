//
// SVMtlFace2D.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlFace2D.h"
#include "../mtl/SVTexture.h"

SVMtlFace2D::SVMtlFace2D(SVInst *_app)
:SVMtlCore(_app,"mask2d")
,m_pMaskTexture(nullptr) {
    m_LogicParamDepth.enable = false;
    m_LogicParamBlend.enable = true;
    m_LogicParamBlend.srcParam = MTL_BLEND_ONE;
    m_LogicParamBlend.dstParam = MTL_BLEND_ONE_MINUS_SRC_ALPHA;
}

SVMtlFace2D::SVMtlFace2D(SVMtlFace2D *_mtl)
:SVMtlCore(_mtl){
    m_pMaskTexture = _mtl->m_pMaskTexture;
}

SVMtlFace2D::~SVMtlFace2D() {
    m_pMaskTexture = nullptr;
}

SVMtlCorePtr SVMtlFace2D::clone() {
    return PointerSharedPtr<SVMtlFace2D>(new SVMtlFace2D(this));
}

void SVMtlFace2D::setMaskTexture(SVTexturePtr texture) {
    if (m_pMaskTexture != texture) {
        m_pMaskTexture = texture;
    }
}

SVTexturePtr SVMtlFace2D::getMaskTexture(){
    return m_pMaskTexture;
}

void SVMtlFace2D::update(float dt) {
    if (m_pMaskTexture) {
        setTexture(0,m_pMaskTexture);
    }
}
