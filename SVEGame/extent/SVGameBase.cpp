//
// Created by lidm on 17/9/5.
//

#include "SVGameBase.h"
#include "SVGameUIMgr.h"

SVGameBase::SVGameBase(SVInst* _app)
:SVEventProc(_app) ,m_pUIMgr(NULL){
    m_pUIMgr = new SVGameUIMgr(_app);
}

SVGameBase::~SVGameBase() {
    SAFE_DELETE(m_pUIMgr);
}

void SVGameBase::init(){
}

void SVGameBase::destroy(){
}

void SVGameBase::update(float _dt){
}

SVGameUIMgr* SVGameBase::getUIMgr(){
    return m_pUIMgr;
}

void SVGameBase::setCallback(cb_func_game _cb){
    m_pCB = _cb;
}

cb_func_game SVGameBase::getCallback(){
    return m_pCB;
}
