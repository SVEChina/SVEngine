//
// SVEventMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVEventMgr.h"
#include "SVEvent.h"
#include "SVEventProc.h"

SVEventMgr::SVEventMgr(SVInst *_app)
: SVSysBase(_app) {
    mActiveEventPool = nullptr;
    mWaitEventPool = nullptr;
}

SVEventMgr::~SVEventMgr() {
}

void SVEventMgr::init() {
    reset();
}

void SVEventMgr::destroy() {
    clearAllEventProcer();
    clearAllEvent();
}

void SVEventMgr::reset() {
    m_waitlock.lock();
    mActiveEventPool = &m_exe_eventList0;
    mWaitEventPool = &m_exe_eventList1;
    m_waitlock.unlock();
}

void SVEventMgr::update(f32 dt) {
    //
    if (mActiveEventPool) {
        for(s32 i=0;i<mActiveEventPool->size();i++) {
            SVEventPtr t_event = (*mActiveEventPool)[i];
            _processEvent(t_event);
        }
        mActiveEventPool->destroy();
    }
    //
    for(s32 i=0;i<mSecendEventPool.size();i++) {
        SVEventPtr t_event = mSecendEventPool[i];
        _processEvent(t_event);
    }
    mSecendEventPool.destroy();
    
    //swap event pool
    m_waitlock.lock();
    EVENTPOOL *tmp = mActiveEventPool;
    mActiveEventPool = mWaitEventPool;
    mWaitEventPool = tmp;
    m_waitlock.unlock();
}

void SVEventMgr::_processEvent(SVEventPtr _event) {
    for (s32 i = 0; i < m_procerArray.size(); i++) {
        if (m_procerArray[i]->procEvent(_event) == false)
            return;
    }
}

void SVEventMgr::pushEvent(SVEventPtr _event) {
    m_waitlock.lock();
    if (mWaitEventPool) {
        mWaitEventPool->append(_event);
    }
    m_waitlock.unlock();
}

void SVEventMgr::pushEventToSecondPool(SVEventPtr _event) {
    mSecendEventPool.append(_event);
}

//消息处理器
bool SVEventMgr::hasRegist(SVEventProcPtr _procer) {
    if (!_procer)
        return true;
    for (s32 i = 0; i < m_procerArray.size(); i++) {
        if (m_procerArray[i] == _procer)
            return true;
    }
    return false;
}

void SVEventMgr::registProcer(SVEventProcPtr _procer) {
    if (hasRegist(_procer))
        return;
    if (_procer) {
        m_procerArray.append(_procer);
    }
}

void SVEventMgr::unregistProcer(SVEventProcPtr _procer) {
    if (!_procer)
        return;
    for (s32 i = 0; i < m_procerArray.size(); i++) {
        if (m_procerArray[i] == _procer) {
            m_procerArray.removeForce(i);
            return;
        }
    }
}

void SVEventMgr::clearAllEvent() {
    mActiveEventPool = nullptr;
    mWaitEventPool = nullptr;
    m_exe_eventList0.clear();
    m_exe_eventList1.clear();
}

void SVEventMgr::clearAllEventProcer() {
    m_procerArray.destroy();
}




















