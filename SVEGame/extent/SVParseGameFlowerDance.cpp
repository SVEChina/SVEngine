//
// SVParseGameFlowerDance.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVParseGameFlowerDance.h"
#include "SVParseGameCommon.h"
#include "../game/SVFlowerDanceMgr.h"
#include "../game/SVFlowerDanceMain.h"
#include "../game/SVFlowerDanceEvent.h"
#include "../game/SVFlowerDanceUnit.h"
#include "../game/SVFlowerDancePlayer.h"
#include "../game/SVFlowerDanceBatUI.h"
#include "../game/SVFlowerDanceHelp.h"
#include "../game/SVGameUIMgr.h"
#include "../particle/SVParticle.h"
#include "../ui/SVUIParamUI.h"
#include "SVParseParticle.h"
#include "../node/SVNode.h"
#include "../node/SVParticleNode.h"

SVParseGameFlowerDance::SVParseGameFlowerDance(SVInst *_app)
:SVParseGameBase(_app)
,m_pHuaFenNode(nullptr){
}

SVParseGameFlowerDance::~SVParseGameFlowerDance() {
    SAFE_DELETE(m_pHuaFenNode);
}

bool SVParseGameFlowerDance::parse(RAPIDJSON_NAMESPACE::Document& doc,cptr8 _filename){
    SVParseGameBase::parse(doc, _filename);
    //游戏规则
    if ( doc.HasMember("gameRule") ) {
        RAPIDJSON_NAMESPACE::Value &gameRule = doc["gameRule"];
        if(gameRule.HasMember("name")){
            m_gameName = gameRule["name"].GetString();
        }
    }
    //资源集合
    if ( doc.HasMember("gameRes") ) {
        RAPIDJSON_NAMESPACE::Value &gameRes = doc["gameRes"];
        if(gameRes.HasMember("flowers")){
            //m_gameName = gameRes["name"].GetString();
        }
        //花粉粒子
        if(gameRes.HasMember("eft_huafen")){
            stringc m_str_huafen = gameRes["eft_huafen"].GetString();
            SVParticleConfig t_particeCfg;
            SVParseParticle::parseParticleConfig(mApp,&t_particeCfg,_filename );
            SVParseParticle t_particlData(mApp);
            t_particlData.particle_config.push_back(t_particeCfg);
            t_particlData.generate(0, _filename );
            //t_particlData.suspend();
            m_pHuaFenNode = t_particlData.m_pParticleNode;
            if(m_pHuaFenNode){
                m_pHuaFenNode->retain();
            }
        }
    }
    
    m_pUIParamUI = new SVUIParamUI(mApp);
    if (doc.HasMember("ui") && doc["ui"].IsArray()){
        RAPIDJSON_NAMESPACE::Value &v = doc["ui"];
        for(s32 i=0;i<v.Size();i++){
            RAPIDJSON_NAMESPACE::Value &GUIItem = v[i];
            UIData* pData = new UIData();
            if (GUIItem.HasMember("uiName") && GUIItem["uiName"].IsString()){
                pData->m_strUIName = GUIItem["uiName"].GetString();
            }
            //
            if (GUIItem.HasMember("uiContent")&& GUIItem["uiContent"].IsArray()){
                RAPIDJSON_NAMESPACE::Value &uiContentsArray = GUIItem["uiContent"];
                for(s32 j=0;j<uiContentsArray.Size();j++){
                    RAPIDJSON_NAMESPACE::Value &uiContent = uiContentsArray[j];
                    SVUIParamBase* pUI = SVParseGameCommon::parseGameUi(mApp , uiContent , m_path.c_str());
                    if (pUI != NULL){
                        pData->m_arrayUI.push_back(pUI);
                    }
                    
                }
            }
            m_pUIParamUI->addUIData(pData);
        }
    }
    return true;
}

SVGameBase* SVParseGameFlowerDance::generate(){
    SVFlowerDanceMain* t_pFlowerDance = new SVFlowerDanceMain(mApp);
    
    SVFlowerDanceMgr* t_DanceMgr = t_pFlowerDance->getFDUnitMgr();
    t_DanceMgr->m_arrAnimate.push_back(m_path + "f1");
    t_DanceMgr->m_arrAnimate.push_back(m_path + "f");
    t_DanceMgr->m_strCircle         = m_path + "circle.png";
    t_DanceMgr->m_strSpineNamePC    = m_path + "pc";
    t_DanceMgr->m_strBgMusic        = m_path + "bg.mp3";

    SVFlowerDancePlayer* t_DancePlayer = t_pFlowerDance->getFDPlayer();
    t_DancePlayer->m_strSpineName = m_path + "bee";
    t_DancePlayer->m_strBgName    = m_path + "bg.png";
    
    SVFlowerDanceBatUI* pBatUi  = t_pFlowerDance->getBatUI();
    pBatUi->setUIName("expGUI");
    
    SVFlowerDanceHelpSys* pHelp = t_pFlowerDance->getHelpSys();
    pHelp->m_strEffect          = m_path + "se.mp3";
    
    if(m_pHuaFenNode){
        t_DancePlayer->setTailNode(m_pHuaFenNode);
    }
    
    t_pFlowerDance->init();
    t_pFlowerDance->getUIMgr()->create(m_pUIParamUI);
    return t_pFlowerDance;
}
