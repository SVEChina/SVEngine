//
// SVWorkDeclare.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_WORKDECLARE_H
#define SV_WORKDECLARE_H

#include "../base/SVDef.h"
#include "../base/SVPreDeclare.h"

namespace sv {

    DECLARE_SHAREPTR(SVLockSpin);
    DECLARE_SHAREPTR(SVCond);
    DECLARE_SHAREPTR(SVSem);
    DECLARE_SHAREPTR(SVThread);
    DECLARE_SHAREPTR(SVThreadGroup);
    DECLARE_SHAREPTR(SVThreadPool);

    //
    DECLARE_SHAREPTR(SVMis);

}//!namespace sv

#endif //SV_WORKDECLARE_H
