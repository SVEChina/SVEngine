//
// SVParseGameQClick.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_PARASEGAME_QCLICK_H
#define SV_PARASEGAME_QCLICK_H

#include "SVParseGameBase.h"

class SVQClickMain;

namespace sv {
    
    class SVParseGameQClick : public SVParseGameBase {
    public:
        SVParseGameQClick(SVInst *_app);
        
        ~SVParseGameQClick();
        
        bool parse(RAPIDJSON_NAMESPACE::Document& doc,cptr8 _filename);
        
        SVGameBase* generate();
        
    protected:
        f32 m_timelen;
        stringc m_ani_press;
        stringc m_ani_end;
        array<stringc> m_bg;
    };
    
//!namespace
}


#endif //SV_PARASEGAME_QCLICK_H
