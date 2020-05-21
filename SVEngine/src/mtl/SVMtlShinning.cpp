//
// SVMtlShinning.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlShinning.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderer.h"

SVMtlShinning::SVMtlShinning(SVInst *_app)
:SVMtlADFilterBase(_app,"filtershinning") {
    m_lerp = 0.0f;
    _resetTime();
    m_state = MTL_ADFILTER_BEGIN;
}

SVMtlShinning::SVMtlShinning(SVMtlShinning *_mtl)
:SVMtlADFilterBase(_mtl){
    
}

SVMtlShinning::~SVMtlShinning() {
    m_state = MTL_ADFILTER_END;
}

SVMtlCorePtr SVMtlShinning::clone() {
    return PointerSharedPtr<SVMtlShinning>(new SVMtlShinning(this));
}

void SVMtlShinning::reset() {
    SVMtlADFilterBase::reset();
    _resetTime();
}

//逻辑更新
void SVMtlShinning::update(f32 dt) {
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

void SVMtlShinning::_submitUniform(SVRendererPtr _render) {
    SVMtlADFilterBase::_submitUniform(_render);
    _render->submitUniformf("lerp", m_lerp);
}

void SVMtlShinning::_resetTime(){
    m_accTime = 0.0f;
    m_time = 0.132f;
    m_intervalTime = 0.297f;
}
