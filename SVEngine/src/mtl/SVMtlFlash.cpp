//
// SVMtlFlash.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlFlash.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"

SVMtlFlash::SVMtlFlash(SVInst *_app)
:SVMtlCore(_app,"filterflash") {
    m_lerp = 0.0f;
    m_accTime = 0.0f;
    m_time = 1.0f;
}

SVMtlFlash::SVMtlFlash(SVMtlFlash *_mtl)
:SVMtlCore(_mtl){
    
}

SVMtlFlash::~SVMtlFlash() {
    
}

SVMtlCorePtr SVMtlFlash::clone() {
    return PointerSharedPtr<SVMtlFlash>(new SVMtlFlash(this));
}

void SVMtlFlash::reset() {
    SVMtlCore::reset();
    m_lerp = 0.0f;
    m_accTime = 0.0f;
    m_time = 1.0f;
}

//逻辑更新
void SVMtlFlash::update(f32 dt) {
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

void SVMtlFlash::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("lerp", m_lerp);
}
