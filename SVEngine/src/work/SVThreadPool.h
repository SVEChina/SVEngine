// SVThreadPool.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_THREADPOOL_H
#define SV_THREADPOOL_H

#include "SVThread.h"

//线程池之
/*  1.单一任务线程
 *  2.线程组 同时进行 同时等待
 */

namespace sv {

    //线程组(控制线程同步逻辑的)

    class SVThreadPool: public SVObject {
    public:
        SVThreadPool();
        
        ~SVThreadPool();
        
        SVThreadPtr applyThread();
        
        SVThreadPtr getThread(s32 && _index);
        
        SVArray<SVThreadPtr> m_threadPool;
    };
    
}//!namespace sv



#endif //SV_THREADPOOL_H
