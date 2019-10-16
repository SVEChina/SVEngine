//
// SVMtlColor.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlColor.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"

SVMtlColor::SVMtlColor(SVInst *_app)
:SVMtlCore(_app,"color") {
    
}

SVMtlColor::SVMtlColor(SVMtlColor *_mtl)
:SVMtlCore(_mtl){
    m_color = _mtl->m_color;
}

SVMtlColor::~SVMtlColor() {
    
}

SVMtlCorePtr SVMtlColor::clone() {
    return PointerSharedPtr<SVMtlColor>(new SVMtlColor(this));
}

void SVMtlColor::reset() {
    SVMtlCore::reset();
 
}

//逻辑更新
void SVMtlColor::update(f32 dt) {
    SVMtlCore::update(dt);
   
}

void SVMtlColor::setColor(f32 _r, f32 _g, f32 _b, f32 _a){
    m_color.setColor(_r, _g, _b, _a);
}

void SVMtlColor::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    SVMtlCore::_submitMtl(_render);
    _render->submitUniformf4("u_color", m_color.r, m_color.g, m_color.b, m_color.a);
//    _render->submitUniformf("u_color", m_color);
}
