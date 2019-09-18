//
// SVPhysicsBodySoft.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICS_BODYSOFT_H
#define SV_PHYSICS_BODYSOFT_H


#include "SVPhysicsBody.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBody.h"
namespace sv {

    namespace logic {
        class SVPhysicsBodySoft : public SVPhysicsBody {
        public:
            SVPhysicsBodySoft(SVInst *_app);

            ~SVPhysicsBodySoft();

            virtual void init();

            virtual void destroy();

            virtual void update(f32 _dt);
            
            btSoftBody* getBody();
        protected:
            btSoftBody* m_softBody;
        };

    }//!namespace logic

}//!namespace sv
#endif //SV_PHYSICS_BODYSOFT_H
