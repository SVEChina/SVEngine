// SVThreadPool.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include <sys/time.h>
#include "SVThreadPool.h"

SVThreadPool::SVThreadPool(){
}

SVThreadPool::~SVThreadPool(){
    m_threadPool.clear();
}

//申请一个线程
SVThreadPtr SVThreadPool::applyThread() {
    for(s32 i=0;i<m_threadPool.size();i++) {
        if( m_threadPool[i]->m_use == false ) {
            m_threadPool[i]->m_use  = true;
            return m_threadPool[i];
        }
    }
    //
    SVThreadPtr t_thread = MakeSharedPtr<SVThread>();
    t_thread->m_use = false;
    m_threadPool.append(t_thread);
    //
    return nullptr;
}

//获取线程
SVThreadPtr SVThreadPool::getThread(s32 && _index) {
    if( _index<m_threadPool.size() ) {
        return m_threadPool[_index];
    }
    return nullptr;
}

