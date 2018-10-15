//
//  SVLookUpFilter.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/9/25.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVLookUpFilter.h"
#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlBasedOn.h"
#include "../../rendercore/renderer/SVRendererBase.h"
#include "../../rendercore/SVRenderMgr.h"


SVLookUpFilter::SVLookUpFilter(SVInst *_app)
:SVFilterBase(_app){
    m_type=SV_FUNC_LOOKUP;
    m_name="SVLookUpFilter";
    m_tex01=nullptr;
    m_tex02=nullptr;
    m_lkMtl     = nullptr;
    m_mtl_back  = nullptr;
    m_pPassNode = nullptr;
}

SVLookUpFilter::~SVLookUpFilter(){
    
}

bool SVLookUpFilter::create(SVTexturePtr _looktex,SVTexturePtr _bgtex,SVTexturePtr _outTex){
    m_tex01=_bgtex;
    m_tex02=_looktex;
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    f32 t_w=m_tex01->getwidth();
    f32 t_h=m_tex01->getwidth();
    SVTexturePtr tex = mApp->getTexMgr()->createUnctrlTexture(m_tex01->getwidth(), m_tex01->getwidth(), GL_RGBA, GL_RGBA);
    //创建材质
    SVMtlCorePtr t_lkMtl=MakeSharedPtr<SVMtlCore>(mApp,"lookup");
    t_lkMtl->setTexcoordFlip(1.0f, 1.0f);
    SVMtlCorePtr t_mtl_back=MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    
    //创建多passnode
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->setname("SVLookUpTableNode");
    m_pPassNode->create(t_w, t_h);
    //创建pass
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->setMtl(t_lkMtl);
    t_pass1->setInTex(0,m_tex01);
    t_pass1->setInTex(1,m_tex02);
    t_pass1->setOutTex(tex);
    m_pPassNode->addPass(t_pass1);
    //
    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    t_pass2->setMtl(t_mtl_back);
    t_pass2->setInTex(0, tex);
    t_pass2->setOutTex(_outTex);
    m_pPassNode->addPass(t_pass2);
    return true;
}

bool SVLookUpFilter::create(){
    return false;
}

void SVLookUpFilter::destroy(){
    if(m_pPassNode){
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
    m_lkMtl     = nullptr;
    m_mtl_back  = nullptr;
    m_pPassNode = nullptr;
    m_tex01=nullptr;
    m_tex02=nullptr;
}

void SVLookUpFilter::update(f32 dt){
}



void SVLookUpFilter::setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type){
}

f32 SVLookUpFilter::getFilterParam(SVFILTERITEMTYPE _type){
    return 0;
}

