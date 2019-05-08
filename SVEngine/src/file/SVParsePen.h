//
// SVParsePen.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEPEN_H
#define SV_PARSEPEN_H

#include "SVParseData.h"

namespace sv {
    
    namespace logic{
        
        class SVParsePen : public SVParseData {
        public:
            SVParsePen(SVInst *_app);
            
            virtual ~SVParsePen();
            
            SVModuleBasePtr parse(cptr8 _path, s32 _resid);
        protected:
            void _parsePen(SVInst *_app, RAPIDJSON_NAMESPACE::Value &_item, s32 _resid,cptr8 _path);
        };
    }//!namespace logic
}//!namespace sv

#endif //SV_PARSEPEN_H
