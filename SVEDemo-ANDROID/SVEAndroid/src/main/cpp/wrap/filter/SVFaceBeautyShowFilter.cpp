//
// SVFaceBeautyShowFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFaceBeautyShowFilter.h"
#include "mtl/SVMtlCore.h"
#include "mtl/SVMtlFair1.h"
#include "mtl/SVMtlBeauty.h"
#include "core/SVPass.h"
#include "mtl/SVTexMgr.h"
#include "mtl/SVTexture.h"
#include "node/SVMultPassNode.h"
#include "mtl/SVMtlBasedOn.h"
#include "rendercore/renderer/SVRendererBase.h"
#include "rendercore/SVRenderMgr.h"


SVFairDataLows::SVFairDataLows(SVInst *_app)
: SVFilterBase(_app) {
    m_type=SV_FUNC_BEAUTY;
    m_name="SVFairDataLows";
    m_mtl_a = nullptr;
    m_mtl_b = nullptr;
    m_mtl_c = nullptr;
    m_mtl_back = nullptr;
    m_pPassNode = nullptr;
    m_radius = 0.0f;
    m_distanceV = 1.8f;
    m_distanceH = 1.8f;
    m_width_factor = 6.610056f;
    m_height_factor =6.610056f;
}

SVFairDataLows::~SVFairDataLows() {
}

bool SVFairDataLows::create() {
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    
    m_mtl_a = MakeSharedPtr<SVMtlFilterBrilaterl>(mApp);
    m_mtl_a->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_a->setTexSizeIndex(0,0.5/t_w,0.5/ t_h);
    
    m_mtl_b = MakeSharedPtr<SVMtlFilterBrilaterl>(mApp);
    m_mtl_b->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_b->setTexSizeIndex(0,0.5/t_w,0.5/ t_h);
    
    m_mtl_c = MakeSharedPtr<SVMtlBlur>(mApp);
    m_mtl_c->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_c->setTexSizeIndex(0,0.5/t_w,0.5/ t_h);
    
    m_mtl_back = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->create(t_w, t_h);
    //
    if(! t_renderer->getSVTex(E_TEX_HELP0) ){
        t_renderer->createSVTex(E_TEX_HELP0,t_w, t_h, GL_RGBA);
    }
    //
    if(! t_renderer->getSVTex(E_TEX_HELP1) ){
        t_renderer->createSVTex(E_TEX_HELP1,t_w, t_h, GL_RGBA);
    }
    
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->setMtl(m_mtl_a);
    t_pass1->setInTex(0, E_TEX_MAIN);
    t_pass1->setOutTex(E_TEX_HELP0);
    m_pPassNode->addPass(t_pass1);
    //
    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    t_pass2->setMtl(m_mtl_b);
    t_pass2->setInTex(0, E_TEX_HELP0);
    t_pass2->setInTex(1, E_TEX_MAIN);
    t_pass2->setOutTex(E_TEX_HELP1);
    m_pPassNode->addPass(t_pass2);
    //
    SVPassPtr t_pass3 = MakeSharedPtr<SVPass>();
    t_pass3->setMtl(m_mtl_c);
    t_pass3->setInTex(0, E_TEX_MAIN);
    t_pass3->setInTex(1, E_TEX_HELP1);
    t_pass3->setOutTex(E_TEX_HELP0);
    m_pPassNode->addPass(t_pass3);
    //写回
    SVPassPtr t_pass4 = MakeSharedPtr<SVPass>();
    t_pass4->setMtl(m_mtl_back);
    t_pass4->setInTex(0, E_TEX_HELP0);
    t_pass4->setOutTex( E_TEX_MAIN );
    m_pPassNode->addPass(t_pass4);
    return true;
}

void SVFairDataLows::destroy() {
    m_mtl_a = nullptr;
    m_mtl_b = nullptr;
    m_mtl_c = nullptr;
    m_mtl_back = nullptr;
    if(m_pPassNode){
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
}

void SVFairDataLows::setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type){
    if(_type==E_BEATY_FILTER){
        setRadius(_smooth/100.0);
    }
}

f32 SVFairDataLows::getFilterParam(SVFILTERITEMTYPE _type){
    if(_type==E_BEATY_FILTER){
        return getRadius();
    }
    return 0;
}

void SVFairDataLows::update(f32 dt) {
    if (m_mtl_a) {
        m_mtl_a->setTexcoordFlip(1.0, 1.0);
        m_mtl_a->setOffset( 0.0f,m_height_factor);
        m_mtl_a->setDistance(m_distanceH);
    }
    if (m_mtl_b) {
        m_mtl_b->setTexcoordFlip(1.0, 1.0);
        m_mtl_b->setOffset(m_width_factor, 0.0f);
        m_mtl_b->setDistance(m_distanceV);
    }

    if (m_mtl_c) {
        m_mtl_c->setTexcoordFlip(1.0, 1.0);
        if (m_radius>0.95) {
            m_mtl_c->setRadius(0.95);
        } else {
            m_mtl_c->setRadius(m_radius);
        }
    }
    //SV_LOG_ERROR("FACE BEAUTY W=%f ,h=%f", m_width_factor, m_height_factor);
}

SVFairDataBlur::SVFairDataBlur(SVInst *_app)
: SVFilterBase(_app) {
    m_type=SV_FUNC_BEAUTY;
    m_name="SVFairDataBlur";
    m_mtl_a = nullptr;
    m_mtl_b = nullptr;
    m_mtl_c = nullptr;
    m_mtl_d = nullptr;
    m_mtl_e = nullptr;
    m_mtl_back = nullptr;
    m_pPassNode = nullptr;
    m_smooth=0.0;
    
}

SVFairDataBlur::~SVFairDataBlur(){
}

bool SVFairDataBlur::create() {
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    //
    m_mtl_a = MakeSharedPtr<SVMtlCore>(mApp,"blur_fair_01");
    m_mtl_a->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_a->setTexSizeIndex(0, 0.0, 2.5/t_h);

    m_mtl_b =  MakeSharedPtr<SVMtlCore>(mApp,"blur_fair_01");
    m_mtl_b->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_b->setTexSizeIndex(0,2.5/t_w,0.0);

    m_mtl_c =  MakeSharedPtr<SVMtlCore>(mApp,"blur_fair_mean");
    m_mtl_c->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_c->setTexSizeIndex(0,2.5/t_w,2.5/ t_h);

    m_mtl_e = MakeSharedPtr<SVMtlCore>(mApp,"blur_fair_01");
    m_mtl_e->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_e->setTexSizeIndex(0, 0.0,  2.5/t_h);

    m_mtl_f = MakeSharedPtr<SVMtlCore>(mApp,"blur_fair_01");
    m_mtl_f->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_f->setTexSizeIndex(0,2.5/t_w,0.0);

    m_mtl_d = MakeSharedPtr<SVMtlBlurFair>(mApp);
    m_mtl_d->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_d->setTexSizeIndex(0,3.5/t_w,3.5/t_h);

    m_mtl_back = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->create(t_w, t_h);

    //
    if(! t_renderer->getSVTex(E_TEX_HELP0) ){
        t_renderer->createSVTex(E_TEX_HELP0,t_w, t_h, GL_RGBA);
    }
    //
    if(! t_renderer->getSVTex(E_TEX_HELP1) ){
        t_renderer->createSVTex(E_TEX_HELP1,t_w, t_h, GL_RGBA);
    }
    //
    if(! t_renderer->getSVTex(E_TEX_HELP2) ){
        t_renderer->createSVTex(E_TEX_HELP2,t_w, t_h, GL_RGBA);
    }
    
    SVPassPtr t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_a);
    t_pass->setInTex(0, E_TEX_MAIN);
    t_pass->setOutTex(E_TEX_HELP0);
    m_pPassNode->addPass(t_pass);

    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_b);
    t_pass->setInTex(0,E_TEX_HELP0);
    t_pass->setOutTex(E_TEX_HELP1);
    m_pPassNode->addPass(t_pass);
    //
    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_c);
    t_pass->setInTex(0, E_TEX_MAIN);
    t_pass->setInTex(1, E_TEX_HELP1);
    t_pass->setOutTex(E_TEX_HELP2);
    m_pPassNode->addPass(t_pass);
    //
    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_e);
    t_pass->setInTex(0, E_TEX_HELP2);
    t_pass->setOutTex(E_TEX_HELP0);
    m_pPassNode->addPass(t_pass);
    //
    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_f);
    t_pass->setInTex(0, E_TEX_HELP0);
    t_pass->setOutTex(E_TEX_HELP2);
    m_pPassNode->addPass(t_pass);
    //
    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_d);
    t_pass->setInTex(0, E_TEX_MAIN);
    t_pass->setInTex(1,  E_TEX_HELP1);
    t_pass->setInTex(2,  E_TEX_HELP2);
    t_pass->setOutTex(E_TEX_HELP0);
    m_pPassNode->addPass(t_pass);
    //写回
    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_back);
    t_pass->setInTex(0, E_TEX_HELP0);
    t_pass->setOutTex( E_TEX_MAIN );
    m_pPassNode->addPass(t_pass);
    return true;
}

void SVFairDataBlur::destroy() {
    m_mtl_a = nullptr;
    m_mtl_b = nullptr;
    m_mtl_c = nullptr;
    m_mtl_d = nullptr;
    m_mtl_e = nullptr;
    m_mtl_back = nullptr;
    if(m_pPassNode){
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
}

void SVFairDataBlur::setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type){
    if(_type==E_BEATY_FILTER){
       setSmooth(_smooth/100.0);
    }
}

f32 SVFairDataBlur::getFilterParam(SVFILTERITEMTYPE _type){
    if(_type==E_BEATY_FILTER){
        return getSmooth();
    }
    return 0;
}

void SVFairDataBlur::update(f32 dt){
    if (m_mtl_a) {
        m_mtl_a->setTexcoordFlip(1.0, 1.0);
    }
    
    if (m_mtl_b) {
        m_mtl_b->setTexcoordFlip(1.0, 1.0);
    }
   
    if (m_mtl_d) {
        m_mtl_d->setTexcoordFlip(1.0, 1.0);
        m_mtl_d->setSmooth(m_smooth);
    }
    
    if (m_mtl_e) {
        m_mtl_e->setTexcoordFlip(1.0, 1.0);
    }
    
    if (m_mtl_f) {
        m_mtl_f->setTexcoordFlip(1.0, 1.0);
    }
}

SVFairLtraLow::SVFairLtraLow(SVInst *_app)
: SVFilterBase(_app) {
    m_type=SV_FUNC_BEAUTY;
    m_name="SVFairLtraLow";
    m_mtl_a = nullptr;
    m_mtl_back = nullptr;
    m_smooth=0.0;
}

SVFairLtraLow::~SVFairLtraLow(){
}

bool SVFairLtraLow::create() {
    SVRendererBasePtr t_renderer = mApp->getRenderMgr()->getRenderer();
    if(!t_renderer)
        return false;
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    m_mtl_a = MakeSharedPtr<SVMtlFairLtraLow>(mApp);
    m_mtl_a->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_back = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    
    if(! t_renderer->getSVTex(E_TEX_HELP0) ){
        t_renderer->createSVTex(E_TEX_HELP0,t_w, t_h, GL_RGBA);
    }
    
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->create(t_w, t_h);
    
    SVPassPtr t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_a);
    t_pass->setInTex(0, E_TEX_MAIN);
    t_pass->setOutTex(E_TEX_HELP0);
    m_pPassNode->addPass(t_pass);
    //写回
    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_back);
    t_pass->setInTex(0, E_TEX_HELP0);
    t_pass->setOutTex( E_TEX_MAIN );
    m_pPassNode->addPass(t_pass);
    return true;
}

void SVFairLtraLow::destroy() {
    m_mtl_a = nullptr;
    m_mtl_back = nullptr;
    if(m_pPassNode){
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
}

void SVFairLtraLow::setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type){
    if(_type==E_BEATY_FILTER){
        setSmooth(_smooth/100.0);
    }
}

f32 SVFairLtraLow::getFilterParam(SVFILTERITEMTYPE _type){
    if(_type==E_BEATY_FILTER){
        return getSmooth();
    }
    return 0;
}

void SVFairLtraLow::update(f32 dt){
    if (m_mtl_a) {
        SVMtlFairLtraLowPtr t_tmp = std::dynamic_pointer_cast<SVMtlFairLtraLow>(m_mtl_a);
        if (t_tmp) {
            t_tmp->setTexcoordFlip(1.0, 1.0);
            t_tmp->setSmooth(m_smooth);
        }
    }
}
