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
}

SVMtl3D::SVMtl3D(SVMtl3D *_mtl)
:SVMtlCore(_mtl){
}

SVMtl3D::~SVMtl3D() {
    
}

SVMtlCorePtr SVMtl3D::clone() {
    return PointerSharedPtr<SVMtl3D>(new SVMtl3D(this));
}

void SVMtl3D::reset() {
    SVMtlCore::reset();
}


