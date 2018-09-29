//
// SVTripleDownBatUI.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVTripleDownBatUI.h"
#include "SVGameDefine.h"
#include "SVGameCommonCombo.h"
#include "SVTripleDownEvent.h"
#include "SVGameUIShow.h"
#include "../logicsys/SVSceneMgr.h"
#include "../node/SVScene.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVSpineNode.h"
#include "../core/SVSpine.h"
#include "../event/SVEventMgr.h"

SVTripleDownBatUI::SVTripleDownBatUI(SVInst *_app)
:SVGameUIBase(_app) {
    m_bShowCurScore = false;
    m_fShowTime = 0.0f;
}

SVTripleDownBatUI::~SVTripleDownBatUI() {
}

void SVTripleDownBatUI::init(SVGameUIMgr* pUIMgr){
    SVGameUIBase::init(pUIMgr);
}

void SVTripleDownBatUI::destroy(){
    SVGameUIBase::destroy();
    exit();
}

void SVTripleDownBatUI::enter(){
    SVGameUIBase::enter();
    enterex();
    
    SVSpineNode* pBackGround = dynamic_cast<SVSpineNode*>(m_pUIShow->getUIWidgetByName("gameQuanPing"));
    if (pBackGround){
        pBackGround->play("animation");
    }
    m_uiGameScore = dynamic_cast<SVBitFontNode*>(m_pUIShow->getUIWidgetByName("GameScore"));
    m_uiCurScore  = dynamic_cast<SVBitFontNode*>(m_pUIShow->getUIWidgetByName("GameCurScore"));
    m_uiCurScore->setvisible(false);
    SVGameCommonCombo* pCombo = dynamic_cast<SVGameCommonCombo*>(m_pUIShow->getUIWidgetByName("combo"));
    if (pCombo){
        pCombo->enter();
    }
}

void SVTripleDownBatUI::exit(){
    SVGameUIBase::exit();
    exitex();
    if (m_pUIShow == NULL){
        return;
    }
    SVGameCommonCombo* pCombo = dynamic_cast<SVGameCommonCombo*>(m_pUIShow->getUIWidgetByName("combo"));
    if (pCombo){
        pCombo->exit();
    }
}

void SVTripleDownBatUI::update(float _dt){
    if (m_bShowCurScore == true){
        m_fShowTime += _dt;
        if (m_fShowTime > 1.0f){
            m_fShowTime = 0.0f;
            m_bShowCurScore = false;
            if (m_uiCurScore){
                m_uiCurScore->setvisible(false);
            }
        }
    }
}

bool SVTripleDownBatUI::procEvent(SVEvent *_event){
    SVEventTotalScore* pScoreEvent = dynamic_cast<SVEventTotalScore*>(_event);
    if (pScoreEvent){
        m_uiGameScore->setText(pScoreEvent->fTotalScore);
        m_uiCurScore->setText(pScoreEvent->fCurScore);
        m_uiCurScore->setPosition(pScoreEvent->vecPos.X, pScoreEvent->vecPos.Y, 0);
        m_uiCurScore->setvisible(true);
        m_bShowCurScore = true;
        m_fShowTime = 0.0f;
    }
    
    SVEventGameTime* pTimeEvent = dynamic_cast<SVEventGameTime*>(_event);
    if (pTimeEvent){
        SVBitFontNode* uiTime = dynamic_cast<SVBitFontNode*>(m_pUIShow->getUIWidgetByName("GameTime"));
        if (uiTime){
            uiTime->setText(pTimeEvent->iTime);
        }
    }
    return true;
}
