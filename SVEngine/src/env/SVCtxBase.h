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
    
    /*
     环境不同，搭载的渲染器自然也不同，所以不必要显示创建渲染器
     */

    class SVCtxBase: public SVObject {
    public:
        SVCtxBase();
        
        ~SVCtxBase();
        
        virtual bool activeContext();           //激活上下文
        
        virtual bool swap();
        
        virtual SVRendererPtr createRenderer(SVInstPtr _handle){ return nullptr;}
    };
    
}//!namespace sv


#endif //SV_CONTEXTBASE_H
