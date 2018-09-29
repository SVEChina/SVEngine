//
// Created by yinlong on 17/10/20.
//

#include "SVGameCommonCombo.h"
#include "SVGameCommonEvent.h"
#include "SVGameCommonEvent.h"
#include "../node/SVSpineNode.h"
#include "../node/SVBitFontNode.h"
#include "../logicsys/SVSceneMgr.h"
#include "../node/SVScene.h"
#include "../core/SVSpine.h"
#include "../event/SVEventMgr.h"
#include "../ui/SVUIParamCombo.h"

SVGameCommonCombo::SVGameCommonCombo(SVInst*_app)
:SVNode(_app) ,m_pUiCombo(NULL),m_pUiComboValue(NULL){
}

SVGameCommonCombo::~SVGameCommonCombo() {
}

void SVGameCommonCombo::init(){
}

void SVGameCommonCombo::destroy(){
    exit();
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(t_sc){
        t_sc->removeNode(m_pUiCombo);
        t_sc->removeNode(m_pUiComboValue);
    }
    if (m_pUiCombo != NULL){
        m_pUiCombo->svdestroy();
        SAFE_DELETE(m_pUiCombo);
    }
    
    if (m_pUiComboValue != NULL){
        m_pUiComboValue->svdestroy();
        SAFE_DELETE(m_pUiComboValue);
    }
}

void SVGameCommonCombo::enter(){
    enterex();
}

void SVGameCommonCombo::exit(){
    exitex();
}

void SVGameCommonCombo::update(float _dt){

}

//创建UI
void SVGameCommonCombo::createUI(SVUIParamBase* _param){
    if(strcmp(_param->m_WidgetName.c_str(),"COMBO_COM") == 0 ){
        SVUIParamCombo* t_spineparam = (SVUIParamCombo*)_param;
        
        stringc t_spine_json = t_spineparam->m_SpineName + ".json";
        stringc t_spine_atlas = t_spineparam->m_SpineName + ".atlas";
        SVSpine *t_spine = SVSpine::createSpine(mApp, t_spine_json.c_str(), t_spine_atlas.c_str(), 1.0f);
        if(t_spine){
            SVSpineNodeParam _spineParam;
            _spineParam.m_spinename = t_spineparam->m_SpineName;
            _spineParam.m_pos.set(t_spineparam->m_px, t_spineparam->m_py, t_spineparam->m_pz);
            _spineParam.m_zorder = 4;
            m_pUiCombo = dynamic_cast<SVSpineNode*>(_spineParam.genNode(mApp));
            m_pUiCombo->setUI(true);
            
            
            SVBitFontNodeParam _fontParam;
            _fontParam.m_fonttex = stringc(t_spineparam->m_FontPngName.c_str());
            _fontParam.m_content = "0";
            _fontParam.m_fontw = t_spineparam->m_FontWidth;
            _fontParam.m_fonth = t_spineparam->m_FontHeight;
            _fontParam.m_pos.set(t_spineparam->m_px, t_spineparam->m_py, t_spineparam->m_pz);
            m_pUiComboValue = dynamic_cast<SVBitFontNode*>(_fontParam.genNode(mApp));
            m_pUiComboValue->setUI(true);
            int iSize = t_spineparam->m_arrAniName.size();
            for (int i = 0;i < iSize; ++i) {
                m_arrAni.push_back(t_spineparam->m_arrAniName[i]);
            }
        }
    }

}

void SVGameCommonCombo::setSceneName(const char* pSceneName){
    m_scenename = pSceneName;
}

bool SVGameCommonCombo::procEvent(SVEvent *_event){
    if (m_arrAni.size() <= 0)
        return true;
    
    //连击开始
    if( m_pUiCombo && m_pUiComboValue ){
        SVEventComboStart* t_event_combo_start = dynamic_cast<SVEventComboStart*>(_event);
        if(t_event_combo_start){
            m_pUiCombo->setvisible(true);
            m_pUiComboValue->setvisible(true);
            SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
            if( t_sc ){
                t_sc->addNode(m_pUiCombo);
                t_sc->addNode(m_pUiComboValue);
            }
        }
        //连击结束
        SVEventComboStop* t_event_combo_end = dynamic_cast<SVEventComboStop*>(_event);
        if(t_event_combo_end){
            m_pUiCombo->setvisible(false);
            m_pUiComboValue->setvisible(false);
            SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
            if(t_sc){
                t_sc->removeNode(m_pUiCombo);
                t_sc->removeNode(m_pUiComboValue);
            }
        }
        //连击更新
        SVEventComboValue* t_event_combo_update = dynamic_cast<SVEventComboValue*>(_event);
        if(t_event_combo_update){
            m_pUiCombo->setvisible(true);
            m_pUiComboValue->setvisible(true);
            m_pUiCombo->setloop(false);
            
            int iShowType = t_event_combo_update->m_iComBoShowType;
            if (iShowType < 0 || iShowType >= m_arrAni.size()){
                iShowType = 0;
            }
            m_pUiCombo->play(m_arrAni[iShowType].c_str());
        }
    }
    return true;
}
