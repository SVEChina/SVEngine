//
// SVSem.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SEM_H
#define SV_SEM_H

#include "SVObject.h"
#include <pthread.h>
#include <semaphore.h>

namespace sv {
    //信号量
    class SVSem :public SVObject{
    public:
        SVSem(cptr8 _name,s32 _resvalue);
        ~SVSem();
        void wait();
        void post();
        void showValue();
    protected:
        sem_t* m_pSem;
        pthread_mutex_t mutex;
        SVString m_semName;
    };
}

#endif //SV_SEM_H
