//
// SVRedPacket.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENPACKET_H
#define SV_RENPACKET_H

#include "SVGameBase.h"
namespace sv{
    //
    class SVRedPacket : public SVGameBase{
    public:
        SVRedPacket(SVInst* _app);
        
        ~SVRedPacket();
        
        virtual void init(SVRedPacketReadyPtr _ready,SVRedPacketRunPtr _run,SVRedPacketEndPtr _end);
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool procEvent(SVEventPtr _event);
        
        void startListen();
        
        void stopListen();
        
        SVRedPacketReadyPtr m_packetReady;
        
        SVRedPacketRunPtr m_packetRun;
        
        SVRedPacketEndPtr m_packetEnd;

    };
}//!namespace sv





#endif //SV_RENPACKET_H
