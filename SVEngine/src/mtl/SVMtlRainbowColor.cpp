//
// SVMtlRainbowColor.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlRainbowColor.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"

SVMtlRainbowColor::SVMtlRainbowColor(SVInst *_app)
:SVMtlCore(_app,"filterrainbowcolor") {
    m_lerp = 0.0f;
    m_accTime = 0.0f;
    m_time = 1.0f;
}

SVMtlRainbowColor::SVMtlRainbowColor(SVMtlRainbowColor *_mtl)
:SVMtlCore(_mtl){
    
}

SVMtlRainbowColor::~SVMtlRainbowColor() {
    
}

SVMtlCorePtr SVMtlRainbowColor::clone() {
    return PointerSharedPtr<SVMtlRainbowColor>(new SVMtlRainbowColor(this));
}

void SVMtlRainbowColor::reset() {
    SVMtlCore::reset();
    m_lerp = 0.0f;
    m_accTime = 0.0f;
    m_time = 1.0f;
}

//逻辑更新
void SVMtlRainbowColor::update(f32 dt) {
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

void SVMtlRainbowColor::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("lerp", m_lerp);
}
