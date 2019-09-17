//
// SVPhysicsWorldBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICS_WORLDBASE_H
#define SV_PHYSICS_WORLDBASE_H

#include "../base/SVGBase.h"
#include "../base/SVLock.h"
#include "../base/SVVec3.h"
#include "btBulletDynamicsCommon.h"
#define PHYSICSWORLDSTEP 1.0f/30.0f
namespace sv {
    
    namespace logic {
        enum SV_PHYSICSWORLDTYPE {
            E_PHYSICS_WORLD_BASE = 0,
            E_PHYSICS_WORLD_RIGID,
            E_PHYSICS_WORLD_SOFT_RIGID
        };
        class SVPhysicsWorldBase : public SVGBase {
        public:
            SVPhysicsWorldBase(SVInst* _app);
            
            ~SVPhysicsWorldBase();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void update(f32 _dt);
            
            const SV_PHYSICSWORLDTYPE getType() const;
            
            void setGravity(const FVec3 &_gravity);
            
            const FVec3& getGravity() const;
            
        protected:
            SV_PHYSICSWORLDTYPE m_type;
            FVec3 m_gravity;
            SVLockPtr m_lock;
        };
            
    }//!namespace logic
    
}//!namespace sv

#endif //SV_PHYSICS_WORLDBASE_H
