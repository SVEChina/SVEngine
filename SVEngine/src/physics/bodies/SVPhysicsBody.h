//
// SVPhysicsBody.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSBODY_H
#define SV_PHYSICSBODY_H

#include "SVPhysicsBase.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsBody : public SVPhysicsBase {
        public:
            SVPhysicsBody(SVInst* _app);
            
            ~SVPhysicsBody();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            PHYSICSBODYTYPE getType();
        protected:
            PHYSICSBODYTYPE m_type;
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICSBODY_H