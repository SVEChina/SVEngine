//
// SVActFollow.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActFollow.h"
#include "../node/SVNode.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVPersonTracker.h"
#include "../basesys/SVConfig.h"
SVActFollow::SVActFollow(SVInst *_app):SVActBase(_app){
    m_validDis = 100.0f;
    m_speed = 300.0f;
    m_targetNodePtr = nullptr;
}

SVActFollow::~SVActFollow() {
    m_targetNodePtr = nullptr;
}

//设置目标节点
void SVActFollow::setTargetNode(SVNodePtr _nodePtr){
    if(m_targetNodePtr!=_nodePtr){
        m_targetNodePtr = _nodePtr;
    }
}

//设置有效距离
void SVActFollow::setValidDis(f32 _valid){
    m_validDis = _valid;
}

//设置相机移动速度
void SVActFollow::setSpeed(f32 _sp){
    m_speed = _sp;
}

//运行
void SVActFollow::run(SVNodePtr _nodePtr, f32 dt) {
    if(m_targetNodePtr && _nodePtr){
        //保持xoy平面的距离
        FVec3 t_pos_dst = m_targetNodePtr->getPosition();
        FVec3 t_pos_src = _nodePtr->getPosition();
        //
        FVec3 t_xoy_dst = FVec3(t_pos_dst.x,t_pos_dst.y,0.0f);
        FVec3 t_xoy_src = FVec3(t_pos_src.x,t_pos_src.y,0.0f);
        f32 t_len = (t_xoy_dst-t_xoy_src).length();// getDistanceFrom(t_xoy_src);
        if(t_len<=m_validDis){
            return;
        }else if(t_len<1.3*m_validDis){
            //朝向目标移动
            FVec3 t_dir = t_xoy_dst - t_xoy_src;
            t_dir.normalize();
            FVec3 t_new_pos = t_xoy_src + t_dir*(dt*m_speed);
            _nodePtr->setPosition(t_new_pos.x,t_new_pos.y,t_pos_src.z);
            
        }else{
            //先保持1.3的距离
            FVec3 t_dir = t_xoy_dst - t_xoy_src;
            t_dir.normalize();
            FVec3 t_new_pos = t_xoy_dst - t_dir*1.3f*m_validDis;
            _nodePtr->setPosition(t_new_pos.x,t_new_pos.y,t_pos_src.z);
        }
    }
}

bool SVActFollow::isEnd() {
    return false;
}


//
SVActCircle::SVActCircle(SVInst *_app):SVActBase(_app){
    m_radius = 10.0f;
    m_center.set(0.0f, 0.0f,0.0f);
    m_angle = 0.0f;
    m_angleSp = 180.0f;
}

SVActCircle::~SVActCircle() {
}

void SVActCircle::setCircleParam(FVec3& _pos,f32 _radius){
    m_center = _pos;
    m_radius = _radius;
}

void SVActCircle::setAngleParam(f32 _sp,f32 _angle){
    m_angleSp = _sp;
    m_angle = _angle;
}

//运行
void SVActCircle::run(SVNodePtr _nodePtr, f32 dt) {
    if( _nodePtr ){
        m_angle += m_angleSp*dt;
        if(m_angle>360.0){
            s32 t_num = ((s32)m_angle)/360;
            m_angle -= 360.0f*t_num;
        }
        //计算位置
        f32 t_new_x = cos( degToRad(m_angle) )*m_radius + m_center.x;
        f32 t_new_y = sin( degToRad(m_angle) )*m_radius + m_center.y;
        f32 t_new_z = m_center.z;
        _nodePtr->setPosition(t_new_x, t_new_y, t_new_z);
        //计算角度
        if( m_angleSp>0 ){
            _nodePtr->setRotation(0.0f,0.0f,m_angle);
        }else{
            _nodePtr->setRotation(0.0f,0.0f,m_angle + 180.0f);
        }
    }
}

bool SVActCircle::isEnd() {
    return false;
}


////
SVActFollowPerson::SVActFollowPerson(SVInst *_app, s32 _personID):SVActBase(_app){
    m_personID = _personID;
    m_bindIndex = 0;
    m_offsetX = 0;
    m_offsetY = 0;
    m_scaleX = 0;
    m_scaleY = 0;
}

SVActFollowPerson::~SVActFollowPerson(){
    
}

void SVActFollowPerson::run(SVNodePtr _nodePtr, f32 dt){
    if (_nodePtr) {
        SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(m_personID);
        if (t_person && t_person->getExist()) {
            _nodePtr->setvisible(true);
            SVPersonTrackerPtr t_personTracker = t_person->getTracker();
            f32 t_pt_x = t_person->getFaceDataX(m_bindIndex);
            f32 t_pt_y = t_person->getFaceDataY(m_bindIndex);
            f32 t_yaw = t_person->getFaceRot().y;
            f32 t_roll = t_person->getFaceRot().z;
            f32 t_pitch = t_person->getFaceRot().x;
            FVec2 t_vecOffset;
            t_vecOffset.x = m_offsetX;
            t_vecOffset.y = m_offsetY;
            f32 t_offsetX = t_vecOffset.x*cosf(t_roll*DEGTORAD)-t_vecOffset.y*sinf(t_roll*DEGTORAD);
            f32 t_offsetY = t_vecOffset.x*sinf(t_roll*DEGTORAD)+t_vecOffset.y*cosf(t_roll*DEGTORAD);
            t_pt_x = t_pt_x + t_offsetX*t_personTracker->m_eyestd_scale;
            t_pt_y = t_pt_y + t_offsetY*t_personTracker->m_noisetd_scale;
            _nodePtr->setPosition(t_pt_x, t_pt_y, 0.0f);
            _nodePtr->setScale(m_scaleX*t_personTracker->m_eyestd_scale, m_scaleY*t_personTracker->m_eyestd_scale, 1.0f);
            _nodePtr->setRotation(t_pitch, -t_yaw, t_roll);
        }else{
            _nodePtr->setvisible(false);
        }
    }
    
}

bool SVActFollowPerson::isEnd(){
    return false;
}

void SVActFollowPerson::setFllowIndex(s32 _index){
    m_bindIndex = _index;
}

void SVActFollowPerson::setBindOffset(f32 _offsetX, f32 _offsetY, f32 _offsetZ){
    m_offsetX = _offsetX;
    m_offsetY = _offsetY;
    m_offsetZ = _offsetZ;
}

void SVActFollowPerson::setScale(f32 _scaleX, f32 _scaleY, f32 _scaleZ){
    m_scaleX = _scaleX;
    m_scaleY = _scaleY;
    m_scaleZ = _scaleZ;
}
