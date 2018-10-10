//
// SVActTime.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActTime.h"
#include "../node/SVNode.h"
//动作
SVActTime::SVActTime(SVInst *_app):SVActBase(_app) {
    m_acttype = "SVActTime";
    m_time = 0.0f;
    m_acctime = 0.0f;
}

SVActTime::~SVActTime(){
    
}

void SVActTime::run(SVNodePtr _node, f32 _dt) {
    SVActBase::run(_node, _dt);
    m_acctime += _dt;
}

void SVActTime::reset(){
    m_acctime = 0.0f;
}

void SVActTime::setTime(f32 _time){
    m_time = _time;
}

bool SVActTime::isEnd(){
    if(m_acctime>=m_time)
        return true;
    return false;
}

//
SVActWait::SVActWait(SVInst *_app):SVActTime(_app){
    m_acttype = "SVActWait";
}

SVActWait::~SVActWait(){
    
}

//
SVActHide::SVActHide(SVInst *_app):SVActWait(_app){
    m_acttype = "SVActHide";
}

SVActHide::~SVActHide(){
    
}

void SVActHide::run(SVNodePtr _node, f32 _dt) {
    SVActWait::run(_node,_dt);
    if(_node){
        _node->setvisible(false);
    }
}

void SVActHide::enter(SVNodePtr _node){
    if(_node){
        _node->setvisible(false);
    }
}

void SVActHide::exit(SVNodePtr _node){
    if(_node){
        _node->setvisible(true);
    }
}
