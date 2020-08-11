//
// SVFilterOilPaint.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFilterOilPaint.h"
#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlBasedOn.h"
#include "../../rendercore/SVRendererBase.h"
#include "../../mtl/SVMtlShinning.h"
#include "../../rendercore/SVRenderMgr.h"
SVFilterOilPaint::SVFilterOilPaint(SVInst *_app)
:SVFilterBase(_app){
    m_type=SV_FUNC_ADBASE;
    m_name="SVFilterOilPaint";

}

SVFilterOilPaint::~SVFilterOilPaint(){
    
}

bool SVFilterOilPaint::create(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    SVTexturePtr t_tex = t_renderer->getSVTex(E_TEX_MAIN);
    s32 t_w = t_tex->getwidth();
    s32 t_h = t_tex->getheight();
    t_renderer->createSVTex(E_TEX_FILTER_1, t_w, t_h, GL_RGBA);
    t_renderer->createSVTex(E_TEX_FILTER_2, t_w, t_h, GL_RGBA);
    //创建多passnode
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->setname("SVAdvanceFilterNode");
    m_pPassNode->create(t_w, t_h);
    m_pPassNode->setRSType(RST_IMGFILTER);
    //创建pass
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_mtl_grayscale= MakeSharedPtr<SVMtlCore>(mApp,"grayScale");
    t_mtl_grayscale->setTexcoordFlip(1.0f, 1.0f);
    t_pass1->setMtl(t_mtl_grayscale);
    t_pass1->setInTex(0,E_TEX_MAIN);
    t_pass1->setOutTex(E_TEX_FILTER_1);
    m_pPassNode->addPass(t_pass1);
    
    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_mtl_structureTensor= MakeSharedPtr<SVMtlCore>(mApp,"structureTensor");
    t_mtl_structureTensor->setTexcoordFlip(1.0f, 1.0f);
    t_mtl_structureTensor->setTexSizeIndex(0, t_w, t_h);
    t_pass2->setMtl(t_mtl_structureTensor);
    t_pass2->setInTex(0,E_TEX_FILTER_1);
    t_pass2->setOutTex(E_TEX_FILTER_2);
    m_pPassNode->addPass(t_pass2);
    
    SVPassPtr t_pass3 = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_mtl_gaussianH= MakeSharedPtr<SVMtlCore>(mApp,"gaussianH");
    t_mtl_gaussianH->setTexcoordFlip(1.0f, 1.0f);
    t_mtl_gaussianH->setTexSizeIndex(0, t_w, t_h);
    t_pass3->setMtl(t_mtl_gaussianH);
    t_pass3->setInTex(0,E_TEX_FILTER_2);
    t_pass3->setOutTex(E_TEX_FILTER_1);
    m_pPassNode->addPass(t_pass3);
    
    SVPassPtr t_pass4 = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_mtl_gaussianV= MakeSharedPtr<SVMtlCore>(mApp,"gaussianV");
    t_mtl_gaussianV->setTexcoordFlip(1.0f, 1.0f);
    t_mtl_gaussianV->setTexSizeIndex(0, t_w, t_h);
    t_pass4->setMtl(t_mtl_gaussianV);
    t_pass4->setInTex(0,E_TEX_FILTER_1);
    t_pass4->setOutTex(E_TEX_FILTER_2);
    m_pPassNode->addPass(t_pass4);
    
    SVPassPtr t_pass5 = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_mtl_eigen= MakeSharedPtr<SVMtlCore>(mApp,"eigen");
    t_mtl_eigen->setTexcoordFlip(1.0f, 1.0f);
    t_mtl_eigen->setTexSizeIndex(0, t_w, t_h);
    t_pass5->setMtl(t_mtl_eigen);
    t_pass5->setInTex(0,E_TEX_FILTER_2);
    t_pass5->setOutTex(E_TEX_FILTER_1);
//    m_pPassNode->addPass(t_pass5);
    
    SVPassPtr t_pass6 = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_mtl_edgeLIC= MakeSharedPtr<SVMtlCore>(mApp,"filterlic2");
    t_mtl_edgeLIC->setTexcoordFlip(1.0f, 1.0f);
    t_mtl_edgeLIC->setTexSizeIndex(0, t_w, t_h);
    t_pass6->setMtl(t_mtl_edgeLIC);
    t_pass6->setInTex(0,E_TEX_MAIN);
    t_pass6->setInTex(1,E_TEX_FILTER_2);
    t_pass6->setOutTex(E_TEX_FILTER_1);
    m_pPassNode->addPass(t_pass6);

    SVPassPtr t_pass = MakeSharedPtr<SVPass>();
    SVMtlCorePtr t_mtl_back= MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    t_pass->setMtl(t_mtl_back);
    t_pass->setInTex(0, E_TEX_FILTER_1);
    t_pass->setOutTex(E_TEX_MAIN);
    m_pPassNode->addPass(t_pass);
    return true;
}

void SVFilterOilPaint::destroy(){
    if(m_pPassNode){
        m_pPassNode->removeFromParent();
        m_pPassNode = nullptr;
    }
    m_pPassNode = nullptr;
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer){
        t_renderer->destroySVTex(E_TEX_FILTER_1);
    }
}

void SVFilterOilPaint::update(f32 dt){
    //
   
}

void SVFilterOilPaint::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                         RAPIDJSON_NAMESPACE::Value &_objValue) {
}

void SVFilterOilPaint::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
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

