//
// SVMtlFlashWhite.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlFlashWhite.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderer.h"

SVMtlFlashWhite::SVMtlFlashWhite(SVInst *_app)
:SVMtlCore(_app,"filterflashwhite") {
    m_time = 0.0;
    m_cycle = 1.0f;
}

SVMtlFlashWhite::SVMtlFlashWhite(SVMtlFlashWhite *_mtl)
:SVMtlCore(_mtl){
    m_time = _mtl->m_time;
    m_cycle = _mtl->m_cycle;
}

SVMtlFlashWhite::~SVMtlFlashWhite() {
    
}

SVMtlCorePtr SVMtlFlashWhite::clone() {
    return PointerSharedPtr<SVMtlFlashWhite>(new SVMtlFlashWhite(this));
}

void SVMtlFlashWhite::reset() {
    SVMtlCore::reset();
 
}

//逻辑更新
void SVMtlFlashWhite::update(f32 dt) {
    SVMtlCore::update(dt);
   
}

void SVMtlFlashWhite::setTime(f32 _time){
    if (m_time != _time) {
        m_time = _time;
    }
}

void SVMtlFlashWhite::setCycle(f32 _cycle){
    if (m_cycle != _cycle) {
        m_cycle = _cycle;
    }
}

void SVMtlFlashWhite::_submitUniform(SVRendererPtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("time", m_time);
    _render->submitUniformf("cycle", m_cycle);
}
