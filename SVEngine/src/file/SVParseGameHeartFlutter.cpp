//
// SVParseGameHeartFlutter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseGameHeartFlutter.h"
#include "../operate/SVOpCreate.h"
#include "../operate/SVOpParse.h"
#include "../module/SVHeartFlutter.h"
#include "../module/SVHeartFlutterReady.h"
#include "../module/SVHeartFlutterRun.h"
#include "../module/SVHeartFlutterEnd.h"
#include "../module/SVModuleSys.h"

//解析面具
void
SVParseGameHeartFlutter::parseGame(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path, SVModuleBasePtr _moduleBase) {
    SVHeartFlutterPtr t_heartFlutter = std::dynamic_pointer_cast<SVHeartFlutter>(_moduleBase);
    //ready
    SVHeartFlutterReadyPtr t_heartFlutterReady = MakeSharedPtr<SVHeartFlutterReady>(_app);
    t_heartFlutterReady->init();
    if (item.HasMember("ready")) {
        
    }
    
    //run
    SVHeartFlutterRunPtr t_heartFlutterRun = MakeSharedPtr<SVHeartFlutterRun>(_app);
    t_heartFlutterRun->init();
    if (item.HasMember("run")) {
        RAPIDJSON_NAMESPACE::Value &runItem = item["run"];
        if (runItem.HasMember("heartflutterunit")) {
            SVHeartUnitMgrPtr t_pHeartUnitMgr = MakeSharedPtr<SVHeartUnitMgr>(_app);
            t_pHeartUnitMgr->init();
            t_heartFlutterRun->setHeartUnitMgr(t_pHeartUnitMgr);
            RAPIDJSON_NAMESPACE::Value &unitItem = runItem["heartflutterunit"];
            RAPIDJSON_NAMESPACE::Document::MemberIterator iter = unitItem.MemberBegin();
            for(; iter != unitItem.MemberEnd(); ++iter){
                RAPIDJSON_NAMESPACE::Value &spineItem = iter->value;
                SVString t_name_json;
                if (spineItem.HasMember("fjson") && spineItem["fjson"].IsString()) {
                    t_name_json = spineItem["fjson"].GetString();
                }
                SVString t_name_altas;
                if (spineItem.HasMember("fatlas") && spineItem["fatlas"].IsString()) {
                    t_name_altas = spineItem["fatlas"].GetString();
                }
                SVString t_root_path = _path;
                SVString t_spine_json = t_root_path + t_name_json;
                SVString t_spine_atlas = t_root_path + t_name_altas;
                t_pHeartUnitMgr->addFileRes(t_spine_json.c_str(), t_spine_atlas.c_str());
            }
            
        }
    }
    
    //end
    SVHeartFlutterEndPtr t_heartFlutterEnd = MakeSharedPtr<SVHeartFlutterEnd>(_app);
    t_heartFlutterEnd->init();
    if (item.HasMember("end")) {
        RAPIDJSON_NAMESPACE::Value &endItem = item["end"];
        
    }
    t_heartFlutter->init(t_heartFlutterReady, t_heartFlutterRun, t_heartFlutterEnd);

}

