// SVThreadPool.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_THREADPOOL_H
#define SV_THREADPOOL_H

#include "../base/SVGBase.h"

namespace sv {

    //线程组(控制线程同步逻辑的)
    class SVThreadPool: public SVGBase {
    public:
        SVThreadPool(SVInst* _app);
        
        ~SVThreadPool();
        
        void init();
        
        void destroy();
        
        void start();
        
        void stop();
        
        inline SVThreadHelpPtr getHelpThread(){ return m_pHelpThread; }
        
        inline SVThreadMainPtr getMainThread(){ return m_pMainThread; }
        
    protected:
        //空
        void _create_workchain_null();
        //停止
        void _create_workchain_stop();
        //预览
        void _create_workchain_preview();
        //相机
        void _create_workchain_camera();
        
        SVThreadHelpPtr m_pHelpThread; //辅助线程
        
        SVThreadMainPtr m_pMainThread; //主线程
    };
    
}//!namespace sv



#endif //SV_THREADPOOL_H
