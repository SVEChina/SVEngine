//
// SVParseMask.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEMASK_H
#define SV_PARSEMASK_H

#include "SVParseData.h"

namespace sv {
    
    namespace logic{
        
        class SVParseMask : public SVParseData {
        public:
            static SVNodePtr parseMask(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path);
        };

        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PARSEMASK_H
