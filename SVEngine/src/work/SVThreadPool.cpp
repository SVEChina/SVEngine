// SVThreadPool.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include <sys/time.h>
#include "SVThreadPool.h"
#include "SVThreadMain.h"
#include "SVThreadHelp.h"
#include "../callback/SVThreadWorkCallback.h"
#include "../app/SVInst.h"
#include "../base/SVLock.h"
#include "../base/SVSign.h"
#include "../operate/SVOpBase.h"
#include "../operate/SVOpThread.h"

SVThreadPool::SVThreadPool(SVInst* _app)
:SVGBase(_app){
    m_pHelpThread = nullptr;
    m_pMainThread = nullptr;
}

SVThreadPool::~SVThreadPool(){
}

void SVThreadPool::init(s32 _sync){
    //帮助线程
    m_pHelpThread = MakeSharedPtr<SVThreadHelp>(mApp);
    m_pHelpThread->startThread();
    m_pHelpThread->notice();
    //主线程
    if(_sync == 0) {
        //异步方式
        m_pMainThread = MakeSharedPtr<SVThreadMain>(mApp);
        m_pMainThread->startThread();
        m_pMainThread->notice();
    }else{
        //同步方式 线程初始化 暂时没有相关代码
    }
}

void SVThreadPool::destroy(){
    if(m_pHelpThread){
        m_pHelpThread->stopThread();
        m_pHelpThread = nullptr;
    }
    //
    if(m_pMainThread){
        m_pMainThread->stopThread();
        m_pMainThread = nullptr;
    }
}

//
void SVThreadPool::start(){
    if(m_pMainThread){
        m_pMainThread->setAuoWait(false);
        m_pMainThread->notice();
    }
}

//
void SVThreadPool::stop(){
    if(m_pMainThread){
        m_pMainThread->setAuoWait(false);
    }
}

