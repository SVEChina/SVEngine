//
// Created by lidm on 17/9/5.
//

#ifndef SV_FACEDANCE_MAIN_H
#define SV_FACEDANCE_MAIN_H

#include "SVGameBase.h"

//游戏主逻辑（生命周期管理）

class SVFaceDanceHelpSys;
class SVFaceDanceBatUI;
class SVFaceDanceResultUI;
class SVFaceDanceUnit;
class SVFaceDanceResConfig;
class SVFaceDanceUnitMgr;
class SVFaceDanceAudioSys;
class SVGameCommonPreBatUI;

class SVFaceDanceMain : public SVGameBase {
    friend class SVParseGameFaceDance;
public:
    SVFaceDanceMain(SVInst*_app);

    ~SVFaceDanceMain();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(f32 _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    s32 getState();
    
    SVFaceDanceUnitMgr* getFDUnitMgr(){ return m_pUnitMgr; }
    SVFaceDanceResConfig* getFDResCfg(){ return m_pResConfig; }
    SVFaceDanceHelpSys* getFDHelpSys(){ return m_pHelpSys; }
    SVFaceDanceBatUI* getFDBatUI(){ return m_pBatUI; }
    SVFaceDanceResultUI* getFDResultUI(){ return m_pResultUI; }
    SVGameCommonPreBatUI* getFDPreBatUI(){ return m_pPreBatUI; }
    SVFaceDanceAudioSys* getFDAudioSys() {return m_pAudioSys;}
    
protected:
    SVFaceDanceUnitMgr* m_pUnitMgr;         //表情管理
    SVFaceDanceResConfig* m_pResConfig;     //资源配置
    SVFaceDanceHelpSys* m_pHelpSys;         //帮助系统
    SVFaceDanceBatUI* m_pBatUI;             //战斗主UI
    SVFaceDanceResultUI* m_pResultUI;       //结算UI
    SVGameCommonPreBatUI* m_pPreBatUI;
    SVFaceDanceAudioSys* m_pAudioSys;       //声音系统
    s32 m_state;        //0开始状态 1进行状态 2暂停状态 3结束状态
};


#endif //SV_FACEDANCE_MAIN_H
