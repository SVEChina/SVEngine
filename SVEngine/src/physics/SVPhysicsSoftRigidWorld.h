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
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void update(f32 _dt);
            
            void addSoftBody(SVPhysicsBodySoftPtr _body);
            
            bool removeSoftBody(SVPhysicsBodySoftPtr _body);
        protected:
            bool _hasSoftBody(SVPhysicsBodySoftPtr _body);
        private:
            btBroadphaseInterface       *m_broadPhase;
            btCollisionConfiguration    *m_collisionConfiguration;
            btCollisionDispatcher       *m_collisionDispatcher;
            btConstraintSolver          *m_solver;
            btSoftRigidDynamicsWorld    *m_softWorld;
            typedef SVArray<SVPhysicsBodySoftPtr> SOFTBODYPOOL;
            SOFTBODYPOOL m_bodies;
            f32                          m_stepVal;
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICS_SOFTRIGIDWORLD_H
