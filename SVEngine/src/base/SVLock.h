//
// Created by yizhou Fu on 2017/3/20.
//

#ifndef SV_LOCK_H
#define SV_LOCK_H

#include "SVObject.h"
#include <pthread.h>

namespace sv {

    class SVLock :public SVObject{
    public:
        SVLock();
        ~SVLock();
        
        void lock();
        void unlock();
        int trylock();
        
        pthread_mutex_t mutex;
    };

}



#endif //SV_LOCK_H
