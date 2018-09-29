//
// SVActDeform.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActDeform.h"
#include "../node/SVNode.h"
/********************************************************/
//param

SVActParamDeform::SVActParamDeform()
:SVActParamTime(){
}

SVActBasePtr SVActParamDeform::genAct(SVInst *_app){
    SVActBasePtr t_act = MakeSharedPtr<SVActDeform>(_app);
    SVActParamPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamDeform>(this->shared_from_this());
    t_act->initParam(t_paramPtr);
    return t_act;
}

SVActParamMove::SVActParamMove():SVActParamTime(){
    
}

SVActBasePtr SVActParamMove::genAct(SVInst *_app){
    SVActBasePtr t_act = MakeSharedPtr<SVActMove>(_app);
    SVActParamPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamMove>(this->shared_from_this());
    t_act->initParam(t_paramPtr);
    return t_act;
}


SVActParamMoveTo::SVActParamMoveTo()
:SVActParamTime(){
    
}


SVActBasePtr SVActParamMoveTo::genAct(SVInst *_app){
    SVActBasePtr t_act = MakeSharedPtr<SVActMoveTo>(_app);
    SVActParamPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamMoveTo>(this->shared_from_this());
    t_act->initParam(t_paramPtr);
    return t_act;
}


SVActParamMoveBy::SVActParamMoveBy():SVActParamTime(){
    
}

SVActBasePtr SVActParamMoveBy::genAct(SVInst *_app){
    SVActBasePtr t_act = MakeSharedPtr<SVActMoveBy>(_app);
    SVActParamPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamMoveBy>(this->shared_from_this());
    t_act->initParam(t_paramPtr);
    return t_act;
}

SVActParamMoveBetween::SVActParamMoveBetween():SVActParamTime(){
    
}


SVActBasePtr SVActParamMoveBetween::genAct(SVInst *_app){
    SVActBasePtr t_act = MakeSharedPtr<SVActMoveBetween>(_app);
    SVActParamMoveBetweenPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamMoveBetween>(this->shared_from_this());
    t_act->initParam(t_paramPtr);
    return t_act;
}

SVActParamScaleBetween::SVActParamScaleBetween():SVActParamTime(){
    
}

SVActBasePtr SVActParamScaleBetween::genAct(SVInst *_app){
    SVActBasePtr t_act = MakeSharedPtr<SVActScaleBetween>(_app);
    SVActParamPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamScaleBetween>(this->shared_from_this());
    t_act->initParam(t_paramPtr);
    return t_act;
}

/********************************************************/

SVActDeform::SVActDeform(SVInst *_app):SVActTime(_app) {
    m_acttype = "SVActDeform";
}

SVActDeform::~SVActDeform(){
    
}

void SVActDeform::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActTime::run(_nodePtr, _dt);
}

//
SVActMove::SVActMove(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActMove";
}

SVActMove::~SVActMove(){
    
}

void SVActMove::initParam(SVActParamPtr _paramPtr){
    SVActParamMovePtr t_paramPtr = std::dynamic_pointer_cast<SVActParamMove>(_paramPtr);
    if(t_paramPtr){
        m_time = t_paramPtr->m_time;
        m_pos = t_paramPtr->m_pos;
    }
}

void SVActMove::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActDeform::run(_nodePtr, _dt);
    if(_nodePtr && m_time>0.0f){
        _nodePtr->setPosition(m_pos);
    }
}

void SVActMove::enter(SVNodePtr _nodePtr){
    if(_nodePtr){
        _nodePtr->setPosition(m_pos);
    }
}

void SVActMove::exit(SVNodePtr _nodePtr){
}

//
SVActMoveTo::SVActMoveTo(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActMoveTo";
}

SVActMoveTo::~SVActMoveTo(){
}

void SVActMoveTo::initParam(SVActParamPtr _paramPtr){
    SVActParamMoveToPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamMoveTo>(_paramPtr);
    if(t_paramPtr){
        m_time      = t_paramPtr->m_time;
        m_target    = t_paramPtr->m_target;
    }
}

void SVActMoveTo::run(SVNodePtr _nodePtr, f32 _dt){
    SVActDeform::run(_nodePtr, _dt);
    if(_nodePtr && m_time>0.0f){
        f32 t_lerp = m_acctime/m_time;
        if(t_lerp<0)
            t_lerp = 0.0f;
        if(t_lerp>1.0f)
            t_lerp = 1.0f;
    
        FVec3 t_result = m_srcpos*(1.0f-t_lerp) + m_target*t_lerp;
        _nodePtr->setPosition(t_result);
        //
        if (t_lerp == 1.0f) {
            if (m_act_callback) {
                m_act_callback(THIS_TO_SHAREPTR(SVActMoveTo), m_p_cb_obj);
            }
        }
    }
}

void SVActMoveTo::enter(SVNodePtr _nodePtr){
    if(_nodePtr){
        m_srcpos = _nodePtr->getPosition();
    }
}

void SVActMoveTo::exit(SVNodePtr _nodePtr){
}

//

SVActMoveBetween::SVActMoveBetween(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActMoveBetween";
    m_begin.set(1.0f,1.0f,1.0f);
    m_end.set(1.0f,1.0f,1.0f);
}

SVActMoveBetween::~SVActMoveBetween(){
    
}

void SVActMoveBetween::initParam(SVActParamPtr _paramPtr){
    SVActParamMoveBetweenPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamMoveBetween>(_paramPtr);
    if(t_paramPtr){
        m_time = t_paramPtr->m_time;
        m_begin = t_paramPtr->m_begin;
        m_end = t_paramPtr->m_end;
    }
}

void SVActMoveBetween::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActDeform::run(_nodePtr, _dt);
    if(_nodePtr && m_time>0.0f){
        f32 t_lerp = m_acctime/m_time;
        if(t_lerp<0){
            t_lerp = 0.0f;
            m_acctime = 0.0f;
        }
        if(t_lerp>1.0f){
            t_lerp = 1.0f;
            m_acctime = 0.0f;
        }
        FVec3 t_result = m_begin*(1.0f-t_lerp) + m_end*t_lerp;
        _nodePtr->setPosition(t_result);
    }
}

void SVActMoveBetween::enter(SVNodePtr _nodePtr){
    if(_nodePtr){
        _nodePtr->setPosition(m_begin);
    }
}

void SVActMoveBetween::exit(SVNodePtr _nodePtr){
    if(_nodePtr){
        _nodePtr->setPosition(m_end);
    }
}

void SVActMoveBetween::setBeginPos(FVec3& _pos){
    m_begin = _pos;
}

void SVActMoveBetween::setEndPos(FVec3& _pos){
    m_end = _pos;
}

//
SVActMoveBy::SVActMoveBy(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActMoveBy";
    m_dir.set(1.0f,1.0f,1.0f);
    m_sp = 100.0f;
}

SVActMoveBy::~SVActMoveBy(){
    
}

void SVActMoveBy::initParam(SVActParamPtr _paramPtr){
    SVActParamMoveByPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamMoveBy>(_paramPtr);
    if(t_paramPtr){
        m_time = t_paramPtr->m_time;
        m_sp   = t_paramPtr->m_Speed;
        m_dir  = t_paramPtr->m_dir;
    }
}

void SVActMoveBy::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActDeform::run(_nodePtr, _dt);
    if(_nodePtr && m_time>0.0f){
        FVec3 t_srcpos = _nodePtr->getPosition();
        FVec3 t_off = m_dir*_dt*m_sp;
        FVec3 t_result = t_srcpos + t_off;
        _nodePtr->setPosition(t_result);
    }
}

void SVActMoveBy::enter(SVNodePtr _nodePtr){
}

void SVActMoveBy::exit(SVNodePtr _nodePtr){
}

void SVActMoveBy::setDir(FVec3& _dir){
    m_dir = _dir;
}

void SVActMoveBy::setSpeed(f32 _sp){
    m_sp = _sp;
}

//
SVActRot::SVActRot(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActRot";
}

SVActRot::~SVActRot(){
    
}

void SVActRot::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActDeform::run(_nodePtr, _dt);
}

//
SVActRotTo::SVActRotTo(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActRotTo";
}

SVActRotTo::~SVActRotTo(){
    
}

void SVActRotTo::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActDeform::run(_nodePtr, _dt);
}

//
SVActRotBetween::SVActRotBetween(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActRotBetween";
}

SVActRotBetween::~SVActRotBetween(){
    
}

void SVActRotBetween::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActDeform::run(_nodePtr, _dt);
}

//
SVActScale::SVActScale(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActScale";
}

SVActScale::~SVActScale(){
    
}

void SVActScale::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActDeform::run(_nodePtr, _dt);
}

//
SVActScaleTo::SVActScaleTo(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActScaleTo";
    m_time = 1.0;
    m_srcscale = FVec3(1.0f, 1.0f, 1.0f);
    m_target = FVec3(2.0f, 2.0f, 2.0f);
}

SVActScaleTo::~SVActScaleTo(){
    
}

void SVActScaleTo::setSrcScale(FVec3 _scale){
    m_srcscale = _scale;
}

void SVActScaleTo::setTargetScale(FVec3 _scale){
    m_target = _scale;
}

void SVActScaleTo::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActDeform::run(_nodePtr, _dt);
    if(_nodePtr && m_time>0.0f){
        f32 t_lerp = m_acctime/m_time;
        if(t_lerp<0){
            t_lerp = 0.0f;
        }
        if(t_lerp>1.0f){
            t_lerp = 1.0f;
        }
        FVec3 t_result = m_srcscale*(1.0f-t_lerp) + m_target*t_lerp;
        _nodePtr->setScale(t_result.x, t_result.y, t_result.z);
        //
        if (t_lerp == 1.0f) {
            if (m_act_callback) {
                m_act_callback(THIS_TO_SHAREPTR(SVActScaleTo), m_p_cb_obj);
            }
        }
    }
  
}

//
SVActScaleBetween::SVActScaleBetween(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActScaleBetween";
    m_begin.set(1.0f,1.0f,1.0f);
    m_end.set(1.0f,1.0f,1.0f);
}

SVActScaleBetween::~SVActScaleBetween(){
    
}

void SVActScaleBetween::initParam(SVActParamPtr _paramPtr){
    SVActParamScaleBetweenPtr t_paramPtr = std::dynamic_pointer_cast<SVActParamScaleBetween>(_paramPtr);
    if(t_paramPtr){
        m_time = t_paramPtr->m_time;
        m_begin = t_paramPtr->m_begin;
        m_end = t_paramPtr->m_end;
    }
}

void SVActScaleBetween::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActDeform::run(_nodePtr, _dt);
    if(_nodePtr && m_time>0.0f){
        f32 t_lerp = m_acctime/m_time;
        if(t_lerp<0){
            t_lerp = 0.0f;
            m_acctime = 0.0f;
        }
        if(t_lerp>1.0f){
            t_lerp = 1.0f;
            m_acctime = 0.0f;
        }
        FVec3 t_result = m_begin*(1.0f-t_lerp) + m_end*t_lerp;
        _nodePtr->setScale(t_result.x, t_result.y, t_result.z);
    }
}

void SVActScaleBetween::enter(SVNodePtr _nodePtr){
    if(_nodePtr){
        _nodePtr->setScale(m_begin);
    }
}

void SVActScaleBetween::exit(SVNodePtr _nodePtr){
    if(_nodePtr){
        _nodePtr->setScale(m_end);
    }
}

void SVActScaleBetween::setBeginScale(FVec3& _scale){
    m_begin = _scale;
}

void SVActScaleBetween::setEndScale(FVec3& _scale){
    m_end = _scale;
}

