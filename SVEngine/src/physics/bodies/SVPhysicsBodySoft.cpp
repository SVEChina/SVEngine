//
// SVPhysicsBodySoft.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBodySoft.h"

SVPhysicsBodySoft::SVPhysicsBodySoft(SVInst* _app):SVPhysicsBody(_app) {
    m_type = E_PHYSICS_BODY_SOFT;
    m_softBody = nullptr;
}

SVPhysicsBodySoft::~SVPhysicsBodySoft() {
    m_softBody = nullptr;
}

void SVPhysicsBodySoft::init(){
    
}

void SVPhysicsBodySoft::destroy(){
   
}

void SVPhysicsBodySoft::update(f32 _dt){
    
}

btSoftBody* SVPhysicsBodySoft::getBody(){
    return m_softBody;
}
