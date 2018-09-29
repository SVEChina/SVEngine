//
// SVParseAnimate.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEANIMATE_H
#define SV_PARSEANIMATE_H

#include "SVParseData.h"
//spine动画解析

namespace sv {
    
    namespace logic{
        
        class SVParseAnimate : public SVParseData {
        public:
            static SVNodePtr parseAnim(SVInst *app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path);
        };
        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PARSEANIMATE_H
