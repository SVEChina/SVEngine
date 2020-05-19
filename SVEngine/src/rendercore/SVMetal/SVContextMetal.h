//
// SVContextMetal.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CONTEXT_METAL_H
#define SV_CONTEXT_METAL_H

#include "SVContextBase.h"
#import <Foundation/Foundation.h>
#import <QuartzCore/CAMetalLayer.h>

namespace sv {
    
    namespace render{
        
        class SVContextMetal: public SVContextBase {
        public:
            SVContextMetal(SVInst* _app);
            
            ~SVContextMetal();
            
            virtual void swapRenderTarget(SVRenderTargetPtr _target);
            
            virtual bool activeContext();
            
            virtual void* getContext();
            
        protected:
            id<CAMetalDrawable> m_pDrawable;
        };

    }//!namespace render
    
}//!namespace sv


#endif  //SV_CONTEXT_METAL_H
