//
// SVPhysicsBodyRigid.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSBODYRIGID_H
#define SV_PHYSICSBODYRIGID_H
#include "SVPhysicsBody.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsBodyRigid : public SVPhysicsBody {
        public:
            SVPhysicsBodyRigid(SVInst* _app);
            
            ~SVPhysicsBodyRigid();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            PHYSICSBODYTYPE getType();
        protected:
            PHYSICSBODYTYPE m_type;
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICSBODYRIGID_H
