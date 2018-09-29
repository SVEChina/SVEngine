//
// Created by lidm on 17/9/5.
//

#include "SVFaceDanceResultUI.h"
#include "SVFaceDanceHelpSys.h"
#include "SVGameUIShow.h"
#include "../node/SVScene.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVSpineNode.h"
#include "../logicsys/SVSceneMgr.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"

SVFaceDanceResultUI::SVFaceDanceResultUI(SVInst*_app)
:SVGameUIBase(_app) ,m_iScore(0){
}

SVFaceDanceResultUI::~SVFaceDanceResultUI() {
}

void SVFaceDanceResultUI::init(SVGameUIMgr* pUiMgr){
    SVGameUIBase::init(pUiMgr);
}

void SVFaceDanceResultUI::destroy(){
    SVGameUIBase::exit();
    SVGameUIBase::destroy();
}

void SVFaceDanceResultUI::enter(){
    enterex();
    SVGameUIBase::enter();
    SVSpineNode* pBackGround = dynamic_cast<SVSpineNode*>(m_pUIShow->getUIWidgetByName("ResultBG"));
    if (pBackGround){
        pBackGround->play("animation");
    }
    
    SVBitFontNode* pBitFont = dynamic_cast<SVBitFontNode*>(m_pUIShow->getUIWidgetByName("GameScore"));
    if (pBitFont){
        pBitFont->setText(m_iScore);
    }
}

void SVFaceDanceResultUI::exit(){
    exitex();
    SVGameUIBase::exit();
}

void SVFaceDanceResultUI::update(f32 _dt){
}

bool SVFaceDanceResultUI::procEvent(SVEvent *_event){
    return true;
}

void SVFaceDanceResultUI::setScore(int iScore){
    m_iScore = iScore;
}
