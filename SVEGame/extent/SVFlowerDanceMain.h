//
// Created by lidm on 17/9/5.
//

#ifndef SV_FLOWER_DANCE_H
#define SV_FLOWER_DANCE_H

#include "SVGameBase.h"

//游戏主逻辑（生命周期管理）

class SVFlowerDanceMgr;
class SVFlowerDancePlayer;
class SVFlowerDanceHelpSys;
class SVFlowerDanceBatUI;
class SVFlowerDanceMain : public SVGameBase {
    friend class SVParseGameFaceDance;
public:
    SVFlowerDanceMain(SVInst*_app);

    ~SVFlowerDanceMain();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(f32 _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    s32 getState();
    
    SVFlowerDanceMgr* getFDUnitMgr(){ return m_pUnitMgr; }
    SVFlowerDancePlayer* getFDPlayer() {return m_pPlayer;}
    SVFlowerDanceHelpSys* getHelpSys() {return m_pHelpSys;}
    SVFlowerDanceBatUI* getBatUI(){ return m_pBatUI;}
protected:
    SVFlowerDanceMgr*       m_pUnitMgr;
    SVFlowerDancePlayer*    m_pPlayer;
    SVFlowerDanceHelpSys*   m_pHelpSys;
    SVFlowerDanceBatUI*     m_pBatUI;
    s32 m_state;        //0开始状态 1进行状态 2暂停状态 3结束状态
};


#endif //SV_FLOWER_DANCE_H
