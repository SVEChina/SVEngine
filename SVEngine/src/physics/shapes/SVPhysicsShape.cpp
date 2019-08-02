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
    if(m_pColShape){
        free(m_pColShape);
        m_pColShape = nullptr;
    }
}

void SVPhysicsShape::update(f32 _dt){
    
}

btCollisionShape* SVPhysicsShape::getShape(){
    return m_pColShape;
}

PHYSICSSHAPETYPE SVPhysicsShape::getType() const{
    return m_type;
}
