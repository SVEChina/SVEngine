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
        if (isEnd()) {
            reset();
        }
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
    if(_nodePtr && m_time>0.0f){
        f32 t_lerp = m_acctime/m_time;
        if(t_lerp<0)
            t_lerp = 0.0f;
        if(t_lerp>1.0f)
            t_lerp = 1.0f;
        
        f32 t_z_rot =  PI2*t_lerp;
        t_z_rot = radToDeg(t_z_rot);
        _nodePtr->setRotation(_nodePtr->getRotation().x, _nodePtr->getRotation().y, t_z_rot);
        if (isEnd()) {
            reset();
        }
    }
}

//
SVActRotTo::SVActRotTo(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActRotTo";
}

SVActRotTo::~SVActRotTo(){
    
}

void SVActRotTo::setSrc(FVec3& _src){
    m_src = _src;
}

void SVActRotTo::setDir(FVec3& _dir){
    m_dir = _dir;
}

void SVActRotTo::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActDeform::run(_nodePtr, _dt);
    if(_nodePtr && m_time>0.0f){
        f32 t_lerp = m_acctime/m_time;
        if(t_lerp<0)
            t_lerp = 0.0f;
        if(t_lerp>1.0f)
            t_lerp = 1.0f;
        
        FVec3 t_result = m_src*(1.0f-t_lerp) + m_dir*t_lerp;
        _nodePtr->setRotation(t_result);
    }
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
SVActAlpha::SVActAlpha(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActAlpha";
    m_srcAlpha = 1.0f;
    m_tarAlpha = 1.0f;
}

SVActAlpha::~SVActAlpha(){
    
}

void SVActAlpha::run(SVNodePtr _nodePtr, f32 _dt) {
    SVActDeform::run(_nodePtr, _dt);
    if(_nodePtr && m_time>0.0f){
        f32 t_lerp = m_acctime/m_time;
        if(t_lerp<0)
            t_lerp = 0.0f;
        if(t_lerp>1.0f)
            t_lerp = 1.0f;
        
        f32 t_result = m_srcAlpha + (m_tarAlpha - m_srcAlpha)*t_lerp;
        _nodePtr->setAlpha(t_result);
    }
}

void SVActAlpha::enter(SVNodePtr _nodePtr){

}

void SVActAlpha::exit(SVNodePtr _nodePtr){
}


void SVActAlpha::setTarAlpha(f32 _alpha){
    m_tarAlpha = _alpha;
}

void SVActAlpha::setSrcAlpha(f32 _alpha){
    m_srcAlpha = _alpha;
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
        if (isEnd()) {
            reset();
        }
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
//
SVActPosition::SVActPosition(SVInst *_app):SVActDeform(_app) {
    m_acttype = "SVActRandomPosition";
    m_enableRandom = true;
    m_minPos.set(0.0f, 0.0f, 0.0f);
    m_maxPos.set(0.0f, 0.0f, 0.0f);
}

SVActPosition::~SVActPosition(){
}

void SVActPosition::run(SVNodePtr _nodePtr, f32 _dt){
    SVActDeform::run(_nodePtr, _dt);
    if(_nodePtr && m_time>0.0f){
        if (isEnd()) {
            if (m_enableRandom) {
                _randomPosition(_nodePtr);
            }
        }
    }
}

void SVActPosition::enter(SVNodePtr _nodePtr){
   
}

void SVActPosition::exit(SVNodePtr _nodePtr){
}

void SVActPosition::setMinPosition(FVec3 _minPos){
    m_minPos = _minPos;
}

void SVActPosition::setMaxPosition(FVec3 _maxPos){
    m_maxPos = _maxPos;
}

void SVActPosition::setEnableRandom(bool _random){
    m_enableRandom = _random;
}

void SVActPosition::_randomPosition(SVNodePtr _nodePtr){
    if (_nodePtr) {
        FVec3 t_n_pos;
        t_n_pos.set(0, 0, 0);
        bool t_dirty = false;
        if (m_maxPos.x > m_minPos.x) {
            t_dirty = true;
            t_n_pos.x = mApp->m_pGlobalParam->getRandomFloat(m_minPos.x,m_maxPos.x);
        }
        if (m_maxPos.y > m_minPos.y) {
            t_dirty = true;
            t_n_pos.y = mApp->m_pGlobalParam->getRandomFloat(m_minPos.y,m_maxPos.y);
        }
        if (m_maxPos.y > m_minPos.y) {
            t_dirty = true;
            t_n_pos.z = mApp->m_pGlobalParam->getRandomFloat(m_minPos.z,m_maxPos.z);
        }
        if (t_dirty) {
            _nodePtr->setPosition(t_n_pos);
        }
    }
}
