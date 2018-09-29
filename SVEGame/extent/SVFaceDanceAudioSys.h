//
// Created by lidm on 17/9/5.
//

#ifndef SV_FACEDANCE_AUDIOSYS_H
#define SV_FACEDANCE_AUDIOSYS_H

#include "../event/SVEventProc.h"

//辅助系统
//连击系统
//暴击系统(全屏消除) 能量条

class SVFaceDanceAudioSys : public SVEventProc {
    friend class SVParseGameFaceDance;
public:
    SVFaceDanceAudioSys(SVInst* _app);

    ~SVFaceDanceAudioSys();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(float _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
protected:
    
    stringc m_strGameBgm;
    int     m_iGameBgmId;
    stringc m_strDeadEffect;
};


#endif //SV_FACEDANCE_AUDIOSYS_H
