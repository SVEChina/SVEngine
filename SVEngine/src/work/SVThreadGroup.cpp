// SVThreadGroup.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVThreadGroup.h"

SVThreadGroup::SVThreadGroup(){
    pthread_mutex_init(&m_mutex, nullptr);
    m_cond = MakeSharedPtr<SVCond>();
    m_sem =  MakeSharedPtr<SVSem>(0);
    m_count = 0;
}

SVThreadGroup::~SVThreadGroup(){
    m_sem = nullptr;
    m_cond = nullptr;
    pthread_mutex_destroy(&m_mutex);
}

void SVThreadGroup::create(s32 _num) {
    if(threadIDPool.size()>0) {
        return ;
    }
    m_count = _num;
    for(s32 i=0;i<m_count;i++) {
        inThreadPtr t_in_td = MakeSharedPtr<inThread>(THIS_TO_SHAREPTR(SVThreadGroup));
        threadIDPool.append(t_in_td);
    }
}

void SVThreadGroup::destroy() {
    threadIDPool.clear();
    m_count = 0;
}

void SVThreadGroup::setMis(s32 _index,SVMisPtr _mis) {
    if( _index<threadIDPool.size() ) {
        threadIDPool[_index]->setMis(_mis,false);
    }
}

//线程组执行线程
void SVThreadGroup::run(bool _sync) {
    if(_sync) {
        //同步模式
        m_count = 0;
        m_cond->broad();   //通知所有子线成跑起来
        //自己阻塞，等待
        m_sem->wait();
    }else{
        //异步模式
        m_count = 0;
        m_cond->broad();   //通知所有子线成跑起来
    }
}

void SVThreadGroup::thread_wait() {
    
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
