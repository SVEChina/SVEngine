//
// SVMtl3D.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtl3D.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"
const u32 MAX_DIFFUSE_LIFHT = 8;
SVMtl3D::SVMtl3D(SVInst *_app, cptr8 _shader)
:SVMtlCore(_app, _shader) {
    m_LogicParamBlend.srcParam = GL_ONE;
    m_LogicParamBlend.dstParam = GL_ONE_MINUS_SRC_ALPHA;
    m_ambientStrength = 1.0f;
    m_ambient_color.set(1.0f, 1.0f, 1.0f);
    for (s32 i=0; i<MAX_DIFFUSE_LIFHT*3; i++) {
        m_diffuseLightPosPool.append(0.0f);
    }
    for (s32 i=0; i<MAX_DIFFUSE_LIFHT*3; i++) {
        m_diffuseLightColorPool.append(0.0f);
    }
}

SVMtl3D::SVMtl3D(SVMtl3D *_mtl)
:SVMtlCore(_mtl){
    m_ambientStrength = _mtl->m_ambientStrength;
    m_ambient_color = _mtl->m_ambient_color;
    m_diffuseLightPosPool = _mtl->m_diffuseLightPosPool;
    m_diffuseLightColorPool = _mtl->m_diffuseLightColorPool;
}

SVMtl3D::~SVMtl3D() {
    
}

SVMtlCorePtr SVMtl3D::clone() {
    return PointerSharedPtr<SVMtl3D>(new SVMtl3D(this));
}

void SVMtl3D::reset() {
    SVMtlCore::reset();
}

void SVMtl3D::setDiffuseLight(u32 _channel, FVec3 _lightPos, FVec3 _lightColor){
    if (_channel >= MAX_DIFFUSE_LIFHT) {
        return;
    }
    m_diffuseLightPosPool[3*_channel + 0] = _lightPos.x;
    m_diffuseLightPosPool[3*_channel + 1] = _lightPos.y;
    m_diffuseLightPosPool[3*_channel + 2] = _lightPos.z;
    m_diffuseLightColorPool[3*_channel + 0] = _lightColor.x;
    m_diffuseLightColorPool[3*_channel + 1] = _lightColor.y;
    m_diffuseLightColorPool[3*_channel + 2] = _lightColor.z;
}

FVec3 SVMtl3D::getDiffuseLightColor(u32 _channel){
    FVec3 lightColor;
    lightColor.x = m_diffuseLightColorPool[3*_channel + 0];
    lightColor.y = m_diffuseLightColorPool[3*_channel + 1];
    lightColor.z = m_diffuseLightColorPool[3*_channel + 2];
    return lightColor;
}

FVec3 SVMtl3D::getDiffuseLightPos(u32 _channel){
    FVec3 lightPos;
    lightPos.x = m_diffuseLightPosPool[3*_channel + 0];
    lightPos.y = m_diffuseLightPosPool[3*_channel + 1];
    lightPos.z = m_diffuseLightPosPool[3*_channel + 2];
    return lightPos;
}

void SVMtl3D::setAmbientLight(f32 _strength, FVec3 _ambientColor){
    m_ambientStrength = _strength;
    m_ambient_color = _ambientColor;
}

f32 SVMtl3D::getAmbientLightStrength(){
    return m_ambientStrength;
}

FVec3 SVMtl3D::getAmbientLightColor(){
    return m_ambient_color;
}

void SVMtl3D::_submitMtl(SVRendererBasePtr _render){
    _render->submitUniformf("u_ambientStrength", m_ambientStrength);
    _render->submitUniformf3v("u_ambient_color", m_ambient_color.get());
    _render->submitUniformf3v("u_diffuse_light_pos", m_diffuseLightPosPool.get(), MAX_DIFFUSE_LIFHT);
    _render->submitUniformf3v("u_diffuse_light_color", m_diffuseLightColorPool.get(), MAX_DIFFUSE_LIFHT);
}


