//
// SVCtxOSXGL.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVCtxOSX.h"
#include "../app/SVInst.h"

#ifdef SV_OSX

//设备上下文 真的不能随意切换啊 否则这这个设备上下文中创建的所有GL资源全部都失效
SVCtxOSXGL::SVCtxOSXGL(void* _context)
:SVCtxBase() {
    m_pGLContext = (__bridge NSOpenGLContext*)_context;
}

SVCtxOSXGL::~SVCtxOSXGL() {
    m_pGLContext = nullptr;
}

////交换场景
//void SVCtxOSXGL::swapRenderTarget(SVRenderTargetPtr _target){
//    SVCtxBase::swapRenderTarget(_target);   //切换到目标的FBO
//    if( m_pGLContext ) {
//        [m_pGLContext flushBuffer];
//    }
//}

bool SVCtxOSXGL::activeContext(){
    if( m_pGLContext ) {
        [m_pGLContext makeCurrentContext];
        return true;
    }
    return false;
}

bool SVCtxOSXGL::swap(){
    if( m_pGLContext ) {
        [m_pGLContext flushBuffer];
        return true;
    }
    return false;
}

//
//设备上下文 真的不能随意切换啊 否则这这个设备上下文中创建的所有GL资源全部都失效
SVCtxOSXMetal::SVCtxOSXMetal()
:SVCtxBase() {
}

SVCtxOSXMetal::~SVCtxOSXMetal() {
}

bool SVCtxOSXMetal::activeContext(){
    return true;
}

bool SVCtxOSXMetal::swap() {
    return false;
}

#endif






