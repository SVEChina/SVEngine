//
// SVPhysicsWorld.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSWORLD_H
#define SV_PHYSICSWORLD_H

#include "SVPhysicsWorldBase.h"

namespace sv {
    
    namespace logic {
        
        class SVPhysicsWorld : public SVPhysicsWorldBase {
        public:
            SVPhysicsWorld(SVInst* _app);
            
            ~SVPhysicsWorld();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void update(f32 _dt);
            
            void addBody(SVPhysicsBodyRigidPtr _body);
            
            void removeBody(SVPhysicsBodyRigidPtr _body);
            
            void addShape(SVPhysicsShapePtr _shape, SVPhysicsBodyPtr _body);
            
            void addJoint(SVPhysicsJointPtr _joint);
            
            void addConstraint(btPoint2PointConstraint* _con);
            
            void removeConstraint(btPoint2PointConstraint* _con);
            
            void setp();
            
            inline void setFps(f32 _timeStep){
                m_timeStep = _timeStep;
            }
            
            
        public :
            btDefaultCollisionConfiguration* m_pCollisionConfiguration;
            btCollisionDispatcher* m_pDispatcher;
            btBroadphaseInterface* m_pOverlappingPairCache;
            btSequentialImpulseConstraintSolver* m_pSolver;
            btDiscreteDynamicsWorld* m_pDynamicsWorld;
        protected:
            f32 m_timeStep;
            SVArray<SVPhysicsBodyRigidPtr> m_bodyArray;
           // BODYARRAY m_bodyArray;
            
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICSWORLD_H
