//
// SVParseGame.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseGame.h"
#include "../operate/SVOpCreate.h"
#include "../operate/SVOpParse.h"
#include "../node/SVSpriteNode.h"
#include "../node/SVBitFontNode.h"
#include "../module/SVRedPacket.h"
#include "../module/SVRedPacketRun.h"
#include "../module/SVRedPacketReady.h"
#include "../module/SVRedPacketEnd.h"
#include "../module/SVModuleSys.h"
#include "SVParseSprite.h"
#include "SVParseBitfont.h"
//解析面具
void
SVParseGame::parseGame(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path, SVModuleBasePtr _moduleBase) {
    //ready
    SVRedPacketReadyPtr t_packetReady = MakeSharedPtr<SVRedPacketReady>(_app);
    t_packetReady->init();
    if (item.HasMember("ready")) {
        RAPIDJSON_NAMESPACE::Value &readyItem = item["ready"];
        s32 i = 0;
        for(auto iter = readyItem.MemberBegin(); iter != readyItem.MemberEnd(); ++iter){
            if(i == 0){
                cptr8 key = (iter->name).GetString();
                if (readyItem.HasMember(key)) {
                    RAPIDJSON_NAMESPACE::Value &spineItem = iter->value;
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_packetReady->generateCountDownUnit(t_spineNode);
                }
            }else if (i == 1){
                cptr8 key = (iter->name).GetString();
                if (readyItem.HasMember(key)) {
                    RAPIDJSON_NAMESPACE::Value &spineItem = iter->value;
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_packetReady->generateReadyUnit(t_spineNode);
                }
            }else if (i == 2){
                cptr8 key = (iter->name).GetString();
                if (readyItem.HasMember(key)) {
                    RAPIDJSON_NAMESPACE::Value &spriteItem = iter->value;
                    SVNodePtr t_node = SVParseSprite::parseSprite(_app, spriteItem, _resid, _path);;
                    SVSpriteNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpriteNode, t_node);
                    t_packetReady->generateBackUnit(t_spineNode);
                }
            }
            i++;
        }
    }
    
    //run
    SVRedPacketRunPtr t_packetRun = MakeSharedPtr<SVRedPacketRun>(_app);
    t_packetRun->init();
    if (item.HasMember("run")) {
        RAPIDJSON_NAMESPACE::Value &runItem = item["run"];
        if (runItem.HasMember("packetunit")) {
            SVRedPacketUnitMgrPtr t_pRedPacketMgr = MakeSharedPtr<SVRedPacketUnitMgr>(_app);
            t_pRedPacketMgr->init();
            t_packetRun->setPacketUnitMgr(t_pRedPacketMgr);
            RAPIDJSON_NAMESPACE::Value &unitItem = runItem["packetunit"];
            //
            auto iter = unitItem.MemberBegin();
            cptr8 key = (iter->name).GetString();
            if (unitItem.HasMember(key)) {
                RAPIDJSON_NAMESPACE::Value &spineItem = iter->value;
                for (s32 i = 0; i<t_pRedPacketMgr->getUnitType1Num(); i++) {
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_pRedPacketMgr->generatePacketUnit(t_spineNode, SVRedPacketUnit::PACKET_TYPE_1);
                }
                for (s32 i = 0; i<t_pRedPacketMgr->getUnitType2Num(); i++) {
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_pRedPacketMgr->generatePacketUnit(t_spineNode, SVRedPacketUnit::PACKET_TYPE_2);
                }
                for (s32 i = 0; i<t_pRedPacketMgr->getUnitType3Num(); i++) {
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_pRedPacketMgr->generatePacketUnit(t_spineNode,SVRedPacketUnit::PACKET_TYPE_3);
                }
                for (s32 i = 0; i<t_pRedPacketMgr->getUnitType3Num(); i++) {
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_pRedPacketMgr->generatePacketUnit(t_spineNode,SVRedPacketUnit::PACKET_TYPE_3);
                }
                for (s32 i = 0; i<t_pRedPacketMgr->getUnitType4Num(); i++) {
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_pRedPacketMgr->generatePacketUnit(t_spineNode,SVRedPacketUnit::PACKET_TYPE_4);
                }
                for (s32 i = 0; i<t_pRedPacketMgr->getUnitType5Num(); i++) {
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_pRedPacketMgr->generatePacketUnit(t_spineNode,SVRedPacketUnit::PACKET_TYPE_5);
                }
                for (s32 i = 0; i<t_pRedPacketMgr->getUnitType6Num(); i++) {
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_pRedPacketMgr->generatePacketUnit(t_spineNode,SVRedPacketUnit::PACKET_TYPE_6);
                }
                for (s32 i = 0; i<t_pRedPacketMgr->getUnitType7Num(); i++) {
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_pRedPacketMgr->generatePacketUnit(t_spineNode,SVRedPacketUnit::PACKET_TYPE_7);
                }
                for (s32 i = 0; i<t_pRedPacketMgr->getUnitType8Num(); i++) {
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_pRedPacketMgr->generatePacketUnit(t_spineNode,SVRedPacketUnit::PACKET_TYPE_8);
                }
            }
            //
            iter++;
            key = (iter->name).GetString();
            RAPIDJSON_NAMESPACE::Value &t_goldSpineItem = iter->value;
            //
            iter++;
            key = (iter->name).GetString();
            RAPIDJSON_NAMESPACE::Value &t_bitfontItem = iter->value;
            for (s32 j = 0; j<t_pRedPacketMgr->getAllFreePacketUnit().size(); j++) {
                SVNodePtr t_node = SVParseAnimate::parseAnim(_app, t_goldSpineItem, _resid, _path);
                SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                SVNodePtr t_nodePtr = SVParseBitfont::parseBitfont(_app, t_bitfontItem, _resid, _path);
                SVBitFontNodePtr t_bitfont_node = DYN_TO_SHAREPTR(SVBitFontNode, t_nodePtr);
                SVRedPacketUnitPtr t_packetUnit = t_pRedPacketMgr->getAllFreePacketUnit()[j];
                t_packetUnit->generateGoldUnit(t_spineNode, t_bitfont_node);
            }
            //
            if (runItem.HasMember("touchunit")) {
                SVTouchAniUnitMgrPtr t_touchAniMgr = MakeSharedPtr<SVTouchAniUnitMgr>(_app);
                t_touchAniMgr->init();
                t_packetRun->setTouchAniUnitMgr(t_touchAniMgr);
                RAPIDJSON_NAMESPACE::Value &unitItem = runItem["touchunit"];
                if (unitItem.HasMember("sv_spine")) {
                    RAPIDJSON_NAMESPACE::Value &spineItem = unitItem["sv_spine"];
                    SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    t_touchAniMgr->generateUnit(t_spineNode);
                }
            }
            
        }
    }
    
    //end
    SVRedPacketEndPtr t_packetEnd = MakeSharedPtr<SVRedPacketEnd>(_app);
    t_packetEnd->init();
    if (item.HasMember("end")) {
        RAPIDJSON_NAMESPACE::Value &endItem = item["end"];
        if (endItem.HasMember("sv_spine")) {
            RAPIDJSON_NAMESPACE::Value &spineItem = endItem["sv_spine"];
            SVNodePtr t_node = SVParseAnimate::parseAnim(_app, spineItem, _resid, _path);
            SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
            t_packetEnd->setSpineNode(t_spineNode);
        }
        if (endItem.HasMember("sv_bitfont")) {
            RAPIDJSON_NAMESPACE::Value &bitfontItem = endItem["sv_bitfont"];
            SVNodePtr t_node = SVParseBitfont::parseBitfont(_app, bitfontItem, _resid, _path);
            SVBitFontNodePtr t_bitfontNode = DYN_TO_SHAREPTR(SVBitFontNode, t_node);
            t_packetEnd->setBitFontNode(t_bitfontNode);
        }
    }
    //其他参数
    if (item.HasMember("time") && item["time"].IsFloat()) {
        t_packetRun->setRunningTime(item["time"].GetFloat());
    }
    if (item.HasMember("minspeed") && item["minspeed"].IsInt()) {
        t_packetRun->getPacketUnitMgr()->setMinSpeed(item["minspeed"].GetInt());
    }
    if (item.HasMember("maxspeed") && item["maxspeed"].IsInt()) {
        t_packetRun->getPacketUnitMgr()->setMaxSpeed(item["maxspeed"].GetInt());
    }
    //
    SVRedPacketPtr t_redPacket = std::dynamic_pointer_cast<SVRedPacket>(_moduleBase);
    t_redPacket->init(t_packetReady, t_packetRun, t_packetEnd);
    
    

}

