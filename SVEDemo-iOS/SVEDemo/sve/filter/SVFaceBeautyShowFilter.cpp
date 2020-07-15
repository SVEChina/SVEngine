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
#include "mtl/SVMtlSmooth.h"
#include "rendercore/SVRendererBase.h"
#include "rendercore/SVRenderMgr.h"

SVFairData::SVFairData(SVInst *_app)
: SVFilterBase(_app) {
    m_type=SV_FUNC_BEAUTY;
    m_name="SVFairData";
    m_mtl_a = nullptr;
    m_mtl_b = nullptr;
    m_mtl_c = nullptr;
    m_mtl_back = nullptr;
    m_radius = 5.0f;
    m_distanceV = 4.2f;
    m_distanceH = 2.9f;
    m_width_factor = 3.9f;
    m_height_factor = 4.9f;
//    重度参数不要干掉
//    m_distanceV = 1.8f;
//    m_distanceH = 1.8f;
//    m_width_factor = 6.610056f;
//    m_height_factor =6.610056f;
}

SVFairData::~SVFairData() {
}

bool SVFairData::create() {
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    //增加pass
    SVMtlFairLtraLowPtr m_mtl_a1 = MakeSharedPtr<SVMtlFairLtraLow>(mApp);
    m_mtl_a1->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_a1->setSmooth(1.0f);

    m_mtl_a = MakeSharedPtr<SVMtlFilterBrilaterl>(mApp);
    m_mtl_a->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_b = MakeSharedPtr<SVMtlFilterBrilaterl2>(mApp);
    m_mtl_b->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_c = MakeSharedPtr<SVMtlGaussian>(mApp);
    m_mtl_c->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_back = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    
    m_mtl_blur01=MakeSharedPtr<SVMtlSmooth>(mApp,"newblur");
    m_mtl_blur01->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_blur01->setImgWH(1.7/(t_w),0.0);
    m_mtl_blur01->setSmooth(1.0);
    
    m_mtl_blur02=MakeSharedPtr<SVMtlSmooth>(mApp,"newblur");
    m_mtl_blur02->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_blur02->setImgWH(0.0,1.7/(t_h));
    m_mtl_blur02->setSmooth(1.0);
    
    m_mtl_mix=MakeSharedPtr<SVMtlSmooth>(mApp,"blur_fair_mix");
    m_mtl_mix->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_mix->setSmooth(1.0);

    
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
    
    if(! t_renderer->getSVTex(E_TEX_FLITER_GUASS) ){
        t_renderer->createSVTex(E_TEX_FLITER_GUASS,t_w, t_h, GL_RGBA);
        t_renderer->createSVTex(E_TEX_FLITER_GUASS01,t_w, t_h, GL_RGBA);
    }
    
    //
//    SVPassPtr t_pass10 = MakeSharedPtr<SVPass>();
//    t_pass10->mTag="filter-Beauty-High-Brilater-Pass";
//    t_pass10->setMtl(m_mtl_a1);
//    t_pass10->setInTex(0, E_TEX_MAIN);
//    t_pass10->setOutTex(E_TEX_HELP2);
//    m_pPassNode->addPass(t_pass10);

    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->mTag="filter-Beauty-High-Brilater-Pass";
    t_pass1->setMtl(m_mtl_a);
    t_pass1->setInTex(0, E_TEX_MAIN);
    t_pass1->setOutTex(E_TEX_HELP0);
    m_pPassNode->addPass(t_pass1);
    //
    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    t_pass2->mTag="filter-Beauty-High-Brilater-Pass2";
    t_pass2->setMtl(m_mtl_b);
    t_pass2->setInTex(0, E_TEX_HELP0);
    t_pass2->setInTex(1, E_TEX_MAIN);
    t_pass2->setOutTex(E_TEX_HELP1);
    m_pPassNode->addPass(t_pass2);
    //
    SVPassPtr t_pass3 = MakeSharedPtr<SVPass>();
    t_pass3->mTag="filter-Beauty-High-Gaussian-Pass";
    t_pass3->setMtl(m_mtl_blur01);
    t_pass3->setInTex(0, E_TEX_HELP1);
    t_pass3->setOutTex(E_TEX_FLITER_GUASS);
    m_pPassNode->addPass(t_pass3);
    
    SVPassPtr t_pass4 = MakeSharedPtr<SVPass>();
    t_pass4->mTag="filter-Beauty-High-Gaussian-Pass";
    t_pass4->setMtl(m_mtl_blur02);
    t_pass4->setInTex(0, E_TEX_FLITER_GUASS);
    t_pass4->setOutTex(E_TEX_FLITER_GUASS01);
    m_pPassNode->addPass(t_pass4);

    //写回
    SVPassPtr t_pass5 = MakeSharedPtr<SVPass>();
    t_pass5->mTag="filter-Beauty-High-mix-Pass";
    t_pass5->setMtl(m_mtl_mix);
    t_pass5->setInTex(0, E_TEX_FLITER_GUASS01);
    t_pass5->setInTex(1, E_TEX_MAIN);
    t_pass5->setOutTex(E_TEX_HELP0);
    m_pPassNode->addPass(t_pass5);
    
    SVPassPtr t_pass6 = MakeSharedPtr<SVPass>();
    t_pass6->mTag="filter-Beauty-High-Back-Pass";
    t_pass6->setMtl(m_mtl_back);
    t_pass6->setInTex(0, E_TEX_HELP0);
    t_pass6->setOutTex(E_TEX_MAIN);
    m_pPassNode->addPass(t_pass6);
    return true;
}

void SVFairData::destroy() {
    m_mtl_a = nullptr;
    m_mtl_b = nullptr;
    m_mtl_c = nullptr;
    m_mtl_back = nullptr;
    if(m_pPassNode) {
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
}

void SVFairData::setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type){
    if(_type==E_BEATY_FILTER){
        if(_smooth==0){
            setRadius(5.0);
        }else{
            setRadius(1.0-_smooth/100.0*1.0);
        }

    }
}

f32 SVFairData::getFilterParam(SVFILTERITEMTYPE _type){
    if(_type==E_BEATY_FILTER){
        return getRadius();
    }
    return 0;
}

void SVFairData::update(f32 dt) {
    if (m_mtl_a) {
        m_mtl_a->setTexcoordFlip(1.0, 1.0);
        m_mtl_a->setOffset(m_width_factor, 0.0f);
        m_mtl_a->setDistance(m_distanceV);
    }

    if (m_mtl_b) {
        m_mtl_b->setTexcoordFlip(1.0, 1.0);
        m_mtl_b->setOffset(0.0f, m_height_factor);
        m_mtl_b->setDistance(m_distanceH);
    }

    if (m_mtl_blur01) {
        m_mtl_blur01->setTexcoordFlip(1.0, 1.0);
        //m_mtl_c->setOffset(0.0f,m_height_factor);
        m_mtl_blur01->setSmooth(m_radius);
    }
    
    if (m_mtl_blur02) {
        m_mtl_blur02->setTexcoordFlip(1.0, 1.0);
        //m_mtl_c->setOffset(0.0f,m_height_factor);
        m_mtl_blur02->setSmooth(m_radius);
    }
}

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



