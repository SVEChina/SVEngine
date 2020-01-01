//
// SVParseFrameAni.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEFRAMEANI_H
#define SV_PARSEFRAMEANI_H

#include "SVParseData.h"
//spine动画解析

namespace sv {
    
    namespace logic{
        
        class SVParseFrameAni : public SVParseData {
        public:
            static SVNodePtr parseFrameAni(SVInst *app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path);
        };
        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PARSEFRAMEANI_H
