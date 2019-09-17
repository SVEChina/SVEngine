//
// SVPhysicsSoftBody.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICS_SOFTBODY_H
#define SV_PHYSICS_SOFTBODY_H

#include "SVPhysicsBody.h"
namespace sv {

    namespace logic {
        class SVPhysicsSoftBody : public SVPhysicsBody {
        public:
            SVPhysicsSoftBody(SVInst *_app);

            ~SVPhysicsSoftBody();

            virtual void init();

            virtual void destroy();

            virtual void update(f32 _dt);

        };

    }//!namespace logic

}//!namespace sv



#endif //SV_PHYSICS_SOFTBODY_H
