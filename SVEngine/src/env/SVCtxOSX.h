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
        SVCtxOSXGL(void* _context);
        
        ~SVCtxOSXGL();
        
        virtual SVRendererPtr createRenderer();
        
        virtual bool activeContext();
        
        virtual bool swap();
        
        NSOpenGLContext* m_pGLContext;
    };

    //
    class SVCtxOSXMetal: public SVCtxBase {
    public:
        SVCtxOSXMetal();
        
        ~SVCtxOSXMetal();
        
        virtual SVRendererPtr createRenderer();
        
        virtual bool activeContext();
        
        virtual bool swap();
    };
    
}//!namespace sv



#endif  //SV_OSX

#endif  //SV_CONTEXTOSX_H
