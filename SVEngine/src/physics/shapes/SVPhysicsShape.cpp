//
// SVPhysicsShape.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsShape.h"

SVPhysicsShape::SVPhysicsShape(SVInst* _app):SVPhysicsBase(_app) {
    m_type = E_PHYSICS_SHAPE_BASE;
}

SVPhysicsShape::~SVPhysicsShape() {
    
}

void SVPhysicsShape::init(){
    
}

void SVPhysicsShape::destroy(){
    
}

void SVPhysicsShape::update(f32 _dt){
    
}

PHYSICSSHAPETYPE SVPhysicsShape::getType() const{
    return m_type;
}
