#include <sys/time.h>
#include "SVThread.h"
#include "SVMis.h"

SVThread::SVThread(){
    m_mis = nullptr;
    m_misclean = true;
    m_cond = MakeSharedPtr<SVCond>();
    m_svTState = TS_INIT;
    m_use = false;
    m_once = true;
    m_run = true;
    m_pThread = nullptr;
    if( m_cond->trylock() == 0 ) {
        m_pThread = new std::thread(&SVThread::_update, this);  //创建线程
        m_pThread->detach();     //线程自动启动，不会等待
        m_cond->unlock();
    }
}

SVThread::SVThread(SVCondPtr _cond) {
    m_mis = nullptr;
    m_misclean = true;
    m_cond = _cond;
    m_svTState = TS_INIT;
    m_use = false;
    m_once = true;
    m_run = true;
    m_pThread = nullptr;
    if( m_cond->trylock() == 0 ) {
       m_pThread = new std::thread(&SVThread::_update, this);  //创建线程
       m_pThread->detach();     //线程自动启动，不会等待
       m_cond->unlock();
    }
}

SVThread::~SVThread(){
    if ( m_pThread && m_pThread->joinable() ) {
        m_pThread->join();
        delete m_pThread;
        m_pThread = nullptr;
    }
    m_cond = nullptr;
}

//如果被多次调用很显然是不行的
bool SVThread::start(bool _once){
    if(m_svTState == TS_FREE) {
        m_cond->notice();
        return true;
    }
    return false;
}

//同步接口
void SVThread::stop(){
    m_run = false;
}

void SVThread::setMis(SVMisPtr _mis,bool _clean) {
    m_mis = _mis;
    m_misclean = _clean;
}

void SVThread::_update(){
    //进入逻辑主循环
    while( 1 ) {
        try {
            //挂起来
            _wait();
            SV_LOG_ERROR(m_pThread->id);
            //检测
            if(!m_run) {
                break;
            }
            //执行逻辑
            m_svTState = TS_WORK;
            //
            if(m_mis) {
                m_mis->exec(0.0f);
            }
            if(m_misclean) {
                m_mis = nullptr;
            }
        }catch( ... ) {
            m_pThread->join();
            throw;
        }
    }
    //表示线程已死
    m_svTState = TS_DESTROY;
}

void SVThread::_wait() {
    m_cond->lock();
    m_svTState = TS_FREE;
    m_cond->wait();
    m_cond->unlock();
}

