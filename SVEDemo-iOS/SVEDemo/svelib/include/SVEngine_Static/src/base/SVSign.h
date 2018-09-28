//
// SVSign.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SIGNAL_H
#define SV_SIGNAL_H

#include "SVObject.h"
#include "SVPreDeclare.h"
#include <pthread.h>

namespace sv {
    //
    class SVSignLock :public SVObject{
    public:
        SVSignLock();
        ~SVSignLock();
        void lock();
        void unlock();
        virtual void wait();
        virtual void notice();
        
    protected:
        SVLockPtr m_pLock;
        pthread_cond_t m_cond;
    };

    //广播信号
    class SVSignLockBroadcast :public SVSignLock{
    public:
        SVSignLockBroadcast();
        virtual void wait();
        virtual void notice();
    };
    
//!namespace
}


#endif //SV_SIGNAL_H
