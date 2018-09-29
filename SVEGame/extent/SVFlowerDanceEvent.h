//
// Created by yinlong on 17/10/20.
//

#ifndef SV_FLOWERDANCE_EVENT_H
#define SV_FLOWERDANCE_EVENT_H

#include "../event/SVEvent.h"
#include "../base/SVVector3d.h"
//事件

class SVEeventFDPlayerPos : public SVEvent {
public:
    SVEeventFDPlayerPos();
    vector3df m_vecPos;
};

class SVEventEnterFlower : public SVEvent {
public:
    SVEventEnterFlower();
    
    f32 m_fFlower_inner;
    f32 m_fFlower_outter;
    vector3df m_vecPos;
    s32 m_iType;
    bool m_bIsOpened;
};

class SVEventInitFlower : public SVEvent {
public:
    SVEventInitFlower();
    
    f32 m_fFlower_inner;
    f32 m_fFlower_outter;
    vector3df m_vecPos;
};

class SVEventFlowerScoreChange: public SVEvent {
public:
    SVEventFlowerScoreChange();
    s32 m_iScore;
};




#endif //SV_GAMECOMMONE_EVENT_H
