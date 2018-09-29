//
// Created by lidm on 17/9/5.
//

#ifndef SV_FLOWERDANCE_HELPSYS_H
#define SV_FLOWERDANCE_HELPSYS_H

#include "../event/SVEventProc.h"

class SVFlowerDanceHelpSys : public SVEventProc {
    friend class SVParseGameFlowerDance;
public:
    SVFlowerDanceHelpSys(SVInst*_app);

    ~SVFlowerDanceHelpSys();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(f32 _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    virtual void enter();

    s32 getScore(){ return m_totalscore; }
    
protected:
    f32 m_energy;       //总能量值
    s32 m_totalscore;   //总分数
    f32 m_gametime;     //游戏时间
    f32 m_lastTime;
    s32 m_iLeiji;
    
    stringc m_strEffect;
  
};


#endif //SV_FLOWERDANCE_HELPSYS_H
