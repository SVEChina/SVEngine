//
// SVCtxIOS.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVCtxIOS.h"
#include "../app/SVInst.h"

#ifdef SV_IOS

//设备上下文 真的不能随意切换啊 否则这这个设备上下文中创建的所有GL资源全部都失效
SVCtxIOS::SVCtxIOS(void* _context,s32 _glversion)
:SVCtxBase() {
    m_glversion = _glversion;
    if(_context){
        EAGLContext* t_context = (__bridge EAGLContext*)_context;
        m_pGLContext = [[EAGLContext alloc] initWithAPI:[t_context API] sharegroup:[t_context sharegroup]];
        SV_LOG_INFO("create context ios new\n");
    }else{
        //创建新的GLEnv
        if (_glversion == 3) {
            m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        }else if(_glversion == 2) {
            m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        }else {
            m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        }
        SV_LOG_INFO("create context ios share\n");
    }
    activeContext();
}

SVCtxIOS::~SVCtxIOS() {
    [EAGLContext setCurrentContext:nil];
    m_pGLContext = nil;
    SV_LOG_INFO("destroy context ios\n");
}

SVRendererPtr SVCtxIOS::createRenderer() {
    return nullptr;
}

bool SVCtxIOS::activeContext(){
    if(m_pGLContext){
        return [EAGLContext setCurrentContext:m_pGLContext];
    }
    NSLog(@"activeContext error!");
    return false;
}

//交换场景
bool SVCtxIOS::swap(){
    if(m_pGLContext){
        bool t_flag = [m_pGLContext presentRenderbuffer:GL_RENDERBUFFER];
        if(!t_flag){
            NSLog(@"presentRenderbuffer error!");
            return false;
        }
        return true;
    }
    return false;
}

#endif






