//
//  SVFilterGlow.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2019/3/27.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVFilterGlow.h"
#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlSmooth.h"
#include "../../rendercore/SVRendererBase.h"
#include "../../rendercore/SVRenderMgr.h"


SVFilterGlow::SVFilterGlow(SVInst *_app)
:SVFilterBase(_app){
    m_type=SV_FUNC_GLOW;
    m_name="SVFilterGlow";
    m_smooth=0.0f;
    m_pPassNode = nullptr;
}

SVFilterGlow::~SVFilterGlow(){
}

bool SVFilterGlow::create(SVTEXTYPE _inType,SVTEXTYPE _outType){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    SVTexturePtr t_tex = t_renderer->getSVTex(_inType);
    s32 t_w = t_tex->getwidth();
    s32 t_h = t_tex->getheight();
    
    if(! t_renderer->getSVTex(E_TEX_FILTER_GLOW_1) ){
        t_renderer->createSVTex(E_TEX_FILTER_GLOW_1,t_w, t_h, GL_RGBA);
    }
    if(! t_renderer->getSVTex(E_TEX_FILTER_GLOW_2) ){
        t_renderer->createSVTex(E_TEX_FILTER_GLOW_2,t_w/2, t_h/2, GL_RGBA);
    }
    if(! t_renderer->getSVTex(E_TEX_FILTER_GLOW_3) ){
        t_renderer->createSVTex(E_TEX_FILTER_GLOW_3,t_w/2, t_h/2, GL_RGBA);
    }
    //创建多passnode
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->setname("SVFilterGlowNode");
    m_pPassNode->create(t_w, t_h);
    m_pPassNode->setRSType(RST_AR);
    //创建pass
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_lkMtl=MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_lkMtl->setTexcoordFlip(1.0f, 1.0f);
    t_pass1->setMtl(t_lkMtl);
    t_pass1->setInTex(0,_inType);
    t_pass1->setOutTex(E_TEX_FILTER_GLOW_1);
    m_pPassNode->addPass(t_pass1);
    
    t_pass1 = MakeSharedPtr<SVPass>();
    SVMtlSmoothPtr t_lkMtl02=MakeSharedPtr<SVMtlSmooth>(mApp,"blurtex");
    t_lkMtl02->setTexcoordFlip(1.0f, 1.0f);
    t_lkMtl02->setImgWH(4.0/t_w,0.0);
    t_lkMtl02->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
    t_lkMtl02->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
    t_pass1->setMtl(t_lkMtl02);
    t_pass1->setInTex(0,E_TEX_FILTER_GLOW_1);
    t_pass1->setOutTex(E_TEX_FILTER_GLOW_2);
    m_pPassNode->addPass(t_pass1);
//
    SVMtlSmoothPtr t_lkMtl01=MakeSharedPtr<SVMtlSmooth>(mApp,"blurtex");
    t_lkMtl01->setTexcoordFlip(1.0f, 1.0f);
    t_lkMtl01->setImgWH(0.0 ,4.0/t_h);
    t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->setMtl(t_lkMtl01);
    t_pass1->setInTex(0,E_TEX_FILTER_GLOW_2);
    t_pass1->setOutTex(E_TEX_FILTER_GLOW_3);
    m_pPassNode->addPass(t_pass1);
    
    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    m_mtl_smooth =MakeSharedPtr<SVMtlSmooth>(mApp,"glow");
    m_mtl_smooth->setTexcoordFlip(1.0, 1.0);
    m_mtl_smooth->setImgWH(t_w,t_h);
    t_pass2->setMtl(m_mtl_smooth);
    t_pass2->setInTex(0, E_TEX_FILTER_GLOW_3);
    t_pass2->setInTex(1, _inType);
    t_pass2->setOutTex(_outType);
    m_pPassNode->addPass(t_pass2);
    return true;
}

void SVFilterGlow::destroy(){
    if(m_pPassNode){
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
    m_pPassNode = nullptr;
    
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer){
        t_renderer->destroySVTex(E_TEX_FILTER_GLOW_1);
    }
    if(t_renderer){
        t_renderer->destroySVTex(E_TEX_FILTER_GLOW_2);
    }
    if(t_renderer){
        t_renderer->destroySVTex(E_TEX_FILTER_GLOW_3);
    }
}


void SVFilterGlow::update(f32 dt){
   // m_mtl_smooth->setSmooth(1.0);
}

void SVFilterGlow::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                         RAPIDJSON_NAMESPACE::Value &_objValue) {
}

void SVFilterGlow::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
}
