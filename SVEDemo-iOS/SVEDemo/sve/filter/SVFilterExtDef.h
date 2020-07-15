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
        
        DECLARE_SHAREPTR(SVFaceMattingFilter);

    }//!namespace logic

    DECLARE_SHAREPTR(SVOpFaceBeautyExt);
    DECLARE_SHAREPTR(SVOpFaceDeform);
    DECLARE_SHAREPTR(SVFaceDeform);
    DECLARE_SHAREPTR(SVOpFaceDeformSmooth);
    DECLARE_SHAREPTR(SVOpFaceDeformEye);
    DECLARE_SHAREPTR(SVOpDeformSmooth);
    DECLARE_SHAREPTR(SVOpFaceDeformParss);

}//!namespace sv



#endif //SV_FILTEREXTDEF_H
