//
// SVMtl3D.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtl3D.h"
#include "../basesys/SVCameraMgr.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "../node/SVCameraNode.h"
SVMtl3D::SVMtl3D(SVInst *_app, cptr8 _shader)
:SVMtlCore(_app, _shader) {
    m_LogicParamBlend.srcParam = GL_ONE;
    m_LogicParamBlend.dstParam = GL_ONE_MINUS_SRC_ALPHA;
    m_ambientStrength = 1.0f;
    m_ambient_color.set(1.0f, 1.0f, 1.0f);
    m_lightColor.set(1.0f, 1.0f, 1.0f);
    m_lightPos.set(800, 800, -800);
}

SVMtl3D::SVMtl3D(SVMtl3D *_mtl)
:SVMtlCore(_mtl){
    m_ambientStrength = _mtl->m_ambientStrength;
    m_ambient_color = _mtl->m_ambient_color;
    m_lightColor = _mtl->m_lightColor;
    m_lightPos = _mtl->m_lightPos;
}

SVMtl3D::~SVMtl3D() {
    
}

SVMtlCorePtr SVMtl3D::clone() {
    return PointerSharedPtr<SVMtl3D>(new SVMtl3D(this));
}

void SVMtl3D::reset() {
    SVMtlCore::reset();
}

void SVMtl3D::setLightPositon(FVec3& _lightPos){
    m_lightPos = _lightPos;
}

void SVMtl3D::setLightColor(FVec3& _lightColor){
    m_lightColor = _lightColor;
}

FVec3& SVMtl3D::getLightColor(){
    return m_lightColor;
}

FVec3& SVMtl3D::getLightPos(){
    return m_lightPos;
}

void SVMtl3D::setAmbientLightIntensit(f32 _intersit){
    m_ambientStrength = _intersit;
}

void SVMtl3D::setAmbientLightColorFactor(FVec3& _ambientColor){
    m_ambient_color = _ambientColor;
}

f32 SVMtl3D::getAmbientLightStrength(){
    return m_ambientStrength;
}

FVec3& SVMtl3D::getAmbientLightColor(){
    return m_ambient_color;
}

void SVMtl3D::_submitMtl(SVRendererBasePtr _render){
    _render->submitUniformf("u_ambientStrength", m_ambientStrength);
    _render->submitUniformf3v("u_ambient_color", m_ambient_color.get());
    _render->submitUniformf3v("u_lightPos", m_lightPos.get());
    _render->submitUniformf3v("u_lightColor", m_lightColor.get());
    SVCameraNodePtr t_cameraNode = mApp->getCameraMgr()->getMainCamera();
//    FVec3 cameraPos = t_cameraNode->getPosition();
    FVec3 cameraPos = FVec3(0.0f, 0.0f, 0.0f);
    _render->submitUniformf3v("u_viewPos", cameraPos.get());
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
    lightObj.AddMember("ambientColor", t_ambientColor, _allocator);
    //
    RAPIDJSON_NAMESPACE::Value t_dirColor(RAPIDJSON_NAMESPACE::kArrayType);
    RAPIDJSON_NAMESPACE::Value t_dirPositon(RAPIDJSON_NAMESPACE::kArrayType);
    t_dirColor.PushBack(m_lightColor.x, _allocator);
    t_dirColor.PushBack(m_lightColor.y, _allocator);
    t_dirColor.PushBack(m_lightColor.z, _allocator);
    lightObj.AddMember("color", t_dirColor, _allocator);
    t_dirPositon.PushBack(m_lightPos.x, _allocator);
    t_dirPositon.PushBack(m_lightPos.y, _allocator);
    t_dirPositon.PushBack(m_lightPos.z, _allocator);
    lightObj.AddMember("position", t_dirPositon, _allocator);
}

void SVMtl3D::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
    //lighting
//    if (item.HasMember("light") && item["light"].IsObject()) {
//        RAPIDJSON_NAMESPACE::Value &t_light = item["light"];
//        if (t_light.HasMember("ambient") && t_light["ambient"].IsObject()) {
//            RAPIDJSON_NAMESPACE::Value &t_ambient = t_light["ambient"];
//            if (t_ambient.HasMember("intensit") && t_ambient["intensit"].IsFloat()) {
//                f32 intensit = t_ambient["intensit"].GetFloat();
//                setAmbientLightIntensit(intensit);
//            }
//            if (t_ambient.HasMember("colorFactor") && t_ambient["colorFactor"].IsArray()) {
//                RAPIDJSON_NAMESPACE::Value &t_color = t_ambient["colorFactor"];
//                if (t_color.Size() > 2) {
//                    f32 r = t_color[0].GetFloat();
//                    f32 g = t_color[1].GetFloat();
//                    f32 b = t_color[2].GetFloat();
//                    setAmbientLightColorFactor(FVec3(r, g, b));
//                }
//            }
//        }
//        if (t_light.HasMember("difuse") && t_light["difuse"].IsArray()) {
//            RAPIDJSON_NAMESPACE::Value &t_difuses = t_light["difuse"];
//            for (s32 i=0; i<t_difuses.Size(); i++) {
//                RAPIDJSON_NAMESPACE::Value &t_difuse = t_difuses[i];
//                s32 channel = 0;
//                if (t_difuse.HasMember("channel") && t_difuse["channel"].IsInt()) {
//                    channel = t_difuse["channel"].GetInt();
//                }
//                if (t_difuse.HasMember("position") && t_difuse["position"].IsArray()) {
//                    RAPIDJSON_NAMESPACE::Value &t_positon = t_difuse["position"];
//                    if (t_positon.Size() > 2) {
//                        f32 postionX = t_positon[0].GetFloat();
//                        f32 postionY = t_positon[1].GetFloat();
//                        f32 postionZ = t_positon[2].GetFloat();
//                        setDiffuseLightPositon(channel, FVec3(postionX, postionY, postionZ));
//                    }
//                }
//                if (t_difuse.HasMember("colorFactor") && t_difuse["colorFactor"].IsArray()) {
//                    RAPIDJSON_NAMESPACE::Value &t_color = t_difuse["colorFactor"];
//                    if (t_color.Size() > 2) {
//                        f32 r = t_color[0].GetFloat();
//                        f32 g = t_color[1].GetFloat();
//                        f32 b = t_color[2].GetFloat();
//                        setDiffuseLightColor(channel, FVec3(r, g, b));
//                    }
//                }
//            }
//        }
//    }
}
