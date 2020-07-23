//
// SVMtlOilPaint.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlOilPaint.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRendererBase.h"

SVMtlOilPaint::SVMtlOilPaint(SVInst *_app)
:SVMtlADFilterBase(_app,"edgeLic") {
    m_time = 0.0f;
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    setTexSizeIndex(0, t_w, t_h);
//    setTexcoordFlip(1.0, -1.0);
    SVTexturePtr texture = mApp->getTexMgr()->getTexture("svres/textures/BeltFlow.png",true);
    setTexture(1, texture);
    setTexSizeIndex(1, texture->getwidth(), texture->getheight());
    
    //
    SVTexturePtr texture2 = mApp->getTexMgr()->getTexture("svres/textures/noise.png",true);
    setTexture(2, texture2);
    setTexSizeIndex(2, texture2->getwidth(), texture2->getheight());
}

SVMtlOilPaint::SVMtlOilPaint(SVMtlOilPaint *_mtl)
:SVMtlADFilterBase(_mtl){
    m_time = _mtl->m_time;
}

SVMtlOilPaint::~SVMtlOilPaint() {
    
}

SVMtlCorePtr SVMtlOilPaint::clone() {
    return PointerSharedPtr<SVMtlOilPaint>(new SVMtlOilPaint(this));
}

void SVMtlOilPaint::reset() {
    SVMtlCore::reset();
 
}

//逻辑更新
void SVMtlOilPaint::update(f32 dt) {
    SVMtlCore::update(dt);
    m_time += dt;
}

void SVMtlOilPaint::_submitUniform(SVRendererBasePtr _render) {
    SVMtlCore::_submitUniform(_render);
    _render->submitUniformf("time", m_time);
}
