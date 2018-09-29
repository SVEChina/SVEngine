//
// Created by yinlong on 17/11/10.
//

#include "SVGameCommonResultUI.h"
#include "SVGameCommonEvent.h"
#include "SVGameUIShow.h"
#include "../logicsys/SVSceneMgr.h"
#include "../core/SVSpine.h"
#include "../node/SVScene.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVSpineNode.h"


SVGameCommonResultUI::SVGameCommonResultUI(SVInst*_app)
:SVGameUIBase(_app) {

}

SVGameCommonResultUI::~SVGameCommonResultUI() {
}

void SVGameCommonResultUI::init(SVGameUIMgr* pUIMgr){
    SVGameUIBase::init(pUIMgr);
}

void SVGameCommonResultUI::destroy(){
    SVGameUIBase::exit();
    SVGameUIBase::destroy();
}

void SVGameCommonResultUI::enter(){
    enterex();
    SVGameUIBase::enter();
    SVSpineNode* pBackGround = dynamic_cast<SVSpineNode*>(m_pUIShow->getUIWidgetByName("ResultBG"));
    if (pBackGround){
        pBackGround->play("animation");
    }
}

void SVGameCommonResultUI::exit(){
    exitex();
    SVGameUIBase::exit();
}

void SVGameCommonResultUI::update(f32 _dt){
}

bool SVGameCommonResultUI::procEvent(SVEvent *_event){
    SVGameBatResult* p_result_event = dynamic_cast<SVGameBatResult*>(_event);
    if (p_result_event){
        SVBitFontNode* pBitFont = dynamic_cast<SVBitFontNode*>(m_pUIShow->getUIWidgetByName("GameScore"));
        if (pBitFont){
            pBitFont->setText(p_result_event->m_TotalScore);
        }
    }
    
    return true;
}
