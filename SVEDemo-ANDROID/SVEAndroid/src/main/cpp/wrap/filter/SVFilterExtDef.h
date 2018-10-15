//
// SVFilterExtDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FILTEREXTDEF_H
#define SV_FILTEREXTDEF_H

#include "base/SVPreDeclare.h"
namespace sv {

    namespace logic {

        DECLARE_SHAREPTR(SVFairData);

        DECLARE_SHAREPTR(SVFairDataLows);

        DECLARE_SHAREPTR(SVFairDataBlur);

        DECLARE_SHAREPTR(SVFairLtraLow);

    }//!namespace logic

    DECLARE_SHAREPTR(SVOpFaceBeautyExt);

}//!namespace sv

DECLARE_SHAREPTR(SVWatermark);

#endif //SV_FILTEREXTDEF_H
