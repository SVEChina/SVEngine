//
// SVRenderStateMetal.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERSTATE_METAL_H
#define SV_RENDERSTATE_METAL_H

#include "../SVRenderState.h"

//渲染状态 主要用于 状态重复调用的控制

namespace sv {
    
    namespace render{
        
        class SVRenderStateMetal : public SVRenderState {
        public:
            SVRenderStateMetal(SVInst* _app);
            
            ~SVRenderStateMetal();
        };
        
    }//!namespace render
    
}//!namespace sv



#endif //SV_RENDERSTATE_METAL_H
