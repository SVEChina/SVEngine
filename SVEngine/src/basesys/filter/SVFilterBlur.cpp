//
//  SVFilterBlur.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2019/4/27.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVFilterBlur.h"

#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlSmooth.h"
#include "../../rendercore/SVRendererBase.h"
#include "../../rendercore/SVRenderMgr.h"


SVFilterBlur::SVFilterBlur(SVInst *_app)
:SVFilterBase(_app){
    m_type=SV_FUNC_BLUR;
    m_name="SVFilterBlur";
    m_smooth=100.0f;
    m_pPassNode = nullptr;
}

SVFilterBlur::~SVFilterBlur(){
}

bool SVFilterBlur::create(SVTEXTYPE _inType,SVTEXTYPE _outType){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    SVTexturePtr t_tex = t_renderer->getSVTex(_inType);
    s32 t_w = t_tex->getwidth();
    s32 t_h = t_tex->getheight();
    
    if(! t_renderer->getSVTex(E_TEX_FILTER_GLOW_2) ){
        t_renderer->createSVTex(E_TEX_FILTER_GLOW_2,t_w/2, t_h/2, GL_RGBA);
        t_renderer->createSVTex(E_TEX_FILTER_GLOW_3,t_w/2, t_h/2, GL_RGBA);
    }
    //创建多passnode
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->setname("SVFilterBlurNode");
    m_pPassNode->create(t_w, t_h);
    m_pPassNode->setRSType(m_rstype);
    //创建pass
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    m_lkMtl01=MakeSharedPtr<SVMtlSmooth>(mApp,"newblur");
    m_lkMtl01->setTexcoordFlip(1.0f, 1.0f);
    m_lkMtl01->setImgWH(1.0/(t_w/4),0.0);
    m_lkMtl01->setSmooth(m_smooth);
    t_pass1->setMtl(m_lkMtl01);
    t_pass1->setInTex(0,_inType);
    t_pass1->setOutTex(E_TEX_FILTER_GLOW_2);
    m_pPassNode->addPass(t_pass1);
    //
    m_lkMtl02=MakeSharedPtr<SVMtlSmooth>(mApp,"newblur");
    m_lkMtl02->setTexcoordFlip(1.0f, 1.0f);
    m_lkMtl02->setImgWH(0.0 ,1.0/(t_h/4));
    m_lkMtl02->setSmooth(m_smooth);
    t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->setMtl(m_lkMtl02);
    t_pass1->setInTex(0,E_TEX_FILTER_GLOW_2);
    t_pass1->setOutTex(E_TEX_FILTER_GLOW_3);
    m_pPassNode->addPass(t_pass1);
    
    SVMtlCorePtr t_lkMtl=MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_lkMtl->setTexcoordFlip(1.0f, 1.0f);
    t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->setMtl(t_lkMtl);
    t_pass1->setInTex(0,E_TEX_FILTER_GLOW_3);
    t_pass1->setOutTex(_outType);
    m_pPassNode->addPass(t_pass1);
   
    return true;
}

void SVFilterBlur::destroy(){
    if(m_pPassNode){
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
    m_pPassNode = nullptr;
    
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer){
        t_renderer->destroySVTex(E_TEX_FILTER_GLOW_2);
    }
}


void SVFilterBlur::update(f32 dt){
    m_lkMtl01->setSmooth(m_smooth);
    m_lkMtl02->setSmooth(m_smooth);
}

void SVFilterBlur::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                          RAPIDJSON_NAMESPACE::Value &_objValue) {
}

void SVFilterBlur::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
}
