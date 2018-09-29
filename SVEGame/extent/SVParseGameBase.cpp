//
// SVParseGameBase.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVParseGameBase.h"
#include "../file/SVFileMgr.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../game/SVQClickMain.h"
#include "../game/SVQClickUnitMgr.h"
#include "../game/SVQClickBatUI.h"

using namespace sv;

SVParseGameBase::SVParseGameBase(SVInst *_app)
: SVGBase(_app) {
}

SVParseGameBase::~SVParseGameBase() {
}

bool SVParseGameBase::parse(RAPIDJSON_NAMESPACE::Document& doc,cptr8 _filename) {
    m_path = stringc(_filename);
    if (doc.HasMember("version")) {
        RAPIDJSON_NAMESPACE::Value &t_version = doc["version"];
        m_version = t_version.GetString();
    }
    return true;
}

SVGameBase* SVParseGameBase::generate() {
    return nullptr;
}
