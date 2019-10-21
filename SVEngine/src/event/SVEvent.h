//
// SVEvent.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_EVENT_H
#define SV_EVENT_H

#include "../base/SVObject.h"
#include "SVEventDef.h"
#include "../node/SVNodeDeclare.h"

namespace sv {
    
    class SVEvent : public SVObject {
    public:
        SVEvent();
        
        ~SVEvent();
        
        virtual bool isEqual(SVEventPtr _event);
        
        SV_EVENT_TYPE eventType;      //消息类型
        SVString eventName;          //消息名称
    };
    
    //回收事件
    class SVEvtRecycle : public SVEvent {
    public:
        SVEvtRecycle();
        
        SVObjectPtr m_obj;
    };
    
    class SVPersonEvent : public SVEvent {
    public:
        SVPersonEvent();
        
        ~SVPersonEvent();
        
        bool isEqual(SVEventPtr _event);
        
        s32 personID;         //关联人物
    };
    
    class SVFaceShapeEvent : public SVEvent {
    public:
        SVFaceShapeEvent();
        
        ~SVFaceShapeEvent();
        
        s32 m_faceSmooth;
        s32 m_eyeSmooth;
    };
    
    class SVAnimateEvent : public SVPersonEvent {
    public:
        SVAnimateEvent();
        
        bool isEqual(SVEventPtr _event);
        
        s32 resid;
        s32 uid;
        SVString m_AnimateName;
    };
    
    class SVParseEvent : public SVEvent {
    public:
        SVParseEvent();
        
        ~SVParseEvent();
        
        bool isloadEffect;
        s32 resid;
        SVString screenName;
    };
    
    //
    class SVPickGetEvent : public SVEvent {
    public:
        SVPickGetEvent(SVNodePtr _node);
        
        ~SVPickGetEvent();
        
        SVNodePtr m_pNode;
    };
    class SVPickGetUIEvent : public SVEvent {
    public:
        SVPickGetUIEvent(SVNodePtr _node);
       
        ~SVPickGetUIEvent();
       
        SVNodePtr m_pNode;
    };
    //
    class SVPickGetNothingEvent : public SVEvent {
    public:
        SVPickGetNothingEvent();
        
        ~SVPickGetNothingEvent();
        
        f32 m_px;
        
        f32 m_py;
    };
    //
    class SVPickLoseEvent : public SVEvent {
    public:
        SVPickLoseEvent(SVNodePtr _node);
        
        ~SVPickLoseEvent();
        
        SVNodePtr m_pNode;
    };
    
    //
    class SVPickChangeEvent : public SVEvent {
    public:
        SVPickChangeEvent(SVNodePtr _getnode,SVNodePtr _losenode);
        
        ~SVPickChangeEvent();
        
        SVNodePtr m_pLastNode;
        SVNodePtr m_pNewNode;
    };
    
    class SVRedPacketEvent : public SVEvent {
    public:
        SVRedPacketEvent();
        
        ~SVRedPacketEvent();
    };
    //
    class SVEffectMusicEvent : public SVEvent {
    public:
        SVEffectMusicEvent();
        
        ~SVEffectMusicEvent();
        
        SVString path;
    };
    
    class SVEventPersonExpression : public SVEvent {
    public:
        SVEventPersonExpression();
        
        ~SVEventPersonExpression();
        
        s32 m_personID;
    };
    
}//!namespace sv


#endif //SV_EVENT_H
