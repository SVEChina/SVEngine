//
// SVModify.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MODIFY_H
#define SV_MODIFY_H

#include "SVGBase.h"

namespace sv {
    
        class SVModify : public SVGBase {
        public:
            SVModify(SVInst *_app);
            
            ~SVModify();
            
            void exec(SVObjectPtr _obj);
        };
    
}//!namespace sv


#endif //SV_MODIFY_H
