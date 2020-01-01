//
// SSVActionUnit.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActionUnit.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../act/SVActBase.h"
#include "SVActionMgr.h"
SVActionUnit::SVActionUnit(SVInst *_app)
:SVAniBase(_app) {
    m_actPtr = nullptr;
    m_nodePtr = nullptr;
    m_isEnd = false;
    m_state = SV_ACTION_STATE_WAIT;
}

SVActionUnit::SVActionUnit(SVInst* _app, SVActBasePtr _action, SVNodePtr _node):SVAniBase(_app){
    m_actPtr = _action;
    m_nodePtr = _node;
    m_isEnd = false;
    m_state = SV_ACTION_STATE_WAIT;
}


SVActionUnit::~SVActionUnit() {
    m_actPtr = nullptr;
    m_nodePtr = nullptr;
    m_state = SV_ACTION_STATE_WAIT;
}

void SVActionUnit::init(){
    
}

void SVActionUnit::destroy(){
    stop();
}

void SVActionUnit::update(f32 _dt) {
    if (m_actPtr && m_nodePtr && (m_state == SV_ACTION_STATE_PLAY)) {
        m_isEnd = m_actPtr->isEnd();
        m_actPtr->run(m_nodePtr, _dt);
    }
}

bool SVActionUnit::isEnd(){
    return m_isEnd;
}

SVActBasePtr SVActionUnit::getAct(){
    return m_actPtr;
}

SVNodePtr SVActionUnit::getNode(){
    return m_nodePtr;
}

void SVActionUnit::play(){
    if (m_actPtr && m_nodePtr) {
        m_isEnd = false;
        m_state = SV_ACTION_STATE_PLAY;
        m_actPtr->enter(m_nodePtr);
    }
}

void SVActionUnit::stop(){
    if (m_actPtr && m_nodePtr) {
        m_isEnd = true;
        m_state = SV_ACTION_STATE_STOP;
        m_actPtr->exit(m_nodePtr);
    }
}
