//
// SVMtlGlitch.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlGlitch.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"

SVMtlGlitch::SVMtlGlitch(SVInst *_app)
:SVMtlCore(_app,"filterglitch") {
    m_drift = nullptr;
    m_jitter = nullptr;
    m_threshHold = nullptr;
    _initializeParameter();
}

SVMtlGlitch::SVMtlGlitch(SVMtlGlitch *_mtl)
:SVMtlCore(_mtl){
    
}

void SVMtlGlitch::_initializeParameter(){
    m_maxFrame = 9;
    //顏色偏移量
    m_drift = new float[m_maxFrame]{0.0f, 0.03f, 0.032f, 0.035f, 0.03f, 0.032f, 0.031f, 0.029f, 0.025f};
    //偏移的x值
    m_jitter = new float[m_maxFrame]{0.0f, 0.03f, 0.01f, 0.02f, 0.05f, 0.055f, 0.03f, 0.02f, 0.025f};
    //偏移的y值
    m_threshHold = new float[m_maxFrame]{1.0f, 0.965f, 0.9f, 0.9f, 0.9f, 0.6f, 0.8f, 0.5f, 0.5f};
    m_curFrame = 0;
}
void SVMtlGlitch::_destroyParameter(){
    if (m_drift) {
        delete m_drift;
        m_drift = nullptr;
    }
    if (m_jitter) {
        delete m_jitter;
        m_jitter = nullptr;
    }
    if (m_threshHold) {
        delete m_threshHold;
        m_threshHold = nullptr;
    }
}

SVMtlGlitch::~SVMtlGlitch() {
    _destroyParameter();
}

SVMtlCorePtr SVMtlGlitch::clone() {
    return PointerSharedPtr<SVMtlGlitch>(new SVMtlGlitch(this));
}

void SVMtlGlitch::reset() {
    SVMtlCore::reset();
    _destroyParameter();
    _initializeParameter();
}

//逻辑更新
void SVMtlGlitch::update(f32 dt) {
    SVMtlCore::update(dt);
    m_curFrame ++;
    if (m_curFrame>=m_maxFrame){
        m_curFrame = 0;
    }
}

void SVMtlGlitch::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("uColorDrift", m_drift[m_curFrame]);
    _render->submitUniformf2("uScanLineJitter", m_jitter[m_curFrame], m_threshHold[m_curFrame]);
}
