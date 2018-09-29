// SVThreadHelp.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_THREAD_HELP_H
#define SV_THREAD_HELP_H

#include "SVThreadWork.h"

//辅助线程 主要用于资源加载

namespace sv {
    
    class SVThreadHelp : public SVThreadWork {
    public:
        SVThreadHelp(SVInst *_app);
        
        ~SVThreadHelp();
        
        virtual bool pushThreadOp(SVOpBasePtr _op);
        
        virtual bool _checkAutoWait();
        
    protected:
        void _innerUpdateBegin();
        
        void _innerUpdate();
        
        void _innerUpdateEnd();
    };
    
}//!namespace sv




#endif //SV_THREAD_HELP_H
