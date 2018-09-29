//
// SVTripleDownDef.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//
#ifndef SV_TRIPLEDOWN_DEFINE_H
#define SV_TRIPLEDOWN_DEFINE_H

enum TRIPLEDOWNSTATE {
    TDS_BEGIN = 0,
    TDS_GAME_BEGIN,
    TDS_PREBAT,
    TDS_BAT,
    TDS_RESULT,
    TDS_GAME_PAUSE,
    TDS_END
};

enum TRIPLEUNITTYPE{
    TUT_HONG = 0,
    TUT_HUANG,
    TUT_LV,
    TUT_TYPE_MAX
};

enum HARPOONTYPE{
    HT_HONG = 0,
    HT_HUANG = 1,
    HT_LV = 2
};

enum TRIPLEUNITSTATE {
    TUS_BEGIN = 0,
    TUS_ALIVE,
    TUS_DEADING,
    TUS_DEADED,
    TUS_END
};

enum MOUTHSTATE{
    MS_INIT,
    MS_OPEN,
    MS_CLOSE
};

enum FACETOWARDS{
    FT_INIT,
    FT_LEFT,
    FT_MIDDLE,
    FT_RIGHT
};

enum CIRCLEANI{
    CA_START,
    CA_LOOP,
    CA_END,
    CA_BAOZHA
};

enum HARPOONANI{
    HA_COMMON,
    HA_SELECT
};
#endif //SV_TRIPLEDOWN_DEFINE_H
