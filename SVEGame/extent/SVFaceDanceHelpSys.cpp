//
// Created by lidm on 17/9/5.
//

#include "SVFaceDanceHelpSys.h"
#include "SVFaceDanceEvent.h"
#include "SVFaceDanceHelpSys.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "SVGameCommonEvent.h"

SVFaceDanceHelpSys::SVFaceDanceHelpSys(SVInst*_app)
:SVEventProc(_app) {
    m_energy = 0.0f;            //能量
    m_totalscore = 0;           //总分数
    m_gametime = 0.0f;          //游戏时间
    //连击
    m_comboEnable = false;
    m_comboAccTime = 0.0f;      //连击累计时间
    m_comboLimit = 1.5f;       //秒
    m_comboValue = 0;
    m_lastDeadTime = 0.0f;
    //
    m_maxComboNum = 0;
    m_maxCritNum = 0;
}

SVFaceDanceHelpSys::~SVFaceDanceHelpSys() {
}

void SVFaceDanceHelpSys::init(){
    enterex();
}

void SVFaceDanceHelpSys::destroy(){
    exitex();
}

void SVFaceDanceHelpSys::update(f32 _dt){
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
    //能量
    if(m_energy>=100.0f){
        m_energy = 0.0f;
        SVEventFDUnitAllDead* t_event = new SVEventFDUnitAllDead("");
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event);
    }
    
    //连击
    if(m_comboEnable){
        m_comboAccTime -= _dt;
        if(m_comboAccTime<0.0f){
            m_comboAccTime = m_comboLimit;
            m_comboValue = 0;
            m_comboEnable = false;
            //连击关闭
            SVEventComboStop* t_event = new SVEventComboStop("");
            mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event);
        }
    }
}

f32 SVFaceDanceHelpSys::getGameTime(){
    return m_gametime;
}

f32 SVFaceDanceHelpSys::getEnergy(){
    return m_energy;
}

f32 SVFaceDanceHelpSys::getTotalScore(){
    return m_totalscore;
}

s32 SVFaceDanceHelpSys::getComboValue(){
    return m_comboValue;
}

s32 SVFaceDanceHelpSys::getMaxComboValue(){
    return m_maxComboNum;
}

s32 SVFaceDanceHelpSys::getMaxCritValue(){
    return m_maxCritNum;
}

bool SVFaceDanceHelpSys::procEvent(SVEvent *_event){
    //清理屏幕
    SVEventFDUnitAllDead* t_event_alldead = dynamic_cast<SVEventFDUnitAllDead*>(_event);
    if(t_event_alldead){
        
    }
    //
    SVEventFDUnitDead* t_event_unitdead = dynamic_cast<SVEventFDUnitDead*>(_event);
    if(t_event_unitdead){
        //单个消亡
        if( t_event_unitdead->m_iscrit ){
            m_maxCritNum++;
        }
        //能量更新
        m_energy += t_event_unitdead->m_energy;
        //判断连击
        if(!m_comboEnable){
            f32 t_dert = (t_event_unitdead->m_gameTime - m_lastDeadTime);
            if(t_dert<m_comboLimit){
                m_comboEnable = true;   //连击开启
                //连击开启
                SVEventComboStart* t_event = new SVEventComboStart("");
                mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event);
            }
        }
        m_lastDeadTime = t_event_unitdead->m_gameTime;
        s32 t_comboscore = 0;   //连击分数增益
        if(m_comboEnable){
            m_comboAccTime = m_comboLimit;
            m_comboValue++;
            //
            t_comboscore = m_comboValue*2;
            //修改最大连击数目
            m_maxComboNum = m_maxComboNum>m_comboValue?m_maxComboNum:m_comboValue;
            //
            SVEventComboValue* t_event = new SVEventComboValue("");
            t_event->m_comboValue = m_comboValue;
            mApp->m_pGlobalMgr->m_pEventMgr->pushEventToSecondPool(t_event);
        }
        //分数更新
        m_totalscore += (t_event_unitdead->m_score + t_comboscore);
        SVEventFDScoreChange* t_event_score = new SVEventFDScoreChange("");
        t_event_score->m_iScore = m_totalscore;
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event_score);
    }
    return true;
}
