//
// SVPhysicsBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSBASE_H
#define SV_PHYSICSBASE_H

#include "SVPhysicsDef.h"
#include "../base/SVGBase.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsBase : public SVGBase {
        public:
            SVPhysicsBase(SVInst* _app);
            
            ~SVPhysicsBase();
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICSBASE_H
