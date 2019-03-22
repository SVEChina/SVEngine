//
// SVParseFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEFILTER_H
#define SV_PARSEFILTER_H

#include "SVParseData.h"
namespace sv {
    
    namespace logic{
        
        class SVParseFilter : public SVParseData {
        public:
            static SVFilterBasePtr parseFilter(SVInst *app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path);
        };
        
    }//!namespace logic
    
}//!namespace sv
#endif //SV_PARSEFILTER_H
