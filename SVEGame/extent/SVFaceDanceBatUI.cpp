//
// Created by lidm on 17/9/5.
//

#include "SVFaceDanceBatUI.h"
#include "SVFaceDanceEvent.h"
#include "SVFaceDanceHelpSys.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVSpriteNode.h"
#include "../node/SVSpineNode.h"
#include "../ui/SVUIParamFont.h"
#include "../ui/SVUIParamSpine.h"
#include "../ui/SVUIParamProgress.h"
#include "../ui/SVUIParamSprite.h"
#include "../logicsys/SVSceneMgr.h"
#include "../core/SVSpine.h"
#include "../event/SVEventMgr.h"
#include "../mtl/SVTexMgr.h"
#include "SVGameDefine.h"
#include "SVGameCommonEvent.h"
#include "SVGameUIShow.h"

SVFaceDanceBatUI::SVFaceDanceBatUI(SVInst *_app)
:SVGameUIBase(_app)
,m_uiComboValue(nullptr)
,m_uiCombo(nullptr) {

}

SVFaceDanceBatUI::~SVFaceDanceBatUI() {
}

void SVFaceDanceBatUI::init(SVGameUIMgr* pUIMgr){
    SVGameUIBase::init(pUIMgr);
}

void SVFaceDanceBatUI::destroy(){
    exit();
    SVGameUIBase::destroy();
}

void SVFaceDanceBatUI::enter(){
    enterex();
    SVGameUIBase::enter();

    mApp->m_pGlobalMgr->m_pEventMgr->registProcer(this);
    SVSpineNode* pBackGround = dynamic_cast<SVSpineNode*>(m_pUIShow->getUIWidgetByName("gameQuanPing"));
    if (pBackGround){
        pBackGround->play("animation");
    }
    
    m_uiComboValue = dynamic_cast<SVBitFontNode*>(m_pUIShow->getUIWidgetByName("GameComboValue"));
    if (m_uiComboValue){
        m_uiComboValue->setvisible(false);
    }
    
    m_uiCombo = dynamic_cast<SVSpineNode*>(m_pUIShow->getUIWidgetByName("GameCombo"));
    if (m_uiCombo){
        m_uiCombo->setvisible(false);
    }
}

void SVFaceDanceBatUI::exit(){
    exitex();
    SVGameUIBase::exit();
}

void SVFaceDanceBatUI::update(float _dt){

}

bool SVFaceDanceBatUI::procEvent(SVEvent *_event){
    //连击开始
    if( m_uiCombo && m_uiComboValue ){
        SVEventComboStart* t_event_combo_start = dynamic_cast<SVEventComboStart*>(_event);
        if(t_event_combo_start){
            m_uiCombo->setvisible(true);
            m_uiComboValue->setvisible(true);
        }
        //连击结束
        SVEventComboStop* t_event_combo_end = dynamic_cast<SVEventComboStop*>(_event);
        if(t_event_combo_end){
            m_uiCombo->setvisible(false);
            m_uiComboValue->setvisible(false);
        }
        //连击更新
        SVEventComboValue* t_event_combo_update = dynamic_cast<SVEventComboValue*>(_event);
        if(t_event_combo_update){
            m_uiCombo->setvisible(true);
            m_uiComboValue->setvisible(true);
            m_uiComboValue->setText(t_event_combo_update->m_comboValue);
            m_uiCombo->setloop(true);
            m_uiCombo->play(m_comboAni.c_str());
        }
    }
    
    SVEventFDScoreChange* t_event_score = dynamic_cast<SVEventFDScoreChange*>(_event);
    if(t_event_score){
        SVBitFontNode* t_game_score = dynamic_cast<SVBitFontNode*>(m_pUIShow->getUIWidgetByName("GameScore"));
        if (t_game_score){
            t_game_score->setText(t_event_score->m_iScore);
        }
    }
    
    SVEventFDTimeChange* t_event_time = dynamic_cast<SVEventFDTimeChange*>(_event);
    if (t_event_time){
        SVBitFontNode* t_game_time = dynamic_cast<SVBitFontNode*>(m_pUIShow->getUIWidgetByName("GameTime"));
        if (t_game_time){
            t_game_time->setText(t_event_time->m_iTime);
        }
    }
    return true;
}
