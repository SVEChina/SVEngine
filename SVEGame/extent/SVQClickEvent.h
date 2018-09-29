//
// Created by lidm on 17/9/5.
//

#ifndef SV_QCLICK_EVENT_H
#define SV_QCLICK_EVENT_H

#include "../event/SVEvent.h"

//加分数
class SVEventAddScore : public SVEvent {
public:
    SVEventAddScore();
    f32 m_score;        //分数
    s32 m_playerCode;  //
};

//设置分数
class SVEventSetScore : public SVEvent {
public:
    SVEventSetScore();
    f32 m_score;        //分数
    s32 m_playerCode;  //
    stringc m_playername;
};

//设置游戏信息
class SVEventGameInfo : public SVEvent {
public:
    SVEventGameInfo();
    f32 m_timelen;       //时间
    s32 m_playerCode;    //角色ID
};


#endif //SV_QCLICK_EVENT_H
