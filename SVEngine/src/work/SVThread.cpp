#include <sys/time.h>
#include "SVThread.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEventThread.h"
#include "../base/SVLock.h"
#include "../base/SVSign.h"
#include "../callback/SVThreadCallback.h"

SVThread::SVThread(){
    m_pThread = new std::thread(&SVThread::_update, this);
    m_pThread.detach(); //线程自动启动，不会等待
    m_pWorkCallback = nullptr;
}

SVThread::~SVThread(){
    //析构线程(这块怎么保证同步销毁？)
    m_pWorkCallback = nullptr;
    if ( m_pThread->joinable() ) {
        m_pThread->join();
        delete m_pThread;
        m_pThread = nullptr;
    }
}

//同步接口
void SVThread::startThread(cb_thread_mission _mission){
    m_pWorkCallback = _mission;
//    if(m_pThread)
//        return;
//    SV_LOG_INFO("thread %s start begin \n",m_name.c_str());
//    m_run = true;
//    m_signrun->lock();
//    m_pThread = new std::thread(&SVThread::_update, this);
//    m_signrun->wait();
//    m_signrun->unlock();
//    SV_LOG_INFO("thread %s start success \n",m_name.c_str());
}

//同步接口
void SVThread::stopThread(){
//    SV_LOG_INFO("thread %s stop begin \n",m_name.c_str());
//    if (m_pThread) {
//        m_signrun->lock();
//        m_run = false;
//        m_signrun->notice();
//        m_signrun->unlock();
//        //析构线程(这块怎么保证同步销毁？)
//        if (m_pThread->joinable()) {
//            m_pThread->join();
//            delete m_pThread;
//            m_pThread = nullptr;
//        }
//    }
//    SV_LOG_INFO("thread %s stop success \n",m_name.c_str());
}

void SVThread::_update(){
    while(1) {
        //挂起来
        
        //
        try {
            if(m_pWorkCallback) {
                m_pWorkCallback();
            }
        }catch( ... ) {
            m_pThread.join();
            throw;
        }
    }
}

//同步接口-挂起
void SVThread::suspend(){
    //m_svTState = THREAD_STATE_SUSPEND;
}

//同步接口-唤醒
void SVThread::resume(){
//    if( m_signrun ) {
//        m_signrun->notice();
//    }
}

void SVThread::notice(){
//    if(m_signrun){
//        m_signrun->notice();
//    }
}

void SVThread::setAuoWait(bool _autowait){
    //m_autoWait = _autowait;
}

void SVThread::swapData(){
}

//    //
//    _innerInit();
//    //启动
//    m_signrun->lock();
//    m_svTState = THREAD_STATE_RUN;
//    m_signrun->notice();
//    m_signrun->wait();
//    m_signrun->unlock();
//    //
//    while(m_run){
//        SV_LOG_INFO("thread %s run begin \n",m_name.c_str());
//        //线程逻辑
//        _innerUpdateBegin();
//        _innerUpdate();
//        _innerUpdateEnd();
//        SV_LOG_INFO("thread %s run end \n",m_name.c_str());
//        //
//        m_signrun->lock();
//        if(m_svTState == THREAD_STATE_RUN){
//            if( _checkAutoWait() ){//自动挂起检测
//                SV_LOG_INFO("thread %s wait \n",m_name.c_str());
//                m_signrun->wait();
//            }
//        }else if( m_svTState == THREAD_STATE_SUSPEND ) {
//            m_signrun->wait();
//            m_svTState = THREAD_STATE_RUN;
//        }
//        m_signrun->unlock();
//    }
//    //
//    SV_LOG_INFO("thread %s out run \n",m_name.c_str());
//    _innerDestroy();    //数据清理

f32 SVThread::getThreadTime(){
    return m_threadTime;
}

void SVThread::setThreadTime(f32 _time){
    m_threadTime = _time;
}

