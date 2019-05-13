//
// SVRenderStream.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderStream.h"
#include "SVRenderCmd.h"
#include "../base/SVLock.h"
SVRenderStream::SVRenderStream() {
    m_lock = MakeSharedPtr<SVLock>();
}

SVRenderStream::~SVRenderStream() {
}

void SVRenderStream::addSVRenderCmd(SVRenderCmdPtr cmd) {
    m_lock->lock();
    if(cmd){
        m_cmdArray.append(cmd);
    }
    m_lock->unlock();
}

void SVRenderStream::clearSVRenderCmd() {
    m_lock->lock();
    m_cmdArray.destroy();
    m_lock->unlock();
}

#if defined(SV_ANDROID)
void SVRenderStream::render() {
    m_lock->lock();
    for (s32 i = 0; i < m_cmdArray.size(); i++) {
        m_cmdArray[i]->render();
    }
    m_cmdArray.destroy();
    m_lock->unlock();
}
#endif

#if  defined(SV_IOS)
void SVRenderStream::render() {
    m_lock->lock();
    for (s32 i = 0; i < m_cmdArray.size(); i++) {
        glPushGroupMarkerEXT(0, m_cmdArray[i]->mTag.c_str());
        m_cmdArray[i]->render();
        glPopGroupMarkerEXT();
    }
    m_cmdArray.destroy();
    m_lock->unlock();
}
#endif

#if defined(SV_OSX)
void SVRenderStream::render() {
    m_lock->lock();
    for (s32 i = 0; i < m_cmdArray.size(); i++) {
        m_cmdArray[i]->render();
    }
    m_cmdArray.destroy();
    m_lock->unlock();
}
#endif
