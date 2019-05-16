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
#include "../module/pendraw/SVPenDraw.h"
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
    if (doc.HasMember("SVPen")) {
        RAPIDJSON_NAMESPACE::Value &penArray = doc["SVPen"];
        _parsePen(mApp, penArray, _resid, _path);
    }
    return nullptr;
}

void
SVParsePen::_parsePen(SVInst *_app, RAPIDJSON_NAMESPACE::Value &_item, s32 _resid, cptr8 _path) {
    SVString t_name = "sv_pen_module";
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_module) {
        SVPenDrawPtr penDraw = DYN_TO_SHAREPTR(SVPenDraw, t_module);
        if (penDraw) {
            penDraw->fromJSON(_item, _path);
        }
    }
}

