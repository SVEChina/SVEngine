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
    m_quadPosW.set(0, 0, 0);
}

SVMtlStrokeBase::SVMtlStrokeBase(SVMtlStrokeBase *_mtl)
:SVMtlCore(_mtl){
    m_quadPosW = _mtl->m_quadPosW;
}

SVMtlStrokeBase::~SVMtlStrokeBase() {
    
}

SVMtlCorePtr SVMtlStrokeBase::clone() {
    return PointerSharedPtr<SVMtlStrokeBase>(new SVMtlStrokeBase(this));
}

void SVMtlStrokeBase::reset() {
    SVMtlCore::reset();
    
}

void SVMtlStrokeBase::setQuadPosW(FVec3 &_quadPosW){
    m_quadPosW.set(_quadPosW.x, _quadPosW.y, _quadPosW.z);
}

void SVMtlStrokeBase::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    SVCameraNodePtr t_cameraNode = mApp->getCameraMgr()->getMainCamera();
    FVec3 cameraPos = t_cameraNode->getPosition();
//    FVec3 cameraPos = FVec3(0.0f, 0.0f, 0.0f);
    _render->submitUniformf3v("u_viewPos", cameraPos.get());
    _render->submitUniformf3v("u_quadPosW", m_quadPosW.get());
}

