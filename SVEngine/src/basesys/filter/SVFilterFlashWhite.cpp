//
// SVFilterFlashWhite.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFilterFlashWhite.h"
#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlBasedOn.h"
#include "../../rendercore/SVRendererBase.h"
#include "../../mtl/SVMtlFlashWhite.h"
#include "../../rendercore/SVRenderMgr.h"
SVFilterFlashWhite::SVFilterFlashWhite(SVInst *_app)
:SVFilterBase(_app){
    m_type = SV_FUNC_FLASHWHITE;
    m_name = "SVFilterFlashWhite";
    m_time = 0.0f;
    m_cycle = 1.0f;
}

SVFilterFlashWhite::~SVFilterFlashWhite(){
    m_pPassNode = nullptr;
    m_mtlFlashWhite = nullptr;
}

bool SVFilterFlashWhite::create(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    SVTexturePtr t_tex = t_renderer->getSVTex(E_TEX_MAIN);
    s32 t_w = t_tex->getwidth();
    s32 t_h = t_tex->getheight();
    t_renderer->createSVTex(E_TEX_FILTER_2, t_w, t_h, GL_RGBA);
    //创建多passnode
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->setname("SVFilterFlashWhiteNode");
    m_pPassNode->create(t_w, t_h);
    m_pPassNode->setRSType(m_rstype);
    //创建pass
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    m_mtlFlashWhite = MakeSharedPtr<SVMtlFlashWhite>(mApp);
    m_mtlFlashWhite->setTexcoordFlip(1.0f, 1.0f);
    t_pass1->setMtl(m_mtlFlashWhite);
    t_pass1->setInTex(0,E_TEX_MAIN);
    t_pass1->setOutTex(E_TEX_FILTER_2);
    m_pPassNode->addPass(t_pass1);

    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_mtl_back= MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    t_pass2->setMtl(t_mtl_back);
    t_pass2->setInTex(0, E_TEX_FILTER_2);
    t_pass2->setOutTex(E_TEX_MAIN);
    m_pPassNode->addPass(t_pass2);
    return true;
}

void SVFilterFlashWhite::destroy(){
    if(m_pPassNode){
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer){
        t_renderer->destroySVTex(E_TEX_FILTER_2);
    }
}

void SVFilterFlashWhite::update(f32 dt){
    //
    if(m_pPassNode && m_mtlFlashWhite){
        m_pPassNode->setvisible(true);
        m_mtlFlashWhite->update(dt);
        m_mtlFlashWhite->setTime(m_time);
        m_mtlFlashWhite->setCycle(m_cycle);
    }
}

void SVFilterFlashWhite::setTime(f32 _time){
    if (m_time != _time) {
        m_time = _time;
    }
}

void SVFilterFlashWhite::setCycle(f32 _cycle){
    if (m_cycle != _cycle) {
        m_cycle = _cycle;
    }
}
