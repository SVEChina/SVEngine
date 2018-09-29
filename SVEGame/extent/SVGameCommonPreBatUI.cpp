//
// Created by yinlong on 17/10/20.
//

#include "SVGameCommonPreBatUI.h"
#include "SVGameCommonEvent.h"
#include "../node/SVSpineNode.h"
#include "../node/SVScene.h"
#include "../logicsys/SVSceneMgr.h"
#include "../core/SVSpine.h"
#include "../event/SVEventMgr.h"
#include "../ui/SVUIParamSpine.h"
#include "SVGameUIMgr.h"
#include "SVGameUIShow.h"

SVGameCommonPreBatUI::SVGameCommonPreBatUI(SVInst*_app)
:SVGameUIBase(_app){
}

SVGameCommonPreBatUI::~SVGameCommonPreBatUI() {
}

void SVGameCommonPreBatUI::init(SVGameUIMgr* pUIMgr){
    SVGameUIBase::init(pUIMgr);
    m_fUiStateTime = 3.5;
}

void SVGameCommonPreBatUI::destroy(){
    SVGameUIBase::destroy();
    exit();
}

void SVGameCommonPreBatUI::enter(){
    SVGameUIBase::enter();
    if (m_pUIShow == NULL){
        return;
    }
    SVSpineNode* pSpine = dynamic_cast<SVSpineNode*>(m_pUIShow->getUIWidgetByName("DaoJiShi"));
    if (pSpine){
        pSpine->play("animation");
    }
    enterex();
}

void SVGameCommonPreBatUI::setSound(cptr8 _pSound){
    m_strDaoShuSound = _pSound;
}

void SVGameCommonPreBatUI::exit(){
    SVGameUIBase::exit();
    exitex();
}

void SVGameCommonPreBatUI::update(f32 _dt){
    if (m_fUiStateTime < 0)
        return;
    //ui和逻辑暂时不分
    s32 iStateTime = s32(m_fUiStateTime);
    m_fUiStateTime -= _dt;
    if (iStateTime != s32(m_fUiStateTime)){
    }
    if (m_fUiStateTime < 0){
        SVEventGameEnterBattle* t_event = new SVEventGameEnterBattle();
        mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event);
    }
}

bool SVGameCommonPreBatUI::procEvent(SVEvent *_event){
    return true;
}
