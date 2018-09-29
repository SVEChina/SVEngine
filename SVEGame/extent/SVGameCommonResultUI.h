//
// Created by yinlong on 17/11/10.
//

#ifndef SV_GAMECOMMON_RESULTUI_H
#define SV_GAMECOMMON_RESULTUI_H

#include "../event/SVEventProc.h"
#include "SVGameUIBase.h"
//结算UI

class SVUIParamBase;
class SVSpineNode;
class SVBitFontNode;
class SVGameUIMgr;
class SVGameCommonResultUI : public SVGameUIBase {
public:
    SVGameCommonResultUI(SVInst*_app);

    ~SVGameCommonResultUI();
    
    virtual void init(SVGameUIMgr* pUIMgr);
    
    virtual void destroy();
    
    virtual void enter();
    
    virtual void exit();
    
    virtual void update(f32 _dt);
    
    virtual bool procEvent(SVEvent *_event);
protected:
    stringc m_EndAni,m_EndAniLoop;
    
};


#endif //SV_GAMECOMMON_RESULTUI_H
