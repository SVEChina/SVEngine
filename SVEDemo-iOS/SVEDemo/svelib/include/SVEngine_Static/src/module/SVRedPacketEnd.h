//
// SVRedPacketEnd.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENPACKET_END_H
#define SV_RENPACKET_END_H

#include "SVGameEnd.h"

namespace sv{
    class SVRedPacketEnd : public SVGameEnd {
    public:
        SVRedPacketEnd(SVInst* _app);
        
        ~SVRedPacketEnd();
        
        void init();
        
        void destroy();
        
        void enter();
        
        void exit();
        
        void update(f32 _dt);
        
        bool isEnd();
        
        void setSpineNode(SVSpineNodePtr _spineNode);
        
        SVSpineNodePtr getSpineNode();
    protected:
        void _sendEndEnter(f32 _pt_x, f32 _pt_y);
        
        f32 m_accTime;
        
        SVSpineNodePtr m_pEndSpine;
    public:
        bool m_isEnd;
    };
}//!namespace sv


#endif //SV_RENPACKET_END_H
