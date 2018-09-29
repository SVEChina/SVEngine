//
// SVRenderStream.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderStream.h"
#include "SVRenderCmd.h"

SVRenderStream::SVRenderStream() {
}

SVRenderStream::~SVRenderStream() {
}

void SVRenderStream::addSVRenderCmd(SVRenderCmdPtr cmd) {
    if(cmd){
        m_cmdArray.append(cmd);
    }
}

void SVRenderStream::clearSVRenderCmd() {
    m_cmdArray.destroy();
}

#if defined(SV_ANDROID)
void SVRenderStream::render() {
    for (s32 i = 0; i < m_cmdArray.size(); i++) {
        m_cmdArray[i]->render();
    }
    m_cmdArray.destroy();
}
#endif

#if  defined(SV_IOS)
void SVRenderStream::render() {
    for (s32 i = 0; i < m_cmdArray.size(); i++) {
        glPushGroupMarkerEXT(0, m_cmdArray[i]->mTag.c_str());
        m_cmdArray[i]->render();
        glPopGroupMarkerEXT();
    }
    m_cmdArray.destroy();
}
#endif

#if defined(SV_OSX)
void SVRenderStream::render() {
    for (s32 i = 0; i < m_cmdArray.size(); i++) {
        m_cmdArray[i]->render();
    }
    m_cmdArray.destroy();
}
#endif
