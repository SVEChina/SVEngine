// SVThreadWork.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_WORKTHREAD_H
#define SV_WORKTHREAD_H

#include <thread>
#include "../base/SVThread.h"
#include "../base/SVArray.h"
#include <sys/time.h>

//工作线程

namespace sv {

    class SVThreadWork : public SVThread {
    public:
        SVThreadWork(SVInst *_app,cptr8 _threadname);
        
        ~SVThreadWork();
        
        virtual bool pushThreadOp(SVOpBasePtr _op);
        
        void clearThreadOp();
        
        void setFPS(s32 _fps);
        
        void setThreadCallback(ThreadCallback _func);
    protected:
        virtual void _innerInit();
        
        virtual void _innerSignSuspend();
        
        virtual void _innerUpdateBegin();
        
        virtual void _innerUpdate();
        
        virtual void _innerUpdateEnd();
        
        virtual void _innerDestroy();
        
        ThreadCallback m_pCallbackFunc;
        
        SVLockPtr m_OpWaitLock;
        SVLockPtr m_OpRunLock;
        typedef SVArray<SVOpBasePtr> OPPOOL;
        OPPOOL* m_opPoolWait;      //等待队列(数据结构一定是链表 才能保证 在运算过程中被插入，而不出错)
        OPPOOL* m_opPoolRun;       //运行队列
        //
        timeval m_timTagLast;
        s32 m_fps;
    };
    
}//!namespace sv





#endif //SV_WORKTHREAD_H
