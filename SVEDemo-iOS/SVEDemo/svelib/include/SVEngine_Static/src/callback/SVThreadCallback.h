//
// SVThreadCallback.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_THREAD_CALLBACK_H
#define SV_THREAD_CALLBACK_H

#include "SVCallbackBase.h"

namespace sv {
    
    class SVThreadCallback : public SVCallbackBase {
    public:
        SVThreadCallback(SVInst* _app);

        ~SVThreadCallback();
    };
    
}//!namespace sv


#endif //SV_THREAD_CALLBACK_H
