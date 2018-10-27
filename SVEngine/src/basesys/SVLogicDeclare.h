//
// SVLogicDeclare.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_LOGICDECLARE_H
#define SV_LOGICDECLARE_H

#include "../base/SVPreDeclare.h"

namespace sv {
    
    namespace logic {
        
        DECLARE_SHAREPTR(SVTrans);
        DECLARE_SHAREPTR(SVTransCPU);
        DECLARE_SHAREPTR(SVTransGPU);
        //
        DECLARE_SHAREPTR(StreamInCore);
        //
        
    }//!namespace logic
    
    //
    DECLARE_SHAREPTR(SVOutMission);
    DECLARE_SHAREPTR(SVOutMissionNor);

}//!namespace sv

#endif //SV_LOGICDECLARE_H
