//
// SVThreadWorkCallback.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_WORKTHREAD_CB_H
#define SV_WORKTHREAD_CB_H

#include "../base/SVDef.h"

namespace sv {
    
    class SVThread;
    
    class SVInst;
    
    //线程回调
    void workThreadCallback(SVInst* _app,SVThread* _thread,SV_THREAD_STAGE _stag);
    
    //线程销毁回调
    void workThreadDestroyCallback(SVInst* _app,SVThread* _thread,SV_THREAD_STAGE _stage);
    
}//!namespace sv




#endif //SV_WORKTHREAD_CB_H
