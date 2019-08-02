//
// SVPhysicsBody.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBody.h"

SVPhysicsBody::SVPhysicsBody(SVInst* _app):SVPhysicsBase(_app) {
    m_type = E_PHYSICS_BODY_BASE;
    m_origin=FVec3(0.0);
    m_pNode = nullptr;
}

SVPhysicsBody::~SVPhysicsBody() {
    
}

void SVPhysicsBody::init(){
    
}

void SVPhysicsBody::destroy(){
    if(m_pBody){
        free(m_pBody);
        m_pBody = nullptr;
    }
    m_pNode = nullptr;
    
}

void SVPhysicsBody::update(f32 _dt){
    
}

btRigidBody* SVPhysicsBody::getBody(){
    return m_pBody;
}

PHYSICSBODYTYPE SVPhysicsBody::getType() const{
    return m_type;
}
