//
// SVMtlPenStrokeGlow.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlPenStrokeGlow.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"
#include "../basesys/SVCameraMgr.h"
#include "../node/SVCameraNode.h"
SVMtlPenStrokeGlow::SVMtlPenStrokeGlow(SVInst *_app)
:SVMtlBillboard(_app) {
    m_mtlname = "penstrokeglow";
    m_strokeColor.set(38, 165, 38, 100);
}

SVMtlPenStrokeGlow::SVMtlPenStrokeGlow(SVMtlPenStrokeGlow *_mtl)
:SVMtlBillboard(_mtl){
    m_strokeColor = _mtl->m_strokeColor;
}

SVMtlPenStrokeGlow::~SVMtlPenStrokeGlow() {
    
}

SVMtlCorePtr SVMtlPenStrokeGlow::clone() {
    return PointerSharedPtr<SVMtlPenStrokeGlow>(new SVMtlPenStrokeGlow(this));
}

void SVMtlPenStrokeGlow::reset() {
    SVMtlBillboard::reset();
}

void SVMtlPenStrokeGlow::setColor(FVec4 &_color){
    m_strokeColor = _color;
}

void SVMtlPenStrokeGlow::_submitUniform(SVRendererBasePtr _render) {
    SVMtlBillboard::_submitUniform(_render);
    FVec4 t_color = m_strokeColor/255.0;
    _render->submitUniformf4v("u_strokecolor", t_color.get());
}

