//
// SVPhysicsSoftRigidWorld.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsSoftRigidWorld.h"
#include "../base/SVLock.h"

SVPhysicsSoftRigidWorld::SVPhysicsSoftRigidWorld(SVInst* _app):SVPhysicsWorldBase(_app) {
    m_type = E_PHYSICS_WORLD_SOFT_RIGID;
    m_broadPhase = nullptr;
    m_collisionConfiguration = nullptr;
    m_collisionDispatcher = nullptr;
    m_solver = nullptr;
    m_softWorld = nullptr;
}

SVPhysicsSoftRigidWorld::~SVPhysicsSoftRigidWorld() {
    m_broadPhase = nullptr;
    m_collisionConfiguration = nullptr;
    m_collisionDispatcher = nullptr;
    m_solver = nullptr;
    m_softWorld = nullptr;
}

void SVPhysicsSoftRigidWorld::init(){
    m_collisionConfiguration =  new btSoftBodyRigidBodyCollisionConfiguration();
    m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_broadPhase = new btDbvtBroadphase();
    m_solver =  new btSequentialImpulseConstraintSolver();
//    mSoftBodySolver
    m_softWorld =
    new btSoftRigidDynamicsWorld(m_collisionDispatcher, m_broadPhase, m_solver, m_collisionConfiguration);
    
}

void SVPhysicsSoftRigidWorld::destroy(){
    if (m_collisionConfiguration) {
        delete m_collisionConfiguration;
    }
    if (m_collisionDispatcher) {
        delete m_collisionDispatcher;
    }
    if (m_broadPhase) {
        delete m_broadPhase;
    }
    if (m_solver) {
        delete m_solver;
    }
    if (m_softWorld) {
        delete m_softWorld;
    }
}

void SVPhysicsSoftRigidWorld::update(f32 _dt){
   
}


