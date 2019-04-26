//
// SVMtlStrokeBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlStrokeBase.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"
#include "../basesys/SVCameraMgr.h"
#include "../node/SVCameraNode.h"
SVMtlStrokeBase::SVMtlStrokeBase(SVInst *_app, cptr8 _shader)
:SVMtlCore(_app,_shader) {
    m_viewPos.set(0, 0, 0);
    m_up.set(0, 0, 0);
}

SVMtlStrokeBase::SVMtlStrokeBase(SVMtlStrokeBase *_mtl)
:SVMtlCore(_mtl){
    m_viewPos = _mtl->m_viewPos;
    m_up = _mtl->m_up;
}

SVMtlStrokeBase::~SVMtlStrokeBase() {
    
}

SVMtlCorePtr SVMtlStrokeBase::clone() {
    return PointerSharedPtr<SVMtlStrokeBase>(new SVMtlStrokeBase(this));
}

void SVMtlStrokeBase::reset() {
    SVMtlCore::reset();
    
}

void SVMtlStrokeBase::setViewPos(FVec3 &_viewPos){
    m_viewPos = _viewPos;
}

void SVMtlStrokeBase::setUp(FVec3 &_up){
    m_up = _up;
}

void SVMtlStrokeBase::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf3v("u_viewPos", m_viewPos.get());
    _render->submitUniformf3v("u_up", m_up.get());
}

