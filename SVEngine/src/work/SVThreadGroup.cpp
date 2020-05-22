// SVThreadGroup.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include <sys/time.h>
#include "SVThreadGroup.h"

SVThreadGroup::SVThreadGroup(){
    m_sem = MakeSharedPtr<SVSem>(0);
    m_count = 0;
}

SVThreadGroup::~SVThreadGroup(){
    m_sem = nullptr;
}

//线程组执行线程
void SVThreadGroup::run(bool _sync) {
    if(_sync) {
        //同步模式
        //1. 执行多个线程，线程都回来了
        for( s32 i=0;i++;i<threadIDPool.size() ) {
            //sem_post(m_pSem);
        }
        //2.卡在这里
         m_sem->wait();
        //3.继续执行
    }else{
        //异步模式
    }
//    while(1) {
//
//    }
}

void SVThreadGroup::thread_start() {
    
}

void SVThreadGroup::thread_back() {
    
    m_count++;
}
