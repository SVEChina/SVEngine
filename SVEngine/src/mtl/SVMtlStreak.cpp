//
// SVMtlStreak.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlStreak.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"

SVMtlStreak::SVMtlStreak(SVInst *_app)
:SVMtlADFilterBase(_app,"filterstreak2") {
    m_lerp = 0.0f;
    _resetTime();
    m_state = MTL_ADFILTER_BEGIN;
}

SVMtlStreak::SVMtlStreak(SVMtlStreak *_mtl)
:SVMtlADFilterBase(_mtl){
    
}

SVMtlStreak::~SVMtlStreak() {
    m_lerp = 0.0f;
    _resetTime();
    m_state = MTL_ADFILTER_BEGIN;
}

SVMtlCorePtr SVMtlStreak::clone() {
    return PointerSharedPtr<SVMtlStreak>(new SVMtlStreak(this));
}

void SVMtlStreak::reset() {
    SVMtlADFilterBase::reset();
 
}

//逻辑更新
void SVMtlStreak::update(f32 dt) {
    SVMtlADFilterBase::update(dt);
    if (m_accTime >= m_intervalTime) {
        if (m_state == MTL_ADFILTER_END) {
            m_state = MTL_ADFILTER_BEGIN;
        }
        _resetTime();
    }
    if (m_accTime >= m_time) {
        m_state = MTL_ADFILTER_END;
    }
    if (m_state == MTL_ADFILTER_BEGIN) {
        m_state = MTL_ADFILTER_RUNNING;
    }
    if (m_state == MTL_ADFILTER_RUNNING) {
        m_lerp = m_accTime/m_time;
        if (m_lerp > 1.0 || m_lerp < 0.0) {
            m_lerp = 0;
        }
    }
    if (m_state == MTL_ADFILTER_END) {
        m_lerp = 0;
    }
    m_accTime += dt;
}

void SVMtlStreak::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("lerp", m_lerp);
}

void SVMtlStreak::_resetTime(){
    m_accTime = 0.0f;
    m_time = 1.0f;
    m_intervalTime = 4.0f;
}
