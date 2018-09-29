// SVThreadWork.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include <unistd.h>
#include <sys/time.h>
#include "SVThreadWork.h"
#include "../operate/SVOpBase.h"
#include "../operate/SVOpCreate.h"
#include "../operate/SVOpLoadBase.h"
#include "../base/SVGBase.h"
#include "../base/SVLock.h"
#include "../base/SVSign.h"

SVThreadWork::SVThreadWork(SVInst *_app,cptr8 _threadname)
:SVThread(_app,_threadname){
    m_fps = 30;
    m_pCallbackFunc = nullptr;
    m_OpWaitLock = MakeSharedPtr<SVLock>();
    m_OpRunLock = MakeSharedPtr<SVLock>();
    //
    m_opPoolWait = new OPPOOL();
    m_opPoolRun = new OPPOOL();
}

SVThreadWork::~SVThreadWork() {
    clearThreadOp();
    m_pCallbackFunc = nullptr;
    m_OpWaitLock = nullptr;
    m_OpRunLock = nullptr;
    delete m_opPoolWait;
    delete m_opPoolRun;
}

void SVThreadWork::setThreadCallback(ThreadCallback _func){
    m_pCallbackFunc = _func;
}

void SVThreadWork::setFPS(s32 _fps){
    m_fps = _fps;
}

void SVThreadWork::_innerInit(){
    SVThread::_innerInit();
    if(m_pCallbackFunc){
        (*m_pCallbackFunc)(mApp,this,THREAD_STAGE_BEGIN);
    }
    gettimeofday(&m_timTagLast, nullptr);
}

void SVThreadWork::_innerSignSuspend(){
    SVThread::_innerSignSuspend();
    if(m_pCallbackFunc){
        (*m_pCallbackFunc)(mApp,this,THREAD_STAGE_SIGN_SUSPEND);
    }
}

void SVThreadWork::_innerUpdateBegin(){
    SVThread::_innerUpdateBegin();
    SV_LOG_INFO("thread run %s \n",m_name.c_str() );
    if(m_pCallbackFunc){
        (*m_pCallbackFunc)(mApp,this,THREAD_STAGE_UPDATE_BEGIN);
    }
}

void SVThreadWork::_innerUpdate(){
    SVThread::_innerUpdate();
    
    //交换操作 swap
    m_OpWaitLock->lock();
    OPPOOL* tmpPool = m_opPoolWait;
    m_opPoolWait = m_opPoolRun;
    m_opPoolRun = tmpPool;
    m_OpWaitLock->unlock();
    
    //执行运行队列
    m_OpRunLock->lock();
    for(s32 i=0;i<m_opPoolRun->size();i++) {
        SVOpBasePtr t_op = (*m_opPoolRun)[i];
        t_op->process(0.0f);
    }
    m_opPoolRun->destroy();
    m_OpRunLock->unlock();
}

void SVThreadWork::_innerUpdateEnd(){
    if(m_pCallbackFunc){
        (*m_pCallbackFunc)(mApp,this,THREAD_STAGE_UPDATE_END);
    }
    //控制帧率使用
    timeval timTagCur;
    gettimeofday(&timTagCur, NULL);
    s32 dt = s32((timTagCur.tv_sec - m_timTagLast.tv_sec) * 1000.0f + (timTagCur.tv_usec - m_timTagLast.tv_usec) * 0.001f);
    s32 t_dert = 1000/m_fps;
    if(dt>t_dert){
        usleep(1000);
        //SV_LOG_INFO("threadname %s too long time \n",m_name.c_str());
    }else{
        s32 t_sleeptime = t_dert - dt;
        //SV_LOG_INFO("threadname %s fps %d sleep time %d ms \n",m_name.c_str(),m_fps,t_sleeptime);
        usleep(t_sleeptime*1000);
    }
    //刷新纪录时间
    gettimeofday(&m_timTagLast, NULL);
    //
    SVThread::_innerUpdateEnd();
}

void SVThreadWork::_innerDestroy(){
    if(m_pCallbackFunc){
        (*m_pCallbackFunc)(mApp,this,THREAD_STAGE_END);
    }
    SVThread::_innerDestroy();
}

bool SVThreadWork::pushThreadOp(SVOpBasePtr _op) {
    if (mApp->getState() == SV_ST_WILLSUSPEND) {
        //将要挂起的时候不能再有操作
        return false;
    }
    m_OpWaitLock->lock();
    if (_op) {
        m_opPoolWait->append(_op);
    }
    m_OpWaitLock->unlock();
    return true;
}

void SVThreadWork::clearThreadOp() {
    //
    m_OpRunLock->lock();
    m_opPoolRun->clear();
    m_OpRunLock->unlock();
    //
    m_OpWaitLock->lock();
    m_opPoolWait->clear();
    m_OpWaitLock->unlock();
}

