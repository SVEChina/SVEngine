//
// SVPhysicsWorld.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsWorld.h"
#include "bodies/SVPhysicsBodyRigid.h"
SVPhysicsWorld::SVPhysicsWorld(SVInst* _app):SVPhysicsWorldBase(_app) {
    m_type = E_PHYSICS_WORLD_RIGID;
    m_pCollisionConfiguration = nullptr;
    m_pDispatcher = nullptr;
    m_pOverlappingPairCache = nullptr;
    m_timeStep=0.0;
}

SVPhysicsWorld::~SVPhysicsWorld() {
    
}

void SVPhysicsWorld::init(){
    //创建世界
    //默认配置 可以自行配置
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    //默认调度程序 并行处理 可以考虑使用 参见 Extras/BulletMultiThreaded
    m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
    // btDbvtBroadphase是一种很好的通用的两步法碰撞检测。你也可以尝试btAxis3Sweep。
    m_pOverlappingPairCache = new btDbvtBroadphase();
    /// 默认约束求解器。对于并行处理，您可以使用不同的解决程序(参见Extras/BulletMultiThreaded)
    m_pSolver = new btSequentialImpulseConstraintSolver;
    m_pDynamicsWorld =
    new btDiscreteDynamicsWorld(m_pDispatcher,m_pOverlappingPairCache,m_pSolver,m_pCollisionConfiguration);
    m_pDynamicsWorld->setGravity(btVector3(0,-10,0));
    
}

void SVPhysicsWorld::destroy(){
    m_bodyArray.clear();
 
    if(m_pDynamicsWorld){
        delete m_pDynamicsWorld;
        m_pDynamicsWorld=nullptr;
    }
    
    if(m_pSolver){
        delete m_pSolver ;
        m_pSolver=nullptr;
    }

    if(m_pOverlappingPairCache){
        delete m_pOverlappingPairCache;
        m_pOverlappingPairCache=nullptr;
    }
    
    if(m_pDispatcher){
        delete m_pDispatcher;
        m_pDispatcher=nullptr;
    }
    
    if(m_pCollisionConfiguration){
        delete m_pCollisionConfiguration;
        m_pCollisionConfiguration=nullptr;
    }
}

void SVPhysicsWorld::update(f32 _dt){
    m_pDynamicsWorld->stepSimulation(1.f/30.f,5);
    for (int j=m_bodyArray.size()-1; j>=0; j--){
        m_bodyArray[j]->update(_dt);
    }
}

void SVPhysicsWorld::setp(){
     m_pDynamicsWorld->stepSimulation(1.f/30.f,5);
}

void SVPhysicsWorld::addBody(SVPhysicsBodyRigidPtr _body){
    m_lock->lock();
    m_pDynamicsWorld->addRigidBody(_body->getBody());
    m_bodyArray.append(_body);
    m_lock->unlock();
}

void SVPhysicsWorld::addShape(SVPhysicsShapePtr _shape, SVPhysicsBodyPtr _body){
    
}

void SVPhysicsWorld::addJoint(SVPhysicsJointPtr _joint){
    
}

void SVPhysicsWorld::addConstraint(btPoint2PointConstraint* _con){
    m_lock->lock();
    m_pDynamicsWorld->addConstraint(_con, true);
    m_lock->unlock();
}

void SVPhysicsWorld::removeConstraint(btPoint2PointConstraint* _con){
    m_lock->lock();
    m_pDynamicsWorld->removeConstraint(_con);
    m_lock->unlock();
}

