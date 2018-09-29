//
// Created by yinlong on 17/10/20.
//

#include "SVGameUIBase.h"
#include "SVGameUIMgr.h"
#include "SVGameUIShow.h"

SVGameUIBase::SVGameUIBase(SVInst*_app)
:SVEventProc(_app)
,m_pUIMgr(NULL)
,m_pUIShow(NULL){
    
}

SVGameUIBase::~SVGameUIBase() {
}

void SVGameUIBase::init(SVGameUIMgr* pUIMgr){
    m_pUIMgr = pUIMgr;
}

void SVGameUIBase::destroy(){
    if (m_pUIShow){
        m_pUIShow->destroy();
    }
}

void SVGameUIBase::enter(){
    if (m_pUIMgr == NULL){
        return;
    }
    m_pUIShow = m_pUIMgr->getUI(m_strUIName.c_str());
    if (m_pUIShow){
        m_pUIShow->enter();
    }
}

void SVGameUIBase::exit(){
    if (m_pUIShow){
        m_pUIShow->exit();
    }
}

void SVGameUIBase::update(float _dt){
}

void SVGameUIBase::setUIName(const char* pUiName){
    m_strUIName = pUiName;
}

