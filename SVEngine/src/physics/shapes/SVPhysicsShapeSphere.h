//
// SVPhysicsShapeSphere.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSSHAPESPHERE_H
#define SV_PHYSICSSHAPESPHERE_H

#include "SVPhysicsShape.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsShapeSphere : public SVPhysicsShape {
        public:
            SVPhysicsShapeSphere(SVInst* _app);
            
            SVPhysicsShapeSphere(SVInst* _app, f32 _radius);
            
            ~SVPhysicsShapeSphere();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            void setRadius(const f32 _radius);
            
            const f32 getRadius() const;
        protected:
            f32 m_radius;
        };
            
    }//!namespace logic
    
}//!namespace sv

#endif //SV_PHYSICSSHAPESPHERE_H
