//
// SVCameraCtrl.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVCameraCtrl.h"
#include "../node/SVCameraNode.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVPickProcess.h"

SVCameraCtrl::SVCameraCtrl(SVInst* _app)
:SVGBase(_app)
,m_linkCam(nullptr){
    m_pos.set(0.0f,0.0f,0.0f);
}

SVCameraCtrl::~SVCameraCtrl() {
    m_linkCam = nullptr;
}

FMat4& SVCameraCtrl::getMat() {
    return m_mat;
}

f32* SVCameraCtrl::getMatPoint() {
    return m_mat.get();
}

FVec3& SVCameraCtrl::getPos() {
    return m_pos;
}

bool SVCameraCtrl::run(SVCameraNodePtr _nodePtr, f32 dt){
    return true;
}

void SVCameraCtrl::bind(SVCameraNodePtr _cam) {
    m_linkCam = _cam;
}

void SVCameraCtrl::unbind() {
    m_linkCam = nullptr;
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

//纯2d平面控制
SVCtrlCamera2D::SVCtrlCamera2D(SVInst* _app)
:SVNodeCtrlCamera(_app){
    m_zoom_x = 5.0f;
    m_pixelUnit = 0.0f;
    m_dis = 100.0f;
    m_target.set(0.0f);
    m_dirty = true;
    reset();
}

SVCtrlCamera2D::~SVCtrlCamera2D() {
}

//重制
void SVCtrlCamera2D::reset() {
    m_target.set(0.0f);
    m_dis = 1000.0f;
    m_dirty = true;
}
//平移
void SVCtrlCamera2D::move(f32 _win_px0,f32 _win_py0,f32 _win_px1,f32 _win_py1) {
    m_dirty = true;
    //计算两个三维位置 然后在计算偏移距离
    SVPickProcessPtr t_pick = mApp->getBasicSys()->getPickModule();
    FVec3 t_p0,t_p1;
    FVec4 t_plane(0.0f,0.0f,1.0f,0.0f);
    bool t_flag0 = t_pick->getCrossPointWithPlane(m_linkCam, _win_px0, _win_py0, t_p0, t_plane);
    bool t_flag1 = t_pick->getCrossPointWithPlane(m_linkCam, _win_px1, _win_py1, t_p1, t_plane);
    if(t_flag0 && t_flag1) {
        FVec3 t_off = t_p1 - t_p0;
        SV_LOG_INFO("off (%f,%f,%f) \n",t_off.x,t_off.y,t_off.z);
        m_target += t_off;
    }
}

//推拉 (30尺度)
void SVCtrlCamera2D::zoom(f32 _dis) {
    SV_LOG_INFO("_dis (%f) \n",_dis);
    f32 t_near = m_linkCam->getProjMethod()->getNear();
    f32 t_far = m_linkCam->getProjMethod()->getFar();
    f32 t_h = m_linkCam->getProjMethod()->getHeight();
    m_pixelUnit = PIXEL_UNIT_H1*fabs(m_dis)/t_h;
    m_dis += _dis*m_pixelUnit*m_zoom_x;
    if(m_dis<t_near + 1.0f) {
        m_dis = t_near + 1.0f;
    }
    if(m_dis>t_far - 1.0f) {
        m_dis = t_far - 1.0f;
    }
    m_dirty = true;
}

//相机控制
bool SVCtrlCamera2D::run(SVCameraNodePtr _nodePtr, f32 dt) {
    if(_nodePtr) {
        if(m_dirty) {
            m_mat = lookAt(FVec3(m_target.x,m_target.y,m_dis),
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
