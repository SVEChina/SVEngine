//
// SVRenderScene.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderScene.h"
#include "SVRenderCmd.h"
#include "SVRenderMgr.h"
#include "SVRenderPipline.h"
#include "renderer/SVRendererBase.h"
#include "../app/SVInst.h"
#include "../base/SVLock.h"
#include <sys/time.h>

//
SVRenderScene::SVRenderScene(SVInst *_app)
:SVGBase(_app){
    m_name = "sve";
    m_suspend = false;
    m_writePipline = MakeSharedPtr<SVRenderPipline>(_app); //写管线
    m_readPipline = MakeSharedPtr<SVRenderPipline>(_app);  //读管线
    m_cachePipline = MakeSharedPtr<SVRenderPipline>(_app);
}

SVRenderScene::~SVRenderScene() {
    clearRenderCmd();
    clearCacheCmd();
    m_writePipline = nullptr;
    m_readPipline = nullptr;
    m_cachePipline = nullptr;
}

cptr8 SVRenderScene::getName() {
    return m_name.c_str();
}

bool SVRenderScene::isSuspend(){
    return m_suspend;
}

void SVRenderScene::swapPipline(){
    SVRenderPiplinePtr tmp = m_readPipline; //写管线
    m_readPipline = m_writePipline;
    m_writePipline = tmp;
}

void SVRenderScene::render() {
    if (m_cachePipline) {
        m_cachePipline->render();
    }
    if(m_readPipline){
        m_readPipline->render();
    }
}

void SVRenderScene::pushRenderCmd(RENDERSTREAMTYPE _rst, SVRenderCmdPtr _rcmd) {
    if(m_writePipline && _rcmd){
        m_writePipline->pushRenderCmd(_rst,_rcmd);
    }
}

void SVRenderScene::clearRenderCmd(){
    if(m_writePipline){
        m_writePipline->clearRenderCmd();
    }
    if (m_readPipline) {
        m_readPipline->clearRenderCmd();
    }
}

void SVRenderScene::pushCacheCmd(RENDERSTREAMTYPE _rst, SVRenderCmdPtr _rcmd){
    if(m_cachePipline && _rcmd){
        m_cachePipline->pushRenderCmd(_rst,_rcmd);
    }
}

void SVRenderScene::clearCacheCmd(){
    if (m_cachePipline) {
        m_cachePipline->clearRenderCmd();
    }
}
