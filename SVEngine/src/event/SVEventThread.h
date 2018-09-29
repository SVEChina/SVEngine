//
// SVEventThread.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_THREADEVENT_H
#define SV_THREADEVENT_H


#include "SVEvent.h"

namespace sv {
    
    //
    enum THREADSTATE{
        E_THREAD_S_NULL = 0,
        E_THREAD_S_START,
        E_THREAD_S_DESTROY
    };
    
    class SVThread;
    class SVEventThread : public SVEvent {
    public:
        SVEventThread();
        SVString m_name;
        THREADSTATE m_tate;
    };
    
}//!namespace sv



#endif //SV_THREADEVENT_H
