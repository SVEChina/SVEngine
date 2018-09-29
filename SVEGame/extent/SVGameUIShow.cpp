//
// Created by yinlong on 17/10/27.
//

#include "SVGameUIShow.h"
#include "SVGameCommonCombo.h"
#include "../app/SVInst.h"
#include "../ui/SVUIParamFont.h"
#include "../ui/SVUIParamSpine.h"
#include "../ui/SVUIParamProgress.h"
#include "../ui/SVUIParamSprite.h"
#include "../ui/SVUIParamCombo.h"
#include "../node/SVSpineNode.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVSpriteNode.h"
#include "../core/SVSpine.h"
#include "../texture/SVTexMgr.h"
#include "../texture/SVTexture.h"
#include "../logicsys/SVSceneMgr.h"
#include "../node/SVScene.h"

SVGameUIShow::SVGameUIShow(SVInst*_app):SVGBase(_app){
    
}

SVGameUIShow::~SVGameUIShow(){
    
}

void SVGameUIShow::init(const char* _strName){
    m_strName = _strName;
}

void SVGameUIShow::destroy(){
    UIELEMENTMAP::Iterator it = m_mapUIShow.getIterator();
    while (it.atEnd() == false) {
        SVNode* pNode = it->getValue();
        SAFE_DELETE(pNode);
        it++;
    }
}

void SVGameUIShow::enter(){
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if (t_sc == NULL){
        return;
    }
    UIELEMENTMAP::Iterator it = m_mapUIShow.getIterator();
    while (it.atEnd() == false) {
        SVNode* pNode = it->getValue();
        t_sc->addNode(pNode);
        it++;
    }
}

void SVGameUIShow::exit(){
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if (t_sc == NULL){
        return;
    }
    UIELEMENTMAP::Iterator it = m_mapUIShow.getIterator();
    while (it.atEnd() == false) {
        SVNode* pNode = it->getValue();
        t_sc->removeNode(pNode);
        it++;
    }
}

void SVGameUIShow::update(f32 _dt){
    
}

SVNode* SVGameUIShow::getUIWidgetByName(const char* _strName){
    UIELEMENTMAP::Node* it = m_mapUIShow.find(_strName);
    if (it != NULL) {
        return it->getValue();
    }
    return NULL;
}

void SVGameUIShow::createUI(SVUIParamBase* _param){
    if (strcmp(_param->m_WidgetName.c_str(),"SPINE") == 0 ){
        SVUIParamSpine* t_spineParam = dynamic_cast<SVUIParamSpine*>(_param);
        if (t_spineParam == NULL){
            return;
        }
        
        SVSpineNodeParam spineparam;
        spineparam.m_spinename = t_spineParam->m_SpineName;
        spineparam.m_aniname = "animation";
        spineparam.m_changeAni = true;
        spineparam.m_loop = t_spineParam->m_loop;
        spineparam.m_pos.set(t_spineParam->m_px, t_spineParam->m_py, t_spineParam->m_pz);
        spineparam.m_zorder = t_spineParam->m_zorder;
        SVSpineNode* t_aniSpineNode = (SVSpineNode*)spineparam.genNode(mApp);
        if (t_aniSpineNode){
            t_aniSpineNode->setUI(true);
            m_mapUIShow.insert(_param->m_secondName.c_str(),t_aniSpineNode);
        }
    }
    else if(strcmp(_param->m_WidgetName.c_str(),"FONT") == 0 ){
        SVUIParamFont* t_fontParam = dynamic_cast<SVUIParamFont*>(_param);
        if (t_fontParam == NULL){
            return;
        }
        SVBitFontNodeParam t_bmp_param;
        t_bmp_param.m_fonttex = stringc(t_fontParam->m_FontPngName);
        t_bmp_param.m_content = "0";
        t_bmp_param.m_fontw = t_fontParam->m_FontWidth;
        t_bmp_param.m_fonth = t_fontParam->m_FontHeight;
        t_bmp_param.m_pos.set(t_fontParam->m_px, t_fontParam->m_py, t_fontParam->m_pz);
        SVBitFontNode* t_bitFontNode = (SVBitFontNode*)t_bmp_param.genNode(mApp);
        t_bitFontNode->setUI(true);
        m_mapUIShow.insert(t_fontParam->m_secondName.c_str(),t_bitFontNode);
    }else if(strcmp(_param->m_WidgetName.c_str(),"PROGRESS") == 0 ){
        /*
        if( strcmp(_param->m_secondName.c_str(),"GameEnery") == 0 ){
            MaUIParamProgress* t_pro_param = (MaUIParamProgress*)_param;
            stringc t_spine_json = t_pro_param->m_SpineName + ".json";
            stringc t_spine_atlas = t_pro_param->m_SpineName + ".atlas";
            MaSpine *t_spine = MaSpine::createSpine(mApp, t_spine_json.c_str(), t_spine_atlas.c_str(), 1.0f);
            if(t_spine){
                m_uiProgress = new MaProgressNode(mApp);
                m_uiProgress->mainit();
                m_uiProgress->setSpine(t_spine);
                m_uiProgress->setloop(t_pro_param->m_loop);
                m_uiProgress->play(t_pro_param->m_AniName.c_str());
                m_gameProgressPos.set(t_pro_param->m_px, t_pro_param->m_py, t_pro_param->m_pz);
            }
        }*/
    }else if(strcmp(_param->m_WidgetName.c_str(),"SPRITE") == 0 ){
        SVUIParamSprite* t_spriteParam = dynamic_cast<SVUIParamSprite*>(_param);
        if (t_spriteParam == NULL){
            return;
        }
        
        SVSpriteNodeParam t_GenSprite_param;
        t_GenSprite_param.m_width = t_spriteParam->m_width;
        t_GenSprite_param.m_height = t_spriteParam->m_height;
        t_GenSprite_param.m_texname = t_spriteParam->m_TextureName.c_str();
        t_GenSprite_param.m_pos.set(t_spriteParam->m_px, t_spriteParam->m_py, t_spriteParam->m_pz);
        t_GenSprite_param.m_zorder = t_spriteParam->m_zorder;
        SVSpriteNode* t_spritNode = (SVSpriteNode*)(t_GenSprite_param.genNode(mApp));
        t_spritNode->setUI(true);
        m_mapUIShow.insert(t_spriteParam->m_secondName.c_str() , t_spritNode);
    }else if (strcmp(_param->m_WidgetName.c_str(),"COMBO_COM") == 0 ){
        SVGameCommonCombo* t_ComboUi = new SVGameCommonCombo(mApp);
        t_ComboUi->createUI(_param);
        t_ComboUi->setUI(true);
        m_mapUIShow.insert(_param->m_secondName.c_str() , t_ComboUi);
    }
}

const char* SVGameUIShow::getName(){
    return m_strName.c_str();
}

