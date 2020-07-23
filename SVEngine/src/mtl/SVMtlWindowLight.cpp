//
// SVMtlWindowLight.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlWindowLight.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"

SVMtlWindowLight::SVMtlWindowLight(SVInst *_app)
:SVMtlADFilterBase(_app,"filterWindowLight") {
    m_time = 0.0f;
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    setTexSizeIndex(0, t_w, t_h);
    setTexcoordFlip(1.0, -1.0);
    SVTexturePtr texture = mApp->getTexMgr()->getTexture("svres/textures/windowlight.png",true);
    setTexture(1, texture);
}

SVMtlWindowLight::SVMtlWindowLight(SVMtlWindowLight *_mtl)
:SVMtlADFilterBase(_mtl){
    m_time = _mtl->m_time;
}

SVMtlWindowLight::~SVMtlWindowLight() {
    
}

SVMtlCorePtr SVMtlWindowLight::clone() {
    return PointerSharedPtr<SVMtlWindowLight>(new SVMtlWindowLight(this));
}

void SVMtlWindowLight::reset() {
    SVMtlCore::reset();
 
}

//逻辑更新
void SVMtlWindowLight::update(f32 dt) {
    SVMtlCore::update(dt);
    m_time += dt;
}

void SVMtlWindowLight::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("time", m_time);
}
