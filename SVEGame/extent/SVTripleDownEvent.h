//
// SVTripleDownEvent.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//
#ifndef SV_TRIPLEDOWN_EVENT_H
#define SV_TRIPLEDOWN_EVENT_H

#include "../event/SVEvent.h"
#include "../base/SVVector2d.h"
#include "SVTripleDownDef.h"
//事件
class SVEventTripleDown : public SVEvent{
public:
    SVEventTripleDown();
    TRIPLEUNITTYPE eMissType;
    HARPOONTYPE    eHarpoonType;
    vector2df         vecPos;
    
};

class SVEventTotalScore : public SVEvent{
public:
    SVEventTotalScore();
    f32 fTotalScore;
    f32 fCurScore;
    vector2df         vecPos;
    
};

class SVEventGameTime : public SVEvent{
public:
    SVEventGameTime();
    s32 iTime;
};
#endif //SV_TRIPLEDOWN_EVENT_H
