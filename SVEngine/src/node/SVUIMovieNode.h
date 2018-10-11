//
// SVUIMovieNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_UIMOVENODE_H
#define SV_UIMOVENODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        class SVUIMovieNode : public SVNode {
        public:
            SVUIMovieNode(SVInst *_app);
            
            ~SVUIMovieNode();
            
            void create(s32 _w,s32 _h);
            
            void destroy();
            
            void update(f32 dt);
            
            void render();
            
        protected:
            void _renderMoview();
            void _renderMovieHead();
            void _renderMovieBody();
            bool _renderDragma();
            bool _renderTimeLine();
            bool _renderKey();
        };

    }//!namespace node

}//!namespace sv



#endif //SV_MULTPASSNODE_H
