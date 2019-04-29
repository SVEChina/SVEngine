//
// SVFaceBeautyFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFaceBeautyFilter.h"
#include "../../mtl/SVMtlCore.h"
#include "../../mtl/SVMtlFair1.h"
#include "../../mtl/SVMtlBeauty.h"
#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlBasedOn.h"
#include "../../rendercore/SVRendererBase.h"
#include "../../rendercore/SVRenderMgr.h"


SVFaceBeautyBase::SVFaceBeautyBase(SVInst *_app)
: SVFilterBase(_app){
    m_pParam= MakeSharedPtr<SVGenFBParam>();
    m_pParam->reset();
}

SVFaceBeautyBase::~SVFaceBeautyBase(){
    
}

bool SVFaceBeautyBase::create(){
    return false;
}

void SVFaceBeautyBase::destroy(){
    
}

void SVFaceBeautyBase::update(f32 dt){
    
}

void SVFaceBeautyBase::setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type){
    
}

f32 SVFaceBeautyBase::getFilterParam(SVFILTERITEMTYPE _type){
    return 0.0f;
}

void SVFaceBeautyBase::refreshData(SVGenFBParamPtr _param){
    if(m_pParam&&_param){
        m_pParam->copy(_param);
    }
}

void SVFaceBeautyBase::setSmooth(f32 _smooth){
    m_pParam->m_smooth = _smooth;
}

void SVFaceBeautyBase::setParam01(f32 _smooth){
       m_pParam->m_param01 = _smooth;
}

void SVFaceBeautyBase::setParam02(f32 _smooth){
     m_pParam->m_param02 = _smooth;
}

void SVFaceBeautyBase::setParam03(f32 _smooth){
     m_pParam->m_param03 = _smooth;
}

void SVFaceBeautyBase::setParam04(f32 _smooth){
     m_pParam->m_param04 = _smooth;
}

void SVFaceBeautyBase::setParam05(f32 _smooth){
     m_pParam->m_param05 = _smooth;
}

void SVFaceBeautyBase::setParam06(f32 _smooth){
     m_pParam->m_param06 = _smooth;
}

//序列化
void SVFaceBeautyBase::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                    RAPIDJSON_NAMESPACE::Value &_objValue){
    
}

void SVFaceBeautyBase::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    
}

SVFairDataBlur::SVFairDataBlur(SVInst *_app)
: SVFaceBeautyBase(_app) {
    m_type=SV_FUNC_BEAUTY;
    m_name="SVFairDataBlur";
    m_mtl_a = nullptr;
    m_mtl_b = nullptr;
    m_mtl_c = nullptr;
    m_mtl_d = nullptr;
    m_mtl_e = nullptr;
    m_mtl_back = nullptr;
    m_pPassNode = nullptr;
    m_pParam->m_smooth=0.0;
    m_pParam->m_param01=2.5;
    m_pParam->m_param02=2.5;
    m_pParam->m_param03=2.5;
    m_pParam->m_param04=2.5;
    m_pParam->m_param05=2.5;
    m_pParam->m_param06=3.5;

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
    m_mtl_a->setTexSizeIndex(0, 0.0, m_pParam->m_param01/t_h);

    m_mtl_b =  MakeSharedPtr<SVMtlCore>(mApp,"blur_fair_01");
    m_mtl_b->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_b->setTexSizeIndex(0,m_pParam->m_param02/t_w,0.0);

    m_mtl_c =  MakeSharedPtr<SVMtlCore>(mApp,"blur_fair_mean");
    m_mtl_c->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_c->setTexSizeIndex(0,m_pParam->m_param03/t_w,m_pParam->m_param03/ t_h);

    m_mtl_e = MakeSharedPtr<SVMtlCore>(mApp,"blur_fair_01");
    m_mtl_e->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_e->setTexSizeIndex(0, 0.0,  m_pParam->m_param04/t_h);

    m_mtl_f = MakeSharedPtr<SVMtlCore>(mApp,"blur_fair_01");
    m_mtl_f->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_f->setTexSizeIndex(0,m_pParam->m_param05/t_w,0.0);

    m_mtl_d = MakeSharedPtr<SVMtlBlurFair>(mApp);
    m_mtl_d->setTexcoordFlip(1.0f, 1.0f);
    m_mtl_d->setTexSizeIndex(0,m_pParam->m_param06/t_w,m_pParam->m_param06/t_h);

    m_mtl_back = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->create(t_w, t_h);
    m_pPassNode->setRSType(RST_BEAUTY);

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
    t_pass->mTag = "blur_fair_01";
    m_pPassNode->addPass(t_pass);

    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_b);
    t_pass->setInTex(0,E_TEX_HELP0);
    t_pass->setOutTex(E_TEX_HELP1);
    t_pass->mTag = "blur_fair_01";
    m_pPassNode->addPass(t_pass);
    //
    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_c);
    t_pass->setInTex(0, E_TEX_MAIN);
    t_pass->setInTex(1, E_TEX_HELP1);
    t_pass->setOutTex(E_TEX_HELP2);
    t_pass->mTag = "blur_fair_mean";
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
    t_pass->mTag = "blur_fair_01";
    m_pPassNode->addPass(t_pass);
    //
    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_d);
    t_pass->setInTex(0, E_TEX_MAIN);
    t_pass->setInTex(1,  E_TEX_HELP1);
    t_pass->setInTex(2,  E_TEX_HELP2);
    t_pass->setOutTex(E_TEX_HELP0);
    t_pass->mTag = "SVMtlBlurFair";
    m_pPassNode->addPass(t_pass);
    //写回
    t_pass = MakeSharedPtr<SVPass>();
    t_pass->setMtl(m_mtl_back);
    t_pass->setInTex(0, E_TEX_HELP0);
    t_pass->setOutTex( E_TEX_MAIN );
    t_pass->mTag = "writeback";
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
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer){
        return ;
    }
    t_renderer->destroySVTex(E_TEX_HELP0);
    t_renderer->destroySVTex(E_TEX_HELP1);
    t_renderer->destroySVTex(E_TEX_HELP2);
}

void SVFairDataBlur::setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type){
    if(_type==E_BEATY_FILTER){
        m_pParam->m_smooth=_smooth/100.0f;
    }
}

f32 SVFairDataBlur::getFilterParam(SVFILTERITEMTYPE _type){
    if(_type==E_BEATY_FILTER){
        return m_pParam->m_smooth;
    }
    return 0;
}

void SVFairDataBlur::update(f32 dt){
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    if (m_mtl_a) {
        m_mtl_a->setTexcoordFlip(1.0, 1.0);
        m_mtl_a->setTexSizeIndex(0, 0.0, m_pParam->m_param01/t_h);
    }
    
    if (m_mtl_b) {
        m_mtl_b->setTexcoordFlip(1.0, 1.0);
         m_mtl_b->setTexSizeIndex(0,m_pParam->m_param02/t_w,0.0);
    }
    
    if(m_mtl_c){
        m_mtl_c->setTexcoordFlip(1.0, 1.0);
        m_mtl_c->setTexSizeIndex(0,m_pParam->m_param03/t_w,m_pParam->m_param03/ t_h);
    }
    
    if (m_mtl_d) {
        m_mtl_d->setTexcoordFlip(1.0, 1.0);
        m_mtl_d->setTexSizeIndex(0,m_pParam->m_param06/t_w,m_pParam->m_param06/t_h);
        m_mtl_d->setSmooth(m_pParam->m_smooth);
    }
    
    if (m_mtl_e) {
        m_mtl_e->setTexcoordFlip(1.0, 1.0);
        m_mtl_e->setTexSizeIndex(0, 0.0,  m_pParam->m_param04/t_h);
    }
    
    if (m_mtl_f) {
        m_mtl_f->setTexcoordFlip(1.0, 1.0);
        m_mtl_f->setTexSizeIndex(0,m_pParam->m_param05/t_w,0.0);
    }
}

//序列化
void SVFairDataBlur::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                              RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);
    locationObj.AddMember("m_smooth",m_pParam->m_smooth, _allocator);
    locationObj.AddMember("param01",m_pParam->m_param01, _allocator);
    locationObj.AddMember("param02",m_pParam->m_param02, _allocator);
    locationObj.AddMember("param03",m_pParam->m_param03, _allocator);
    locationObj.AddMember("param04",m_pParam->m_param04, _allocator);
    locationObj.AddMember("param05",m_pParam->m_param05, _allocator);
    locationObj.AddMember("param06",m_pParam->m_param06, _allocator);
    _objValue.AddMember("facebeauty01", locationObj, _allocator);  //添加object到Document中
}

void SVFairDataBlur::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    
    if (item.HasMember("m_smooth") && item["m_smooth"].IsFloat()) {
        m_pParam->m_smooth=item["m_smooth"].GetFloat();
    }
    
    if (item.HasMember("param01") && item["param01"].IsFloat()) {
         m_pParam->m_param01=item["param01"].GetFloat();
    }
    
    if (item.HasMember("param02") && item["param02"].IsFloat()) {
        m_pParam->m_param02=item["param02"].GetFloat();
    }
    
    if (item.HasMember("param03") && item["param03"].IsFloat()) {
        m_pParam->m_param03=item["param03"].GetFloat();
    }
    
    if (item.HasMember("param04") && item["param04"].IsFloat()) {
        m_pParam->m_param04=item["param04"].GetFloat();
    }
    
    if (item.HasMember("param05") && item["param05"].IsFloat()) {
        m_pParam->m_param05=item["param05"].GetFloat();
    }
    
    if (item.HasMember("param06") && item["param06"].IsFloat()) {
        m_pParam->m_param06=item["param06"].GetFloat();
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
    m_pPassNode->setRSType(RST_BEAUTY);
    
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

