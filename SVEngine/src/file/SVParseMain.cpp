//
// SVParseMain.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseMain.h"
#include "../operate/SVOpParse.h"
#include "../work/SVThreadPool.h"
#include "../file/SVFileMgr.h"
#include "../base/SVDataChunk.h"
#include "../app/SVInst.h"
#include "../mtl/SVTexMgr.h"
#include "../operate/SVOpCreate.h"
#include "../basesys/SVConfig.h"
#include "SVParseMask.h"
#include "SVParseSprite.h"
#include "SVParseBitfont.h"
#include "../module/SVEffectPackage.h"

SVParseMain::SVParseMain(SVInst *_app)
:SVGBase(_app) {
    m_app = _app;
    _registDictionary("sv_spine", SVParseAnimate::parseAnim);
    _registDictionary("sv_mask", SVParseMask::parseMask);
    _registDictionary("sv_sprite", SVParseSprite::parseSprite);
    _registDictionary("sv_bitfont", SVParseBitfont::parseBitfont);
//    _registDictionary("sv_particle", SVParseParticle::parseParticle);
    //////////////!!!!!!!!!!!!!!!!!!
}

SVParseMain::~SVParseMain(){
    parse_dictionary.clear();
}

void SVParseMain::_registDictionary(cptr8 type, prase_content_fun fun) {
    if (!_hasRegist(type)) {
        parse_dictionary.append(type, fun);
    }
}

void SVParseMain::_unregistDictionary(cptr8 type) {
    if ( _hasRegist(type) ) {
        parse_dictionary.remove(type);
    }
}

bool SVParseMain::_hasRegist(cptr8 type) {
    PARSEPOOL::Iterator it = parse_dictionary.find(type);
    if(it!=parse_dictionary.end())
        return true;
    return false;
}

SVModuleBasePtr SVParseMain::parse(cptr8 path, s32 resid) {
    RAPIDJSON_NAMESPACE::Document t_doc;
    //解析效果包
    SVString t_path = SVString(path) + SVString("/");
    SV_LOG_INFO("SVParseMain::parse begin\n");
    SVString config_name = t_path + "config.json";
    SVDataChunk tDataStream;
    SV_LOG_ERROR("SVParseMain::parse begin config name %s\n", config_name.get());
    bool tflag = mApp->m_pGlobalMgr->m_pFileMgr->loadFileContentStr(&tDataStream, config_name.get());
    if (!tflag)
        return nullptr;
    SV_LOG_ERROR("SVParseMain::load effect sucess\n");
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
    SVString t_type = "gift";
    if (doc.HasMember("type") && doc["type"].IsString()) {
        RAPIDJSON_NAMESPACE::Value &type = doc["type"];
        t_type = type.GetString();
    }
    
    SVEffectPackagePtr t_bundle = MakeSharedPtr<SVEffectPackage>(m_app);
    for(auto iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter){
        cptr8 key = (iter->name).GetString();
        if (doc.HasMember(key)) {
            RAPIDJSON_NAMESPACE::Value &item = iter->value;
            SVNodePtr t_node = _callTypeParse(key, item, resid, t_path.get());
            if (t_node) {
                t_bundle->addEffectUnit(t_node);
            }
        }
    }
    SV_LOG_ERROR("SVParseMain::parse end\n");
    return t_bundle;
}

SVNodePtr SVParseMain::_callTypeParse(cptr8 type, RAPIDJSON_NAMESPACE::Value &item, s32 resid, cptr8 _path) {
    if(_hasRegist(type)) {
        prase_content_fun t_fun = parse_dictionary[type];
        if (t_fun) {
            return (*t_fun)(mApp, item, resid,_path);
        }
    }
    return nullptr;
}



















