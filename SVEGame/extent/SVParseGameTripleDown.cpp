//
// SVParseGameTripleDown.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVParseGameTripleDown.h"
#include "SVParseGameCommon.h"
#include "../game/SVTripleDownMain.h"
#include "../game/SVTripleDownUnit.h"
#include "../game/SVTripleDownMgr.h"
#include "../game/SVTripleDownHelpSys.h"
#include "../game/SVTripleDownPlaySys.h"
#include "../game/SVTripleDownBatUI.h"
#include "../game/SVGameCommonPreBatUI.h"
#include "../game/SVGameCommonResultUI.h"
#include "../game/SVGameUIMgr.h"
#include "../ui/SVUIParamUI.h"
#include "../logicsys/SVLogicMgr.h"
#include "../app/SVInst.h"

SVParseGameTripleDown::SVParseGameTripleDown(SVInst *_app)
:SVParseGameBase(_app),m_pUIParamUI(NULL) ,m_gametime(0) , m_donutWidth(0) , m_donutHeight(0){
}

SVParseGameTripleDown::~SVParseGameTripleDown() {
}

bool SVParseGameTripleDown::parse(RAPIDJSON_NAMESPACE::Document& doc,cptr8 _filename){
    SVParseGameBase::parse(doc, _filename);
    
    //游戏规则
    if ( doc.HasMember("gameRule") ) {
        RAPIDJSON_NAMESPACE::Value &gameRule = doc["gameRule"];
        if(gameRule.HasMember("name")){
            m_gameName = gameRule["name"].GetString();
        }
        if(gameRule.HasMember("gametime") && gameRule["gametime"].IsInt() ){
            m_gametime = gameRule["gametime"].GetInt();
        }
        if(gameRule.HasMember("donutWidth") && gameRule["donutWidth"].IsInt() ){
            m_donutSize.X = gameRule["donutWidth"].GetInt();
        }
        if(gameRule.HasMember("donutHeight") && gameRule["donutHeight"].IsInt() ){
            m_donutSize.Y = gameRule["donutHeight"].GetInt();
        }
        if(gameRule.HasMember("donutLeftName") && gameRule["donutLeftName"].IsString() ){
            m_donutLeftName = gameRule["donutLeftName"].GetString();
        }
        if(gameRule.HasMember("donutMidName") && gameRule["donutMidName"].IsString() ){
            m_donutMidName = gameRule["donutMidName"].GetString();
        }
        if(gameRule.HasMember("donutRightName") && gameRule["donutRightName"].IsString() ){
            m_donutRightName = gameRule["donutRightName"].GetString();
        }
        if(gameRule.HasMember("harpoonLeftName") && gameRule["harpoonLeftName"].IsString() ){
            m_harpoonLeftName = gameRule["harpoonLeftName"].GetString();
        }
        if(gameRule.HasMember("harpoonMidName") && gameRule["harpoonMidName"].IsString() ){
            m_harpoonMidName = gameRule["harpoonMidName"].GetString();
        }
        if(gameRule.HasMember("harpoonRightName") && gameRule["harpoonRightName"].IsString() ){
            m_harpoonRightName = gameRule["harpoonRightName"].GetString();
        }
        if(gameRule.HasMember("harpoonWidth") && gameRule["harpoonWidth"].IsInt() ){
            m_harpoonSize.X = gameRule["harpoonWidth"].GetInt();
        }
        if(gameRule.HasMember("harpoonHeight") && gameRule["harpoonHeight"].IsInt() ){
            m_harpoonSize.Y = gameRule["harpoonHeight"].GetInt();
        }
        if(gameRule.HasMember("harpoonLeftPosx") && gameRule["harpoonLeftPosx"].IsInt() ){
            m_harpoonLeftPos.X = gameRule["harpoonLeftPosx"].GetInt();
        }
        if(gameRule.HasMember("harpoonLeftPosy") && gameRule["harpoonLeftPosy"].IsInt() ){
            m_harpoonLeftPos.Y = gameRule["harpoonLeftPosy"].GetInt();
        }
        if(gameRule.HasMember("harpoonMidPosx") && gameRule["harpoonMidPosx"].IsInt() ){
            m_harpoonMidPos.X = gameRule["harpoonMidPosx"].GetInt();
        }
        if(gameRule.HasMember("harpoonMidPosy") && gameRule["harpoonMidPosy"].IsInt() ){
            m_harpoonMidPos.Y = gameRule["harpoonMidPosy"].GetInt();
        }
        if(gameRule.HasMember("harpoonRightPosx") && gameRule["harpoonRightPosx"].IsInt() ){
            m_harpoonRightPos.X = gameRule["harpoonRightPosx"].GetInt();
        }
        if(gameRule.HasMember("harpoonRightPosy") && gameRule["harpoonRightPosy"].IsInt() ){
            m_harpoonRightPos.Y = gameRule["harpoonRightPosy"].GetInt();
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

SVGameBase* SVParseGameTripleDown::generate(){
    SVTripleDownMain* t_pGameTripleDown = new SVTripleDownMain(mApp);
    
    //战前准备
    SVGameCommonPreBatUI* t_FDPreBatUI = t_pGameTripleDown->getFDPreBatUI();
    stringc strDjsSound = m_path + "daojishi.mp3";
    t_FDPreBatUI->setSound(strDjsSound.c_str());
    t_FDPreBatUI->setUIName("preBatGUI");
    
    SVGameCommonResultUI* pResultUI = t_pGameTripleDown->getResultUI();
    pResultUI->setUIName("resultGUI");
    
    SVTripleDownHelpSys* pHelpSys   = t_pGameTripleDown->getHelpSys();
    pHelpSys->m_gametime            = m_gametime;
    
    SVTripleDownMgr* pTripleMgr     = t_pGameTripleDown->getUnitMgr();
    pTripleMgr->m_scenename         = m_SceneName;
    pTripleMgr->m_donutWidth        = m_donutSize.X;
    pTripleMgr->m_donutHeight       = m_donutSize.Y;
    pTripleMgr->m_arrAnimate.push_back(m_path + m_donutLeftName);
    pTripleMgr->m_arrAnimate.push_back(m_path + m_donutMidName);
    pTripleMgr->m_arrAnimate.push_back(m_path + m_donutRightName);
    
    SVTripleDownPlaySys* pTriplePlay    = t_pGameTripleDown->getPlaySys();
    pTriplePlay->m_strSceneName         = m_SceneName;
    pTriplePlay->m_strHarpoonAniLeft    = m_path + m_harpoonLeftName;
    pTriplePlay->m_strHarpoonAniMiddle  = m_path + m_harpoonMidName;
    pTriplePlay->m_strHarpoonAniRight   = m_path + m_harpoonRightName;
    pTriplePlay->m_strBgMusic           = m_path + "quanquanBgm.mp3";
    pTriplePlay->m_strXiaoChu           = m_path + "baopo.mp3";
    pTriplePlay->m_posLeft              = vector3df(m_harpoonLeftPos.X,m_harpoonLeftPos.Y,0);
    pTriplePlay->m_posMiddle            = vector3df(m_harpoonMidPos.X,m_harpoonMidPos.Y,0);
    pTriplePlay->m_posRight             = vector3df(m_harpoonRightPos.X,m_harpoonRightPos.Y,0);
    pTriplePlay->m_width                = m_harpoonSize.X;
    pTriplePlay->m_height               = m_harpoonSize.Y;
    
    SVTripleDownBatUI* pBatUI           = t_pGameTripleDown->getBatUI();
    pBatUI->setUIName("expGUI");
    
    t_pGameTripleDown->init();
    t_pGameTripleDown->getUIMgr()->create(m_pUIParamUI);
 
    return t_pGameTripleDown;
}

