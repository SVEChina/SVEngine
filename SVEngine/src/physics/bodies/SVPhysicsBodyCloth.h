//
// SVPhysicsBodyCloth.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BODIES_PHYSICSBODYCLOTH_H
#define SV_BODIES_PHYSICSBODYCLOTH_H
#include "SVPhysicsBodySoft.h"
#include "../../base/SVVec3.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsBodyCloth : public SVPhysicsBodySoft {
        public:
            SVPhysicsBodyCloth(SVInst* _app, btSoftBodyWorldInfo& _worldInfo, const FVec3& _corner00,
                               const FVec3& _corner10,
                               const FVec3& _corner01,
                               const FVec3& _corner11,
                               s32 _resx,
                               s32 _resy,
                               s32 _fixeds,
                               bool gendiags);
            
            ~SVPhysicsBodyCloth();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void update(f32 _dt);
            
        protected:
            
            
        };
    }//!namespace logic
}//!namespace sv

#endif //SV_BODIES_PHYSICSBODYCLOTH_H
