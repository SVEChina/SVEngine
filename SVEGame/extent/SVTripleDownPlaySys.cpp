//
// SVTripleDownPlaySys.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../node/SVSpineNode.h"
#include "../node/SVScene.h"
#include "../core/SVSpine.h"
#include "../logicsys/SVSceneMgr.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectBase.h"
#include "SVGameCommonEvent.h"
#include "SVGameDefine.h"
#include "SVTripleDownPlaySys.h"
#include "SVTripleDownDef.h"
#include "SVTripleDownMgr.h"
#include "SVTripleDownUnit.h"
#include "SVTripleDownHarpoon.h"
#include "SVTripleDownEvent.h"

SVTripleDownPlaySys::SVTripleDownPlaySys(SVInst *_app, SVTripleDownMgr* pUnitMgr)
:SVEventProc(_app),m_pUnitMgr(pUnitMgr),m_eMouthState(MS_INIT),m_eFaceTowards(FT_INIT),m_pCurGen(NULL)
,m_pHarpoonLeft(NULL),m_pHarpoonMiddle(NULL),m_pHarpoonRight(NULL),m_iBgMusic(-1){

}

SVTripleDownPlaySys::~SVTripleDownPlaySys() {
}

void SVTripleDownPlaySys::init(){
    m_pHarpoonLeft      = new SVTripleDownHarpoon(mApp);
    m_pHarpoonLeft->init(m_strSceneName.c_str(), m_strHarpoonAniLeft.c_str());
    m_pHarpoonLeft->setPosition(m_posLeft);
    m_pHarpoonLeft->setWidth(m_width);
    m_pHarpoonLeft->setHeight(m_height);
    m_pHarpoonLeft->setType(HT_HONG);
    
    m_pHarpoonMiddle    = new SVTripleDownHarpoon(mApp);
    m_pHarpoonMiddle->init(m_strSceneName.c_str(), m_strHarpoonAniMiddle.c_str());
    m_pHarpoonMiddle->setPosition(m_posMiddle);
    m_pHarpoonMiddle->setWidth(m_width);
    m_pHarpoonMiddle->setHeight(m_height);
    m_pHarpoonMiddle->setType(HT_HUANG);
    
    m_pHarpoonRight     = new SVTripleDownHarpoon(mApp);
    m_pHarpoonRight->init(m_strSceneName.c_str(), m_strHarpoonAniRight.c_str());
    m_pHarpoonRight->setPosition(m_posRight);
    m_pHarpoonRight->setWidth(m_width);
    m_pHarpoonRight->setHeight(m_height);
    m_pHarpoonRight->setType(HT_LV);
}

void SVTripleDownPlaySys::destroy(){
    exit();
    if (m_pHarpoonLeft){
        m_pHarpoonLeft->destroy();
        SAFE_DELETE(m_pHarpoonLeft);
    }
    
    if (m_pHarpoonRight){
        m_pHarpoonRight->destroy();
        SAFE_DELETE(m_pHarpoonRight);
    }
    
    if (m_pHarpoonMiddle){
        m_pHarpoonMiddle->destroy();
        SAFE_DELETE(m_pHarpoonMiddle);
    }
}

void SVTripleDownPlaySys::update(float _dt){
    _updateCurrent();
    
    if (m_pHarpoonLeft){
        m_pHarpoonLeft->update(_dt,0);
    }
    
    if (m_pHarpoonRight){
        m_pHarpoonRight->update(_dt, 0);
    }
    
    if (m_pHarpoonMiddle){
        m_pHarpoonMiddle->update(_dt, 0);
    }
}

void SVTripleDownPlaySys::enter(){
    enterex();
    if (m_pHarpoonLeft){
        m_pHarpoonLeft->enter();
        m_pHarpoonLeft->showAni(HA_COMMON);
    }
    
    if (m_pHarpoonRight){
        m_pHarpoonRight->enter();
        m_pHarpoonRight->showAni(HA_COMMON);
    }
    
    if (m_pHarpoonMiddle){
        m_pHarpoonMiddle->enter();
        m_pHarpoonMiddle->showAni(HA_COMMON);
    }
}

void SVTripleDownPlaySys::exit(){
    exitex();
    if (m_pHarpoonLeft){
        m_pHarpoonLeft->exit();
    }
    
    if (m_pHarpoonRight){
        m_pHarpoonRight->exit();
    }
    
    if (m_pHarpoonMiddle){
        m_pHarpoonMiddle->exit();
    }
    
    if (m_iBgMusic != -1){
    }
}

bool SVTripleDownPlaySys::procEvent(SVEvent *_event){
    if(!_event)
        return true;
    
    SVPersonEvent* pPerEvent = dynamic_cast<SVPersonEvent*>(_event);
    if (pPerEvent){
        if (pPerEvent->personID != 0){
            return true;
        }
        if(pPerEvent->eventType == EVN_T_MOUSE_OPEN){
            if (m_eMouthState != MS_OPEN){
                m_eMouthState = MS_OPEN;
                _openMouth();
            }
            if (m_eMouthState == MS_OPEN){
                
            }
        }else if(pPerEvent->eventType == EVN_T_MOUSE_CLOSE){
            if (m_eMouthState != MS_CLOSE){
                if (m_pCurGen){
                    m_pCurGen->setScale(1.0f);
                }
                m_eMouthState = MS_CLOSE;
                _closeMouth();
            }
        }else if (pPerEvent->eventType == EVN_T_LEFT_TUREN_HEAD){
            if (m_eFaceTowards != FT_LEFT){
                _showHarpoonSelect(FT_LEFT);
            }
        }else if (pPerEvent->eventType == EVN_T_RIGHT_TUREN_HEAD){
            if (m_eFaceTowards != FT_RIGHT){
                _showHarpoonSelect(FT_RIGHT);
            }
        }else if (pPerEvent->eventType == EVN_T_NONE_TUREN_HEAD){
            if (m_eFaceTowards != FT_MIDDLE){
                _showHarpoonSelect(FT_MIDDLE);
            }
        }
    }
    
    SVEventTripleDown* pTripleEvent = dynamic_cast<SVEventTripleDown*>(_event);
    if (pTripleEvent){
    }
    return true;
}


void SVTripleDownPlaySys::_openMouth(){
    SVTripleDownUnit* t_unit = m_pUnitMgr->getFreeUnit();
    if(t_unit){
        t_unit->enter();
        t_unit->ShowAni(CA_START);
        m_pCurGen = t_unit;
    }
}

void SVTripleDownPlaySys::_closeMouth(){
    if (m_eFaceTowards == FT_LEFT){
        m_pHarpoonLeft->addCircle(m_pCurGen);
    }else if(m_eFaceTowards == FT_MIDDLE){
        m_pHarpoonMiddle->addCircle(m_pCurGen);
    }else if(m_eFaceTowards == FT_RIGHT){
        m_pHarpoonRight->addCircle(m_pCurGen);
    }
    m_pCurGen = NULL;
}

void SVTripleDownPlaySys::_updateCurrent(){
    if (m_pCurGen == NULL){
        return;
    }
    
    SVDetectBase* pDetect = mApp->m_pGlobalMgr->m_pDetectMgr->getDetect();
    if (!pDetect){
        return;
    }
    SVPerson* t_bind_person = pDetect->getPerson(0);
    if (t_bind_person == NULL || t_bind_person->getExist() == false) {
        return;
    }
    
    SVPersonTracker* t_tracker = t_bind_person->getTracker();
    if (t_tracker == NULL ){
        return;
    }
    
    vector3df vecPos(t_tracker->m_mouse_pt.X - 360.0f , t_tracker->m_mouse_pt.Y - 640.0f, 0);
    m_pCurGen->setPosition(vecPos);
}

void SVTripleDownPlaySys::_showHarpoonSelect(FACETOWARDS eTowards){
    if (m_eFaceTowards == FT_LEFT){
        m_pHarpoonLeft->showAni(HA_COMMON);
    }else if (m_eFaceTowards == FT_MIDDLE){
        m_pHarpoonMiddle->showAni(HA_COMMON);
    }else if (m_eFaceTowards == FT_RIGHT){
        m_pHarpoonRight->showAni(HA_COMMON);
    }
    
    m_eFaceTowards = eTowards;
    
    if (m_eFaceTowards == FT_LEFT){
        m_pHarpoonLeft->showAni(HA_SELECT);
    }else if (m_eFaceTowards == FT_MIDDLE){
        m_pHarpoonMiddle->showAni(HA_SELECT);
    }else if (m_eFaceTowards == FT_RIGHT){
        m_pHarpoonRight->showAni(HA_SELECT);
    }
}
