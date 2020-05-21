//
// SVCtxBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CONTEXTBASE_H
#define SV_CONTEXTBASE_H

#include "../base/SVObject.h"
#include "../rendercore/SVRenderDeclare.h"
#include "SVEnvDeclare.h"

namespace sv {
    
    class SVCtxBase: public SVObject {
    public:
        SVCtxBase();
        
        ~SVCtxBase();
        
        virtual bool activeContext();           //激活上下文
        
        virtual bool swap();
        
        virtual SVRendererPtr createRenderer(){ return nullptr;}
    };
    
}//!namespace sv


#endif //SV_CONTEXTBASE_H
