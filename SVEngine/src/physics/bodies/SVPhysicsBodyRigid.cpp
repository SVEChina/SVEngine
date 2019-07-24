//
// SVPhysicsBodyRigid.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBodyRigid.h"

SVPhysicsBodyRigid::SVPhysicsBodyRigid(SVInst* _app):SVPhysicsBody(_app) {
    m_type = E_PHYSICS_BODY_RIGID;
}

SVPhysicsBodyRigid::~SVPhysicsBodyRigid() {
    
}

void SVPhysicsBodyRigid::init(){
    
}

void SVPhysicsBodyRigid::destroy(){
    
}

void SVPhysicsBodyRigid::update(f32 _dt){
    
}

PHYSICSBODYTYPE SVPhysicsBodyRigid::getType(){
    return m_type;
}
