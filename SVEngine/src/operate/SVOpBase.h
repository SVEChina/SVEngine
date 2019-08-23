//
// SVOpBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OPERATEBASE_H
#define SV_OPERATEBASE_H


#include "../base/SVGBase.h"
#include "../node/SVNodeDeclare.h"

namespace sv {
    
    class SVOpBase : public SVGBase {
    public:
        SVOpBase(SVInst *_app);
        
        ~SVOpBase();
        
        virtual void process(f32 dt);
        
        void setCallBack(cb_func_op _cb, void* _obj, cptr8 _info);
        
    protected:
        virtual void _process(f32 dt);
        
        cb_func_op m_pCB;
        
        void *m_obj;
        
        SVString m_info;
    };
    
    
    
    
}//!namespace sv



#endif //SV_OPERATEBASE_H
