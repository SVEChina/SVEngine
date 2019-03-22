//
// SVADFilterBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVADFilterBase.h"
#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlBasedOn.h"
#include "../../rendercore/renderer/SVRendererBase.h"
#include "../../mtl/SVMtlShinning.h"
#include "../../rendercore/SVRenderMgr.h"
SVADFilterBase::SVADFilterBase(SVInst *_app, SVMtlCorePtr _mtl)
:SVFilterBase(_app){
    m_type = SV_FUNC_ADBASE;
    m_name = "SVADFilterBase";
    m_mtl = _mtl;
}

SVADFilterBase::~SVADFilterBase(){
    
}

bool SVADFilterBase::create(){
    if (!m_mtl) {
        return false;
    }
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    SVTexturePtr t_tex = t_renderer->getSVTex(E_TEX_MAIN);
    s32 t_w = t_tex->getwidth();
    s32 t_h = t_tex->getheight();
    t_renderer->createSVTex(E_TEX_FILTER_1, t_w, t_h, GL_RGBA);
    //创建多passnode
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->setname("SVAdvanceFilterNode");
    m_pPassNode->create(t_w, t_h);
    //创建pass
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    m_mtl->setTexcoordFlip(1.0f, 1.0f);
    t_pass1->setMtl(m_mtl);
    t_pass1->setInTex(0,E_TEX_MAIN);
    t_pass1->setOutTex(E_TEX_FILTER_1);
    m_pPassNode->addPass(t_pass1);

    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_mtl_back= MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    t_pass2->setMtl(t_mtl_back);
    t_pass2->setInTex(0, E_TEX_FILTER_1);
    t_pass2->setOutTex(E_TEX_MAIN);
    m_pPassNode->addPass(t_pass2);
    return true;
}

void SVADFilterBase::destroy(){
    if(m_pPassNode){
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
    m_pPassNode = nullptr;
    m_mtl = nullptr;
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer){
        t_renderer->destroySVTex(E_TEX_FILTER_1);
    }
}

void SVADFilterBase::update(f32 dt){
    //
    if(m_pPassNode && m_mtl){
        m_pPassNode->setvisible(true);
        m_mtl->update(dt);
    }
}

void SVADFilterBase::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                         RAPIDJSON_NAMESPACE::Value &_objValue) {
}

void SVADFilterBase::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
//    if (item.HasMember("data") && item["data"].IsString()) {
//        SVRendererBasePtr t_renderer = mApp->getRenderer();
//        if(t_renderer){
//            t_renderer->createSVTex(E_TEX_FILTER_LUT, 512, 512, GL_RGBA,GL_RGBA);
//            SVTexturePtr t_tex=t_renderer->getSVTex(E_TEX_FILTER_LUT);
//            t_tex->setTexData((void*)item["data"].GetString(), item["data"].GetStringLength());
//            setLUTTex(t_tex);
//        }
//    }
}

