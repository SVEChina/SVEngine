//
// SVActDeclare.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#ifndef SV_ACTDECLARE_H
#define SV_ACTDECLARE_H

#include "../base/SVPreDeclare.h"

namespace sv {
    //
    namespace logic {
        //
        DECLARE_SHAREPTR(SVAniBase);
        DECLARE_SHAREPTR(SVActionSys);
        DECLARE_SHAREPTR(SVActionUnit);
        //act部分
        DECLARE_SHAREPTR(SVActBase);
        DECLARE_SHAREPTR(SVActDeform);
        DECLARE_SHAREPTR(SVActMove);
        DECLARE_SHAREPTR(SVActMoveTo);
        DECLARE_SHAREPTR(SVActMoveBetween);
        DECLARE_SHAREPTR(SVActMoveBy);
        DECLARE_SHAREPTR(SVActRot);
        DECLARE_SHAREPTR(SVActRotTo);
        DECLARE_SHAREPTR(SVActAlpha);
        DECLARE_SHAREPTR(SVActHide);
        DECLARE_SHAREPTR(SVActWait);
        DECLARE_SHAREPTR(SVActRotBetween);
        DECLARE_SHAREPTR(SVActScale);
        DECLARE_SHAREPTR(SVActScaleTo);
        DECLARE_SHAREPTR(SVActScaleBetween);
        DECLARE_SHAREPTR(SVActFollow);
        DECLARE_SHAREPTR(SVActFollowPerson);
        DECLARE_SHAREPTR(SVActCircle);
        DECLARE_SHAREPTR(SVActSequence);
        DECLARE_SHAREPTR(SVActTime);
        DECLARE_SHAREPTR(SVTexAttachment);
        
        //MOVIE
        DECLARE_SHAREPTR(SVMovie);
        DECLARE_SHAREPTR(SVDragma);
        DECLARE_SHAREPTR(SVTimeLine);
        DECLARE_SHAREPTR(SVKeyFrame);
        DECLARE_SHAREPTR(SVKeyDeform);
        DECLARE_SHAREPTR(SVKeyMtl);
        DECLARE_SHAREPTR(SVKeyEvent);
        
    }//!namespace logic
}


#endif //SV_ACTDECLARE_H
