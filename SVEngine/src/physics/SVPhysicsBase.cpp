//
// SVPhysicsBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBase.h"

SVPhysicsBase::SVPhysicsBase(SVInst* _app):SVGBase(_app) {
    
}

SVPhysicsBase::~SVPhysicsBase() {
    
}

void SVPhysicsBase::setGravity(const FVec3 &_gravity){
    m_gravity = _gravity;
}

const FVec3& SVPhysicsBase::getGravity() const{
    return m_gravity;
}
