//
//  SVSharpFilter.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2019/4/1.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVSharpFilter.h"

#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlSmooth.h"
#include "../../rendercore/renderer/SVRendererBase.h"
#include "../../rendercore/SVRenderMgr.h"


SVSharpFilter::SVSharpFilter(SVInst *_app)
:SVFilterBase(_app){
    m_type=SV_FUNC_SHARP;
    m_name="SVFilterSharp";
    m_smooth=0.0f;
    m_pPassNode = nullptr;
}

SVSharpFilter::~SVSharpFilter(){
}

bool SVSharpFilter::create(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    SVTexturePtr t_tex = t_renderer->getSVTex(E_TEX_MAIN);
    s32 t_w = t_tex->getwidth();
    s32 t_h = t_tex->getheight();
    
    if(! t_renderer->getSVTex(E_TEX_HELP0) ){
        t_renderer->createSVTex(E_TEX_HELP0,t_w, t_h, GL_RGBA);
        
    }
    //创建多passnode
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->setname("SVFilterSharpNode");
    m_pPassNode->create(t_w, t_h);
    m_pPassNode->setRSType(RST_IMGFILTER);
    //创建pass
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_lkMtl=MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_lkMtl->setTexcoordFlip(1.0f, 1.0f);
    t_pass1->setMtl(t_lkMtl);
    t_pass1->setInTex(0,E_TEX_MAIN);
    t_pass1->setOutTex(E_TEX_HELP0);
    m_pPassNode->addPass(t_pass1);
    
    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    m_mtl_smooth =MakeSharedPtr<SVMtlSmooth>(mApp,"sharp");
    m_mtl_smooth->setTexcoordFlip(1.0, 1.0);
    m_mtl_smooth->setImgWH(t_w,t_h);
    t_pass2->setMtl(m_mtl_smooth);
    t_pass2->setInTex(0, E_TEX_HELP0);
    t_pass2->setOutTex(E_TEX_MAIN);
    m_pPassNode->addPass(t_pass2);
    return true;
}

void SVSharpFilter::destroy(){
    if(m_pPassNode){
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
    m_pPassNode = nullptr;
}

void SVSharpFilter::update(f32 dt){
    m_mtl_smooth->setSmooth(m_smooth);
}

void SVSharpFilter::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                          RAPIDJSON_NAMESPACE::Value &_objValue) {
}

void SVSharpFilter::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
}
