//
// SVParseSprite.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSESPRITE_H
#define SV_PARSESPRITE_H

#include "SVParseData.h"

namespace sv {
    
    namespace logic{
        
        class SVParseSprite : public SVParseData {
        public:
            static SVNodePtr parseSprite(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid,cptr8 _path);
        };

        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PARSESPRITE_H
