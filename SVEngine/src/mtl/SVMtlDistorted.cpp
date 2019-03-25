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
:SVMtlCore(_app,"filterdistorted") {
    m_accTime = 0.0f;
    m_time = 3.0f;
}

SVMtlDistorted::SVMtlDistorted(SVMtlDistorted *_mtl)
:SVMtlCore(_mtl){
    
}

SVMtlDistorted::~SVMtlDistorted() {
    
}

SVMtlCorePtr SVMtlDistorted::clone() {
    return PointerSharedPtr<SVMtlDistorted>(new SVMtlDistorted(this));
}

void SVMtlDistorted::reset() {
    SVMtlCore::reset();
    m_accTime = 0.0f;
    m_time = 3.0f;
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
