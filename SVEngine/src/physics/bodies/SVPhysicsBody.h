//
// SVPhysicsBody.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSBODY_H
#define SV_PHYSICSBODY_H

#include "../../base/SVGBase.h"
#include "../../node/SVNodeDef.h"
namespace sv {

    namespace logic {
        enum PHYSICSBODYTYPE {
            E_PHYSICS_BODY_BASE = 0,
            E_PHYSICS_BODY_RIGID,
            E_PHYSICS_BODY_SOFT,
            E_PHYSICS_BODY_ROPE
        };

        class SVPhysicsBody : public SVGBase {
        public:
            SVPhysicsBody(SVInst *_app);

            ~SVPhysicsBody();

            virtual void init();

            virtual void destroy();

            virtual void update(f32 _dt);
            
            PHYSICSBODYTYPE getType() const;

            void setNode(SVNodePtr _node);
            
            SVNodePtr getNode();
        protected:
            PHYSICSBODYTYPE m_type;
            
            SVNodePtr m_pNode;
        };

    }//!namespace logic

}//!namespace sv



#endif //SV_PHYSICSBODY_H
