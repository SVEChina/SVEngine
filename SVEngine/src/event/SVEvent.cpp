//
// SVEvent.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVEvent.h"

SVEvent::SVEvent() {
    eventType = EVN_T_NONE;
    eventName = "";
}

SVEvent::~SVEvent() {
}

bool SVEvent::isEqual(SVEventPtr _event) {
    return strcmp(eventName.c_str(), _event->eventName.c_str()) == 0;
}

//
SVEvtRecycle::SVEvtRecycle(){
    m_obj = nullptr;
}

//
SVPersonEvent::SVPersonEvent() {
    personID = -1;
}

SVPersonEvent::~SVPersonEvent() {

}

bool SVPersonEvent::isEqual(SVEventPtr _event) {
    bool t_flag = SVEvent::isEqual(_event);
    SVPersonEventPtr t_person_event = DYN_TO_SHAREPTR(SVPersonEvent, _event);
    if(t_person_event->personID!=personID){
        t_flag = false;
    }
    return t_flag;
}

//
SVAnimateEvent::SVAnimateEvent() {
    eventType = EVN_T_ANIMATE;
    resid = 0;
    m_AnimateName = "";
}

bool SVAnimateEvent::isEqual(SVEventPtr _event) {
    bool t_flag = SVPersonEvent::isEqual(_event);
    if (t_flag) {
        SVAnimateEventPtr tempEvent = DYN_TO_SHAREPTR(SVAnimateEvent,_event);
        if (!tempEvent || m_AnimateName != tempEvent->m_AnimateName)
            t_flag = false;
    }
    return t_flag;
}

//解析事件
SVParseEvent::SVParseEvent() {
    eventType = EVN_T_PARSE;
    isloadEffect = false;
    resid = 0;
    screenName = "";
}

SVParseEvent::~SVParseEvent() {
}

//
SVFaceShapeEvent::SVFaceShapeEvent(){
    m_faceSmooth = 0;
    m_eyeSmooth = 0;
}

SVFaceShapeEvent::~SVFaceShapeEvent(){
    m_faceSmooth = 0;
    m_eyeSmooth = 0;
}

//拾取焦点信息
SVPickGetEvent::SVPickGetEvent(SVNodePtr _node){
    eventType = EVN_T_PICK_GET_NODE;
    m_pNode = _node;
}

SVPickGetEvent::~SVPickGetEvent(){
    m_pNode = nullptr;
}

SVPickGetNothingEvent::SVPickGetNothingEvent(){
    eventType = EVN_T_PICK_GET_NOTHING;
}

SVPickGetNothingEvent::~SVPickGetNothingEvent(){
    
}

//丢失焦点
SVPickLoseEvent::SVPickLoseEvent(SVNodePtr _node){
    m_pNode = _node;
}

SVPickLoseEvent::~SVPickLoseEvent(){
    m_pNode = nullptr;
}

//改变信息
SVPickChangeEvent::SVPickChangeEvent(SVNodePtr _getnode,SVNodePtr _losenode){
    m_pLastNode = _getnode;
    m_pNewNode = _losenode;
}

SVPickChangeEvent::~SVPickChangeEvent(){
    m_pLastNode = nullptr;
    m_pNewNode = nullptr;
}

//准备动画进入
SVReadyEnterEvent::SVReadyEnterEvent(){
    
}

SVReadyEnterEvent::~SVReadyEnterEvent(){
    
}

//红包结束动画进入
SVPacketEndEnterEvent::SVPacketEndEnterEvent(){
    
}

SVPacketEndEnterEvent::~SVPacketEndEnterEvent(){
    
}

//
SVRedPacketEvent::SVRedPacketEvent(){
    
}

SVRedPacketEvent::~SVRedPacketEvent(){
    
}

SVEffectMusicEvent::SVEffectMusicEvent(){
    eventType = EVN_T_EFFECT_MUSIC_LOAD;
    eventName = "SVEffectMusicEvent";
}

SVEffectMusicEvent::~SVEffectMusicEvent(){
    
}
