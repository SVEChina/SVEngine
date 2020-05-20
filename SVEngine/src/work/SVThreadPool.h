// SVThreadPool.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_THREADPOOL_H
#define SV_THREADPOOL_H

#include "../base/SVGBase.h"
#include "SVThreadMain.h"
#include "SVThreadHelp.h"

//线程池之
/*  1.单一任务线程
 *  2.线程组 同时进行 同时等待
 */

namespace sv {

    //线程组(控制线程同步逻辑的)

    class SVThreadPool: public SVGBase {
    public:
        SVThreadPool(SVInst* _app);
        
        ~SVThreadPool();
        
        void init(s32 _sync);
        
        void destroy();
        
        void start();
        
        void stop();
        

    protected:
        SVThreadHelpPtr m_pHelpThread; //辅助线程
        
        SVThreadMainPtr m_pMainThread; //主线程
        
        SVThreadSyncPtr m_pSyncThread; //同步线程
        
        s32 m_sync;
    };
    
}//!namespace sv



#endif //SV_THREADPOOL_H
