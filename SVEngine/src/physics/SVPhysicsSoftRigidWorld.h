//
// SVPhysicsSoftRigidWorld.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICS_SOFTRIGIDWORLD_H
#define SV_PHYSICS_SOFTRIGIDWORLD_H

#include "SVPhysicsWorldBase.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsSoftRigidWorld : public SVPhysicsWorldBase {
        public:
            SVPhysicsSoftRigidWorld(SVInst* _app);
            
            ~SVPhysicsSoftRigidWorld();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
//            void addSoftBody()
        private:
            btBroadphaseInterface       *m_broadPhase;
            btCollisionConfiguration    *m_collisionConfiguration;
            btCollisionDispatcher       *m_collisionDispatcher;
            btConstraintSolver          *m_solver;
            btSoftRigidDynamicsWorld    *m_softWorld;
            f32                          m_stepVal;
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICS_SOFTRIGIDWORLD_H
