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
:SVMtlCore(_app,"normal2d_billboard") {
    m_quadPosW.set(0, 0, 0);
}

SVMtlBillboard::SVMtlBillboard(SVMtlBillboard *_mtl)
:SVMtlCore(_mtl){
    m_quadPosW = _mtl->m_quadPosW;
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
    m_quadPosW.set(_quadPosW.x, _quadPosW.y, _quadPosW.z);
}

void SVMtlBillboard::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    SVCameraNodePtr t_cameraNode = mApp->getCameraMgr()->getMainCamera();
    FVec3 cameraPos = t_cameraNode->getPosition();
//    FVec3 cameraPos = FVec3(0.0f, 0.0f, 0.0f);
    _render->submitUniformf3v("u_viewPos", cameraPos.get());
    _render->submitUniformf3v("u_quadPosW", m_quadPosW.get());
}

