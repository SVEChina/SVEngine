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

void SVThreadPool::init(){
    //帮助线程
    m_pHelpThread = MakeSharedPtr<SVThreadHelp>(mApp);
    m_pHelpThread->startThread();
    m_pHelpThread->notice();
    //主线程
    m_pMainThread = MakeSharedPtr<SVThreadMain>(mApp);
    m_pMainThread->startThread();
    m_pMainThread->notice();
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

//空
void SVThreadPool::_create_workchain_null(){
}

//停止
void SVThreadPool::_create_workchain_stop(){
}

//预览
void SVThreadPool::_create_workchain_preview(){
}

//相机
void SVThreadPool::_create_workchain_camera(){
}
