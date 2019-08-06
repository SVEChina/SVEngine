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
    m_pColShape = nullptr;
}

SVPhysicsShapeSphere::SVPhysicsShapeSphere(SVInst* _app, f32 _radius):SVPhysicsShape(_app) {
    m_type = E_PHYSICS_SHAPE_SPHERE;
    setRadius(_radius);
    m_pColShape = nullptr;
}

SVPhysicsShapeSphere::~SVPhysicsShapeSphere() {
    
}

void SVPhysicsShapeSphere::init(){
    m_pColShape = new btSphereShape(btScalar(m_radius));
    // 刚体是动态的如果且仅当质量为非零时，否则是静止的
    bool t_isDynamic = (m_mass != 0.f);
    btVector3 t_localInertia(m_localInertia.x,m_localInertia.y,m_localInertia.z);
    if (t_isDynamic)
        m_pColShape->calculateLocalInertia(m_mass,t_localInertia);
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
