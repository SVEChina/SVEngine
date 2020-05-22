// SVThreadGroup.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_THREADGROUP_H
#define SV_THREADGROUP_H

#include "SVThread.h"

//协同 线程组

namespace sv {

    //线程组(控制线程同步逻辑的)

    class SVThreadGroup: public SVObject {
    public:
        SVThreadGroup();
        
        ~SVThreadGroup();
        
        void run(bool _sync);
        
        void thread_wait();
        
        void thread_back();
        
    private:
        //
        SVArray<s32> threadIDPool;    //线程的id池子
        SVCondPtr m_cond;
        SVSemPtr m_sem;
        s32 m_count;
        pthread_mutex_t m_mutex;
    };
    
}//!namespace sv



#endif //SV_THREADGROUP_H
