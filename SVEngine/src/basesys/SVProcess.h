//
// SVBasicSys.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PROCESS_H
#define SV_PROCESS_H

#include "../event/SVEventProc.h"
#include "SVLogicDeclare.h"

//一种处理....

namespace sv{
    
    class SVProcess : public SVEventProc {
    public:
        SVProcess(SVInst* _app);
        
        ~SVProcess();
        
        void update(f32 _dt){}
        
    protected:
        SVString m_subProcType;
    };
    
}//!namespace sv





#endif //SV_PROCESS_H
