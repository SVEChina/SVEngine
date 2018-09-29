//
// Created by lidm on 17/9/5.
//

#include "SVFlowerDanceBatUI.h"
#include "SVFlowerDanceEvent.h"
#include "SVGameCommonCombo.h"
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
#include "../texture/SVTexMgr.h"
#include "SVGameDefine.h"
#include "SVGameCommonEvent.h"
#include "SVGameUIShow.h"

SVFlowerDanceBatUI::SVFlowerDanceBatUI(SVInst *_app)
:SVGameUIBase(_app)
,m_uiComboValue(nullptr)
,m_uiCombo(nullptr) {

}

SVFlowerDanceBatUI::~SVFlowerDanceBatUI() {
}

void SVFlowerDanceBatUI::init(SVGameUIMgr* pUIMgr){
    SVGameUIBase::init(pUIMgr);
}

void SVFlowerDanceBatUI::destroy(){
    exit();
    SVGameUIBase::destroy();
}

void SVFlowerDanceBatUI::enter(){
    enterex();
    SVGameUIBase::enter();

    mApp->m_pGlobalMgr->m_pEventMgr->registProcer(this);
    SVSpineNode* pBackGround = dynamic_cast<SVSpineNode*>(m_pUIShow->getUIWidgetByName("gameQuanPing"));
    if (pBackGround){
        pBackGround->play("animation");
    }
    SVGameCommonCombo* pCombo = dynamic_cast<SVGameCommonCombo*>(m_pUIShow->getUIWidgetByName("combo"));
    if (pCombo){
        pCombo->enter();
    }
    /*
    m_uiCombo = dynamic_cast<SVSpineNode*>(m_pUIShow->getUIWidgetByName("combo"));
    if (m_uiCombo){
        m_uiCombo->setvisible(false);
    }*/
}

void SVFlowerDanceBatUI::exit(){
    exitex();
    SVGameUIBase::exit();
    SVGameCommonCombo* pCombo = dynamic_cast<SVGameCommonCombo*>(m_pUIShow->getUIWidgetByName("combo"));
    if (pCombo){
        pCombo->exit();
    }
}

void SVFlowerDanceBatUI::update(float _dt){

}

bool SVFlowerDanceBatUI::procEvent(SVEvent *_event){
    /*
    //连击开始
    if( m_uiCombo ){
        SVEventComboStart* t_event_combo_start = dynamic_cast<SVEventComboStart*>(_event);
        if(t_event_combo_start){
            m_uiCombo->setvisible(true);
        }
        //连击结束
        SVEventComboStop* t_event_combo_end = dynamic_cast<SVEventComboStop*>(_event);
        if(t_event_combo_end){
            m_uiCombo->setvisible(false);
        }
        //连击更新
        SVEventComboValue* t_event_combo_update = dynamic_cast<SVEventComboValue*>(_event);
        if(t_event_combo_update){
            m_uiCombo->setvisible(true);
            //m_uiComboValue->setvisible(true);
            //m_uiComboValue->setText(t_event_combo_update->m_comboValue);
            m_uiCombo->setloop(true);
            m_uiCombo->play(m_comboAni.c_str());
        }
    }
    */
    SVEventFlowerScoreChange* t_event_score = dynamic_cast<SVEventFlowerScoreChange*>(_event);
    if(t_event_score){
        SVBitFontNode* t_game_score = dynamic_cast<SVBitFontNode*>(m_pUIShow->getUIWidgetByName("GameScore"));
        if (t_game_score){
            t_game_score->setText(t_event_score->m_iScore);
        }
    }
    
    return true;
}
