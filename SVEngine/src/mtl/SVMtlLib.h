//
// SVShaderMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLLIB_H
#define SV_MTLLIB_H

#include "SVMtlDef.h"
#include "../base/SVObject.h"

namespace sv {
    
    /*引擎提供的默认材质库*/
    
    class SVMtlLib : public SVObject {
    public:
        SVMtlLib();
        
        static SVMtlCorePtr getSkinMtl();
        
        static SVMtlCorePtr get3DNorMtl();

    };
    
}//!namespace sv


#endif //SV_MTLLIB_H
