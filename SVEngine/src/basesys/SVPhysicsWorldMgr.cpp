//
// SVPhysicsWorldMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPhysicsWorldMgr.h"
#include "../physics/SVPhysicsWorldBase.h"
#include "../physics/SVPhysicsSoftRigidWorld.h"
#include "../physics/SVPhysicsWorld.h"
SVPhysicsWorldMgr::SVPhysicsWorldMgr(SVInst *_app)
:SVSysBase(_app) {
    m_subsysType = "SVPhysicsWorldMgr";
    m_rigidWorld = nullptr;
    m_softWorld = nullptr;
}

SVPhysicsWorldMgr::~SVPhysicsWorldMgr() {
    m_rigidWorld = nullptr;
    m_softWorld = nullptr;
}

void SVPhysicsWorldMgr::init() {
    m_rigidWorld = MakeSharedPtr<SVPhysicsWorld>(mApp);
    m_rigidWorld->init();
    m_softWorld = MakeSharedPtr<SVPhysicsSoftRigidWorld>(mApp);
    m_softWorld->init();
}

void SVPhysicsWorldMgr::destroy() {
    if (m_rigidWorld) {
        m_rigidWorld->destroy();
    }
    if (m_softWorld) {
        m_softWorld->destroy();
    }
}

void SVPhysicsWorldMgr::update(f32 _dt) {
    if (m_rigidWorld) {
        m_rigidWorld->update(_dt);
    }
    if (m_softWorld) {
        m_softWorld->update(_dt);
    }
}

void SVPhysicsWorldMgr::setRigidWorld(SVPhysicsWorldPtr _world){
    if (_world) {
        m_rigidWorld = _world;
    }
}

void SVPhysicsWorldMgr::setSoftWorld(SVPhysicsSoftRigidWorldPtr _world){
    if (_world) {
        m_softWorld = _world;
    }
}

SVPhysicsWorldPtr SVPhysicsWorldMgr::getRigidWorld(){
    return m_rigidWorld;
}

SVPhysicsSoftRigidWorldPtr SVPhysicsWorldMgr::getSoftWorld(){
    return m_softWorld;
}
