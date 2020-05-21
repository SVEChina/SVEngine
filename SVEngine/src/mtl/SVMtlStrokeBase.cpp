//
// SVMtlStrokeBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlStrokeBase.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderer.h"
#include "../basesys/SVCameraMgr.h"
#include "../node/SVCameraNode.h"
SVMtlStrokeBase::SVMtlStrokeBase(SVInst *_app, cptr8 _shader)
:SVMtlCore(_app,_shader) {
  
}

SVMtlStrokeBase::SVMtlStrokeBase(SVMtlStrokeBase *_mtl)
:SVMtlCore(_mtl){
   
}

SVMtlStrokeBase::~SVMtlStrokeBase() {
    
}

SVMtlCorePtr SVMtlStrokeBase::clone() {
    return PointerSharedPtr<SVMtlStrokeBase>(new SVMtlStrokeBase(this));
}

void SVMtlStrokeBase::reset() {
    SVMtlCore::reset();
    
}

void SVMtlStrokeBase::_submitUniform(SVRendererPtr _render) {
    SVMtlCore::_submitUniform(_render);
}

