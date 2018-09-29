//
// SVTripleDownHelpSys.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVTripleDownHelpSys.h"
#include "SVTripleDownDef.h"
#include "SVGameCommonEvent.h"
#include "SVTripleDownEvent.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../event/SVEventMgr.h"

#define TRIPLELIANJI 3.0f
SVTripleDownHelpSys::SVTripleDownHelpSys(SVInst *_app)
:SVEventProc(_app) {
    m_energy = 0.0f;            //能量
    m_totalscore = 0.0f;           //总分数
    m_gametime = 0.0f;          //游戏时间
    m_baseScore = 100;
    m_fLastDown = 0;
    m_iLianJi   = 0;
}

SVTripleDownHelpSys::~SVTripleDownHelpSys() {
}

void SVTripleDownHelpSys::init(){
    enterex();
}

void SVTripleDownHelpSys::destroy(){
    exitex();
}

void SVTripleDownHelpSys::update(float _dt){
    int i_gameTime = (int)m_gametime;
    m_gametime -= _dt;
    
    if (i_gameTime != (int)(m_gametime)){
        _sendDataChange();
    }
    
    if(m_gametime<=0.0f){
        //游戏结束
        SVGameState* t_event = new SVGameState("" , TDS_RESULT);
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event);
        
        SVGameBatResult* t_result = new SVGameBatResult("");
        t_result->m_TotalScore = m_totalscore;
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_result);
    }
}

f32 SVTripleDownHelpSys::getGameTime(){
    return m_gametime;
}

f32 SVTripleDownHelpSys::getEnergy(){
    return m_energy;
}

f32 SVTripleDownHelpSys::getTotalScore(){
    return m_totalscore;
}

void SVTripleDownHelpSys::_sendDataChange(){
    if (m_gametime < 0){
        return;
    }
    SVEventGameTime* pGameTime= new SVEventGameTime();
    pGameTime->iTime = int(m_gametime);
    mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pGameTime);
}

bool SVTripleDownHelpSys::procEvent(SVEvent *_event){
    SVEventTripleDown* pTripleEvent = dynamic_cast<SVEventTripleDown*>(_event);
    if (pTripleEvent){
        float fScore = m_baseScore;
        if (int(pTripleEvent->eHarpoonType) == int(pTripleEvent->eMissType)){
            fScore *= 1.5;
        }
        
        float fDert = m_gametime - m_fLastDown;
        if (fabs(fDert) < TRIPLELIANJI){
            //连击加分
            ++m_iLianJi;
            fScore *= (1.0 + m_iLianJi * 0.5);
            
            SVEventComboStart* pComboStart = new SVEventComboStart("");
            mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pComboStart);
            
            SVEventComboValue* pComboValue = new SVEventComboValue("");
            if (m_iLianJi == 1){
                pComboValue->m_iComBoShowType = 0;
                pComboValue->m_bShowComboValue = false;
            }else if(m_iLianJi == 2){
                pComboValue->m_iComBoShowType = 1;
                pComboValue->m_bShowComboValue = false;
            }else if(m_iLianJi == 3){
                pComboValue->m_iComBoShowType = 2;
                pComboValue->m_bShowComboValue = false;
            }else {
                pComboValue->m_iComBoShowType = 2;
                pComboValue->m_bShowComboValue = true;
                pComboValue->m_comboValue = m_iLianJi - 3;
            }
            mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pComboValue);
        }else{
            m_iLianJi = 0;
            SVEventComboStop* pComboStop = new SVEventComboStop("");
            mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pComboStop);
        }
        m_totalscore += fScore;
        m_fLastDown = m_gametime;
        
        SVEventTotalScore* pEventScore = new SVEventTotalScore();
        pEventScore->fTotalScore = m_totalscore;
        pEventScore->fCurScore   = fScore;
        pEventScore->vecPos.X    = pTripleEvent->vecPos.X;
        pEventScore->vecPos.Y    = pTripleEvent->vecPos.Y;
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(pEventScore);
    }
    return true;
}
