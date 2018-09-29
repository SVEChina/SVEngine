//
// Created by lidm on 17/9/5.
//

#ifndef SV_QCLICK_MAIN_H
#define SV_QCLICK_MAIN_H

#include "SVGameBase.h"

//游戏主逻辑（生命周期管理）

class SVQClickBatUI;
class SVQClickUnit;
class SVQClickUnitMgr;

//快速点击小游戏
class SVQClickMain : public SVGameBase {
public:
    SVQClickMain(SVInst*_app,cptr8 _respath);

    ~SVQClickMain();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(f32 _dt);
    
    virtual void pushData(cptr8 _data);
    
    virtual void setHead(s32 _playercode,s32 _width,s32 _height,void* _pData);
    
    virtual bool procEvent(SVEvent *_event);
    
    void setTotalTime(f32 _time){ m_gametime = _time; };
    
    s32 getState(){ return m_state; }
    
    f32 getGameTime(){ return m_curgametime; }
    
    s32 getPlayerCode(){ return m_playerCode; }
    
    s32 getMingci(){ return m_mingci; }
    
    cptr8 getResPath();
    
    SVQClickUnitMgr* getFDUnitMgr(){ return m_pUnitMgr; }
    
    SVQClickBatUI* getFDBatUI(){ return m_pBatUI; }
    
protected:
    SVQClickUnitMgr* m_pUnitMgr;         //点击触发
    SVQClickBatUI* m_pBatUI;             //战斗主UI
    s32 m_state;        //0开始状态 1进行状态 2暂停状态 3结束状态
    f32 m_curgametime;
    f32 m_gametime;
    s32 m_playerCode;
    s32 m_mingci;
    stringc m_respath;
};


#endif //SV_QCLICK_MAIN_H
