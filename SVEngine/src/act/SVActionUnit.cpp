//
// SSVActionUnit.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActionUnit.h"
#include "../act/SVActBase.h"
SVActionUnit::SVActionUnit(SVInst *_app)
:SVGBase(_app) {
    m_actPtr = nullptr;
    m_nodePtr = nullptr;
}

SVActionUnit::~SVActionUnit() {
    m_actPtr = nullptr;
    m_nodePtr = nullptr;
}

void SVActionUnit::init() {
   
}

void SVActionUnit::destroy() {
    
}

void SVActionUnit::enter(){
    if (m_actPtr && m_nodePtr) {
        m_actPtr->enter(m_nodePtr);
    }
}

void SVActionUnit::exit(){
    if (m_actPtr && m_nodePtr) {
        m_actPtr->exit(m_nodePtr);
    }
}

void SVActionUnit::update(f32 _dt) {
    
    if (m_actPtr && m_nodePtr) {
        m_actPtr->run(m_nodePtr, _dt);
    }

}

void SVActionUnit::setAct(SVActBasePtr _actPtr){
    if (m_actPtr != _actPtr) {
        m_actPtr = _actPtr;
    }
}

void SVActionUnit::setNode(SVNodePtr _nodePtr){
    if (m_nodePtr != _nodePtr) {
        m_nodePtr = _nodePtr;
    }
}
