//
// SVMtlFlash.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlFlash.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderer.h"

SVMtlFlash::SVMtlFlash(SVInst *_app)
:SVMtlADFilterBase(_app,"filterflash") {
    _resetTime();
    m_state = MTL_ADFILTER_BEGIN;
}

SVMtlFlash::SVMtlFlash(SVMtlFlash *_mtl)
:SVMtlADFilterBase(_mtl){
    
}

SVMtlFlash::~SVMtlFlash() {
    m_state = MTL_ADFILTER_END;
}

SVMtlCorePtr SVMtlFlash::clone() {
    return PointerSharedPtr<SVMtlFlash>(new SVMtlFlash(this));
}

void SVMtlFlash::reset() {
    SVMtlADFilterBase::reset();
    _resetTime();
}

//逻辑更新
void SVMtlFlash::update(f32 dt) {
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

void SVMtlFlash::_submitUniform(SVRendererPtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("lerp", m_lerp);
}

void SVMtlFlash::_resetTime(){
    m_time = 0.15f;
    m_accTime = 0.0f;
    m_intervalTime = 0.05f;
}
