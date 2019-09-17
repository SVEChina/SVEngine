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


SVProjMethod::SVProjMethod() {
    m_width = 720.0f;
    m_height = 1280.0f;
    m_zfar = 2000.0f;
    m_znear = 500.0f;
    m_projMat.setIdentity();
}

FMat4& SVProjMethod::getMat(){
    return m_projMat;
}

f32* SVProjMethod::getMatPoint(){
    return m_projMat.get();
}

void SVProjMethod::reset() {
    m_projMat.setIdentity();
}

void SVProjMethod::refresh() {
    m_projMat.setIdentity();
}

void SVProjMethod::setWidth(f32 _w) {
    m_width = _w;
}

void SVProjMethod::setHeight(f32 _h) {
    m_height = _h;
}

void SVProjMethod::setNear(f32 _near) {
    m_znear = _near;
}

void SVProjMethod::setFar(f32 _far) {
    m_zfar = _far;
}


//透视投影
SVProject::SVProject() {
    m_fovy = 60.0f;
}

void SVProject::setFovy(f32 _fovy) {
    m_fovy = _fovy;
}

void SVProject::reset() {
    m_width = 720.0f;
    m_height = 1280.0f;
    m_zfar = 2000.0f;
    m_znear = 500.0f;
    refresh();
}

void SVProject::refresh() {
    m_projMat = perspective(m_fovy,m_width/m_height, m_znear, m_zfar);
}


//正交投影
SVOrtho::SVOrtho() {
}

void SVOrtho::reset() {
    m_width = 720.0f;
    m_height = 1280.0f;
    m_zfar = 2000.0f;
    m_znear = 500.0f;
    refresh();
}

void SVOrtho::refresh() {
    m_projMat = ortho( -m_width/2 ,
                      m_width/2 ,
                      -m_height/2 ,
                      m_height/2  ,
                      m_znear  ,
                      m_zfar );   //投影矩阵
}

SVARProj::SVARProj() {
}

void SVARProj::reset() {
}

void SVARProj::refresh() {
}

void SVARProj::setProjMat(FMat4& _mat) {
    m_projMat = _mat;
}


SVCameraCtrl::SVCameraCtrl(SVInst* _app)
:SVGBase(_app)
,m_linkCam(nullptr){
    m_pos.set(0.0f,0.0f,0.0f);
}

SVCameraCtrl::~SVCameraCtrl() {
    m_linkCam = nullptr;
}

void SVCameraCtrl::reset(){
    m_pos.set(0.0f,0.0f,0.0f);
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
基础Proj控制
 */

SVCamCtrlProj::SVCamCtrlProj(SVInst* _app)
:SVCameraCtrl(_app) {
}

SVCamCtrlProj::~SVCamCtrlProj() {
}

void SVCamCtrlProj::reset(f32 _w,f32 _h) {
    f32 t_pos_z = (_h*0.5f) / tan(30.0f * DEGTORAD);
    setPosition(0.0f, 0.0,t_pos_z);
    setTarget(0.0f, 0.0f, 0.0f);
    setUp(0.0f,1.0f,0.0f);
}

void SVCamCtrlProj::setPosition(f32 _x, f32 _y, f32 _z){
    m_pos.set(_x,_y,_z);
    m_dirty = true;
}

void SVCamCtrlProj::setTarget(f32 _x, f32 _y, f32 _z) {
    m_targetEx.set(_x,_y,_z);
    m_dirty = true;
}

void SVCamCtrlProj::setDirection(f32 _x, f32 _y, f32 _z) {
    m_direction.set(_x, _y, _z);
    m_direction.normalize();
    m_dirty = true;
}

void SVCamCtrlProj::setUp(f32 _x, f32 _y, f32 _z) {
    m_upEx.set(_x, _y, _z);
    m_upEx.normalize();
    m_dirty = true;
}

FVec3& SVCamCtrlProj::getUp(){
    return m_upEx;
}

FVec3& SVCamCtrlProj::getDirection(){
    return m_direction;
}

void SVCamCtrlProj::reset(){
    f32 t_height = mApp->m_pGlobalParam->m_inner_height;
    f32 t_pos_z = (t_height*0.5f) / tan(30.0f * DEGTORAD);
    m_pos.set(0.0f,0.0f,t_pos_z);
    m_targetEx.set(0.0f,0.0f,0.0f);
    m_upEx.set(0.0f, 1.0f, 0.0f);
}

//相机控制
bool SVCamCtrlProj::run(SVCameraNodePtr _nodePtr, f32 dt) {
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
 基础正交控制
 */

SVCamCtrlOrtho::SVCamCtrlOrtho(SVInst* _app)
:SVCameraCtrl(_app) {
    m_width = 720.0f;
    m_height = 1280.0f;
    m_sc = 1.0f;
    m_dirty = true;
}

SVCamCtrlOrtho::~SVCamCtrlOrtho() {
}

void SVCamCtrlOrtho::reset(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_pos.set(0.5f*m_width*m_sc,0.5f*m_height*m_sc,1000.0f);
    m_dirty = true;
}

void SVCamCtrlOrtho::reset(){
    m_sc = 1.0f;
    m_pos.set(0.5f*m_width*m_sc,-0.5f*m_height*m_sc,1000.0f);
    m_dirty = true;
}

//相机控制
bool SVCamCtrlOrtho::run(SVCameraNodePtr _nodePtr, f32 dt) {
    if(_nodePtr) {
        if(m_dirty) {
            m_dirty = false;
            m_mat = lookAt(m_pos,
                           FVec3(m_pos.x, m_pos.y, 0.0f),
                           FVec3(0.0f, 1.0f, 0.0f) );
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

bool SVCtrlCamereAR::run(SVCameraNodePtr _nodePtr, f32 dt){
    return true;
}

void SVCtrlCamereAR::setEur(f32 _yaw,f32 _pitch,f32 _roll) {
    
}

void SVCtrlCamereAR::setPos(FVec3& _pos) {
    m_pos = _pos;
}

void SVCtrlCamereAR::setViewMat(FMat4& _mat) {
    m_mat = _mat;
}
