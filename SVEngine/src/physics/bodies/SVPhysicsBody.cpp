//
// SVPhysicsBody.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBody.h"

SVPhysicsBody::SVPhysicsBody(SVInst* _app):SVPhysicsBase(_app) {
    m_type = E_PHYSICS_BODY_BASE;
}

SVPhysicsBody::~SVPhysicsBody() {
    
}

void SVPhysicsBody::init(){
    
}

void SVPhysicsBody::destroy(){
    
}

void SVPhysicsBody::update(f32 _dt){
    
}

PHYSICSBODYTYPE SVPhysicsBody::getType(){
    return m_type;
}
