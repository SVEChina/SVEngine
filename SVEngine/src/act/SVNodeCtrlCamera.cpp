//
// SVNodeCtrlCamera.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVNodeCtrlCamera.h"
#include "../node/SVCameraNode.h"

SVCameraCtrl::SVCameraCtrl(SVInst* _app)
:SVNodeCtrl(_app) {
    //m_mat
}

SVCameraCtrl::~SVCameraCtrl() {
}

FMat4& SVCameraCtrl::getMat() {
    return m_mat;
}

f32* SVCameraCtrl::getMatPoint() {
    return m_mat.get();
}

bool SVCameraCtrl::run(SVCameraNodePtr _nodePtr, f32 dt){
    return true;
}

/*
基础相机控制
 */

SVCamCtrlBase::SVCamCtrlBase(SVInst* _app)
:SVCameraCtrl(_app) {
}

SVCamCtrlBase::~SVCamCtrlBase() {
}

void SVCamCtrlBase::setPosition(f32 _x, f32 _y, f32 _z){
    m_pos.set(_x,_y,_z);
    m_dirty = true;
}

void SVCamCtrlBase::setTarget(f32 _x, f32 _y, f32 _z) {
    m_targetEx.set(_x,_y,_z);
    m_dirty = true;
}

void SVCamCtrlBase::setDirection(f32 _x, f32 _y, f32 _z) {
    m_direction.set(_x, _y, _z);
    m_direction.normalize();
    m_dirty = true;
}

void SVCamCtrlBase::setUp(f32 _x, f32 _y, f32 _z) {
    m_upEx.set(_x, _y, _z);
    m_upEx.normalize();
    m_dirty = true;
}

FVec3& SVCamCtrlBase::getUp(){
    return m_upEx;
}

FVec3& SVCamCtrlBase::getDirection(){
    return m_direction;
}

//相机控制
bool SVCamCtrlBase::run(SVCameraNodePtr _nodePtr, f32 dt) {
    if(_nodePtr) {
        if(m_dirty) {
            m_dirty = false;
            m_mat = lookAt(FVec3(m_pos.x,m_pos.y,m_pos.z),
                           FVec3(m_targetEx.x,m_targetEx.y,m_targetEx.z),
                           FVec3(m_upEx.x,m_upEx.y,m_upEx.z) );
        }
    }
    return true;
}

/*
姿态相机控制
 */
SVCamCtrlAttr::SVCamCtrlAttr(SVInst* _app)
:SVCameraCtrl(_app) {
}

SVCamCtrlAttr::~SVCamCtrlAttr() {
}

bool SVCamCtrlAttr::run(SVCameraNodePtr _nodePtr, f32 dt) {
    return true;
}

/*
相机控制
*/

SVNodeCtrlCamera::SVNodeCtrlCamera(SVInst* _app)
:SVCameraCtrl(_app) {
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
bool SVNodeCtrlCamera::run(SVCameraNodePtr _nodePtr, f32 dt) {
    return true;
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
bool SVCtrlCamera2D::run(SVCameraNodePtr _nodePtr, f32 dt) {
    if(_nodePtr) {
        if(m_dirty) {
            m_mat = lookAt(FVec3(m_pos.x,m_pos.y,m_pos.z),
                           FVec3(m_target.x,m_target.y,0.0f),
                           FVec3(0.0f,1.0f,0.0f) );
            m_dirty = false;
        }
    }
    return true;
}

//AR控制器
SVCtrlCamereAR::SVCtrlCamereAR(SVInst* _app)
:SVNodeCtrlCamera(_app){
    
}

SVCtrlCamereAR::~SVCtrlCamereAR() {
    
}
