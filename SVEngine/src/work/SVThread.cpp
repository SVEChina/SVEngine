#include <sys/time.h>
#include "SVThread.h"

SVThread::SVThread(){
    m_svTState = TS_INIT;
    m_use = false;
    m_once = true;
    m_run = true;
    m_pThread = nullptr;
    pthread_mutex_init(&m_mutex, nullptr);
    if( pthread_mutex_trylock(&m_mutex) == 0 ) {
        pthread_cond_init(&m_cond, nullptr);    //创建信号量
        m_pThread = new std::thread(&SVThread::_update, this);  //创建线程
        m_pThread->detach();     //线程自动启动，不会等待
        pthread_mutex_unlock(&m_mutex); //释放锁
    }
}

SVThread::~SVThread(){
    if ( m_pThread && m_pThread->joinable() ) {
        m_pThread->join();
        delete m_pThread;
        m_pThread = nullptr;
    }
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

//如果被多次调用很显然是不行的
bool SVThread::start(bool _once){
    if(m_svTState == TS_FREE) {
        pthread_cond_signal(&m_cond);
        return true;
    }
    return false;
}

//同步接口
void SVThread::stop(){
    m_run = false;
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
            
        }catch( ... ) {
            m_pThread->join();
            throw;
        }
    }
    //表示线程已死
    m_svTState = TS_DESTROY;
}

void SVThread::_wait() {
    pthread_mutex_lock(&m_mutex);
    m_svTState = TS_FREE;
    pthread_cond_wait(&m_cond, &m_mutex);
    pthread_mutex_unlock(&m_mutex);
}

