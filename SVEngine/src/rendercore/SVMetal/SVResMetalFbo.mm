//
// SVResMetalFbo.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVResMetalFbo.h"
#include "SVRendererMetal.h"
#include "../../app/SVInst.h"
#include "../../mtl/SVMtlDef.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../base/SVDataChunk.h"
#include "../../file/SVFileMgr.h"
#include "../../base/SVDataSwap.h"
#include "../../base/SVLock.h"
#include "../../rendercore/SVRenderMgr.h"
#include "../SVRenderer.h"


SVResMetalFBO::SVResMetalFBO(SVInst* _app)
:SVResFBO(_app)
,m_pEncoder(nullptr){
    
}

SVResMetalFBO::~SVResMetalFBO() {
    
}

void SVResMetalFBO::create(SVRendererPtr _renderer) {
    
}

void SVResMetalFBO::destroy(SVRendererPtr _renderer) {
    
}

id<MTLRenderCommandEncoder> SVResMetalFBO::getEncoder() {
    return m_pEncoder;
}
