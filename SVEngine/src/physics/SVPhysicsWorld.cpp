//
// SVPhysicsWorld.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsWorld.h"
#include "bodies/SVPhysicsBody.h"

SVPhysicsWorld::SVPhysicsWorld(SVInst* _app):SVPhysicsBase(_app) {
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
    m_pSolver = new btSequentialImpulseConstraintSolver();
    m_pDynamicsWorld =
    new btDiscreteDynamicsWorld(m_pDispatcher,m_pOverlappingPairCache,m_pSolver,m_pCollisionConfiguration);
    m_pDynamicsWorld->setGravity(btVector3(0,-10,0));
//    test();
//    test2();
    
}

void SVPhysicsWorld::destroy(){
    m_bodyArray.clear();
    if(m_pCollisionConfiguration){
        free(m_pCollisionConfiguration);
        m_pCollisionConfiguration=nullptr;
    }
    if(m_pDispatcher){
        free(m_pDispatcher);
        m_pDispatcher=nullptr;
    }
    if(m_pOverlappingPairCache){
        free(m_pOverlappingPairCache);
        m_pOverlappingPairCache=nullptr;
    }
    if(m_pSolver){
        free(m_pSolver);
        m_pSolver=nullptr;
    }
    if(m_pDynamicsWorld){
        free(m_pDynamicsWorld);
        m_pDynamicsWorld=nullptr;
    }
}

void SVPhysicsWorld::update(f32 _dt){
    m_pDynamicsWorld->stepSimulation(1.0/150.0,10);
    for (int j=m_bodyArray.size()-1; j>=0; j--){
        m_bodyArray[j]->update(_dt);
    }
}

void SVPhysicsWorld::addBody(SVPhysicsBodyPtr _body){
    m_pDynamicsWorld->addRigidBody(_body->getBody());
    m_bodyArray.append(_body);
}

void SVPhysicsWorld::addShape(SVPhysicsShapePtr _shape, SVPhysicsBodyPtr _body){
    
}

void SVPhysicsWorld::addJoint(SVPhysicsJointPtr _joint){
    
}

void SVPhysicsWorld::test(){
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
    
    // 地面是在y=-56的位置上的一个立方体。
    // 球在y=-6处击中，中心为-5
    {
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(100.),btScalar(1.),btScalar(100.)));
        collisionShapes.push_back(groundShape);
        
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0.0,-10,0));
        
        btScalar mass(0.0);
        
        // 刚体是动态的如果且仅当质量为非零时，否则是静止的
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0.0,0,0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);
        
        // 使用MotionState是可选的，它提供了插值功能，并且只同步“活动”对象
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState, groundShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        body->setRestitution(btScalar(0.5));
        
        // 将物体添加到动力学世界
        m_pDynamicsWorld->addRigidBody(body);
        
    }
    
}

void SVPhysicsWorld::test2(){
    // 创建一个动态的刚体
    
     //btCollisionShape* colShape = new btBoxShape(btVector3(50,50,50));
    btCollisionShape* colShape = new btSphereShape(btScalar(50.));
    
    /// 创建动态对象
    btTransform startTransform;
    startTransform.setIdentity();
    
    btScalar mass(1.f);
    
    // 刚体是动态的如果且仅当质量为非零时，否则是静止的
    bool isDynamic = (mass != 0.f);
    
    btVector3 localInertia(0,0,0);
    if (isDynamic)
        colShape->calculateLocalInertia(mass,localInertia);
    
    startTransform.setOrigin(btVector3(10,300,0));
    
    // 推荐使用motionstate，它提供插值功能，只同步“活动”对象
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);
    body->setRestitution(btScalar(1.0));
    
    m_pDynamicsWorld->addRigidBody(body);
  
    
}
