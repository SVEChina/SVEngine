//
// SVContextBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CONTEXTBASE_H
#define SV_CONTEXTBASE_H

#include "../../base/SVGBase.h"
#include "../SVRenderDef.h"

namespace sv {
    
    namespace render{
        
        class SVContextBase: public SVGBase {
        public:
            SVContextBase(SVInst* _app);
            
            ~SVContextBase();
            
            virtual bool activeRenderTarget(SVRenderTargetPtr _target);
            
            virtual void swapRenderTarget(SVRenderTargetPtr _target);
            
            virtual bool activeContext();           //激活上下文
            
            virtual void* getContext();             //获取上下文
            
        protected:
            cb_func_op m_call_create;
            SVString m_info_create;
        };
        
    }//!namespace render
    
}//!namespace sv


#endif //SV_CONTEXTBASE_H
