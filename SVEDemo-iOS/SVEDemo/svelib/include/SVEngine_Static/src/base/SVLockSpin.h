//
// Created by yizhou Fu on 2017/3/20.
//

#ifndef SV_SPINLOCK_H
#define SV_SPINLOCK_H

#include "SVObject.h"
#include <pthread.h>
//#include <atomic.h>
//#icnlude <spinlock.h>

namespace sv {

    //自旋锁
    
    class SVLockSpin :public SVObject{
    public:
        SVLockSpin();
        ~SVLockSpin();
        
        void lock();
        void unlock();
        int trylock();
        
        //pthread_spinlock_t mutex;
    };

}



#endif //SV_SPINLOCK_H
