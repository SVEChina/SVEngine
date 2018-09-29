#include <sys/time.h>
#include "SVThread.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEventThread.h"
#include "../base/SVLock.h"
#include "../base/SVSign.h"
#include "../callback/SVThreadCallback.h"

SVThread::SVThread(SVInst* _app,cptr8 _name)
:SVGBase(_app){
    m_name = _name;
    m_run = false;
    m_autoWait = true;
    m_pThread = nullptr;
    m_threadTime = 0.0f;
    m_svTState = THREAD_STATE_NULL;
    m_signrun = MakeSharedPtr<SVSignLock>();
}

SVThread::~SVThread(){
    m_signrun = nullptr;
    SV_LOG_INFO("thread destroy %s \n",m_name.c_str());
}

//同步接口
void SVThread::startThread(){
    if(m_pThread)
        return;
    SV_LOG_INFO("thread %s start begin \n",m_name.c_str());
    m_run = true;
    m_signrun->lock();
    m_pThread = new std::thread(&SVThread::_update, this);
    m_signrun->wait();
    m_signrun->unlock();
    SV_LOG_INFO("thread %s start success \n",m_name.c_str());
}

//同步接口
void SVThread::stopThread(){
    SV_LOG_INFO("thread %s stop begin \n",m_name.c_str());
    if (m_pThread) {
        m_signrun->lock();
        m_run = false;
        m_signrun->notice();
        m_signrun->unlock();
        //析构线程
        if (m_pThread->joinable()) {
            m_pThread->join();
            delete m_pThread;
            m_pThread = nullptr;
        }
    }
    SV_LOG_INFO("thread %s stop success \n",m_name.c_str());
}

//同步接口-挂起
void SVThread::suspend(){
    m_svTState = THREAD_STATE_SUSPEND;
}

//同步接口-唤醒
void SVThread::resume(){
    if( m_signrun ) {
        m_signrun->notice();
    }
}

void SVThread::notice(){
    if(m_signrun){
        m_signrun->notice();
    }
}

void SVThread::setAuoWait(bool _autowait){
    m_autoWait = _autowait;
}

cptr8 SVThread::getName(){
    return m_name.c_str();
}

void SVThread::swapData(){
}

void SVThread::_update(){
    //
    _innerInit();
    //启动
    m_signrun->lock();
    m_svTState = THREAD_STATE_RUN;
    m_signrun->notice();
    m_signrun->wait();
    m_signrun->unlock();
    //
    while(m_run){
        SV_LOG_INFO("thread %s run begin \n",m_name.c_str());
        //线程逻辑
        _innerUpdateBegin();
        _innerUpdate();
        _innerUpdateEnd();
        SV_LOG_INFO("thread %s run end \n",m_name.c_str());
        //
        m_signrun->lock();
        if(m_svTState == THREAD_STATE_RUN){
            if( _checkAutoWait() ){//自动挂起检测
                SV_LOG_INFO("thread %s wait \n",m_name.c_str());
                m_signrun->wait();
            }
        }else if( m_svTState == THREAD_STATE_SUSPEND ) {
            m_signrun->wait();
            m_svTState = THREAD_STATE_RUN;
        }
        m_signrun->unlock();
    }
    //
    SV_LOG_INFO("thread %s out run \n",m_name.c_str());
    _innerDestroy();    //数据清理
}

bool SVThread::_checkAutoWait(){
    return m_autoWait;
}

void SVThread::_innerInit(){
    SVEventThreadPtr t_event = MakeSharedPtr<SVEventThread>();
    t_event->m_name = m_name;
    t_event->m_tate = E_THREAD_S_START;
    mApp->getEventMgr()->pushEvent(t_event);
}

void SVThread::_innerSignSuspend(){
}

void SVThread::_innerUpdateBegin(){
}

void SVThread::_innerUpdate(){
}

void SVThread::_innerUpdateEnd(){
}

void SVThread::_innerDestroy(){
    m_svTState = THREAD_STATE_DESTROY;
    //
    SVEventThreadPtr t_event = MakeSharedPtr<SVEventThread>();
    t_event->m_name = m_name;
    t_event->m_tate = E_THREAD_S_DESTROY;
    mApp->getEventMgr()->pushEvent(t_event);
}

f32 SVThread::getThreadTime(){
    return m_threadTime;
}

void SVThread::setThreadTime(f32 _time){
    m_threadTime = _time;
}

