//
// SVEventMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_EVENTMGR_H
#define SV_EVENTMGR_H

#include "../basesys/SVSysBase.h"
#include "../base/SVArray.h"
#include "../work/SVTdCore.h"
#include "SVEventDef.h"

namespace sv {
    
    class SVEventMgr : public SVSysBase {
    public:
        SVEventMgr(SVInst* _app);
        
        ~SVEventMgr();
        
        void init();
        
        void destroy();
        
        void update(f32 dt);
        
        void reset();
        
        //推送事件
        void pushEvent(SVEventPtr _event);
        
        void pushEventToSecondPool(SVEventPtr _event);
        
        //注册事件处理器
        bool hasRegist(SVEventProcPtr _procer);
        
        void registProcer(SVEventProcPtr _procer);
        
        void unregistProcer(SVEventProcPtr _procer);
        
    private:
        void clearAllEvent();
        
        void clearAllEventProcer();
        
        void _processEvent(SVEventPtr _event);
        
    private:
        typedef SVArray<SVEventPtr> EVENTPOOL;
        EVENTPOOL m_exe_eventList0;     //事件队列0
        EVENTPOOL m_exe_eventList1;     //事件队列1
        
        EVENTPOOL *mActiveEventPool;    //当前执行的消息池
        EVENTPOOL *mWaitEventPool;      //等待消息池
        EVENTPOOL mSecendEventPool;     //第二执行队列
        
        typedef SVArray<SVEventProcPtr> PROCERPOOL;
        PROCERPOOL m_procerArray;
        
        SVLock m_waitlock;
    };
    
}//!namespace sv

//
#endif //SV_EVENTMGR_H
