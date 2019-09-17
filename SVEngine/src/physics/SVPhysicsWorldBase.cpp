//
// SVPhysicsWorldBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsWorldBase.h"
SVPhysicsWorldBase::SVPhysicsWorldBase(SVInst* _app):SVGBase(_app) {
    m_type = E_PHYSICS_WORLD_BASE;
    m_lock= MakeSharedPtr<SVLock>();
}

SVPhysicsWorldBase::~SVPhysicsWorldBase() {
    m_lock = nullptr;
}

void SVPhysicsWorldBase::init(){
    
}

void SVPhysicsWorldBase::destroy(){
    
}

void SVPhysicsWorldBase::update(f32 _dt){
    
}

const SV_PHYSICSWORLDTYPE SVPhysicsWorldBase::getType() const{
    return m_type;
}

void SVPhysicsWorldBase::setGravity(const FVec3 &_gravity){
    m_gravity = _gravity;
}

const FVec3& SVPhysicsWorldBase::getGravity() const{
    return m_gravity;
}
