//
// SVActPhysics.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACTPHYSICS_H
#define SV_ACTPHYSICS_H

#include "SVActBase.h"
namespace sv {
    
    namespace logic {
        
        class SVActPhysics : public SVActBase {
        public:
            SVActPhysics(SVInst *_app, SVPhysicsBodyPtr _physicsBody);
        
            ~SVActPhysics();
        
            void run(SVNodePtr _nodePtr, f32 dt);
        
            bool isEnd();
        
        protected:
            SVPhysicsBodyPtr m_physicsBody;
        };
    
        
        class SVActBodyRope : public SVActPhysics {
        public:
            SVActBodyRope(SVInst *_app, SVPhysicsBodyPtr _physicsBody);
        
            ~SVActBodyRope();
        
            void run(SVNodePtr _nodePtr, f32 dt);
        
            bool isEnd();
        
        protected:
            
        };
    
    
        class SVActBodyCloth : public SVActPhysics {
        public:
            SVActBodyCloth(SVInst *_app, SVPhysicsBodyPtr _physicsBody);
        
            ~SVActBodyCloth();
        
            void run(SVNodePtr _nodePtr, f32 dt);
        
            bool isEnd();
        
        protected:
            
        };
        
    }//!namespace logic
    
}//!namespace sv

#endif //SV_ACTPHYSICS_H
