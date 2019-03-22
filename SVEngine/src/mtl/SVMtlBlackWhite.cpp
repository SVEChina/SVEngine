//
// SVMtlBlackWhite.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlBlackWhite.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"

SVMtlBlackWhite::SVMtlBlackWhite(SVInst *_app)
:SVMtlCore(_app,"filterblackwhite") {
    m_mode = 5.0;
}

SVMtlBlackWhite::SVMtlBlackWhite(SVMtlBlackWhite *_mtl)
:SVMtlCore(_mtl){
    
}

SVMtlBlackWhite::~SVMtlBlackWhite() {
    
}

SVMtlCorePtr SVMtlBlackWhite::clone() {
    return PointerSharedPtr<SVMtlBlackWhite>(new SVMtlBlackWhite(this));
}

void SVMtlBlackWhite::reset() {
    SVMtlCore::reset();
 
}

//逻辑更新
void SVMtlBlackWhite::update(f32 dt) {
    SVMtlCore::update(dt);
   
}

void SVMtlBlackWhite::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("mode", m_mode);
}
