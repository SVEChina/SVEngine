//
// Created by lidm on 17/9/5.
//

#include "SVParseGameFaceDance.h"
#include "SVParseAnimate.h"
#include "SVParseGameCommon.h"
#include "../node/SVSpineNode.h"
#include "../node/SVGameWrapNode.h"
#include "../logicsys/SVLogicMgr.h"
#include "../work/SVThreadPool.h"
#include "../operate/SVOpParse.h"
//游戏系统
#include "../ui/SVUIParamUI.h"
#include "../game/SVFaceDanceMain.h"
#include "../game/SVFaceDanceHelpSys.h"
#include "../game/SVFaceDanceBatUI.h"
#include "../game/SVFaceDanceResultUI.h"
#include "../game/SVFaceDanceUnit.h"
#include "../game/SVFaceDanceUnitMgr.h"
#include "../game/SVFaceDanceEvent.h"
#include "../game/SVFaceDanceResConfig.h"
#include "../game/SVFaceDanceAudioSys.h"
#include "../game/SVGameCommonPreBatUI.h"
#include "../game/SVGameUIMgr.h"

SVParseGameFaceDance::SVParseGameFaceDance(SVInst *_app)
:SVParseDataEvent(_app)
,m_pUIParamUI(NULL) {
}

SVParseGameFaceDance::~SVParseGameFaceDance() {
}

void SVParseGameFaceDance::parseGameFaceDanceStatic(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid,cptr8 _path) {
    SVParseGameFaceDance *t_parseFaceGame = new SVParseGameFaceDance(_app);
    SVParse::parseBase(item, t_parseFaceGame);
    //t_parseFaceGame->resid = _resid;
    t_parseFaceGame->id = t_parseFaceGame->name.c_str();
    //t_parseFaceGame->m_SceneName = nullptr;
    //游戏规则
    if ( item.HasMember("gameRule") ) {
        RAPIDJSON_NAMESPACE::Value &gameRule = item["gameRule"];
        if(gameRule.HasMember("name")){
            t_parseFaceGame->m_gameName = gameRule["name"].GetString();
        }
        //教学模式
        t_parseFaceGame->m_teachmode = false;
        if(gameRule.HasMember("teachmode")){
            t_parseFaceGame->m_teachmode = true;
        }
        if(gameRule.HasMember("spinename")){
            t_parseFaceGame->m_SpineName = stringc(_path) + gameRule["spinename"].GetString();
        }
        if(gameRule.HasMember("animationdead")){
            t_parseFaceGame->m_deadAni = gameRule["animationdead"].GetString();
        }
        if(gameRule.HasMember("fontscore")){
            t_parseFaceGame->m_scorepng = stringc(_path) + gameRule["fontscore"].GetString();
        }
        if(gameRule.HasMember("speedmin")){
            t_parseFaceGame->m_minSpeed = gameRule["speedmin"].GetFloat();
        }
        if(gameRule.HasMember("speedmax")){
            t_parseFaceGame->m_maxSpeed = gameRule["speedmax"].GetFloat();
        }
        if(gameRule.HasMember("score")){
            t_parseFaceGame->m_score = gameRule["score"].GetInt();
        }
        if(gameRule.HasMember("animationset") && gameRule["animationset"].IsArray() ){
            RAPIDJSON_NAMESPACE::Value &aniSet = gameRule["animationset"];
            for(int i = 0;i<aniSet.Size();i++){
                stringc t_ani = aniSet[i].GetString();
                t_parseFaceGame->m_aniNamePool.push_back(t_ani);
            }
        }
        if(gameRule.HasMember("gametime") && gameRule["gametime"].IsInt() ){
            t_parseFaceGame->m_gametime = gameRule["gametime"].GetInt();
        }
        if(gameRule.HasMember("enery") ){
            t_parseFaceGame->m_enery = gameRule["enery"].GetFloat();
        }
        if(gameRule.HasMember("genrate") ){
            t_parseFaceGame->m_genrate = gameRule["genrate"].GetFloat();
        }
    }
    
    if (item.HasMember("audio")){
        RAPIDJSON_NAMESPACE::Value &gameAudio = item["audio"];
        if (gameAudio.HasMember("gameBgm")){
            t_parseFaceGame->m_bgMusic = _path;
            t_parseFaceGame->m_bgMusic.append(gameAudio["gameBgm"].GetString());
        }
        
        if (gameAudio.HasMember("deadEffect")){
            t_parseFaceGame->m_deadEffect = _path;
            t_parseFaceGame->m_deadEffect.append(gameAudio["deadEffect"].GetString());
        }
        
        if (gameAudio.HasMember("endEffect")){
            t_parseFaceGame->m_endEffect = _path;
            t_parseFaceGame->m_endEffect.append(gameAudio["endEffect"].GetString());
        }
        
        if (gameAudio.HasMember("djsMusic")){
            t_parseFaceGame->m_djsEffect = _path;
            t_parseFaceGame->m_djsEffect.append(gameAudio["djsMusic"].GetString());
        }
    }
    
    t_parseFaceGame->m_pUIParamUI = new SVUIParamUI(_app);
    if (item.HasMember("ui") && item["ui"].IsArray()){
        RAPIDJSON_NAMESPACE::Value &v = item["ui"];
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
                    SVUIParamBase* pUI = SVParseGameCommon::parseGameUi(_app , uiContent , _path);
                    if (pUI != NULL){
                        pData->m_arrayUI.push_back(pUI);
                    }
                    
                }
            }
            t_parseFaceGame->m_pUIParamUI->addUIData(pData);
        }
    }
    
    SVOpGenerate *t_op = new SVOpGenerate(_app, t_parseFaceGame,_resid,_path);
    //_app->m_pTPool->getLogicThread()->pushThreadOp(t_op);
    t_op->release();
    t_parseFaceGame->release();
    
}

void SVParseGameFaceDance::generate(s32 _resid,cptr8 _path) {
    SVParseData::generate(_resid, _path);
    //背景音乐
    //死亡音乐
    //生成游戏
    SVFaceDanceMain* t_pGameFaceDance = new SVFaceDanceMain(mApp);
    t_pGameFaceDance->getUIMgr()->create(m_pUIParamUI);
    //
    SVFaceDanceHelpSys* t_FDHelpSys = t_pGameFaceDance->getFDHelpSys();
    //t_FDHelpSys->m_scenename = m_SceneName;
    t_FDHelpSys->m_gametime = m_gametime;
    //Face产生规则
    SVFaceDanceUnitMgr* t_FDUnitMgr = t_pGameFaceDance->getFDUnitMgr();
    t_FDUnitMgr->m_scenename = m_SceneName;
    t_FDUnitMgr->m_numPerSec = m_genrate;   //每秒产生的个数
    t_FDUnitMgr->m_maxNum = 30;     //最大同时支持的笑脸数
    t_FDUnitMgr->m_maxspeed = m_maxSpeed;
    t_FDUnitMgr->m_minspeed = m_minSpeed;
    t_FDUnitMgr->m_anidead = m_deadAni;     //死亡动画名
    t_FDUnitMgr->m_spinename = m_SpineName;
    t_FDUnitMgr->m_scorepng = m_scorepng;
    t_FDUnitMgr->m_enery   = m_enery;
    
    if(m_teachmode){
        t_FDUnitMgr->enalbeTeachMode();
    }
    
    SVFaceDanceAudioSys* t_FDAudioSys = t_pGameFaceDance->getFDAudioSys();
    t_FDAudioSys->m_strGameBgm = m_bgMusic;
    t_FDAudioSys->m_strDeadEffect = m_deadEffect;
    
    for(u32 i=0;i<m_aniNamePool.size();i++ ){
        t_FDUnitMgr->m_faceTypePool.push_back(m_aniNamePool[i]);
    }
    //战斗UI
    SVFaceDanceBatUI* t_FDBatUI = t_pGameFaceDance->getFDBatUI();
    t_FDBatUI->setUIName("expGUI");
    
    //结算UI
    SVFaceDanceResultUI* t_FDResultUI = t_pGameFaceDance->getFDResultUI();
    t_FDResultUI->setUIName("resultGUI");
    
    //战前准备
    SVGameCommonPreBatUI* t_FDPreBatUI = t_pGameFaceDance->getFDPreBatUI();
    t_FDPreBatUI->setSound(m_djsEffect.c_str());
    t_FDPreBatUI->setUIName("preBatGUI");
    
    mApp->m_pGlobalMgr->m_pLogicMgr->setGame(t_pGameFaceDance);
    t_pGameFaceDance->init();
    SVGameWrapNode* tNode = new SVGameWrapNode(mApp);
    if (tNode) {
        tNode->setresUID(_resid);
        _addNode(tNode);
    }
    
    t_pGameFaceDance->release();
    
}

