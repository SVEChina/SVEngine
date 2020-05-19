//
// SVCtxBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CONTEXTBASE_H
#define SV_CONTEXTBASE_H

#include "../base/SVObject.h"
#include "SVEnvDeclare.h"

namespace sv {
    
    class SVCtxBase: public SVObject {
    public:
        SVCtxBase();
        
        ~SVCtxBase();
        
        virtual bool activeContext();           //激活上下文
        
        virtual void* getContext();             //获取上下文
        
    protected:
        cb_func_op m_call_create;
        SVString m_info_create;
    };
    
}//!namespace sv


#endif //SV_CONTEXTBASE_H
