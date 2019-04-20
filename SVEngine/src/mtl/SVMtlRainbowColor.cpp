//
// SVMtlRainbowColor.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlRainbowColor.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"

SVMtlRainbowColor::SVMtlRainbowColor(SVInst *_app)
:SVMtlADFilterBase(_app,"filterrainbowcolor") {
    _resetTime();
}

SVMtlRainbowColor::SVMtlRainbowColor(SVMtlRainbowColor *_mtl)
:SVMtlADFilterBase(_mtl){
    
}

SVMtlRainbowColor::~SVMtlRainbowColor() {
    
}

SVMtlCorePtr SVMtlRainbowColor::clone() {
    return PointerSharedPtr<SVMtlRainbowColor>(new SVMtlRainbowColor(this));
}

void SVMtlRainbowColor::reset() {
    SVMtlADFilterBase::reset();
    _resetTime();
}

//逻辑更新
void SVMtlRainbowColor::update(f32 dt) {
    SVMtlADFilterBase::update(dt);
    m_lerp = m_accTime/m_time;
    if(m_lerp<0){
        m_lerp = 0.0f;
        m_accTime = 0.0f;
    }
    if(m_lerp>1.0f){
        m_lerp = 1.0f;
        m_accTime = 0.0f;
    }
    m_accTime += dt;
}

void SVMtlRainbowColor::_submitUniform(SVRendererBasePtr _render) {
    SVMtlADFilterBase::_submitUniform(_render);
    _render->submitUniformf("lerp", m_lerp);
}

void SVMtlRainbowColor::_resetTime(){
    m_lerp = 0.0f;
    m_accTime = 0.0f;
    m_time = 1.0f;
}
