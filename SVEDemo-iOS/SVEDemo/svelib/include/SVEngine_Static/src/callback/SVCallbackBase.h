//
// SVCallbackBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CALLBACKBASE_H
#define SV_CALLBACKBASE_H

#include "../base/SVGBase.h"

namespace sv {
    
    class SVCallbackBase : public SVGBase {
    public:
        SVCallbackBase(SVInst* _app);
        
        ~SVCallbackBase();
    };

    
}//!namespace sv



#endif //SV_CALLBACKBASE_H
