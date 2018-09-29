//
// Created by lidm on 17/9/5.
//

#ifndef SV_FACEDANCE_EVENT_H
#define SV_FACEDANCE_EVENT_H

#include "../event/SVEvent.h"

//事件

//某一个消亡
class SVEventFDUnitDead : public SVEvent {
public:
    SVEventFDUnitDead(cptr8 _sceneName);
    stringc m_aniname;   //动画名称
    f32 m_gameTime;     //系统时间
    f32 m_score;        //分数
    f32 m_energy;       //能量值
    bool m_iscrit;
};

//清屏
class SVEventFDUnitAllDead : public SVEvent {
public:
    SVEventFDUnitAllDead(cptr8 _sceneName);
};

class SVEventFDTimeChange : public SVEvent {
public:
    SVEventFDTimeChange(cptr8 _sceneName);
    s32 m_iTime;
};

class SVEventFDScoreChange: public SVEvent {
public:
    SVEventFDScoreChange(cptr8 _sceneName);
    s32 m_iScore;
};


#endif //SV_FACEDANCE_EVENT_H
