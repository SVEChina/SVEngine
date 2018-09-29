//
// Created by lidm on 17/9/5.
//

#include "SVFlowerDanceHelp.h"
#include "SVGameCommonEvent.h"
#include "SVFlowerDanceEvent.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"

SVFlowerDanceHelpSys::SVFlowerDanceHelpSys(SVInst*_app)
:SVEventProc(_app) {
    m_energy    = 0.0f;            //能量
    m_totalscore= 0;           //总分数
    m_gametime  = 0.0f;          //游戏时间
    m_lastTime  = 0.0;
    m_iLeiji    = 0;
}

SVFlowerDanceHelpSys::~SVFlowerDanceHelpSys() {
}

void SVFlowerDanceHelpSys::init(){
    m_lastTime  = 0.0;
    m_iLeiji    = 0;
    enterex();
}

void SVFlowerDanceHelpSys::destroy(){
    exitex();
}

void SVFlowerDanceHelpSys::update(f32 _dt){
    m_gametime += _dt;
    /*
    s32 iGameTimeLast = (s32)m_gametime;
    m_gametime -= _dt;
    s32 iGameTimeCur = (s32)m_gametime;
    if (iGameTimeCur != iGameTimeLast){
        SVEventFDTimeChange* t_event_timeChange = new SVEventFDTimeChange("");
        t_event_timeChange->m_iTime = iGameTimeCur;
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event_timeChange);
    }
    
    if(m_gametime<=0.0f){
        //游戏结束
        SVGameOver* t_event = new SVGameOver("");
        t_event->m_iScole = m_totalscore;
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event);
    }
   */
}
bool SVFlowerDanceHelpSys::procEvent(SVEvent *_event){
    SVEventEnterFlower* pEnterEvent = dynamic_cast<SVEventEnterFlower*>(_event);
    if (pEnterEvent && pEnterEvent->m_bIsOpened == false){
        if (pEnterEvent->m_iType == 1){
            m_totalscore += 1;
        }else if (pEnterEvent->m_iType == 2){
            m_totalscore += 3;
        }
        
        if (m_gametime - m_lastTime < 1.5){
            m_iLeiji += 2;
            m_totalscore += m_iLeiji;
            
            SVEventComboStart* pComboStart = new SVEventComboStart("");
            mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pComboStart);
            
            SVEventComboValue* pComboValue = new SVEventComboValue("");
            int iType = m_iLeiji/2;
            if (iType == 1){
                pComboValue->m_iComBoShowType = 0;
                pComboValue->m_bShowComboValue = false;
            }else if(iType == 2){
                pComboValue->m_iComBoShowType = 1;
                pComboValue->m_bShowComboValue = false;
            }else if(iType == 3){
                pComboValue->m_iComBoShowType = 2;
                pComboValue->m_bShowComboValue = false;
            }else {
                pComboValue->m_iComBoShowType = 2;
                pComboValue->m_bShowComboValue = true;
                pComboValue->m_comboValue = iType - 3;
            }
            mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pComboValue);
        }else{
            m_iLeiji = 0;
            SVEventComboStop* pComboStop = new SVEventComboStop("");
            mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pComboStop);
        }
        
        SVEventFlowerScoreChange* pEventScore = new SVEventFlowerScoreChange();
        pEventScore->m_iScore = m_totalscore;
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pEventScore);

        m_lastTime = m_gametime;
    }
    return true;
}

void SVFlowerDanceHelpSys::enter(){
    
}
