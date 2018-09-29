//
// Created by lidm on 17/9/5.
//

#ifndef SV_FACEDANCE_RESULTUI_H
#define SV_FACEDANCE_RESULTUI_H

#include "SVGameUIBase.h"
class SVGameUIMgr;
//结算UI
class SVFaceDanceResultUI : public SVGameUIBase {
    friend class SVParseGameFaceDance;
public:
    SVFaceDanceResultUI(SVInst*_app);

    ~SVFaceDanceResultUI();
    
    virtual void init(SVGameUIMgr* pUIMgr);
    
    virtual void destroy();
    
    virtual void enter();
    
    virtual void exit();
    
    virtual void update(f32 _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    void setScore(int iScore);
    
protected:
    stringc m_EndAni,m_EndAniLoop;
    int m_iScore;
};


#endif //SV_FaceDanceResultUI_H
