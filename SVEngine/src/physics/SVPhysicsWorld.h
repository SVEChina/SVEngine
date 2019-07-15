//
// SVPhysicsWorld.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSWORLD_H
#define SV_PHYSICSWORLD_H

#include "../base/SVGBase.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsWorld : public SVGBase {
        public:
            SVPhysicsWorld(SVInst* _app);
            
            ~SVPhysicsWorld();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            void addBody(SVPhysicsBodyPtr _body);
            
            void addShape(SVPhysicsShapePtr _shape, SVPhysicsBodyPtr _body);
            
            void addJoint(SVPhysicsJointPtr _joint);
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICSWORLD_H
