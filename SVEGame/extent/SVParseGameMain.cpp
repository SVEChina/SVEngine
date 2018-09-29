//
// SVParseGameMain.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVParseGameMain.h"
#include "SVParseGamePiple.h"
#include "SVParseGameQClick.h"
#include "SVParseGameTripleDown.h"
#include "SVParseGameFaceDance.h"
#include "SVParseGameFlowerDance.h"
#include "../file/SVFileMgr.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../game/SVPipleMain.h"
#include "../game/SVPipleGridMgr.h"
#include "../game/SVPipleBatUI.h"

using namespace sv;

SVParseGameMain::SVParseGameMain(SVInst *_app)
:SVGBase(_app)
,m_pParseGame(nullptr){
}

SVParseGameMain::~SVParseGameMain() {
    SAFE_DELETE(m_pParseGame);
}

bool SVParseGameMain::parse(cptr8 _filename) {
    m_path = stringc(_filename);
    stringc t_config = m_path + "config.json";
    DataChunk tDataStream;
    bool tflag = mApp->m_pGlobalMgr->m_pFileMgr->loadFileContentStr(&tDataStream, t_config.c_str());
    if (!tflag)
        return false;
    if (!tDataStream.m_data) {
        SV_LOG_ERROR("data stream is null");
        return false;
    }
    RAPIDJSON_NAMESPACE::Document doc;
    doc.Parse(tDataStream.m_data);
    if (doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = doc.GetParseError();
        SV_LOG_ERROR("rapidjson error code:%d \n", code);
        return false;
    }
    if (doc.HasMember("gametype")) {
        RAPIDJSON_NAMESPACE::Value &t_gametype = doc["gametype"];
        m_gametype = t_gametype.GetString();
        if( m_gametype == "quick_click"){
            m_pParseGame = new SVParseGameQClick(mApp);
            m_pParseGame->parse(doc, _filename);
        }else if( m_gametype == "piple"){
            m_pParseGame = new SVParseGamePiple(mApp);
            m_pParseGame->parse(doc, _filename);
        }else if( m_gametype == "face_dance"){

            
        }else if( m_gametype == "face_fly"){
            
        }else if( m_gametype == "tripledown"){
            m_pParseGame = new SVParseGameTripleDown(mApp);
            m_pParseGame->parse(doc, _filename);
        }else if ( m_gametype == "bee3"){
            m_pParseGame = new SVParseGameFlowerDance(mApp);
            m_pParseGame->parse(doc, _filename);
        }
    }
    return true;
}

SVGameBase* SVParseGameMain::generate() {
    if(m_pParseGame){
        SVGameBase* t_game = m_pParseGame->generate();
        SAFE_DELETE(m_pParseGame);
        return t_game;
    }
    return nullptr;
}
