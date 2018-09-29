//
// SVTripleDownMain.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_TRIPLEDOWN_MAIN_H
#define SV_TRIPLEDOWN_MAIN_H

#include "SVGameBase.h"
#include "SVTripleDownDef.h"

class SVGameCommonPreBatUI;
class SVGameCommonResultUI;
class SVTripleDownHelpSys;
class SVTripleDownPlaySys;
class SVTripleDownMgr;
class SVTripleDownBatUI;
//游戏主逻辑（生命周期管理）
class SVTripleDownMain : public SVGameBase {
public:
    SVTripleDownMain(SVInst *_app);

    ~SVTripleDownMain();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(float _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    SVGameCommonPreBatUI* getFDPreBatUI(){return m_pPreBatUI;}
    SVGameCommonResultUI* getResultUI(){return m_pResultUI;}
    SVTripleDownHelpSys*  getHelpSys(){return m_pGameSys;}
    SVTripleDownPlaySys*  getPlaySys(){return m_pPlaySys;}
    SVTripleDownMgr*      getUnitMgr(){return m_pUnitMgr;}
    SVTripleDownBatUI*    getBatUI(){return m_pBatUI;}
protected:
    SVGameCommonPreBatUI* m_pPreBatUI;
    SVGameCommonResultUI* m_pResultUI;
    SVTripleDownMgr*      m_pUnitMgr;
    SVTripleDownHelpSys*  m_pGameSys;
    SVTripleDownPlaySys*  m_pPlaySys;
    SVTripleDownBatUI*    m_pBatUI;
    TRIPLEDOWNSTATE m_eGameState;
    
private:
    int                   m_iTrackRate;
    float                 m_fLookLeftAngle;
    float                 m_fLookRightAngle;
    float                 m_fOpenMouthAdjust;
};


#endif //TESTMAGICENGINE_MaTripleDownMain_H
