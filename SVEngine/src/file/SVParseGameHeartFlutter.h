//
// SVParseGameHeartFlutter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEGAMEHEARTFLUTTER_H
#define SV_PARSEGAMEHEARTFLUTTER_H

#include "SVParseGame.h"

namespace sv {
    
    namespace logic{
        
        class SVParseGameHeartFlutter : public SVParseGame {
        public:
            static void parseGame(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid,cptr8 _path, SVModuleBasePtr _moduleBase);
        };

        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PARSEGAMEHEARTFLUTTER_H
