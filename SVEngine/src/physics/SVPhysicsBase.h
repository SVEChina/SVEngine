//
// SVPhysicsBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSBASE_H
#define SV_PHYSICSBASE_H

#include "../base/SVGBase.h"
#include "../base/SVVec3.h"
#include "btBulletDynamicsCommon.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsBase : public SVGBase {
        public:
            SVPhysicsBase(SVInst* _app);
            
            ~SVPhysicsBase();
            
            void setGravity(const FVec3 &_gravity);
            
            const FVec3& getGravity() const;
            
        protected:
            FVec3 m_gravity;
            
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICSBASE_H
