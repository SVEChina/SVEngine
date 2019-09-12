//
// SVParseAniTrigger.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FILE_PARSEANITRIGGER_H
#define SV_FILE_PARSEANITRIGGER_H

#include "SVParseData.h"
#include "../act/SVActDeclare.h"
namespace sv {
    
    namespace logic{
        
        class SVParseAniTrigger : public SVParseData {
        public:
            static SVAniTriggerPtr parseAniTrigger(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid,cptr8 _path);
        };

        
    }//!namespace logic
    
}//!namespace sv
#endif //SV_FILE_PARSEANITRIGGER_H
