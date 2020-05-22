//
// Created by yizhou Fu on 2017/3/20.
//

#ifndef SV_TDCORE_H
#define SV_TDCORE_H

#include "../base/SVObject.h"
#include <pthread.h>
#include <semaphore.h>
#include <atomic>

/*
 多线程 core 包含各种锁，信号量，互斥量等
 */

namespace sv {
    
    /*自旋锁
     1.循环等待
     2.无线程挂起-切换等操作
     3.开销小于线程等待等操作
     4.c++11不带自旋锁，需要自己手动实现
     */

    class SVLockSpin :public SVObject{
    public:
        SVLockSpin();
        
        ~SVLockSpin();
        
        void lock();
        
        void unlock();
        
    private:
        std::atomic_flag mflag;     //原子操作
    };

    /*
     1.正常的线程锁
     */

    class SVLock :public SVObject{
    public:
        SVLock();
        
        ~SVLock();
        
        void lock();
        
        void unlock();
        
        s32 trylock();
        
        pthread_mutex_t mutex;
    };

    /*条件变量
     */
    class SVCond :public SVObject{
    public:
        SVCond();
        
        ~SVCond();
        
        s32 trylock();
        
        void lock();
        
        void unlock();
        
        virtual void wait();
        
        virtual void notice();
        
        virtual void broad();
        
    protected:
        pthread_cond_t m_cond;
        pthread_mutex_t m_mutex;
    };

    /*信号量
     */
    class SVSem :public SVObject{
    public:
        SVSem(s32 _resvalue);
        
        ~SVSem();
        
        void wait();
        
        void post();
        
        void lastcheck();
        
        sem_t psem;
        
    };
}



#endif //SV_TDCORE_H
