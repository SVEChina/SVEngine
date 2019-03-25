//
// SVNodeCtrlCamera.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVNodeCtrlCamera.h"
#include "../node/SVCameraNode.h"

SVNodeCtrlCamera::SVNodeCtrlCamera(SVInst* _app)
:SVNodeCtrl(_app) {
    m_width = 720;
    m_height = 1280;
    m_dis = 100.0f;
}

SVNodeCtrlCamera::~SVNodeCtrlCamera() {
    
}

//
void SVNodeCtrlCamera::reset() {
    
}
//
void SVNodeCtrlCamera::reset(s32 _w,s32 _h) {
    
}
//
void SVNodeCtrlCamera::resize(s32 _w,s32 _h) {
    
}
//平移 像素dert x,y
void SVNodeCtrlCamera::move(s32 _px,s32 py) {
    
}
//角度旋转 像素dert x,y
void SVNodeCtrlCamera::angle(s32 _px,s32 py) {
    
}
//推拉
void SVNodeCtrlCamera::zoom(f32 _dis) {
    
}



//相机控制
void SVNodeCtrlCamera::run(SVCameraNodePtr _nodePtr, f32 dt) {
    //
    
}


/****************************************/
SVCtrlCamera2D::SVCtrlCamera2D(SVInst* _app)
:SVNodeCtrlCamera(_app){
    m_width = 720;
    m_height = 1280;
    m_dis = 100.0f;
    m_angle = 0.0f;
    m_target.set(0.0f);
    m_pos.set(0.0f);
    m_pos.z = m_dis;
    m_dirty = true;
}

SVCtrlCamera2D::~SVCtrlCamera2D() {
}

//重制
void SVCtrlCamera2D::reset() {
    m_target.set(0.0f);
    m_pos.set(0.0f);
    //根据 宽高 计算dis
    m_dis = 1000.0f;
    m_pos.z = m_dis;
    m_dirty = true;
}

void SVCtrlCamera2D::reset(s32 _w,s32 _h) {
    m_width = _w;
    m_height = _h;
    reset();
}

void SVCtrlCamera2D::resize(s32 _w,s32 _h) {
    
}

//平移
void SVCtrlCamera2D::move(s32 _px,s32 py) {
    m_dirty = true;
//    m_target = m_target + _off;
//    m_pos = m_target;
//    m_pos.z += m_dis;
//
}

//角度旋转
void SVCtrlCamera2D::angle(s32 _px,s32 py) {
    m_dirty = false;
}

//推拉
void SVCtrlCamera2D::zoom(f32 _dis) {
    m_dis += _dis;
    m_dirty = true;
}

//相机控制
void SVCtrlCamera2D::run(SVCameraNodePtr _nodePtr, f32 dt) {
    if(_nodePtr) {
        if(m_dirty) {
            _nodePtr->setTarget(m_target.x, m_target.y,m_target.z);
            _nodePtr->setPosition(m_pos);
            m_dirty = false;
        }
    }
}
