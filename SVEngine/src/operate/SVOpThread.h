//
// SVOpThread.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OPERATETHREAD_H
#define SV_OPERATETHREAD_H

#include "SVOpBase.h"
#include "../base/SVDatatDef.h"

namespace sv {
    
    class SVOpAppMode : public SVOpBase {
    public:
        SVOpAppMode(SVInst* _app,SV_STATE _mode);
        
        ~SVOpAppMode();
        
    protected:
        void _process(f32 _dt);
        
        SV_STATE m_EngMode;
    };
    
}//!namespace sv



#endif //SV_OPERATETHREAD_H
