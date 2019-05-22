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
        
        inline SVThreadHelpPtr getHelpThread(){ return m_pHelpThread; }
        
        inline SVThreadMainPtr getMainThread(){
            if (m_sync == 0) {
                return m_pMainThread;
            }else{
                return m_pSyncThread;
            }
        }
    protected:
        SVThreadHelpPtr m_pHelpThread; //辅助线程
        
        SVThreadMainPtr m_pMainThread; //主线程
        
        SVThreadSyncPtr m_pSyncThread; //同步线程
        
        s32 m_sync;
    };
    
}//!namespace sv



#endif //SV_THREADPOOL_H
