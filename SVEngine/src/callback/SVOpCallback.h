//
// SVOpCallback.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OPCALLBACK_H
#define SV_OPCALLBACK_H

#include "SVCallbackBase.h"

namespace sv {
    
    class SVOpCallback : public SVCallbackBase {
    public:
        SVOpCallback(SVInst* _app);
        
        ~SVOpCallback();
        
        virtual void opBegin(SVOpBase* _op);
        
        virtual void opEnd(SVOpBase* _op);
    };
    
}//!namespace sv



#endif //SV_OPCALLBACK_H
