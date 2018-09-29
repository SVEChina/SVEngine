//
// SVPipleMain.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_PIPLE_MAIN_H
#define SV_PIPLE_MAIN_H

#include "SVGameBase.h"

//游戏主逻辑（生命周期管理）

namespace sv{
    
    class SVPipleBatUI;
    class SVPipleGrid;
    class SVPipleGridMgr;
    
    //管道游戏
    class SVPipleMain : public SVGameBase {
    public:
        SVPipleMain(SVInst*_app,cptr8 _respath);
        
        ~SVPipleMain();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void pushData(cptr8 _data);
        
        virtual bool procEvent(SVEvent *_event);
        
        void setTotalTime(f32 _time){ m_gametime = _time; };
        
        s32 getState(){ return m_state; }
        
        f32 getGameTime(){ return m_curgametime; }
        
        s32 getPlayerCode(){ return m_playerCode; }
        
        s32 getMingci(){ return m_mingci; }
        
        cptr8 getResPath();
        
        SVPipleGridMgr* getPipleMgr(){ return m_pPipleMgr; }
        
        SVPipleBatUI* getPipleBatUI(){ return m_pBatUI; }
        
    protected:
        SVPipleGridMgr* m_pPipleMgr;
        SVPipleBatUI* m_pBatUI;
        s32 m_state;        //0开始状态 1进行状态 2暂停状态 3结束状态
        f32 m_curgametime;
        f32 m_gametime;
        s32 m_playerCode;
        s32 m_mingci;
        stringc m_respath;
    };

}


#endif //SV_PIPLE_MAIN_H
