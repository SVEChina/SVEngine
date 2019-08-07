//
// SVPhysicsBodyRigid.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBodyRigid.h"
#include "../../node/SVNode.h"
#include "../shapes/SVPhysicsShape.h"
#include "../../app/SVGlobalMgr.h"
#include "../SVPhysicsWorld.h"

SVPhysicsBodyRigid::SVPhysicsBodyRigid(SVInst* _app):SVPhysicsBody(_app) {
    m_type = E_PHYSICS_BODY_RIGID;
    m_pShape = nullptr;
    m_pBody=nullptr;
    m_pMyMotionState = nullptr;
    p2p=nullptr;
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
    bool t_isDynamic = (m_pShape->getMass()!= 0.f);
    btVector3 t_localInertia(m_pShape->getLocalInertia().x,m_pShape->getLocalInertia().y,m_pShape->getLocalInertia().z);
    if (t_isDynamic){
        m_pShape->getShape()->calculateLocalInertia(m_pShape->getMass(),t_localInertia);
    }
    m_pMyMotionState = new btDefaultMotionState(t_startTransform);
   // btVector3 t_localInertia(m_pShape->getLocalInertia().x,m_pShape->getLocalInertia().y,m_pShape->getLocalInertia().z);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_pShape->getMass(),m_pMyMotionState,m_pShape->getShape(),t_localInertia);
    m_pBody = new btRigidBody(rbInfo);
    m_pBody->setUserIndex(-1);
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
        m_pNode->setPosition(trans.getOrigin().getX()*250, trans.getOrigin().getY()*250-640, trans.getOrigin().getZ()*250);
        btQuaternion t_bodyquat = trans.getRotation();
        SVQuat t_quat=SVQuat(FVec4(t_bodyquat[0],t_bodyquat[1],t_bodyquat[2],t_bodyquat[3]));
        m_pNode->setQuat(t_quat);
        printf("world pos  = %f,%f,%f\n", trans.getOrigin().getX(),
               trans.getOrigin().getY(),
               trans.getOrigin().getZ());

        printf("world t_quat  = %f,%f,%f,%f\n", t_bodyquat.getX(),
               t_bodyquat.getY(),
               t_bodyquat.getZ(),t_bodyquat.getAngle());
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

void SVPhysicsBodyRigid::addConstraint(){
  //  int m_savedState=m_pBody->getActivationState();
//m_pBody->setActivationState(DISABLE_DEACTIVATION);
    //printf("pickPos=%f,%f,%f\n",pickPos.getX(),pickPos.getY(),pickPos.getZ());
    btVector3 localPivot = btVector3(0.03,0.1,0.1);
    p2p = new btPoint2PointConstraint(*m_pBody, localPivot);
    mApp->m_pGlobalMgr->m_pPhysics->addConstraint(p2p);
    btScalar mousePickClamping = 30.f;
    p2p->m_setting.m_impulseClamp = mousePickClamping;
    p2p->m_setting.m_tau = 0.001f;
    btVector3 newPivotB(0,3,0);
    p2p->setPivotB(newPivotB);
    //m_pBody->setActivationState(m_savedState);
}

void SVPhysicsBodyRigid::removeConstraint(){
    if(p2p){
        mApp->m_pGlobalMgr->m_pPhysics->removeConstraint(p2p);
        p2p=nullptr;
    }
}
