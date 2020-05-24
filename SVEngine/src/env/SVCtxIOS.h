//
// SVCtxIOS.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CONTEXTIOS_H
#define SV_CONTEXTIOS_H
#include "SVCtxBase.h"

#ifdef SV_IOS

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>

namespace sv {
    
    //苹果手机上跑的 只有gles20,gles30,metle的渲染器

    class SVCtxIOS: public SVCtxBase {
    public:
        SVCtxIOS(void* _context,s32 _glversion);
        
        ~SVCtxIOS();

        virtual bool activeContext();
        
        virtual bool swap();
        
        virtual SVRendererPtr createRenderer(SVInstPtr _handle);
        
    protected:
        s32 m_glversion;
        EAGLContext* m_pGLContext;
    };

    //Metal渲染器
    class SVCtxIOSMetal: public SVCtxBase {
    public:
        SVCtxIOSMetal();
        
        ~SVCtxIOSMetal();

        virtual bool activeContext();
        
        virtual bool swap();
        
        virtual SVRendererPtr createRenderer(SVInstPtr _handle);
        
    protected:
        s32 m_glversion;
    };
    
}//!namespace sv



#endif  //SV_IOS

#endif  //SV_CONTEXTIOS_H
