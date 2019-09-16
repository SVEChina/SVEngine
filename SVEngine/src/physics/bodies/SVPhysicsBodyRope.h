//
// SVPhysicsBodyRope.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BODIES_PHYSICSBODYROPE_H
#define SV_BODIES_PHYSICSBODYROPE_H
#include "SVPhysicsBody.h"
#include "btSoftBodyRigidBodyCollisionConfiguration.h"
#include "btSoftBodyHelpers.h"
#include "btSoftBody.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsBodyRope : public SVPhysicsBody {
        public:
            SVPhysicsBodyRope(SVInst* _app, btSoftBodyWorldInfo& _worldInfo, FVec3 &_from, FVec3 &_to, s32 _resCount, s32 _fixeds = 1+2);
            
            ~SVPhysicsBodyRope();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void update(f32 _dt);
            
            PHYSICSBODYTYPE getType();
            
        protected:
            btSoftBody* m_softBody;
            
        };
    }//!namespace logic
}//!namespace sv

#endif //SV_BODIES_PHYSICSBODYROPE_H
