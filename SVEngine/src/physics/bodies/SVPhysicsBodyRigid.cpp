//
// SVPhysicsBodyRigid.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBodyRigid.h"
#include "../node/SVNode.h"
#include "../shapes/SVPhysicsShape.h"

SVPhysicsBodyRigid::SVPhysicsBodyRigid(SVInst* _app):SVPhysicsBody(_app) {
    m_type = E_PHYSICS_BODY_RIGID;
    m_pShape = nullptr;
    m_pMyMotionState = nullptr;
}

SVPhysicsBodyRigid::SVPhysicsBodyRigid(SVInst* _app , SVPhysicsShapePtr _shape):SVPhysicsBody(_app) {
    m_type = E_PHYSICS_BODY_RIGID;
    m_pShape = _shape;
    m_pBody = nullptr;
    m_pMyMotionState = nullptr;
}

SVPhysicsBodyRigid::~SVPhysicsBodyRigid() {
    
}

void SVPhysicsBodyRigid::init(){
    btTransform t_startTransform;
    t_startTransform.setIdentity();
    t_startTransform.setOrigin(btVector3(m_origin.x,m_origin.y,m_origin.z));
    // 推荐使用motionstate，它提供插值功能，只同步“活动”对象
    m_pMyMotionState = new btDefaultMotionState(t_startTransform);
    btVector3 t_localInertia(m_pShape->getLocalInertia().x,m_pShape->getLocalInertia().y,m_pShape->getLocalInertia().z);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_pShape->getMass(),m_pMyMotionState,m_pShape->getShape(),t_localInertia);
    m_pBody = new btRigidBody(rbInfo);
    m_pBody->setRestitution(btScalar(0.6));
}

void SVPhysicsBodyRigid::destroy(){
    if(m_pShape){
        m_pShape->destroy();
        m_pShape = nullptr;
    }
    
    if(m_pMyMotionState){
        free(m_pMyMotionState);
        m_pMyMotionState = nullptr;
    }

}

void SVPhysicsBodyRigid::update(f32 _dt){
    btTransform trans;
    if (m_pBody && m_pBody->getMotionState()){
         m_pBody->getMotionState()->getWorldTransform(trans);
    }else{
        trans = m_pBody->getWorldTransform();
    }
    
    if(m_pNode){
        m_pNode->setPosition(trans.getOrigin().getX(), trans.getOrigin().getY()*250, trans.getOrigin().getZ());
        btQuaternion t_bodyquat= m_pBody->getOrientation();
        SVQuat t_quat=SVQuat(t_bodyquat.getX(),t_bodyquat.getY(),t_bodyquat.getZ(),t_bodyquat.getW());
        m_pNode->setQuat(t_quat);
        printf("world pos  = %f,%f,%f\n", trans.getOrigin().getX(),
               trans.getOrigin().getY(),
               trans.getOrigin().getZ());
    }
}

PHYSICSBODYTYPE SVPhysicsBodyRigid::getType(){
    return m_type;
}

void SVPhysicsBodyRigid::setApplyCentralForce(FVec3 _pos){
    if(m_pBody){
        m_pBody->applyCentralForce(btVector3(_pos.x,_pos.y,_pos.z));
    }
}

void SVPhysicsBodyRigid::setApplyTorque(FVec3 _pos){
    if(m_pBody){
        m_pBody->applyTorque(btVector3(_pos.x,_pos.y,_pos.z));
    }
}
