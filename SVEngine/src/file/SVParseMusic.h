//
// SVParseMusic.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEMUSIC_H
#define SV_PARSEMUSIC_H

#include "SVParseData.h"
#include "../module/SVModuleDef.h"
namespace sv {
    
    namespace logic{
        
        class SVParseMusic : public SVParseData {
        public:
            static SVEffectMusicPtr parseMusic(SVInst *app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path);
        };
        
    }//!namespace logic
    
}//!namespace sv
#endif //SV_PARSEMUSIC_H

