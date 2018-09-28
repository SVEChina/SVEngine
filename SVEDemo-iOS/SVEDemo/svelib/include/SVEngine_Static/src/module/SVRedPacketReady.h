//
// SVRedPacketReady.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_REDPACKET_READY_H
#define SV_REDPACKET_READY_H

#include "SVGameReady.h"

namespace sv{
    enum READYMODE{
        READY_MODE_54321,
        READY_MODE_321
    };
    enum READYUNITSTATE{
        READY_UNIT_WAIT = 0,
        READY_UNIT_COUNTDONW,
        READY_UNIT_READY1,
        READY_UNIT_READY2,
        READY_UNIT_READY3
    };
    
    class SVRedPacketReady : public SVGameReady{
    public:
        SVRedPacketReady(SVInst* _app);
        
        ~SVRedPacketReady();
        
        void init();
        
        void destroy();
        
        void reset();
        
        void enter();
        
        void exit();
        
        void update(f32 _dt);
        
        bool isEnd();
        
        void generateCountDownUnit(SVSpineNodePtr _spineNode);
        
        void generateReadyUnit(SVSpineNodePtr _spineNode);
        
        void generateBackUnit(SVSpriteNodePtr _spriteNode);
        
        void sendEventReadyEnter();
        
        void setReadyMode(READYMODE _mode);
        
    protected:
        SVSpriteNodePtr m_backSprite;
        SVRedPacketReadyUnitPtr m_CountDownUnit;
        SVRedPacketReadyUnitPtr m_ReadyUnit;
        READYMODE m_mode;
    public:
        bool m_isEnd;
        f32 m_accTime;
        READYUNITSTATE m_readyState;
    };
    
    class SVRedPacketReadyUnit : public SVGBase{
    public:
        SVRedPacketReadyUnit(SVInst *_app);
        
        ~SVRedPacketReadyUnit();
        
        void init();
        
        void destroy();
        
        void update(f32 _dt);
        
        void setEnd();
        
        bool isEnd();
        
        void enter();
        
        void exit();
        
        void play(cptr8 _name);
        
        void stop();
        
        void setSpineNode(SVSpineNodePtr _spineNode);
        
        SVSpineNodePtr getSpineNode();
    protected:
        SVSpineNodePtr m_spineNode;
        bool m_isEnd;
        f32 m_accTime;
    };
}//!namespace sv


#endif //SV_REDPACKET_READY_H
