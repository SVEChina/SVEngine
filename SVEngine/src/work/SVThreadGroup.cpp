// SVThreadGroup.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include <sys/time.h>
#include "SVThreadGroup.h"

SVThreadGroup::SVThreadGroup(){
    pthread_mutex_init(&m_mutex, nullptr);
    m_cond_threads = MakeSharedPtr<SVCondBroadcast>();
    m_sem =  MakeSharedPtr<SVSem>(0);
    m_count = 0;
}

SVThreadGroup::~SVThreadGroup(){
    m_sem = nullptr;
    m_cond_threads = nullptr;
    pthread_mutex_destroy(&m_mutex);
}

//线程组执行线程
void SVThreadGroup::run(bool _sync) {
    if(_sync) {
        //同步模式
        m_count = 0;
        m_cond_threads->notice();   //通知所有子线成跑起来
        //自己阻塞，等待
        m_sem->wait();
    }else{
        //异步模式
        m_count = 0;
        m_cond_threads->notice();   //通知所有子线成跑起来
    }
}

void SVThreadGroup::thread_start() {
    
}

void SVThreadGroup::thread_back() {
    //线程回来了
    pthread_mutex_lock(&m_mutex);
    m_count++;
    pthread_mutex_unlock(&m_mutex);
    if( m_count == threadIDPool.size() ) {
        //自己继续执行
        m_sem->post();
    }
}
