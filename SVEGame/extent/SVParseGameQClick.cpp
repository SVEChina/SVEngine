//
// SVParseGameQClick.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVParseGameQClick.h"
#include "../file/SVFileMgr.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../game/SVQClickMain.h"
#include "../game/SVQClickUnitMgr.h"
#include "../game/SVQClickBatUI.h"
#include "../third/rapidjson/document.h"

using namespace sv;

SVParseGameQClick::SVParseGameQClick(SVInst *_app)
: SVParseGameBase(_app) {
    m_timelen = 0.0f;
}

SVParseGameQClick::~SVParseGameQClick() {
}

bool SVParseGameQClick::parse(RAPIDJSON_NAMESPACE::Document& doc,cptr8 _filename) {
    SVParseGameBase::parse(doc, _filename);
    if (doc.HasMember("timelen")) {
        RAPIDJSON_NAMESPACE::Value &t_timelen = doc["timelen"];
        m_timelen = t_timelen.GetFloat();
    }
    if (doc.HasMember("ani_press")) {
        RAPIDJSON_NAMESPACE::Value &t_ain_press = doc["ani_press"];
        m_ani_press = m_path + t_ain_press.GetString();
    }
    if (doc.HasMember("ani_end")) {
        RAPIDJSON_NAMESPACE::Value &t_ani_end = doc["ani_end"];
        m_ani_end = m_path + t_ani_end.GetString();
    }
    if (doc.HasMember("bg")) {
        RAPIDJSON_NAMESPACE::Value &t_bgs = doc["bg"];
        if(t_bgs.IsArray()){
            for(s32 i=0;i<t_bgs.Size();i++){
                RAPIDJSON_NAMESPACE::Value &t_bg = t_bgs[i];
                m_bg.push_back( t_bg.GetString() );
            }
        }
    }
    return true;
}

SVGameBase* SVParseGameQClick::generate() {
    SVQClickMain* t_game = new SVQClickMain( mApp,m_path.c_str() );
    t_game->init();
    //
    t_game->setTotalTime(m_timelen);
    //
    SVQClickUnitMgr* t_unitmgr = t_game->getFDUnitMgr();
    if(t_unitmgr){
        t_unitmgr->setSpineName(m_ani_press.c_str());
    }
    //
    SVQClickBatUI* t_batui = t_game->getFDBatUI();
    if(t_batui){
        t_batui->setResultAni(m_ani_end.c_str());
    }
    return t_game;
}
