//
// SVContextOSX.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CONTEXTOSX_H
#define SV_CONTEXTOSX_H

#include "SVContextBase.h"

#ifdef SV_OSX

#import <Foundation/Foundation.h>
#import <AppKit/NSOpenGL.h>

namespace sv {
    
    namespace render{
        
        class SVContextOSX: public SVContextBase {
        public:
            SVContextOSX(SVInst* _app,void* _context,void* _pixelFormate,s32 _glversion);
            
            ~SVContextOSX();
            
            virtual void swapRenderTarget(SVRenderTargetPtr _target);
            
            virtual bool activeContext();
            
            virtual void* getContext();
            
        protected:
            s32 m_glversion;
            NSOpenGLContext* m_pGLContext;
        };

    }//!namespace render
    
}//!namespace sv



#endif  //SV_OSX

#endif  //SV_CONTEXTOSX_H
