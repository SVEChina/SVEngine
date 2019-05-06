//
// SVParsePen.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParsePen.h"
#include "../app/SVGlobalMgr.h"
#include "../file/SVFileMgr.h"
#include "../operate/SVOpCreate.h"
#include "../operate/SVOpParse.h"
#include "../module/pendraw/SVPendraw.h"
#include "../module/SVModuleSys.h"
SVParsePen::SVParsePen(SVInst *_app)
:SVParseData(_app) {
    
}

SVParsePen::~SVParsePen(){
    
}


SVModuleBasePtr SVParsePen::parse(cptr8 _path, s32 _resid){
    RAPIDJSON_NAMESPACE::Document t_doc;
    //解析效果包
    SVString t_path = SVString(_path) + SVString("/");
    SV_LOG_INFO("SVParsePen::parse begin\n");
    SVString config_name = t_path + "config.json";
    SVDataChunk tDataStream;
    SV_LOG_ERROR("SVParsePen::parse begin config name %s\n", config_name.get());
    bool tflag = mApp->m_pGlobalMgr->m_pFileMgr->loadFileContentStr(&tDataStream, config_name.get());
    if (!tflag)
        return nullptr;
    SV_LOG_ERROR("SVParsePen::load effect sucess\n");
    SV_LOG_ERROR("filedata %s\n", tDataStream.m_data);
    if (!tDataStream.m_data) {
        SV_LOG_ERROR("data stream is null");
        return nullptr;
    }
    
    RAPIDJSON_NAMESPACE::Document doc;
    doc.Parse(tDataStream.m_data);
    if (doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = doc.GetParseError();
        SV_LOG_ERROR("rapidjson error code:%d \n", code);
        return nullptr;
    }
    if (doc.HasMember("version")) {
        RAPIDJSON_NAMESPACE::Value &version = doc["version"];
    }
    if (doc.HasMember("SVPen") && doc["SVPen"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &penObj = doc["SVPen"];
        _parsePen(mApp, penObj, _resid, _path);
    }
    return nullptr;
}

void
SVParsePen::_parsePen(SVInst *_app, RAPIDJSON_NAMESPACE::Value &_item, s32 _resid, cptr8 _path) {
    f32 t_strokeWidth = 0.005;
    if (_item.HasMember("stroke_width") && _item["stroke_width"].IsFloat()) {
        RAPIDJSON_NAMESPACE::Value &t_value = _item["stroke_width"];
        t_strokeWidth = t_value.GetFloat();
    }
    FVec4 t_strokeColor;
    t_strokeColor.set(255.0f, 255.0f, 255.0f, 255.0f);
    if (_item.HasMember("stroke_color") && _item["stroke_color"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_values = _item["stroke_color"];
        if (t_values.Size() > 3) {
            t_strokeColor.x = t_values[0].GetFloat();
            t_strokeColor.y = t_values[1].GetFloat();
            t_strokeColor.z = t_values[2].GetFloat();
            t_strokeColor.w = t_values[3].GetFloat();
        }
    }
    f32 t_strokeGlowWidth = 0.1;
    if (_item.HasMember("stroke_glowidth") && _item["stroke_glowidth"].IsFloat()) {
        RAPIDJSON_NAMESPACE::Value &t_value = _item["stroke_glowidth"];
        t_strokeGlowWidth = t_value.GetFloat();
    }
    FVec4 t_strokeGlowColor;
    t_strokeGlowColor.set(0.0f, 255.0f, 127.0f, 255.0f);
    if (_item.HasMember("stroke_glowcolor") && _item["stroke_glowcolor"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_values = _item["stroke_glowcolor"];
        if (t_values.Size() > 3) {
            t_strokeGlowColor.x = t_values[0].GetFloat();
            t_strokeGlowColor.y = t_values[1].GetFloat();
            t_strokeGlowColor.z = t_values[2].GetFloat();
            t_strokeGlowColor.w = t_values[3].GetFloat();
        }
    }
    
    SVString t_name = "sv_pen_module";
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_module) {
        SVPendrawPtr penDraw = DYN_TO_SHAREPTR(SVPendraw, t_module);
        if (penDraw) {
            penDraw->setStrokeWidth(t_strokeWidth);
            penDraw->setStrokeColor(t_strokeColor);
            penDraw->setGlowWidth(t_strokeGlowWidth);
            penDraw->setGlowColor(t_strokeGlowColor);
        }
   
    }
    
}

