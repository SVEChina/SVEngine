//
// SVThread.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_THREAD_H
#define SV_THREAD_H

//线程基类

#include "../base/SVObject.h"
#include "SVWorkDeclare.h"
#include "SVTdCore.h"
#include <thread>

namespace sv {
    
    /*
     单纯线程
     */

    class SVThread: public SVObject {
    public:
        SVThread();
        
        SVThread(SVCondPtr _cond);
        
        ~SVThread();
        
        //同步接口-开启(创建)
        bool start(bool _once = true);
        
        //同步接口-停止(销毁)
        void stop();
        
        void setMis(SVMisPtr _mis,bool _clean = false);
        
        //执行一次
        void execonce();
        
    protected:
        void _wait();       //wait阶段
        
        virtual void _update();
        
    public:
        SV_THREAD_STATE m_svTState;     //SV线程状态
        SVCondPtr m_cond;
        std::thread *m_pThread;         //线程
        SVMisPtr m_mis;
        bool m_use;
        bool m_once;
        bool m_run;
        bool m_misclean;
    };
    
//!namespace
}



#endif //SV_MAMGRBASE_H
