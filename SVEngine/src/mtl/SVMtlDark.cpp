//
// SVMtlDark.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlDark.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"

SVMtlDark::SVMtlDark(SVInst *_app)
:SVMtlCore(_app,"filterdark") {
    m_intensity = 1.0;
}

SVMtlDark::SVMtlDark(SVMtlDark *_mtl)
:SVMtlCore(_mtl){
    m_intensity = _mtl->m_intensity;
}

SVMtlDark::~SVMtlDark() {
    
}

SVMtlCorePtr SVMtlDark::clone() {
    return PointerSharedPtr<SVMtlDark>(new SVMtlDark(this));
}

void SVMtlDark::reset() {
    SVMtlCore::reset();
 
}

//逻辑更新
void SVMtlDark::update(f32 dt) {
    SVMtlCore::update(dt);
   
}

void SVMtlDark::setDarkIntensity(f32 _intensity){
    if (m_intensity != _intensity) {
        m_intensity = _intensity;
    }
}

void SVMtlDark::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("intensity", m_intensity);
}
