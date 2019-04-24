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
    m_objPos.set(0, 0, 0);
    m_viewPos.set(0, 0, 0);
    m_up.set(0, 0, 0);
}

SVMtlBillboard::SVMtlBillboard(SVMtlBillboard *_mtl)
:SVMtlCore(_mtl){
    m_objPos = _mtl->m_objPos;
    m_viewPos = _mtl->m_viewPos;
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

void SVMtlBillboard::setObjectPos(FVec3 &_pos){
    m_objPos.set(_pos);
}

void SVMtlBillboard::setViewPos(FVec3 &_viewPos){
    m_viewPos.set(_viewPos);
}

void SVMtlBillboard::setUp(FVec3 &_up){
    m_up.set(_up);
}

void SVMtlBillboard::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf3v("u_up", m_up.get());
    _render->submitUniformf3v("u_viewPos", m_viewPos.get());
    _render->submitUniformf3v("u_objPos", m_objPos.get());
}

