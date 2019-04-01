//
// SVMtlDistorted.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlDistorted.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"

SVMtlDistorted::SVMtlDistorted(SVInst *_app)
:SVMtlADFilterBase(_app,"filterdistorted") {
    _resetTime();
}

SVMtlDistorted::SVMtlDistorted(SVMtlDistorted *_mtl)
:SVMtlADFilterBase(_mtl){
    
}

SVMtlDistorted::~SVMtlDistorted() {
    
}

SVMtlCorePtr SVMtlDistorted::clone() {
    return PointerSharedPtr<SVMtlDistorted>(new SVMtlDistorted(this));
}

void SVMtlDistorted::reset() {
    SVMtlADFilterBase::reset();
    _resetTime();
}

//逻辑更新
void SVMtlDistorted::update(f32 dt) {
    SVMtlCore::update(dt);
    if (m_accTime > m_time) {
        m_accTime = 0.0f;
    }
    m_accTime += dt;
}

void SVMtlDistorted::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("time", m_accTime/m_time);
}

void SVMtlDistorted::_resetTime(){
    m_accTime = 0.0f;
    m_time = 3.0f;
}
