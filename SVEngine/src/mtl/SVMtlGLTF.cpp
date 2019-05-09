//
// SVMtlGLTF.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlGLTF.h"
#include "../mtl/SVTexture.h"

SVMtlGLTF::SVMtlGLTF(SVInst *_app)
:SVMtlCore(_app,"gltf") {
    m_pBaseColorTex = nullptr;
    m_pMetallicRoughnessTex = nullptr;
    m_pNormalTex = nullptr;
    m_pOcclusionTex = nullptr;
    m_pEmissiveTex = nullptr;
    m_baseColorFactor = FVec4(0.0f,0.0f,0.0f,0.0f);
    m_metallicFactor = 1.0f;
    m_roughtnessFactor = 1.0f;
    m_normalScale = 1.0f;
    m_occlusionStrength = 1.0f;
    m_emissiveFactor = FVec3(0.0f,0.0f,0.0f);
}

SVMtlGLTF::SVMtlGLTF(SVMtlGLTF *_mtl)
:SVMtlCore(_mtl){
    m_pBaseColorTex = _mtl->m_pBaseColorTex;
    m_pMetallicRoughnessTex = _mtl->m_pMetallicRoughnessTex;
    m_pNormalTex = _mtl->m_pNormalTex;
    m_pOcclusionTex = _mtl->m_pOcclusionTex;
    m_pEmissiveTex = _mtl->m_pEmissiveTex;
    m_baseColorFactor = _mtl->m_baseColorFactor;
    m_metallicFactor = _mtl->m_metallicFactor;
    m_roughtnessFactor = _mtl->m_roughtnessFactor;
    m_normalScale = _mtl->m_normalScale;
    m_occlusionStrength = _mtl->m_occlusionStrength;
    m_emissiveFactor =  _mtl->m_emissiveFactor;
}

SVMtlGLTF::~SVMtlGLTF() {
}

SVMtlCorePtr SVMtlGLTF::clone() {
    return PointerSharedPtr<SVMtlGLTF>(new SVMtlGLTF(this));
}

//void SVMtlGLTF::update(float dt) {
//}
