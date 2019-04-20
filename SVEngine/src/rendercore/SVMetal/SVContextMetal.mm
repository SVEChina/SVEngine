//
// SVContextMetal.mm
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVContextMetal.h"
#include "../../app/SVInst.h"

//设备上下文 真的不能随意切换啊 否则这这个设备上下文中创建的所有GL资源全部都失效
SVContextMetal::SVContextMetal(SVInst *_app)
:SVContextBase(_app) {
//    m_glversion = _glversion;
//    if(_context){
//        EAGLContext* t_context = (__bridge EAGLContext*)_context;
//        m_pGLContext = [[EAGLContext alloc] initWithAPI:[t_context API] sharegroup:[t_context sharegroup]];
//        SV_LOG_INFO("create context ios new\n");
//    }else{
//        //创建新的GLEnv
//        if (_glversion == 3) {
//            m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
//        }else if(_glversion == 2) {
//            m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
//        }else {
//            m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
//        }
//        SV_LOG_INFO("create context ios share\n");
//    }
//    activeContext();
}

SVContextMetal::~SVContextMetal() {
    SV_LOG_INFO("destroy context metal\n");
}

//交换场景
void SVContextMetal::swapRenderTarget(SVRenderTargetPtr _target){
    SVContextBase::swapRenderTarget(_target);
//    if(m_pGLContext){
//        bool t_flag = [m_pGLContext presentRenderbuffer:GL_RENDERBUFFER];
//        if(!t_flag){
//            NSLog(@"presentRenderbuffer error!");
//        }
//    }
}

bool SVContextMetal::activeContext(){
    return true;
}

void* SVContextMetal::getContext(){
    return nullptr;
}






