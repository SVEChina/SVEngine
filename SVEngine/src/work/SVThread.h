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

    //线程任务回调
    typedef void (*cb_thread_mission)();
    
    //线程
    class SVThread: public SVObject {
    public:
        SVThread();
        
        ~SVThread();
        
        //同步接口-开启(创建)
        bool startThread(cb_thread_mission _mission,bool _once = true);
        
        //同步接口-停止(销毁)
        void stopThread();
        
        //执行一次
        void execonce();
        
    protected:
        void _wait();       //wait阶段
        
        virtual void _update();
        
    public:
        pthread_mutex_t m_mutex;
        pthread_cond_t m_cond;
        //
        SV_THREAD_STATE m_svTState;     //SV线程状态
        std::thread *m_pThread;         //线程
        cb_thread_mission m_pWorkCallback;
        bool m_use;
        bool m_once;
        bool m_run;

        f32 getThreadTime();
        
        void setThreadTime(f32 _time);
    };
    
//!namespace
}



#endif //SV_MAMGRBASE_H
