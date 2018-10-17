//
// SVDataFilter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVDataFilter.h"
#include "../base/SVPreDeclare.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVPictureProcess.h"
#include "../basesys/filter/SVBasedonFilter.h"
#include "../basesys/filter/SVFilterGenLUT.h"

SVDataFilter::SVDataFilter(SVInst *_app)
: SVDataBase(_app){
    m_pFilterGenLUT=MakeSharedPtr<SVFilterGenLUT>(mApp);
    m_dataType = MODELDATA_FILTER;
    m_beauty_rad = 0.0f;
    m_shadow = 0.0f;
    m_constrast = 0.0f;
    m_saturation = 0.0f;
    m_acutance = 0.0f;
    m_brightness = 0.0f;
    m_whitebalance = 0.0f;
    m_hightlight = 0.0f;
    m_gamma = 0.0f;
    m_rshift = 0.0f;
    m_gshift = 0.0f;
    m_bshift = 0.0f;
    m_sd_rshift = 0.0f;
    m_sd_gshift = 0.0f;
    m_sd_bshift = 0.0f;
    m_hh_rshift = 0.0f;
    m_hh_gshift = 0.0f;
    m_hh_bshift = 0.0f;
    m_temperature = 0.0f;
    m_tint = 0.0f;
}

SVDataFilter::~SVDataFilter(){
    m_pFilterGenLUT=nullptr;
    m_beauty_rad = 0.0f;
    m_shadow = 0.0f;
    m_constrast = 0.0f;
    m_saturation = 0.0f;
    m_acutance = 0.0f;
    m_brightness = 0.0f;
    m_whitebalance = 0.0f;
    m_hightlight = 0.0f;
    m_gamma = 0.0f;
    m_rshift = 0.0f;
    m_gshift = 0.0f;
    m_bshift = 0.0f;
    m_sd_rshift = 0.0f;
    m_sd_gshift = 0.0f;
    m_sd_bshift = 0.0f;
    m_hh_rshift = 0.0f;
    m_hh_gshift = 0.0f;
    m_hh_bshift = 0.0f;
    m_temperature = 0.0f;
    m_tint = 0.0f;
}

void SVDataFilter::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    locationObj.AddMember("rad",m_beauty_rad, _allocator);
    locationObj.AddMember("shadow",m_shadow, _allocator);
    locationObj.AddMember("constrast",m_constrast, _allocator);
    locationObj.AddMember("saturation",m_saturation, _allocator);
    locationObj.AddMember("acutance",m_acutance, _allocator);
    locationObj.AddMember("brightness",m_brightness, _allocator);
    locationObj.AddMember("whitebalance",m_whitebalance, _allocator);
    locationObj.AddMember("highlight",m_hightlight, _allocator);
    locationObj.AddMember("gamma",m_gamma, _allocator);
    locationObj.AddMember("rshift",m_rshift, _allocator);
    locationObj.AddMember("bshift",m_gshift, _allocator);
    locationObj.AddMember("gshift",m_bshift, _allocator);
    locationObj.AddMember("sd_rshift",m_sd_rshift, _allocator);
    locationObj.AddMember("sd_gshift",m_sd_gshift, _allocator);
    locationObj.AddMember("sd_bshift",m_sd_bshift, _allocator);
    locationObj.AddMember("hh_rshift",m_hh_rshift, _allocator);
    locationObj.AddMember("hh_gshift",m_hh_gshift, _allocator);
    locationObj.AddMember("hh_bshift",m_hh_bshift, _allocator);
    locationObj.AddMember("temperature",m_temperature, _allocator);
    locationObj.AddMember("tint",m_tint, _allocator);
    _objValue.AddMember("filter", locationObj, _allocator);  //添加object到Document中
}

void SVDataFilter::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    m_beauty_rad = item["rad"].GetFloat();
    m_shadow = item["shadow"].GetFloat();
    m_constrast = item["constrast"].GetFloat();
    m_saturation = item["saturation"].GetFloat();
    m_acutance = item["acutance"].GetFloat();
    m_brightness = item["brightness"].GetFloat();
    m_whitebalance = item["whitebalance"].GetFloat();
    m_hightlight = item["highlight"].GetFloat();
    m_gamma = item["gamma"].GetFloat();
    m_rshift = item["rshift"].GetFloat();
    m_gshift = item["gshift"].GetFloat();
    m_bshift = item["bshift"].GetFloat();
    m_sd_rshift = item["sd_rshift"].GetFloat();
    m_sd_gshift = item["sd_gshift"].GetFloat();
    m_sd_bshift = item["sd_bshift"].GetFloat();
    m_hh_rshift = item["hh_rshift"].GetFloat();
    m_hh_gshift = item["hh_gshift"].GetFloat();
    m_hh_bshift = item["hh_bshift"].GetFloat();
    m_temperature = item["temperature"].GetFloat();
    m_tint = item["tint"].GetFloat();
}

SVNodePtr SVDataFilter::toNode(){
    SVPictureProcessPtr t_pic = mApp->getBasicSys()->getPicProc();
    SVNodePtr t_node;
    if(t_pic){
        m_pFilterGenLUT->create();
        t_pic->addFilter(m_pFilterGenLUT);
        t_pic->openFilter(m_pFilterGenLUT);
        t_node=m_pFilterGenLUT->getNode();
    }
    return t_node;
}

//刷入
//E_BEATY_FILTER=0,
void SVDataFilter::refreshIn() {
    SVPictureProcessPtr t_pic = mApp->getBasicSys()->getPicProc();
    if(t_pic) {
        t_pic->setFilterParam(m_beauty_rad, E_BEATY_FILTER);
        t_pic->setFilterParam(m_shadow, E_SHADOWS_FILTER);
        t_pic->setFilterParam(m_constrast, E_CONTRAST_FILTER);
        t_pic->setFilterParam(m_saturation, E_SATURATION_FILTER);
        t_pic->setFilterParam(m_acutance, E_ACUTANCE_FILTER);
        t_pic->setFilterParam(m_brightness, E_BRIGHTNESS_FILTER);
        t_pic->setFilterParam(m_whitebalance, E_WHITENING_FILTER);
        t_pic->setFilterParam(m_hightlight, E_HIGHLIGHTS_FILTER);
        t_pic->setFilterParam(m_gamma, E_GAMMA_FILTER);
        t_pic->setFilterParam(m_rshift, E_REDSHIFT_FILTER);
        t_pic->setFilterParam(m_gshift, E_GREENSHIFT_FILTER);
        t_pic->setFilterParam(m_bshift, E_BLUESHIFT_FILTER);
        t_pic->setFilterParam(m_sd_rshift, E_SDREDSHIFT_FILTER);
        t_pic->setFilterParam(m_sd_gshift, E_SDGREENSHIFT_FILTER);
        t_pic->setFilterParam(m_sd_bshift, E_SDBLUESHIFT_FILTER);
        t_pic->setFilterParam(m_hh_rshift, E_HHREDSHIFT_FILTER);
        t_pic->setFilterParam(m_hh_gshift, E_HHGREENSHIFT_FILTER);
        t_pic->setFilterParam(m_hh_bshift, E_HHBLUESHIFT_FILTER);
        t_pic->setFilterParam(m_temperature, E_TEMPERATURE_FILTER);
        t_pic->setFilterParam(m_tint, E_TINT_FILTER);
    }
}

//刷出
void SVDataFilter::refreshOut() {
    SVPictureProcessPtr t_pic = mApp->getBasicSys()->getPicProc();
    if(t_pic) {
        m_beauty_rad = t_pic->getFilterParam(E_BEATY_FILTER);
        m_shadow = t_pic->getFilterParam(E_SHADOWS_FILTER);
        m_constrast = t_pic->getFilterParam(E_CONTRAST_FILTER);
        m_saturation = t_pic->getFilterParam(E_SATURATION_FILTER);
        m_acutance = t_pic->getFilterParam(E_ACUTANCE_FILTER);
        m_brightness = t_pic->getFilterParam(E_BRIGHTNESS_FILTER);
        m_whitebalance = t_pic->getFilterParam(E_WHITENING_FILTER);
        m_hightlight = t_pic->getFilterParam(E_HIGHLIGHTS_FILTER);
        m_gamma = t_pic->getFilterParam(E_GAMMA_FILTER);
        m_rshift = t_pic->getFilterParam(E_REDSHIFT_FILTER);
        m_gshift = t_pic->getFilterParam(E_GREENSHIFT_FILTER);
        m_bshift = t_pic->getFilterParam(E_BLUESHIFT_FILTER);
        m_sd_rshift = t_pic->getFilterParam(E_SDREDSHIFT_FILTER);
        m_sd_gshift = t_pic->getFilterParam(E_SDGREENSHIFT_FILTER);
        m_sd_bshift = t_pic->getFilterParam(E_SDBLUESHIFT_FILTER);
        m_hh_rshift = t_pic->getFilterParam(E_HHREDSHIFT_FILTER);
        m_hh_gshift = t_pic->getFilterParam(E_HHGREENSHIFT_FILTER);
        m_hh_bshift = t_pic->getFilterParam(E_HHBLUESHIFT_FILTER);
        m_temperature = t_pic->getFilterParam(E_TEMPERATURE_FILTER);
        m_tint = t_pic->getFilterParam(E_TINT_FILTER);
    }
}
