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
    //
    m_actAlphaUnit = MakeSharedPtr<SVActionUnit>(mApp);
    m_actAlphaUnit->init();
    SVActAlphaPtr t_actAlpha = MakeSharedPtr<SVActAlpha>(mApp);
    t_actAlpha->setSrcAlpha(m_srcAlpha);
    t_actAlpha->setTarAlpha(m_tarAlpha);
    t_actAlpha->setTime(m_alphaTime);
    m_actAlphaUnit->setAct(t_actAlpha);
    m_actAlphaUnit->setNode(m_bmFontNode);
    //
    m_actPositionUnit = MakeSharedPtr<SVActionUnit>(mApp);
    m_actPositionUnit->init();
    SVActRandomPositionPtr t_actPos = MakeSharedPtr<SVActRandomPosition>(mApp);
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    t_actPos->setMinPosition(FVec3(-t_w*0.4f, -t_h*0.4f, 0.0f));
    t_actPos->setMaxPosition(FVec3(t_w*0.4f, t_h*0.4f, 0.0f));
    t_actPos->setTime(m_appearTime);
    m_actPositionUnit->setAct(t_actPos);
    m_actPositionUnit->setNode(m_bmFontNode);
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
    SVActionSysPtr t_actSys = mApp->m_pGlobalMgr->m_pActionSys;
    if (m_actAlphaUnit && t_actSys) {
        m_actAlphaUnit->enter();
        t_actSys->addActionUnit(m_actAlphaUnit);
    }
    if (m_actPositionUnit && t_actSys) {
        m_actPositionUnit->enter();
        t_actSys->addActionUnit(m_actPositionUnit);
    }
}

void SVMark::close() {
    SVModuleBase::close();
    if (m_bmFontNode) {
        m_bmFontNode->removeFromParent();
    }
    SVActionSysPtr t_actSys = mApp->m_pGlobalMgr->m_pActionSys;
    if (m_actAlphaUnit && t_actSys) {
        t_actSys->removeActionUnit(m_actAlphaUnit);
        m_actAlphaUnit->destroy();
    }
    if (m_actPositionUnit && t_actSys) {
        t_actSys->removeActionUnit(m_actPositionUnit);
        m_actPositionUnit->destroy();
    }
}

bool SVMark::isOpen(){
    return SVModuleBase::isOpen();
}

void SVMark::update(f32 _dt) {
    SVModuleBase::update(_dt);
    if (m_actPositionUnit && m_actPositionUnit->isEnd()) {
        
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

void SVMark::setAppearRate(f32 _time){
    if (m_appearTime != _time) {
        m_appearTime = _time;
        if (m_actPositionUnit) {
            SVActRandomPositionPtr t_actPos = DYN_TO_SHAREPTR(SVActRandomPosition, m_actPositionUnit->getAct());
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


