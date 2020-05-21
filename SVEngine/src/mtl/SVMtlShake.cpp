//
// SVMtlShake.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlShake.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderer.h"

SVMtlShake::SVMtlShake(SVInst *_app)
:SVMtlCore(_app,"filtershake") {
    m_time = 0.0f;
    m_speed = 3.0f;
    m_amount = 0.4f;
}

SVMtlShake::SVMtlShake(SVMtlShake *_mtl)
:SVMtlCore(_mtl){
    m_time = _mtl->m_time;
    m_amount = _mtl->m_amount;
    m_speed = _mtl->m_speed;
}

SVMtlShake::~SVMtlShake() {
    
}

SVMtlCorePtr SVMtlShake::clone() {
    return PointerSharedPtr<SVMtlShake>(new SVMtlShake(this));
}

void SVMtlShake::reset() {
    SVMtlCore::reset();
 
}

//逻辑更新
void SVMtlShake::update(f32 dt) {
    SVMtlCore::update(dt);
   
}

void SVMtlShake::setTime(f32 _time){
    if (m_time != _time) {
        m_time = _time;
    }
}

void SVMtlShake::setAmount(f32 _amount){
    if (m_amount != _amount) {
        m_amount = _amount;
    }
}

void SVMtlShake::setSpeed(f32 _speed){
    if (m_speed != _speed) {
        m_speed = _speed;
    }
}

void SVMtlShake::_submitUniform(SVRendererPtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("time", m_time);
    _render->submitUniformf("amount", m_amount);
    _render->submitUniformf("speed", m_speed);
}
