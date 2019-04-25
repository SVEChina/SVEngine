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
    m_objPos.set(0, 0, 0);
    m_viewPos.set(0, 0, 0);
    m_up.set(0, 0, 0);
}

SVMtlPenStrokeGlow::SVMtlPenStrokeGlow(SVMtlPenStrokeGlow *_mtl)
:SVMtlBillboard(_mtl){
    m_objPos = _mtl->m_objPos;
    m_viewPos = _mtl->m_viewPos;
    m_up = _mtl->m_up;
}

SVMtlPenStrokeGlow::~SVMtlPenStrokeGlow() {
    
}

SVMtlCorePtr SVMtlPenStrokeGlow::clone() {
    return PointerSharedPtr<SVMtlPenStrokeGlow>(new SVMtlPenStrokeGlow(this));
}

void SVMtlPenStrokeGlow::reset() {
    SVMtlBillboard::reset();
}

void SVMtlPenStrokeGlow::_submitUniform(SVRendererBasePtr _render) {
    SVMtlBillboard::_submitUniform(_render);
}

