//
// SVModuleDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MODULEDEF_H
#define SV_MODULEDEF_H

#include "../base/SVPreDeclare.h"

namespace sv {
    
    DECLARE_SHAREPTR(SVGameBase);
    DECLARE_SHAREPTR(SVGameReady);
    DECLARE_SHAREPTR(SVGameRun);
    DECLARE_SHAREPTR(SVGameEnd);
    DECLARE_SHAREPTR(SVRedPacket);
    //
    DECLARE_SHAREPTR(SVPenStroke);
    DECLARE_SHAREPTR(SVPenDraw);
    DECLARE_SHAREPTR(SVPenCurve);
    //
    DECLARE_SHAREPTR(SVEffectUnit);
    DECLARE_SHAREPTR(SVEffectPackage);
    DECLARE_SHAREPTR(SVEffectMusic);
    DECLARE_SHAREPTR(SVDivisonFilter);
    DECLARE_SHAREPTR(SVMark);
    
}//!namespace sv



#endif //SV_MODULEDEF_H
