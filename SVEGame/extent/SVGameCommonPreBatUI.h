//
// Created by yinlong on 17/10/20.
//

#ifndef SV_GAMECOMMON_PREBATUI_H
#define SV_GAMECOMMON_PREBATUI_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"
#include "SVGameUIBase.h"

//战斗前UI
class SVSpineNode;
class SVUIParamBase;
class SVGameUIMgr;
class SVGameCommonPreBatUI : public SVGameUIBase {
public:
    SVGameCommonPreBatUI(SVInst*_app);

    ~SVGameCommonPreBatUI();
    
    virtual void init(SVGameUIMgr* pUIMgr);
    
    virtual void destroy();
    
    virtual void enter();
    
    virtual void exit();
    
    virtual void update(f32 _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    void setSound(cptr8 _pSound);
    
protected:
    vector3df m_vecDaoShu;
    stringc m_strDaoShuAni;
    stringc m_strDaoShuSound;
    f32 m_fUiStateTime;
};


#endif //SV_GAMECOMMON_PREBATUI_H
