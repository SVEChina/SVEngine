//
// SVMtlShake.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlShake.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"

SVMtlShake::SVMtlShake(SVInst *_app)
:SVMtlCore(_app,"filtershake") {
    m_time = 0.0;
}

SVMtlShake::SVMtlShake(SVMtlShake *_mtl)
:SVMtlCore(_mtl){
    m_time = _mtl->m_time;
}

SVMtlShake::~SVMtlShake() {
    
}

SVMtlCorePtr SVMtlShake::clone() {
    return PointerSharedPtr<SVMtlShake>(new SVMtlShake(this));
}

void SVMtlShake::reset() {
    SVMtlCore::reset();
 
}

//逻辑更新
void SVMtlShake::update(f32 dt) {
    SVMtlCore::update(dt);
   
}

void SVMtlShake::setTime(f32 _time){
    if (m_time != _time) {
        m_time = _time;
    }
}

void SVMtlShake::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("time", m_time);
}
