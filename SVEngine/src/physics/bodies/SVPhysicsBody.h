//
// SVPhysicsBody.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSBODY_H
#define SV_PHYSICSBODY_H

#include "../SVPhysicsBase.h"
#include "../../node/SVNodeDef.h"

namespace sv {

    namespace logic {
        enum PHYSICSBODYTYPE {
            E_PHYSICS_BODY_BASE = 0,
            E_PHYSICS_BODY_RIGID
        };

        class SVPhysicsBody : public SVPhysicsBase {
        public:
            SVPhysicsBody(SVInst *_app);

            ~SVPhysicsBody();

            virtual void init();

            virtual void destroy();

            virtual void update(f32 _dt);

            void setNode(SVNodePtr _node);

            inline void setOrigin(FVec3 _origin) {
                m_origin = _origin;
            }

            inline FVec3 getOrigin() {
                return m_origin;
            }

            btRigidBody *getBody();

            PHYSICSBODYTYPE getType() const;

        protected:
            PHYSICSBODYTYPE m_type;
            FVec3 m_origin;
            SVNodePtr m_pNode;
            btRigidBody *m_pBody;
        };

    }//!namespace logic

}//!namespace sv



#endif //SV_PHYSICSBODY_H
