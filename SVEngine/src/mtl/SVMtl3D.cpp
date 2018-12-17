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

void SVMtl3D::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                    RAPIDJSON_NAMESPACE::Value &_objValue){
    _toJsonData(_allocator, _objValue);
    //lighting
    RAPIDJSON_NAMESPACE::Value lightObj(RAPIDJSON_NAMESPACE::kObjectType);
    //ambient
    RAPIDJSON_NAMESPACE::Value ambientObj(RAPIDJSON_NAMESPACE::kObjectType);
    lightObj.AddMember("intensit", m_ambientStrength, _allocator);
    RAPIDJSON_NAMESPACE::Value t_ambientColor(RAPIDJSON_NAMESPACE::kArrayType);
    t_ambientColor.PushBack(m_ambient_color.x, _allocator);
    t_ambientColor.PushBack(m_ambient_color.y, _allocator);
    t_ambientColor.PushBack(m_ambient_color.z, _allocator);
    lightObj.AddMember("ambient", t_ambientColor, _allocator);
    //diffuse
    RAPIDJSON_NAMESPACE::Value t_diffuses(RAPIDJSON_NAMESPACE::kArrayType);
    RAPIDJSON_NAMESPACE::Value diffuseObj(RAPIDJSON_NAMESPACE::kObjectType);
    RAPIDJSON_NAMESPACE::Value t_diffuseColor(RAPIDJSON_NAMESPACE::kArrayType);
    RAPIDJSON_NAMESPACE::Value t_diffusePositon(RAPIDJSON_NAMESPACE::kArrayType);
    diffuseObj.AddMember("channel", 0, _allocator);
    t_diffuseColor.PushBack(getDiffuseLightColor(0).x, _allocator);
    t_diffuseColor.PushBack(getDiffuseLightColor(1).x, _allocator);
    t_diffuseColor.PushBack(getDiffuseLightColor(2).x, _allocator);
    diffuseObj.AddMember("colorFactor", t_diffuseColor, _allocator);
    t_diffusePositon.PushBack(getDiffuseLightPos(0).x, _allocator);
    t_diffusePositon.PushBack(getDiffuseLightPos(1).x, _allocator);
    t_diffusePositon.PushBack(getDiffuseLightPos(2).x, _allocator);
    diffuseObj.AddMember("position", t_diffusePositon, _allocator);
    t_diffuses.PushBack(diffuseObj, _allocator);
}

void SVMtl3D::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
    //lighting
    if (item.HasMember("light") && item["light"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &t_light = item["light"];
        if (t_light.HasMember("ambient") && t_light["ambient"].IsObject()) {
            RAPIDJSON_NAMESPACE::Value &t_ambient = t_light["ambient"];
            f32 intensit = 1.0;
            f32 r = 0.0f;
            f32 g = 0.0f;
            f32 b = 0.0f;
            if (t_ambient.HasMember("intensit") && t_ambient["intensit"].IsFloat()) {
                intensit = t_ambient["intensit"].GetFloat();
            }
            if (t_ambient.HasMember("colorFactor") && t_ambient["colorFactor"].IsArray()) {
                RAPIDJSON_NAMESPACE::Value &t_color = t_ambient["colorFactor"];
                if (t_color.Size() > 2) {
                    r = t_color[0].GetFloat();
                    g = t_color[1].GetFloat();
                    b = t_color[2].GetFloat();
                }
            }
            setAmbientLight(intensit, FVec3(r, g, b));
        }
        if (t_light.HasMember("difuse") && t_light["difuse"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_difuses = t_light["difuse"];
            for (s32 i=0; i<t_difuses.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_difuse = t_difuses[i];
                s32 channel = 0;
                f32 postionX = 10000.0f;
                f32 postionY = 10000.0f;
                f32 postionZ = 10000.0f;
                f32 r = 0.0f;
                f32 g = 0.0f;
                f32 b = 0.0f;
                if (t_difuse.HasMember("channel") && t_difuse["channel"].IsInt()) {
                    channel = t_difuse["channel"].GetInt();
                }
                if (t_difuse.HasMember("position") && t_difuse["position"].IsArray()) {
                    RAPIDJSON_NAMESPACE::Value &t_positon = t_difuse["position"];
                    if (t_positon.Size() > 2) {
                        postionX = t_positon[0].GetFloat();
                        postionY = t_positon[1].GetFloat();
                        postionZ = t_positon[2].GetFloat();
                    }
                }
                if (t_difuse.HasMember("colorFactor") && t_difuse["colorFactor"].IsArray()) {
                    RAPIDJSON_NAMESPACE::Value &t_color = t_difuse["colorFactor"];
                    if (t_color.Size() > 2) {
                        r = t_color[0].GetFloat();
                        g = t_color[1].GetFloat();
                        b = t_color[2].GetFloat();
                    }
                }
                setDiffuseLight(channel, FVec3(postionX, postionY, postionZ), FVec3(r, g, b));
            }
        }
    }
}
