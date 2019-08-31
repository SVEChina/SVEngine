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
void SVNodeCtrlCamera::move(f32 _px,f32 py) {
    
}
//角度旋转 像素dert x,y
void SVNodeCtrlCamera::angle(f32 _px,f32 py) {
    
}
//推拉
void SVNodeCtrlCamera::zoom(f32 _dis) {
    
}



//相机控制
void SVNodeCtrlCamera::run(SVCameraNodePtr _nodePtr, f32 dt) {
    //
    
}

#define PIXEL_UNIT_H1 1.1547
//2*tan(30.0度)
/****************************************/
SVCtrlCamera2D::SVCtrlCamera2D(SVInst* _app)
:SVNodeCtrlCamera(_app){
    m_width = 720;
    m_height = 1280;
    m_dis = 100.0f;
    m_pixelUnit = 0.0f;
    m_angle = 0.0f;
    m_target.set(0.0f);
    m_pos.set(0.0f);
    m_pos.z = m_dis;
    m_dirty = true;
    reset();
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
    m_pixelUnit = PIXEL_UNIT_H1*fabs(m_pos.z)/m_height;
    m_dirty = true;
}

void SVCtrlCamera2D::reset(s32 _w,s32 _h) {
    m_width = _w;
    m_height = _h;
    reset();
}

void SVCtrlCamera2D::resize(s32 _w,s32 _h) {
    m_width = _w;
    m_height = _h;
    // 2*tan30*dis/height
    m_pixelUnit = PIXEL_UNIT_H1*fabs(m_pos.z)/m_height;
}

//平移
void SVCtrlCamera2D::move(f32 _px,f32 _py) {
    m_dirty = true;
    m_target.x -= PIXEL_UNIT_H1*_px;
    m_target.y -= PIXEL_UNIT_H1*_py;
    m_pos.x = m_target.x;
    m_pos.y = m_target.y;
}

//角度旋转
void SVCtrlCamera2D::angle(f32 _px,f32 py) {
    m_dirty = false;
}

//推拉
void SVCtrlCamera2D::zoom(f32 _dis) {
    m_dis += _dis*m_pixelUnit*2.0f;
    m_pos.z = m_dis;
    //做个限定
    
    //
    m_pixelUnit = PIXEL_UNIT_H1*fabs(m_pos.z)/m_height;
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

//AR控制器
SVCtrlCamereAR::SVCtrlCamereAR(SVInst* _app)
:SVNodeCtrlCamera(_app){
    
}

SVCtrlCamereAR::~SVCtrlCamereAR() {
    
}
