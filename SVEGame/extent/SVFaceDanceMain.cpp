//
// Created by lidm on 17/9/5.
//

#include "SVFaceDanceMain.h"
#include "SVFaceDanceUnitMgr.h"
#include "SVFaceDanceHelpSys.h"
#include "SVFaceDanceBatUI.h"
#include "SVFaceDanceResultUI.h"
#include "SVFaceDanceEvent.h"
#include "SVFaceDanceResConfig.h"
#include "SVFaceDanceAudioSys.h"
#include "SVGameCommonEvent.h"
#include "SVGameCommonPreBatUI.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"

SVFaceDanceMain::SVFaceDanceMain(SVInst*_app)
:SVGameBase(_app) {
    m_state = 0;
    m_pResConfig = new SVFaceDanceResConfig(mApp);
    m_pUnitMgr = new SVFaceDanceUnitMgr(mApp);
    m_pHelpSys = new SVFaceDanceHelpSys(mApp);
    m_pBatUI = new SVFaceDanceBatUI(mApp);
    m_pResultUI = new SVFaceDanceResultUI(mApp);
    m_pAudioSys = new SVFaceDanceAudioSys(mApp);
    m_pPreBatUI = new SVGameCommonPreBatUI(mApp);
}

SVFaceDanceMain::~SVFaceDanceMain() {
    SAFE_DELETE(m_pUnitMgr);
    SAFE_DELETE(m_pResConfig);
    SAFE_DELETE(m_pHelpSys);
    SAFE_DELETE(m_pBatUI);
    SAFE_DELETE(m_pResultUI);
    SAFE_DELETE(m_pAudioSys);
    SAFE_DELETE(m_pPreBatUI);
}

void SVFaceDanceMain::init(){
    m_pResConfig->init();
    m_pUnitMgr->init();
    m_pHelpSys->init();
    m_pBatUI->init(getUIMgr());
    m_pResultUI->init(getUIMgr());
    m_pAudioSys->init();
    m_pPreBatUI->init(getUIMgr());
    m_state = -1;
    //
    enterex();
}

void SVFaceDanceMain::destroy(){
    //
    exitex();
    //
    m_pUnitMgr->destroy();
    m_pResConfig->destroy();
    m_pHelpSys->destroy();
    m_pBatUI->destroy();
    m_pResultUI->destroy();
    m_pAudioSys->destroy();
    m_pPreBatUI->destroy();
}

void SVFaceDanceMain::update(f32 _dt){
    if (m_state == 0){
        m_pPreBatUI->update(_dt);
    }else if(m_state == 1){
        m_pHelpSys->update(_dt);
        m_pUnitMgr->update(_dt,m_pHelpSys->getGameTime());
        m_pBatUI->update(_dt);
        m_pAudioSys->update(_dt);
    }else if(m_state == 3){
        //更新战斗UI
        m_pResultUI->update(_dt);
    }
}

bool SVFaceDanceMain::procEvent(SVEvent *_event){
    SVEventGameStart* t_event_game_start = dynamic_cast<SVEventGameStart*>(_event);
    if(t_event_game_start){
        m_state = 0;
        if (m_pPreBatUI){
            m_pPreBatUI->enter();
        }
        
    }
    
    SVEventGameEnterBattle* t_event_enter_battle = dynamic_cast<SVEventGameEnterBattle*>(_event);
    if (t_event_enter_battle){
        m_state = 1;    //游戏进行状态
        if( m_pPreBatUI ){
            m_pPreBatUI->exit();
        }
        if( m_pBatUI ){
            m_pBatUI->enter();
        }
    }
    
    SVGameOver* t_event_game_over = dynamic_cast<SVGameOver*>(_event);
    if(t_event_game_over && m_state != 3){
        m_state = 3;    //游戏结束状态
        if( m_pBatUI ){
            m_pBatUI->exit();
        }
        if( m_pResultUI ){
            m_pResultUI->setScore(m_pHelpSys->getTotalScore());
            m_pResultUI->enter();
        }
        if(m_pUnitMgr){
            m_pUnitMgr->stop();
        }
    }
    SVGamePause* t_event_game_pause = dynamic_cast<SVGamePause*>(_event);
    if(t_event_game_pause){
        m_state = 2;    //游戏暂停状态
    }
    
    return true;
}

s32 SVFaceDanceMain::getState(){
    return m_state;
}




