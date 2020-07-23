//
// SVMtlShake.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlArenaLight.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"

SVMtlArenaLight::SVMtlArenaLight(SVInst *_app)
:SVMtlADFilterBase(_app,"filterArenaLight") {
    m_time = 0.0f;
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    setTexSizeIndex(0, t_w, t_h);
//    setTexcoordFlip(1.0, 1.0);
}

SVMtlArenaLight::SVMtlArenaLight(SVMtlArenaLight *_mtl)
:SVMtlADFilterBase(_mtl){
    m_time = _mtl->m_time;
}

SVMtlArenaLight::~SVMtlArenaLight() {
    
}

SVMtlCorePtr SVMtlArenaLight::clone() {
    return PointerSharedPtr<SVMtlArenaLight>(new SVMtlArenaLight(this));
}

void SVMtlArenaLight::reset() {
    SVMtlCore::reset();
 
}

//逻辑更新
void SVMtlArenaLight::update(f32 dt) {
    SVMtlCore::update(dt);
    m_time += dt;
}

void SVMtlArenaLight::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("time", m_time);
//    _render->submitUniformf("amount", m_amount);
//    _render->submitUniformf("speed", m_speed);
}
