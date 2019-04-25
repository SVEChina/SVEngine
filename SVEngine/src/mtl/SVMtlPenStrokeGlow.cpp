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
    m_strokeColor.set(0.15, 0.65, 0.15, 1.0);
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
    _render->submitUniformf4v("u_strokecolor", m_strokeColor.get());
}

