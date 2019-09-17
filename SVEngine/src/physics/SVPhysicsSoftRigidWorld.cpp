//
// SVPhysicsSoftRigidWorld.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsSoftRigidWorld.h"
#include "bodies/SVPhysicsBodySoft.h"
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
    removeAllSoftBodies();
    //
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
    if (m_softWorld) {
        m_softWorld->stepSimulation(PHYSICSWORLDSTEP);
    }
    for (s32 i = 0; i < m_bodies.size(); i++) {
        SVPhysicsBodySoftPtr t_body = m_bodies[i];
        t_body->update(_dt);
    }
}

void SVPhysicsSoftRigidWorld::addSoftBody(SVPhysicsBodySoftPtr _body){
    if (!_body || !m_softWorld) {
        return;
    }
    if (!_hasSoftBody(_body)) {
        m_lock->lock();
        m_softWorld->addSoftBody(_body->getBody());
        m_bodies.append(_body);
        m_lock->unlock();
    }
}

bool SVPhysicsSoftRigidWorld::removeSoftBody(SVPhysicsBodySoftPtr _body){
    if (!_body || !m_softWorld) {
        return false;
    }
    bool t_ret = false;
    if (_hasSoftBody(_body)) {
        m_lock->lock();
        for (s32 i = 0; i < m_bodies.size(); i++) {
            SVPhysicsBodySoftPtr t_body = m_bodies[i];
            if (t_body == _body) {
                t_body->destroy();
                m_bodies.removeForce(i);
                t_ret = true;
                break;
            }
        }
        if (t_ret) {
            m_softWorld->removeSoftBody(_body->getBody());
        }
        m_lock->unlock();
    }
    return t_ret;
}

bool SVPhysicsSoftRigidWorld::removeAllSoftBodies(){
    bool t_ret = false;
    if (m_bodies.size() > 0) {
        t_ret = true;
        for (s32 i = 0; i<m_bodies.size(); i++) {
            SVPhysicsBodySoftPtr t_body = m_bodies[i];
            t_body->destroy();
        }
        m_bodies.destroy();
    }
    return t_ret;
}

bool SVPhysicsSoftRigidWorld::_hasSoftBody(SVPhysicsBodySoftPtr _body){
    bool t_ret = false;
    m_lock->lock();
    for (s32 i = 0; i<m_bodies.size(); i++) {
        SVPhysicsBodySoftPtr t_body = m_bodies[i];
        if (t_body == _body) {
            t_ret = true;
            break;
        }
    }
    m_lock->unlock();
    return t_ret;
}
