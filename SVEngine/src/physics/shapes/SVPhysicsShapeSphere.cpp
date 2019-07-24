//
// SVPhysicsShapeSphere.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsShapeSphere.h"

SVPhysicsShapeSphere::SVPhysicsShapeSphere(SVInst* _app):SVPhysicsShape(_app) {
    m_type = E_PHYSICS_SHAPE_SPHERE;
    setRadius(0.0f);
}

SVPhysicsShapeSphere::SVPhysicsShapeSphere(SVInst* _app, f32 _radius):SVPhysicsShape(_app) {
    m_type = E_PHYSICS_SHAPE_SPHERE;
    setRadius(_radius);
}

SVPhysicsShapeSphere::~SVPhysicsShapeSphere() {
    
}

void SVPhysicsShapeSphere::init(){
    
}

void SVPhysicsShapeSphere::destroy(){
    
}

void SVPhysicsShapeSphere::update(f32 _dt){
    
}

void SVPhysicsShapeSphere::setRadius(const f32 _radius){
    m_radius = _radius;
}

const f32 SVPhysicsShapeSphere::getRadius() const{
    return m_radius;
}
