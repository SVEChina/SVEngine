//
// SVActDeform.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActDeform.h"
#include "../node/SVNode.h"
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

void SVActMoveTo::setBeginPos(FVec3& _pos){
    m_srcpos = _pos;
}

void SVActMoveTo::setEndPos(FVec3& _pos){
    m_target = _pos;
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
            m_isEnd = true;
        }
    }
}

void SVActMoveTo::enter(SVNodePtr _nodePtr){
    if(_nodePtr){
        m_isEnd = false;
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
            m_isEnd = true;
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

