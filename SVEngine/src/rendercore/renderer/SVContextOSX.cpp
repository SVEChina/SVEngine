//
// SVContextOSX.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVContextOSX.h"
#include "../../app/SVInst.h"

#ifdef SV_OSX

//设备上下文 真的不能随意切换啊 否则这这个设备上下文中创建的所有GL资源全部都失效
SVContextOSX::SVContextOSX(SVInst* _app,void* _context,void* _pixelFormate,s32 _glversion)
:SVContextBase(_app) {
    m_glversion = _glversion;
    if(_context){
        NSOpenGLContext* t_context = (__bridge NSOpenGLContext*)_context;
        NSOpenGLPixelFormat* t_pixel = (__bridge NSOpenGLPixelFormat*)_pixelFormate;
        m_pGLContext = t_context;   //mac 不用share直接赋值 就好了啊
    }
}

SVContextOSX::~SVContextOSX() {
    m_pGLContext = nullptr;
}

//交换场景
void SVContextOSX::swapRenderTarget(SVRenderTargetPtr _target){
    SVContextBase::swapRenderTarget(_target);   //切换到目标的FBO
    if( m_pGLContext ) {
        [m_pGLContext flushBuffer];
    }
}

bool SVContextOSX::activeContext(){
    if( m_pGLContext ) {
        [m_pGLContext makeCurrentContext];
        return true;
    }
    return false;
}

void* SVContextOSX::getContext(){
    return (__bridge void *)m_pGLContext;
}

#endif






