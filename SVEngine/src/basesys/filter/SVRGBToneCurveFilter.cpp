//
// SVRGBToneCurveFilter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRGBToneCurveFilter.h"
#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlBasedOn.h"
#include "../../rendercore/SVRendererBase.h"
#include "../../rendercore/SVRenderMgr.h"

SVRGBToneCurveFilter::SVRGBToneCurveFilter(SVInst *_app)
:SVFilterBase(_app){
    m_name="SVRGBToneCurveFilter";
    m_type=SV_FUNC_BSPLINE;
    m_mtl_rgba = nullptr;
    m_mtl_back = nullptr;
    texrgb = nullptr;
}

SVRGBToneCurveFilter::~SVRGBToneCurveFilter(){
    destroy();
}

bool SVRGBToneCurveFilter::create() {
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    //增加pass
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->create(t_w, t_h);
    m_pPassNode->setRSType(RST_IMGFILTER);
    //
    texrgb = mApp->getTexMgr()->createUnctrlTexture(256, 1, GL_RGBA, GL_RGBA);
    //
    if(! t_renderer->getSVTex(E_TEX_HELP1) ){
        t_renderer->createSVTex(E_TEX_HELP1,t_w, t_h, GL_RGBA);
    }
    //
    m_mtl_rgba = MakeSharedPtr<SVMtlCore>(mApp,"rgbaFilter");
    m_mtl_rgba->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_rgba->setTexSizeIndex(1,0.5/t_w,0.5/ t_h);
    SVPassPtr t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_rgba);
    t_pass->setInTex(0, E_TEX_MAIN);
    t_pass->setInTex(1, texrgb);
    t_pass->setOutTex(E_TEX_HELP0);
    m_pPassNode->addPass(t_pass);
    //写回
    m_mtl_back = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_back);
    t_pass->setInTex(0, E_TEX_HELP0);
    t_pass->setOutTex( E_TEX_MAIN );
    m_pPassNode->addPass(t_pass);
    return true;
}

void SVRGBToneCurveFilter::destroy() {
    m_mtl_rgba = nullptr;
    m_mtl_back = nullptr;
    texrgb = nullptr;
    if(m_pPassNode) {
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
}

void SVRGBToneCurveFilter::update(f32 dt){
    
}
