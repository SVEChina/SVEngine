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
:SVMtlCore(_app,"filterstreak") {

}

SVMtlStreak::SVMtlStreak(SVMtlStreak *_mtl)
:SVMtlCore(_mtl){
    
}

SVMtlStreak::~SVMtlStreak() {
    
}

SVMtlCorePtr SVMtlStreak::clone() {
    return PointerSharedPtr<SVMtlStreak>(new SVMtlStreak(this));
}

void SVMtlStreak::reset() {
    SVMtlCore::reset();
 
}

//逻辑更新
void SVMtlStreak::update(f32 dt) {
    SVMtlCore::update(dt);
   
}

void SVMtlStreak::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);

}
