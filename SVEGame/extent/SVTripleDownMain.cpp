//
// SVTripleDownMain.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVTripleDownMain.h"
#include "SVTripleDownHelpSys.h"
#include "SVTripleDownPlaySys.h"
#include "SVTripleDownMgr.h"
#include "SVTripleDownBatUI.h"
#include "SVGameCommonEvent.h"
#include "SVGameCommonPreBatUI.h"
#include "SVGameCommonResultUI.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../detect/SVPerson.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectBase.h"
#include "../detect/SVPersonExpressionExt.h"
#include "../detect/SVPersonTracker.h"

SVTripleDownMain::SVTripleDownMain(SVInst *_app)
:SVGameBase(_app),m_pPreBatUI(NULL){
    m_pPreBatUI = new SVGameCommonPreBatUI(mApp);
    m_pResultUI = new SVGameCommonResultUI(mApp);
    m_pGameSys  = new SVTripleDownHelpSys(mApp);
    m_pUnitMgr  = new SVTripleDownMgr(mApp);
    m_pPlaySys  = new SVTripleDownPlaySys(mApp , m_pUnitMgr);
    m_pBatUI    = new SVTripleDownBatUI(mApp);
}

SVTripleDownMain::~SVTripleDownMain() {
    SAFE_DELETE(m_pPreBatUI);
    SAFE_DELETE(m_pResultUI);
    SAFE_DELETE(m_pGameSys);
    SAFE_DELETE(m_pPlaySys);
    SAFE_DELETE(m_pUnitMgr);
    SAFE_DELETE(m_pBatUI);
}

void SVTripleDownMain::init(){
    enterex();
    m_pPreBatUI->init(getUIMgr());
    m_pResultUI->init(getUIMgr());
    m_pGameSys->init();
    m_pUnitMgr->init();
    m_pPlaySys->init();
    m_pBatUI->init(getUIMgr());
    
    SVDetectBase* pDetect = mApp->m_pGlobalMgr->m_pDetectMgr->getDetect();
    if (!pDetect){
        return;
    }
    SVPerson* t_bind_person = pDetect->getPerson(0);
    if (t_bind_person == NULL ) {
        return;
    }

    if (t_bind_person){
        SVPersonExpressionExt *t_expression = t_bind_person->getTracker()->getPersonExpression();
        if (t_expression){
            m_iTrackRate        = t_expression->getDetectExtRate();
            m_fLookLeftAngle    = t_expression->_getLookLeftAngle();
            m_fLookRightAngle   = t_expression->_getLookRightAngle();
            m_fOpenMouthAdjust  = t_expression->_getMouthOpenAdjust();
            
            t_expression->setDetectExtRate(50);
            t_expression->_setLookLeftAngle(7);
            t_expression->_setLookRightAngle(7);
            t_expression->_setMouthOpenAdjust(4);
        }
    }
}

void SVTripleDownMain::destroy(){
    exitex();
    m_pPreBatUI->destroy();
    m_pResultUI->destroy();
    m_pGameSys->destroy();
    m_pPlaySys->destroy();
    m_pUnitMgr->destroy();
    m_pBatUI->destroy();
    
    SVDetectBase* pDetect = mApp->m_pGlobalMgr->m_pDetectMgr->getDetect();
    if (!pDetect){
        return;
    }
    SVPerson* t_bind_person = pDetect->getPerson(0);
    if (t_bind_person == NULL ) {
        return;
    }
    if (t_bind_person){
        SVPersonExpressionExt *t_expression = t_bind_person->getTracker()->getPersonExpression();
        if (t_expression){
            t_expression->setDetectExtRate(m_iTrackRate);
            t_expression->_setLookLeftAngle(m_fLookLeftAngle);
            t_expression->_setLookRightAngle(m_fLookRightAngle);
            t_expression->_setMouthOpenAdjust(m_fOpenMouthAdjust);
        }
    }
}

void SVTripleDownMain::update(float _dt){
    float fTime = _dt;
    if (m_eGameState == TDS_PREBAT){
        m_pPreBatUI->update(fTime);
    }
    if (m_eGameState == TDS_BAT){
        m_pGameSys->update(fTime);
        m_pUnitMgr->update(fTime,0);
        m_pPlaySys->update(fTime);
        m_pBatUI->update(fTime);
    }
    if (m_eGameState == TDS_RESULT){
        m_pResultUI->update(fTime);
    }
}

bool SVTripleDownMain::procEvent(SVEvent *_event){
    SVGameState* pGState = dynamic_cast<SVGameState*>(_event);
    if (pGState != NULL){
        TRIPLEDOWNSTATE eTripleState = (TRIPLEDOWNSTATE)pGState->getGameState();
        switch (eTripleState) {
            case TDS_GAME_BEGIN:
                m_pPreBatUI->enter();
                m_eGameState = TDS_PREBAT;
                break;
            case TDS_PREBAT:
                break;
            case TDS_BAT:{
                m_eGameState = TDS_BAT;
                if (m_pPlaySys){
                    m_pPlaySys->enter();
                }
                if (m_pBatUI){
                    m_pBatUI->enter();
                }
            }
                break;
            case TDS_RESULT:{
                if (m_pPlaySys){
                    m_pPlaySys->exit();
                }
                if (m_pBatUI){
                    m_pBatUI->exit();
                }
                if (m_pUnitMgr){
                    m_pUnitMgr->destroy();
                }
                m_eGameState = TDS_RESULT;
                m_pResultUI->enter();
                SVGameOver* t_event = new SVGameOver("");
                t_event->m_iScole = m_pGameSys->getTotalScore();
                mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event);
            }
                break;
            default:
                break;
        }
    }
    
    SVEventGameStart* pGameStart   = dynamic_cast<SVEventGameStart*>(_event);
    if (pGameStart != NULL){
        SVGameState* t_event = new SVGameState("" , TDS_GAME_BEGIN);
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event);
    }
    
    SVEventGameEnterBattle* pEnterBattle = dynamic_cast<SVEventGameEnterBattle*>(_event);
    if ( pEnterBattle != NULL){
        SVGameState* t_event = new SVGameState("" , TDS_BAT);
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event);
    }
    return true;
}





