//
// SVMtlBillboard.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlBillboard.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"
#include "../basesys/SVCameraMgr.h"
#include "../node/SVCameraNode.h"
SVMtlBillboard::SVMtlBillboard(SVInst *_app)
:SVMtlCore(_app,"normal3d_billboard") {
    m_quadPosW.set(0, 0, 0);
    m_viewPosW.set(0, 0, 0);
    m_up.set(0, 0, 0);
}

SVMtlBillboard::SVMtlBillboard(SVMtlBillboard *_mtl)
:SVMtlCore(_mtl){
    m_quadPosW = _mtl->m_quadPosW;
    m_viewPosW = _mtl->m_viewPosW;
    m_up = _mtl->m_up;
}

SVMtlBillboard::~SVMtlBillboard() {
    
}

SVMtlCorePtr SVMtlBillboard::clone() {
    return PointerSharedPtr<SVMtlBillboard>(new SVMtlBillboard(this));
}

void SVMtlBillboard::reset() {
    SVMtlCore::reset();
}

void SVMtlBillboard::setQuadPosW(FVec3 &_quadPosW){
    m_quadPosW.set(_quadPosW);
}

void SVMtlBillboard::setViewPosW(FVec3 &_viewPosW){
    m_viewPosW.set(_viewPosW);
}

void SVMtlBillboard::setUp(FVec3 &_up){
    m_up.set(_up);
}

void SVMtlBillboard::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf3v("u_up", m_up.get());
    _render->submitUniformf3v("u_viewPos", m_viewPosW.get());
    _render->submitUniformf3v("u_quadPosW", m_quadPosW.get());
}

