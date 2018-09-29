//
// SVParseBitfont.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEBITFONT_H
#define SV_PARSEBITFONT_H

#include "SVParseData.h"

namespace sv {
    
    namespace logic{
        
        class SVParseBitfont : public SVParseData {
        public:
            static SVNodePtr parseBitfont(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid,cptr8 _path);
        };

        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PARSEBITFONT_H
