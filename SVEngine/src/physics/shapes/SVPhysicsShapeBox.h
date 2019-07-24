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
            
            SVPhysicsShapeBox(SVInst* _app, FVec3 _size);
            
            ~SVPhysicsShapeBox();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            void setSize(const FVec3& _size);
            
            const FVec3& getSize() const;
        };
            
    }//!namespace logic
    
}//!namespace sv

#endif //SV_PHYSICSSHAPEBOX_H
