//
// SVParseGamePiple.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVParseGamePiple.h"
#include "../file/SVFileMgr.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../game/SVPipleMain.h"
#include "../game/SVPipleGridMgr.h"
#include "../game/SVPipleBatUI.h"
#include "../third/rapidjson/document.h"

using namespace sv;

SVParseGamePiple::SVParseGamePiple(SVInst *_app)
: SVParseGameBase(_app) {
    m_timelen = 0.0f;
}

SVParseGamePiple::~SVParseGamePiple() {
}

bool SVParseGamePiple::parse(RAPIDJSON_NAMESPACE::Document& doc,cptr8 _filename) {
    SVParseGameBase::parse(doc, _filename);
//    "gameRule": {
//        "size_x":9,
//        "size_y":9,
//        "grid_size":80.0,
//        "grid_boder":3
//    },
//    "bg_img": "piple_bg.png",
//    "line": "A",
//    "wave": "B"
    if (doc.HasMember("timelen")) {
        RAPIDJSON_NAMESPACE::Value &t_timelen = doc["timelen"];
        m_timelen = t_timelen.GetFloat();
    }
    if (doc.HasMember("bg_img")) {
        RAPIDJSON_NAMESPACE::Value &t_ain_press = doc["bg_img"];
        m_sp_bg = m_path + t_ain_press.GetString();
    }
    if (doc.HasMember("line")) {
        RAPIDJSON_NAMESPACE::Value &t_ani_line = doc["line"];
        m_ani_line = m_path + t_ani_line.GetString();
    }
    if (doc.HasMember("wave")) {
        RAPIDJSON_NAMESPACE::Value &t_ani_wave = doc["wave"];
        m_ani_wave = m_path + t_ani_wave.GetString();
    }
    if (doc.HasMember("gameRule")) {
        RAPIDJSON_NAMESPACE::Value &t_gameRule = doc["gameRule"];
        if( t_gameRule.IsObject() ){
            if (t_gameRule.HasMember("size_x")) {
                RAPIDJSON_NAMESPACE::Value &t_size_x = t_gameRule["size_x"];
                m_piple_size_x = t_size_x.GetInt();
            }
            if (t_gameRule.HasMember("size_y")) {
                RAPIDJSON_NAMESPACE::Value &t_size_y = t_gameRule["size_y"];
                m_piple_size_y = t_size_y.GetInt();
            }
            if (t_gameRule.HasMember("grid_size")) {
                RAPIDJSON_NAMESPACE::Value &t_grid_size = t_gameRule["grid_size"];
                m_grid_size = t_grid_size.GetFloat();
            }
            if (t_gameRule.HasMember("grid_boder")) {
                RAPIDJSON_NAMESPACE::Value &t_grid_boder = t_gameRule["grid_boder"];
                m_grid_boder = t_grid_boder.GetFloat();
            }
        }
    }
    return true;
}

SVGameBase* SVParseGamePiple::generate() {
    SVPipleMain* t_game = new SVPipleMain( mApp,m_path.c_str() );
    t_game->init();
    //
    t_game->setTotalTime(m_timelen);
    //
    SVPipleGridMgr* t_piplemgr = t_game->getPipleMgr();
    if(t_piplemgr){
        t_piplemgr->setSpineRes(m_ani_wave.c_str(), m_ani_line.c_str());
        t_piplemgr->genNet(m_piple_size_x, m_piple_size_y);
    }
    //
    SVPipleBatUI* t_batui = t_game->getPipleBatUI();
    if(t_batui){
        t_batui->setPipleBG(m_sp_bg.c_str());
    }
    //
    //t_game->enter();
    
    return t_game;
}
