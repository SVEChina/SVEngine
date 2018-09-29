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
}

SVRenderScene::~SVRenderScene() {
    clearRenderCmd();
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
        m_writePipline = nullptr;
    }
    if (m_readPipline) {
        m_readPipline->clearRenderCmd();
        m_readPipline = nullptr;
    }
}
