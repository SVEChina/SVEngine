//
// SVPhysicsShapeBox.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSSHAPEBOX_H
#define SV_PHYSICSSHAPEBOX_H

#include "SVPhysicsShape.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsShapeBox : public SVPhysicsShape {
        public:
            SVPhysicsShapeBox(SVInst* _app);
            
            ~SVPhysicsShapeBox();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
        };
            
    }//!namespace logic
    
}//!namespace sv

#endif //SV_PHYSICSSHAPEBOX_H
