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
#include "SVPreDeclare.h"
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
        virtual void startThread(cb_thread_mission _mission);
        
        //同步接口-停止(销毁)
        virtual void stopThread();
        
        //同步接口-挂起
        virtual void suspend();
        
        //同步接口-唤醒
        virtual void resume();
        
        //设置自动等待
        virtual void setAuoWait(bool _autowait);
        
        //通知
        virtual void notice();
        
    protected:
        
        virtual void _update();
        
    public:
        SV_THREAD_STATE m_svTState;     //SV线程状态
        std::thread *m_pThread;         //线程
        cb_thread_mission m_pWorkCallback;
        
    public:
        virtual void swapData();
        
        f32 getThreadTime();
        
        void setThreadTime(f32 _time);
    };
    
//!namespace
}



#endif //SV_MAMGRBASE_H
