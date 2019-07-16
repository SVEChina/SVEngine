//
// SVFilterShake.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFilterShake.h"
#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlBasedOn.h"
#include "../../rendercore/SVRendererBase.h"
#include "../../mtl/SVMtlShake.h"
#include "../../rendercore/SVRenderMgr.h"
SVFilterShake::SVFilterShake(SVInst *_app)
:SVFilterBase(_app){
    m_type = SV_FUNC_SHAKE;
    m_name = "SVFilterShake";
    m_time = 0.0f;
    m_amount = 0.4f;
    m_speed = 3.0f;
}

SVFilterShake::~SVFilterShake(){
    m_pPassNode = nullptr;
    m_mtlShake = nullptr;
}

bool SVFilterShake::create(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    SVTexturePtr t_tex = t_renderer->getSVTex(E_TEX_MAIN);
    s32 t_w = t_tex->getwidth();
    s32 t_h = t_tex->getheight();
    t_renderer->createSVTex(E_TEX_FILTER_SHAKE, t_w, t_h, GL_RGBA);
    //创建多passnode
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->setname("SVFilterShakeNode");
    m_pPassNode->create(t_w, t_h);
    m_pPassNode->setRSType(m_rstype);
    //创建pass
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    m_mtlShake = MakeSharedPtr<SVMtlShake>(mApp);
    m_mtlShake->setTexcoordFlip(1.0f, 1.0f);
    t_pass1->setMtl(m_mtlShake);
    t_pass1->setInTex(0,E_TEX_MAIN);
    t_pass1->setOutTex(E_TEX_FILTER_SHAKE);
    m_pPassNode->addPass(t_pass1);

    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_mtl_back= MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    t_pass2->setMtl(t_mtl_back);
    t_pass2->setInTex(0, E_TEX_FILTER_SHAKE);
    t_pass2->setOutTex(E_TEX_MAIN);
    m_pPassNode->addPass(t_pass2);
    return true;
}

void SVFilterShake::destroy(){
    if(m_pPassNode){
        m_pPassNode->removeFromParent();
    }
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer){
        t_renderer->destroySVTex(E_TEX_FILTER_SHAKE);
    }
}

void SVFilterShake::update(f32 dt){
    //
    if(m_pPassNode && m_mtlShake){
        m_pPassNode->setvisible(true);
        m_mtlShake->update(dt);
        m_mtlShake->setTime(m_time);
        m_mtlShake->setAmount(m_amount);
        m_mtlShake->setSpeed(m_speed);
    }
}

void SVFilterShake::setTime(f32 _time){
    if (m_time != _time) {
        m_time = _time;
    }
}

void SVFilterShake::setAmount(f32 _amount){
    if (m_amount != _amount) {
        m_amount = _amount;
    }
}

void SVFilterShake::setSpeed(f32 _speed){
    if (m_speed != _speed) {
        m_speed = _speed;
    }
}
