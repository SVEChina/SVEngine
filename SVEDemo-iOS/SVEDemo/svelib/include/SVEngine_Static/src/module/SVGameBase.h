//
// SVGameBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GAMEBASE_H
#define SV_GAMEBASE_H

#include "SVModuleBase.h"

namespace sv{
    //
    class SVGameBase : public SVModuleBase {
    public:
        //游戏阶段
        enum SVGameStage {
            E_G_STAGE_WAIT,
            E_G_STAGE_BEGIN,
            E_G_STAGE_RUN,
            E_G_STAGE_END
        };
        SVGameBase(SVInst* _app);
        
        ~SVGameBase();
        
        virtual void init(SVGameReadyPtr _ready,SVGameRunPtr _run,SVGameEndPtr _end);
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool procEvent(SVEventPtr _event);
        
    protected:
        SVGameReadyPtr m_pGameReady;
        SVGameRunPtr m_pGameRun;
        SVGameEndPtr m_pGameEnd;
        SVGameStage m_stage;
    };
}//!namespace sv





#endif //SV_GAMEBASE_H
