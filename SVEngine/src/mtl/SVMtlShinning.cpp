//
// SVMtlShinning.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlShinning.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"

SVMtlShinning::SVMtlShinning(SVInst *_app)
:SVMtlCore(_app,"filtershinning") {
    m_accTime = 0.0f;
    m_time = 0.6f;
}

SVMtlShinning::SVMtlShinning(SVMtlShinning *_mtl)
:SVMtlCore(_mtl){
    
}

SVMtlShinning::~SVMtlShinning() {
    
}

SVMtlCorePtr SVMtlShinning::clone() {
    return PointerSharedPtr<SVMtlShinning>(new SVMtlShinning(this));
}

void SVMtlShinning::reset() {
    SVMtlCore::reset();
    m_time = 0.0f;
    m_accTime = 0.6f;
}

//逻辑更新
void SVMtlShinning::update(f32 dt) {
    SVMtlCore::update(dt);
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

void SVMtlShinning::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("lerp", m_lerp);
}
