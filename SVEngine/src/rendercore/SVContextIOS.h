//
// SVContextIOS.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CONTEXTIOS_H
#define SV_CONTEXTIOS_H
#include "SVContextBase.h"

#ifdef SV_IOS

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>

namespace sv {
    
    namespace render{
        
        class SVContextIOS: public SVContextBase {
        public:
            SVContextIOS(SVInst* _app,void* _context,s32 _glversion);
            
            ~SVContextIOS();
            
            virtual void swapRenderTarget(SVRenderTargetPtr _target);
            
            virtual bool activeContext();
            
            virtual void* getContext();
            
        protected:
            s32 m_glversion;
            EAGLContext* m_pGLContext;
        };

    }//!namespace render
    
}//!namespace sv



#endif  //SV_IOS

#endif  //SV_CONTEXTIOS_H
