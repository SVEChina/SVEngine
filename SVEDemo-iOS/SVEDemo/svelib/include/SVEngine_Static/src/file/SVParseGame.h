//
// SVParseGame.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEGAME_H
#define SV_PARSEGAME_H

#include "SVParseData.h"

namespace sv {
    
    namespace logic{
        
        class SVParseGame : public SVParseData {
        public:
            static void parseGame(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid,cptr8 _path, SVModuleBasePtr _moduleBase);
        };

        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PARSEGAME_H
