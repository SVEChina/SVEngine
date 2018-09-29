//
// Created by lidm on 17/9/5.
//

#include "SVFlowerDanceMain.h"
#include "SVFlowerDanceMgr.h"
#include "SVGameCommonEvent.h"
#include "SVFlowerDancePlayer.h"
#include "SVFlowerDanceHelp.h"
#include "SVFlowerDanceBatUI.h"

#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"

SVFlowerDanceMain::SVFlowerDanceMain(SVInst*_app)
:SVGameBase(_app)
,m_pPlayer(nullptr) {
    m_state = 0;
    m_pUnitMgr = new SVFlowerDanceMgr(mApp);
    m_pPlayer  = new SVFlowerDancePlayer(mApp);
    m_pHelpSys = new SVFlowerDanceHelpSys(mApp);
    m_pBatUI   = new SVFlowerDanceBatUI(mApp);
}

SVFlowerDanceMain::~SVFlowerDanceMain() {
    SAFE_DELETE(m_pUnitMgr);
    SAFE_DELETE(m_pPlayer);
    SAFE_DELETE(m_pHelpSys);
    SAFE_DELETE(m_pBatUI);
}

void SVFlowerDanceMain::init(){
    m_pUnitMgr->init();
    m_pPlayer->init();
    m_pHelpSys->init();
    m_pBatUI->init(getUIMgr());
    m_state = -1;
    //
    enterex();
}

void SVFlowerDanceMain::destroy(){
    //
    exitex();
    //
    m_pUnitMgr->destroy();
    m_pPlayer->destroy();
    m_pHelpSys->destroy();
    m_pBatUI->destroy();
}

void SVFlowerDanceMain::update(f32 _dt){
    if (m_state == 0){
        //m_pPreBatUI->update(_dt);
    }else if(m_state == 1){
        m_pUnitMgr->update(_dt);
        m_pPlayer->update(_dt);
        m_pHelpSys->update(_dt);
        m_pBatUI->update(_dt);
    }else if(m_state == 3){
        //更新战斗UI
        //m_pResultUI->update(_dt);
    }
}

bool SVFlowerDanceMain::procEvent(SVEvent *_event){
    SVEventGameStart* t_event_game_start = dynamic_cast<SVEventGameStart*>(_event);
    if(t_event_game_start){
        m_state = 1;
        m_pPlayer->enter();
        m_pHelpSys->enter();
        m_pBatUI->enter();
    }
    
   /* SVEventGameEnterBattle* t_event_enter_battle = dynamic_cast<SVEventGameEnterBattle*>(_event);
    if (t_event_enter_battle){
        m_state = 1;    //游戏进行状态
    }*/
    
    SVGameOver* t_event_game_over = dynamic_cast<SVGameOver*>(_event);
    if(t_event_game_over && m_state != 3){
        m_state = 3;    //游戏结束状态
        //传递分数
        if(m_pCB && m_pHelpSys){
            s32 t_score = m_pHelpSys->getScore();
            stringc tt_store = stringc(t_score);
            (*m_pCB)( 2, tt_store.c_str() );
        }
        //
        if(m_pUnitMgr){
            m_pUnitMgr->stop();
            m_pPlayer->exit();
            m_pBatUI->exit();
        }
    }
    SVGamePause* t_event_game_pause = dynamic_cast<SVGamePause*>(_event);
    if(t_event_game_pause){
        m_state = 2;    //游戏暂停状态
    }
    
    return true;
}

s32 SVFlowerDanceMain::getState(){
    return m_state;
}




