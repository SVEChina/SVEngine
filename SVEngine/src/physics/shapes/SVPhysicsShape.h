//
// SVPhysicsShape.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSSHAPE_H
#define SV_PHYSICSSHAPE_H

#include "../SVPhysicsBase.h"
namespace sv {
    
    namespace logic {
        enum PHYSICSSHAPETYPE{
            E_PHYSICS_SHAPE_BASE = 0,
            E_PHYSICS_SHAPE_BOX,
            E_PHYSICS_SHAPE_SPHERE
        };
        class SVPhysicsShape : public SVPhysicsBase {
        public:
            SVPhysicsShape(SVInst* _app);
            
            ~SVPhysicsShape();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            PHYSICSSHAPETYPE getType() const;
        protected:
            PHYSICSSHAPETYPE m_type;
            FVec3 m_size;
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICSSHAPE_H
