//
// SVPhysicsShapeBox.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsShapeBox.h"

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
