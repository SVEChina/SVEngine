//
// SVMtlADFilterBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlADFilterBase.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"

SVMtlADFilterBase::SVMtlADFilterBase(SVInst *_app, cptr8 _shader)
:SVMtlCore(_app,_shader) {
    _resetTime();
}

SVMtlADFilterBase::SVMtlADFilterBase(SVMtlADFilterBase *_mtl)
:SVMtlCore(_mtl){
    
}

SVMtlADFilterBase::~SVMtlADFilterBase() {
    _resetTime();
}

SVMtlCorePtr SVMtlADFilterBase::clone() {
    return PointerSharedPtr<SVMtlADFilterBase>(new SVMtlADFilterBase(this));
}

void SVMtlADFilterBase::reset() {
    SVMtlCore::reset();
}

//逻辑更新
void SVMtlADFilterBase::update(f32 dt) {
    SVMtlCore::update(dt);
}

void SVMtlADFilterBase::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    
}

void SVMtlADFilterBase::_resetTime(){

}
