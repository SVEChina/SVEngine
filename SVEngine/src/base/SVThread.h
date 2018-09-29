//
// SVThread.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_THREAD_H
#define SV_THREAD_H

//线程基类

#include "SVGBase.h"
#include "SVPreDeclare.h"
#include <thread>

namespace sv {

    class SVThread: public SVGBase {
    public:
        SVThread(SVInst* _app,cptr8 _name);
        
        ~SVThread();
        
        //同步接口-开启(创建)
        virtual void startThread();
        
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
        
        virtual bool _checkAutoWait();      //检测是否自动等待
        
        virtual void _innerInit();
        
        virtual void _innerSignSuspend();   //信号挂起
        
        virtual void _innerUpdateBegin();
        
        virtual void _innerUpdate();
        
        virtual void _innerUpdateEnd();
        
        virtual void _innerDestroy();
        
        SVString m_name;                     //线程名字
        std::thread *m_pThread;             //线程
        bool m_run;                         //线程运行状态
        bool m_autoWait;                    //线程自动等待
        f32 m_threadTime;                   //线程时间(视频引擎专属需要的属性)
        SV_THREAD_STATE m_svTState;         //SV线程状态
        
        SVSignLockPtr m_signrun;            //运行控制信号
        
    public:
        cptr8 getName();
        
        virtual void swapData();
        
        f32 getThreadTime();
        
        void setThreadTime(f32 _time);
    };
    
//!namespace
}



#endif //SV_MAMGRBASE_H
