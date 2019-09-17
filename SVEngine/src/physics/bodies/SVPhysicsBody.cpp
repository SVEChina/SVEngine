//
// SVPhysicsBody.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBody.h"

SVPhysicsBody::SVPhysicsBody(SVInst* _app):SVGBase(_app) {
    m_type = E_PHYSICS_BODY_BASE;
    m_pNode = nullptr;
}

SVPhysicsBody::~SVPhysicsBody() {
    m_pNode = nullptr;
}

void SVPhysicsBody::init(){
    
}

void SVPhysicsBody::destroy(){
    
}

void SVPhysicsBody::update(f32 _dt){
    
}

PHYSICSBODYTYPE SVPhysicsBody::getType() const{
    return m_type;
}

void SVPhysicsBody::setNode(SVNodePtr _node) {
    if (_node) {
        m_pNode = _node;
    }
}

SVNodePtr SVPhysicsBody::getNode(){
    return m_pNode;
}
