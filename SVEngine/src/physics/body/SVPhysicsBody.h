//
//  SVPhysicsBody.h
//  SVEngine
//
//  Created by 徐子昱 on 2019/7/29.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#ifndef SV_PHYSICS_BODY_H
#define SV_PHYSICS_BODY_H
#include "../SVPhysicsBase.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsBody : public SVPhysicsBase {
        public:
            SVPhysicsBody(SVInst* _app);
            
            ~SVPhysicsBody();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
        };
        
    }//!namespace logic
    
}//!namespace sv


#endif /* SV_PHYSICS_BODY_H */
