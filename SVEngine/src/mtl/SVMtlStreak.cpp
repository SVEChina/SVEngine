//
// SVMtlStreak.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlStreak.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"

SVMtlStreak::SVMtlStreak(SVInst *_app)
:SVMtlADFilterBase(_app,"filterstreak") {

}

SVMtlStreak::SVMtlStreak(SVMtlStreak *_mtl)
:SVMtlADFilterBase(_mtl){
    
}

SVMtlStreak::~SVMtlStreak() {
    
}

SVMtlCorePtr SVMtlStreak::clone() {
    return PointerSharedPtr<SVMtlStreak>(new SVMtlStreak(this));
}

void SVMtlStreak::reset() {
    SVMtlADFilterBase::reset();
 
}

//逻辑更新
void SVMtlStreak::update(f32 dt) {
    SVMtlADFilterBase::update(dt);
   
}

void SVMtlStreak::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);

}
