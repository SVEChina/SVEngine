//
// SVContextBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVContextBase.h"
#include "../app/SVInst.h"
#include "SVRenderMgr.h"
#include "SVRendererBase.h"
#include "SVGL/SVRendererGL.h"
#include "SVRenderTarget.h"

SVContextBase::SVContextBase(SVInst *_app)
:SVGBase(_app)
,m_call_create(nullptr){
}

SVContextBase::~SVContextBase() {
}

bool SVContextBase::activeRenderTarget(SVRenderTargetPtr _target){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer && _target) {
        t_renderer->svBindFrameBuffer(_target->getFboID());
        t_renderer->svPushViewPort(0, 0,_target->getWidth(),_target->getHeight());
        glClearColor(0.0,0.0,0.0,0.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        return true;
    }
    return false;
}

void SVContextBase::swapRenderTarget(SVRenderTargetPtr _target){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer && _target){
        t_renderer->svPopViewPort();
        t_renderer->svBindFrameBuffer(_target->getFboID());
        glBindRenderbuffer(GL_RENDERBUFFER,_target->getColorID());
    }
}

bool SVContextBase::activeContext(){
    return false;
}

void* SVContextBase::getContext(){
    return nullptr;
}
