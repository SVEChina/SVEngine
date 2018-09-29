//
// SVParseGamePiple.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_PARASEGAME_PIPLE_H
#define SV_PARASEGAME_PIPLE_H

#include "SVParseGameBase.h"

namespace sv {
    
    class SVPipleMain;

    class SVParseGamePiple : public SVParseGameBase {
    public:
        SVParseGamePiple(SVInst *_app);

        ~SVParseGamePiple();

        bool parse(RAPIDJSON_NAMESPACE::Document& doc,cptr8 _filename);

        SVGameBase* generate();

    protected:
        f32 m_timelen;
        stringc m_sp_bg;
        stringc m_ani_wave;
        stringc m_ani_line;
        s32 m_piple_size_x;
        s32 m_piple_size_y;
        f32 m_grid_size;
        f32 m_grid_boder;
    };
//!namespace
}


#endif //SV_PARASEGAME_PIPLE_H
