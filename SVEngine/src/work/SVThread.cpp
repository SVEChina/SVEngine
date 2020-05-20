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
    m_pWorkCallback = nullptr;
    if ( m_pThread && m_pThread->joinable() ) {
        m_pThread->join();
        delete m_pThread;
        m_pThread = nullptr;
    }
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

//如果被多次调用很显然是不行的
bool SVThread::startThread(cb_thread_mission _mission,bool _once){
    if( m_svTState == TS_FREE ) {
        m_once = _once;
        m_pWorkCallback = _mission;
        if( pthread_mutex_trylock(&m_mutex) == 0 ) {
            pthread_cond_signal(&m_cond);   //加锁的目的 是防止被多次调用
            pthread_mutex_unlock(&m_mutex); //释放锁
        }
        return true;
    }
    return false;
}

//同步接口
void SVThread::stopThread(){
    m_run = false;
    //空闲状态，让他自动跑起来，结束掉线城
    if(m_svTState == TS_FREE) {
        
    }
}

void SVThread::_update(){
    //刚进来，就要挂起来
    pthread_mutex_lock(&m_mutex);
    m_svTState = TS_FREE;
    pthread_cond_wait(&m_cond, &m_mutex);
    pthread_mutex_unlock(&m_mutex);
    //进入逻辑主循环
    m_svTState = TS_WORK;
    while( m_run ) {
        try {
            //执行逻辑
            if(m_pWorkCallback) {
                m_pWorkCallback();
            }
            if(m_once) {
                m_pWorkCallback = nullptr;
            }
            SV_LOG_ERROR(m_pThread->id);
            //挂起
            pthread_mutex_lock(&m_mutex);
            m_svTState = TS_FREE;
            pthread_cond_wait(&m_cond, &m_mutex);
            pthread_mutex_unlock(&m_mutex);
        }catch( ... ) {
            m_pThread->join();
            throw;
        }
    }
    //表示线程已死
    m_svTState = TS_DESTROY;
}
