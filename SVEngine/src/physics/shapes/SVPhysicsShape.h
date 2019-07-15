//
// SVPhysicsShape.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSSHAPE_H
#define SV_PHYSICSSHAPE_H

#include "../base/SVGBase.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsShape : public SVGBase {
        public:
            SVPhysicsShape(SVInst* _app);
            
            ~SVPhysicsShape();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICSSHAPE_H
