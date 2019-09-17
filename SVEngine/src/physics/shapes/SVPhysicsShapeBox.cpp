//
// SVPhysicsShapeBox.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsShapeBox.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
SVPhysicsShapeBox::SVPhysicsShapeBox(SVInst* _app):SVPhysicsShape(_app) {
    m_type = E_PHYSICS_SHAPE_BOX;
    setSize(FVec3_zero);
}

SVPhysicsShapeBox::SVPhysicsShapeBox(SVInst* _app, FVec3 _size):SVPhysicsShape(_app) {
    m_type = E_PHYSICS_SHAPE_BOX;
    setSize(_size);
}

SVPhysicsShapeBox::~SVPhysicsShapeBox() {
    
}

void SVPhysicsShapeBox::init(){
    m_pColShape = new btBoxShape(btVector3(m_size.x,m_size.y,m_size.z));
    // 刚体是动态的如果且仅当质量为非零时，否则是静止的
    bool t_isDynamic = (m_mass != 0.f);
    btVector3 t_localInertia(m_localInertia.x,m_localInertia.y,m_localInertia.z);
    if (t_isDynamic){
        m_pColShape->calculateLocalInertia(m_mass,t_localInertia);
    }
}

void SVPhysicsShapeBox::destroy(){
    
}

void SVPhysicsShapeBox::update(f32 _dt){
    
}

void SVPhysicsShapeBox::setSize(const FVec3& _size){
    m_size = _size;
}

const FVec3& SVPhysicsShapeBox::getSize() const{
    return m_size;
}
