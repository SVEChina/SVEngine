//
// SVMtlGLTF.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlGLTF.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderer.h"
#include "../core/SVAnimateSkin.h"

SVMtlGLTF::SVMtlGLTF(SVInst *_app)
:SVMtlCore(_app,"gltf3d") {
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

//保护构造函数
SVMtlGLTF::SVMtlGLTF(SVInst *_app,cptr8 _name)
:SVMtlCore(_app,_name) {
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

SVMtlGLTF::~SVMtlGLTF() {
}

SVMtlCorePtr SVMtlGLTF::clone() {
    return PointerSharedPtr<SVMtlGLTF>(new SVMtlGLTF(this));
}

void SVMtlGLTF::update(f32 dt) {
    SVMtlCore::update(dt);
    setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
    setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
    refresh();
}

void SVMtlGLTF::refresh() {
    if(m_pBaseColorTex) {
        setTexture(0,m_pBaseColorTex);
        setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
        setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
    }
    if(m_pMetallicRoughnessTex) {
        setTexture(1,m_pMetallicRoughnessTex);
    }
    if(m_pNormalTex) {
        setTexture(2,m_pNormalTex);
    }
    if(m_pOcclusionTex) {
        setTexture(3,m_pOcclusionTex);
    }
    if(m_pEmissiveTex) {
        setTexture(4,m_pEmissiveTex);
    }
}

//
SVMtlGLTFSkin::SVMtlGLTFSkin(SVInst *_app)
:SVMtlGLTF(_app,"gltfskin") {
    memset(m_vecBoneMatrix,0.0f,MAX_BONES_DATA_SIZE);
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

SVMtlGLTFSkin::SVMtlGLTFSkin(SVMtlGLTFSkin *_mtl)
:SVMtlGLTF(_mtl){
    memcpy(m_vecBoneMatrix,_mtl->m_vecBoneMatrix,MAX_BONES_DATA_SIZE);
    m_pSke = _mtl->m_pSke;
}

SVMtlGLTFSkin::~SVMtlGLTFSkin() {
    m_pSke = nullptr;
}

SVMtlCorePtr SVMtlGLTFSkin::clone() {
    return PointerSharedPtr<SVMtlGLTFSkin>(new SVMtlGLTFSkin(this));
}

void SVMtlGLTFSkin::update(f32 dt) {
    SVMtlGLTF::update(dt);
    //获取骨架数据
    if(m_pSke) {
        //更新30根骨头
        s32 t_boneNum = m_pSke->getBoneNum();
        if(t_boneNum<MAX_BONES) {
            for(s32 i=0;i<t_boneNum;i++) {
                SVBonePtr t_bone = m_pSke->getBoneByID(i);
                if(t_bone) {
                    s32 t_flag = i*16;
                    if( t_flag < MAX_BONES_DATA) {
                        f32* t_pointer = t_bone->m_resultMat.get();
                        memcpy(&m_vecBoneMatrix[t_flag], t_pointer, sizeof(FMat4));
                    }
                }
            }
        }
    }
}

void SVMtlGLTFSkin::refresh() {
    SVMtlGLTF::refresh();
}

void SVMtlGLTFSkin::_submitUniform(SVRendererPtr _render){
    SVMtlCore::_submitUniform(_render);
    //传递骨骼数据
    _render->submitUniformMatrixArray("uBoneMatrix", m_vecBoneMatrix,MAX_BONES);
}

void SVMtlGLTFSkin::bindSke(SVSkeletonPtr _ske) {
    m_pSke = _ske;
}

void SVMtlGLTFSkin::unbindSke() {
    m_pSke = nullptr;
}
