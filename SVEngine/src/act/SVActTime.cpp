//
// SVActTime.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActTime.h"
#include "../node/SVNode.h"
//param
SVActParamTime::SVActParamTime():SVActParam(){
    m_time = 0.0f;
}

SVActBasePtr SVActParamTime::genAct(SVInst *_app){
    SVActBasePtr t_act = MakeSharedPtr<SVActTime>(_app);
    SVActParamPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamTime>(this->shared_from_this());
    if (t_paramPtr) {
        t_act->initParam(t_paramPtr);
    }
    return t_act;
}

SVActParamWait::SVActParamWait():SVActParamTime(){
    
}

SVActBasePtr SVActParamWait::genAct(SVInst *_app){
    SVActBasePtr t_act = MakeSharedPtr<SVActWait>(_app);
    SVActParamPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamWait>(this->shared_from_this());
    if (t_paramPtr) {
        t_act->initParam(t_paramPtr);
    }
    return t_act;
}

SVActParamHide::SVActParamHide():SVActParamTime(){
    
}

SVActBasePtr SVActParamHide::genAct(SVInst *_app){
    SVActBasePtr t_act = MakeSharedPtr<SVActHide>(_app);
    SVActParamPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamHide>(this->shared_from_this());
    if (t_paramPtr) {
        t_act->initParam(t_paramPtr);
    }
    return t_act;
}

//动作
SVActTime::SVActTime(SVInst *_app):SVActBase(_app) {
    m_acttype = "SVActTime";
    m_time = 0.0f;
    m_acctime = 0.0f;
}

SVActTime::~SVActTime(){
    
}


void SVActTime::initParam(SVActParamPtr _paramPtr){
    SVActParamTimePtr t_paramPtr = std::dynamic_pointer_cast<SVActParamTime>(_paramPtr);
    if(t_paramPtr){
        m_time = t_paramPtr->m_time;
    }
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
