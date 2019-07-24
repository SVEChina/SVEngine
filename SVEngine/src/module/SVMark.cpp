//
// SVMark.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMark.h"
#include "../node/SVNode.h"
#include "../base/SVLock.h"
#include "../base/SVPreDeclare.h"
#include "../basesys/SVSceneMgr.h"
#include "../node/SVScene.h"
#include "../node/SVNode.h"
#include "../core/SVBMFont.h"
#include "../node/SVBMFontNode.h"
#include "../base/SVPreDeclare.h"
#include "../act/SVActDeform.h"
#include "../act/SVActionSys.h"
#include "../app/SVGlobalMgr.h"
#include "../act/SVActionUnit.h"
#include "../file/SVFileMgr.h"
SVMark::SVMark(SVInst* _app)
:SVModuleBase(_app){
    m_bmFontNode = nullptr;
    m_actAlphaUnit= nullptr;
    m_actPositionUnit = nullptr;
    m_alpha = 1.0f;
    m_scale = 1.0f;
    m_rotation = 0.0f;
    m_alphaTime = 1.0f;
    m_appearTime = 5.0f;
    m_srcAlpha = 0.2f;
    m_tarAlpha = 0.0f;
    m_content = "ABCDEFG";
}

SVMark::~SVMark(){
    m_bmFontNode = nullptr;
    m_actAlphaUnit= nullptr;
    m_actPositionUnit = nullptr;
}

void SVMark::init(){
    SVModuleBase::init();
    SVString t_filePath = mApp->getFileMgr()->getFileFullName("svres/fontsan.fnt");
    SVBMFontPtr t_font = SVBMFont::creatFnt(t_filePath.c_str(), mApp);
    m_bmFontNode = MakeSharedPtr<SVBMFontNode>(mApp);
    m_bmFontNode->setFont(t_font);
    m_bmFontNode->setText(m_content);
    m_bmFontNode->setSpacing(1);
    m_bmFontNode->setPosition(0.0f, 0.0f, 0.0f);
}

void SVMark::destroy(){
    SVModuleBase::destroy();
}

void SVMark::open(){
    SVModuleBase::open();
    SVScenePtr t_scene = mApp->getSceneMgr()->getScene();
    if(m_bmFontNode && t_scene){
        t_scene->addNode(m_bmFontNode);
    }
    //
    SVActAlphaPtr t_actAlpha = MakeSharedPtr<SVActAlpha>(mApp);
    t_actAlpha->setSrcAlpha(m_srcAlpha);
    t_actAlpha->setTarAlpha(m_tarAlpha);
    t_actAlpha->setTime(m_alphaTime);
    m_actAlphaUnit = mApp->getActionSys()->runAction(t_actAlpha, m_bmFontNode);
    //
    SVActPositionPtr t_actPos = MakeSharedPtr<SVActPosition>(mApp);
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    t_actPos->setMinPosition(FVec3(-t_w*0.4f, -t_h*0.4f, 0.0f));
    t_actPos->setMaxPosition(FVec3(t_w*0.4f, t_h*0.4f, 0.0f));
    t_actPos->setTime(m_appearTime);
    m_actPositionUnit = mApp->getActionSys()->runAction(t_actPos, m_bmFontNode);
}

void SVMark::close() {
    SVModuleBase::close();
    if (m_bmFontNode) {
        m_bmFontNode->removeFromParent();
    }
    if (m_actAlphaUnit) {
        m_actAlphaUnit->stop();
    }
    if (m_actPositionUnit) {
        m_actPositionUnit->stop();
    }
}

bool SVMark::isOpen(){
    return SVModuleBase::isOpen();
}

void SVMark::update(f32 _dt) {
    SVModuleBase::update(_dt);
    SVActAlphaPtr t_actAlpha = DYN_TO_SHAREPTR(SVActAlpha, m_actAlphaUnit->getAct());
    SVActPositionPtr t_actPos = DYN_TO_SHAREPTR(SVActPosition, m_actPositionUnit->getAct());
    if (t_actAlpha && t_actPos && t_actPos->isEnd()) {
        t_actAlpha->reset();
        t_actPos->reset();
    }
}

void SVMark::setContent(SVString _content){
    if (m_content != _content) {
        m_content = _content;
        if (m_bmFontNode) {
            m_bmFontNode->setText(m_content);
        }
    }
}

void SVMark::setAlphaTime(f32 _time){
    if (m_alphaTime != _time) {
        m_alphaTime = _time;
        if (m_actAlphaUnit) {
            SVActAlphaPtr t_actAlpha = DYN_TO_SHAREPTR(SVActAlpha, m_actAlphaUnit->getAct());
            if (t_actAlpha) {
                t_actAlpha->setTime(m_alphaTime);
            }
        }
    }
}

void SVMark::setScale(f32 _scale){
    if (m_scale != _scale) {
        m_scale = _scale;
        if (m_bmFontNode) {
            m_bmFontNode->setScale(m_scale, m_scale, 1.0f);
        }
    }
}

void SVMark::setRotation(f32 _rot){
    if (m_rotation != _rot) {
        m_rotation = _rot;
        if (m_bmFontNode) {
            m_bmFontNode->setRotation(0.0f, 0.0f, _rot);
        }
    }
}

void SVMark::setAlpha(f32 _alpha){
    if (m_alpha != _alpha) {
        m_alpha = _alpha;
        if (m_bmFontNode) {
            m_bmFontNode->setAlpha(m_alpha);
        }
    }
}

void SVMark::setPosition(f32 _posX, f32 _posY, f32 _posZ){
    if (m_bmFontNode) {
        m_bmFontNode->setPosition(_posX, _posY, _posZ);
    }
}

void SVMark::setEnableRandomPosition(bool _enable){
    if (m_actPositionUnit) {
        SVActPositionPtr t_actPos = DYN_TO_SHAREPTR(SVActPosition, m_actPositionUnit->getAct());
        if (t_actPos) {
            t_actPos->setEnableRandom(_enable);
        }
    }
}

void SVMark::setAppearRate(f32 _time){
    if (m_appearTime != _time) {
        m_appearTime = _time;
        if (m_actPositionUnit) {
            SVActPositionPtr t_actPos = DYN_TO_SHAREPTR(SVActPosition, m_actPositionUnit->getAct());
            if (t_actPos) {
                t_actPos->setTime(m_appearTime);
            }
        }
    }
}

void SVMark::setAlphaRange(f32 _src, f32 _tar){
    SVActAlphaPtr t_actAlpha = nullptr;
    if (m_actAlphaUnit) {
        t_actAlpha = DYN_TO_SHAREPTR(SVActAlpha, m_actAlphaUnit->getAct());
    }
    if (m_srcAlpha != _src) {
        m_srcAlpha = _src;
        if (t_actAlpha) {
            t_actAlpha->setSrcAlpha(m_srcAlpha);
        }
    }
    if (m_tarAlpha != _tar) {
        m_tarAlpha = _tar;
        if (t_actAlpha) {
            t_actAlpha->setTarAlpha(m_tarAlpha);
        }
    }
}

bool SVMark::procEvent(SVEventPtr _event) {
    return  true;
}


