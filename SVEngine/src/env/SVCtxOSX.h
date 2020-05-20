//
// SVCtxOSXGL.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CONTEXTOSX_H
#define SV_CONTEXTOSX_H

#include "SVCtxBase.h"

#ifdef SV_OSX

#import <Foundation/Foundation.h>
#import <AppKit/NSOpenGL.h>

namespace sv {

    //mac上跑的 只有gl20,gl30,metle的渲染器

    class SVCtxOSXGL: public SVCtxBase {
    public:
        SVCtxOSXGL(void* _context,void* _pixelFormate,s32 _glversion);
        
        ~SVCtxOSXGL();
        
        virtual bool activeContext();
        
        NSOpenGLContext* m_pGLContext;
    };

    //
    class SVCtxOSXMetal: public SVCtxBase {
    public:
        SVCtxOSXMetal();
        
        ~SVCtxOSXMetal();
        
        virtual bool activeContext();
        
    };
    
}//!namespace sv



#endif  //SV_OSX

#endif  //SV_CONTEXTOSX_H
