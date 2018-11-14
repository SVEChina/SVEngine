//
// SVBezierNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li

#ifndef SV_BEZIERNODE_H
#define SV_BEZIERNODE_H
#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        class SVBezierNode : public SVNode {
        public:
            SVBezierNode(SVInst *_app);
            
            ~SVBezierNode();
            
            void create(s32 _w,s32 _h);
            
            void destroy();
            
            void update(f32 dt);
            
            void render();
            
        };
        
    }//!namespace node
    
}//!namespace sv


#endif /* SV_BEZIERNODE_H */
