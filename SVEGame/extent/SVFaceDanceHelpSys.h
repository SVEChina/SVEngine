//
// Created by lidm on 17/9/5.
//

#ifndef SV_FACEDANCE_HELPSYS_H
#define SV_FACEDANCE_HELPSYS_H

#include "../event/SVEventProc.h"

//辅助系统
//连击系统
//暴击系统(全屏消除) 能量条
class SVFaceDanceHelpSys : public SVEventProc {
    friend class SVParseGameFaceDance;
public:
    SVFaceDanceHelpSys(SVInst*_app);

    ~SVFaceDanceHelpSys();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(f32 _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    f32 getGameTime();
    f32 getEnergy();
    f32 getTotalScore();
    s32 getComboValue();
    s32 getMaxComboValue();
    s32 getMaxCritValue();
    
protected:
    f32 m_energy;       //总能量值
    s32 m_totalscore;   //总分数
    f32 m_gametime;     //游戏时间
    //
    f32 m_comboAccTime; //连击累计时间
    f32 m_comboLimit;   //连击限定值
    s32 m_comboValue;   //连击值
    bool m_comboEnable; //连击开关
    f32 m_lastDeadTime;
    //
    s32 m_maxComboNum;
    s32 m_maxCritNum;
};


#endif //SV_FACEDANCE_HELPSYS_H
