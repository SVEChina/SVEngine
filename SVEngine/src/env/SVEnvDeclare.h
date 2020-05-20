//
// SVEnvDeclare.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ENVDECLARE_H
#define SV_ENVDECLARE_H

#include "../base/SVDef.h"
#include "../base/SVPreDeclare.h"

namespace sv {
    
    //环境
    DECLARE_SHAREPTR(SVCtxBase);
#ifdef SV_IOS
    DECLARE_SHAREPTR(SVCtxIOS);
    //DECLARE_SHAREPTR(SVContextMetal);
#endif
        
#ifdef SV_ANDROID
    DECLARE_SHAREPTR(SVContextAndriod);
    DECLARE_SHAREPTR(SVEGLContext);
#endif
        
#ifdef SV_OSX
    DECLARE_SHAREPTR(SVCtxOSXGL);
#endif


}//!namespace sv

#endif //SV_ENVDECLARE_H
