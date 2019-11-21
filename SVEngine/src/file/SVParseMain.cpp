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
#include "SVParseDeform.h"
#include "SVParseMask.h"
#include "SVParseSprite.h"
#include "SVParseBitfont.h"
#include "SVParseBMFont.h"
#include "SVParseParticles.h"
#include "SVParseFrameAni.h"
#include "SVParseBackground.h"
#include "SVParseTexAttachment.h"
#include "SVParseAniTrigger.h"
#include "SVParseMusic.h"
#include "SVParseFilter.h"
#include "../module/SVEffectPackage.h"
#include "../basesys/filter/SVParseLUTFilter.h"

SVParseMain::SVParseMain(SVInst *_app)
:SVGBase(_app) {
    m_app = _app;
    _registDictionary("SVSpineNode", SVParseAnimate::parseAnim);
    _registDictionary("SVFrameAniNode", SVParseFrameAni::parseFrameAni);
    _registDictionary("SV2DFaceMaskSTNode", SVParseMask::parseMask);
    _registDictionary("SVSpriteNode", SVParseSprite::parseSprite);
    _registDictionary("SVBitFontNode", SVParseBitfont::parseBitfont);
    _registDictionary("SVBMFontNode", SVParseBMFont::parseBMFont);
    _registDictionary("SVParticlesNode", SVParseParticles::parseParticles);
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
    t_bundle->init();
    if (doc.HasMember("nodearray") && doc["nodearray"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &nodearray = doc["nodearray"];
        for(s32 i = 0; i<nodearray.Size(); i++){
            RAPIDJSON_NAMESPACE::Value &node = nodearray[i];
            for(auto iter = node.MemberBegin(); iter != node.MemberEnd(); ++iter){
                cptr8 key = (iter->name).GetString();
                if (node.HasMember(key)) {
                    RAPIDJSON_NAMESPACE::Value &item = iter->value;
                    SVNodePtr t_node = _callTypeParse(key, item, resid, t_path.get());
                    if (t_node) {
                        t_bundle->addEffectUnit(t_node);
                    }
                }
            }
        }
    }

    //parse tex attachment
    if (doc.HasMember("TexAttachment") && doc["TexAttachment"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &attachments = doc["TexAttachment"];
        for (s32 i = 0; i<attachments.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &attachment = attachments[i];
            SVAniTexAttachmentPtr t_texAttachment = SVParseTexAttachment::parseTexAttachmet(mApp, attachment, 102, t_path.get());
            t_bundle->addAttachment(t_texAttachment);
        }
    }
    
    //parse trigger
    if (doc.HasMember("triggerarray") && doc["triggerarray"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &triggers = doc["triggerarray"];
        for (s32 i = 0; i<triggers.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &trigger = triggers[i];
            SVAniTriggerPtr t_trigger = SVParseAniTrigger::parseAniTrigger(mApp, trigger, 102, t_path.get());
            t_bundle->addTrigger(t_trigger);
        }
    }
    
    if (doc.HasMember("SVLutFlter") && doc["SVLutFlter"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &attachments = doc["SVLutFlter"];
        SVString t_name = attachments["name"].GetString();
        SVString t_pathLut = t_path + t_name;
        SVFilterBasePtr t_baseFilter = SVParseLUTFilter::parseLUT(mApp,t_pathLut, 107);
        t_bundle->addFilter(t_baseFilter);
    }
    
    if (doc.HasMember("SVFilter") && doc["SVFilter"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &filterObj = doc["SVFilter"];
        SVFilterBasePtr t_baseFilter = SVParseFilter::parseFilter(mApp, filterObj, resid, t_path.get());
        if (t_baseFilter) {
            t_bundle->addFilter(t_baseFilter);
        }
    }
    
    if (doc.HasMember("SVDeform") && doc["SVDeform"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &deformObj = doc["SVDeform"];
        SVDeformImageMovePtr t_deform = SVParseDeform::parseDeform(mApp, deformObj, resid, t_path.get());
        if (t_deform) {
            t_bundle->addDefrom(t_deform);
        }
    }
    
    if (doc.HasMember("SVMusic") && doc["SVMusic"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &musicObj = doc["SVMusic"];
        SVEffectMusicPtr t_music = SVParseMusic::parseMusic(mApp, musicObj, resid, t_path.c_str());
        if (t_music) {
            t_bundle->setEffectMusic(t_music);
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



















