//
// SVTripleDownHelpSys.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_TRIPLEDOWN_HELPSYS_H
#define SV_TRIPLEDOWN_HELPSYS_H

#include "../event/SVEventProc.h"

class SVTripleDownHelpSys : public SVEventProc {
    friend class SVParseGameTripleDown;
public:
    SVTripleDownHelpSys(SVInst *_app);

    ~SVTripleDownHelpSys();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(float _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    f32 getGameTime();
    f32 getEnergy();
    f32 getTotalScore();
protected:
    void _sendDataChange();
    f32 m_energy;       //总能量值
    f32 m_totalscore;   //总分数
    f32 m_gametime;     //游戏时间
    f32 m_baseScore;
    
    f32 m_fLastDown;
    s32 m_iLianJi;
};


#endif //SV_TRIPLEDOWN_HELPSYS_H
