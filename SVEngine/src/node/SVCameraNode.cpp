//
// SVCameraNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVCameraNode.h"
#include "../basesys/SVConfig.h"
#include "../rendercore/SVFboObject.h"
#include "../act/SVNodeCtrlCamera.h"

//
SVCameraNode::SVCameraNode(SVInst *_app)
: SVNode(_app) {
    ntype = "SVCameraNode";
    m_resLock = MakeSharedPtr<SVLock>();
    
    m_mat_proj.setIdentity();
    m_mat_view.setIdentity();
    m_mat_vp.setIdentity();

    m_pCtrl = nullptr;
    m_angle_yaw = 0.0f;
    m_angle_pitch = 0.0f;
}

SVCameraNode::~SVCameraNode() {
    m_fbobjectPool.destroy();
    m_resLock = nullptr;
    m_pCtrl = nullptr;
}

//LINK FBO
void SVCameraNode::addLinkFboObject(SVFboObjectPtr _fbo){
    if (_fbo) {
        for(s32 i=0;i<m_fbobjectPool.size();i++) {
            if(m_fbobjectPool[i] == _fbo) {
                return ;
            }
        }
        m_fbobjectPool.append(_fbo);
    }
}

void SVCameraNode::_removeUnuseLinkFboObject(){
    m_resLock->lock();
    //小心复值引用计数会加 1！！！！！！！！！！！！！！ 晓帆。。
    for(s32 i=0;i<m_fbobjectPool.size();) {
        if(m_fbobjectPool[i].use_count() == 2) {
            m_fbobjectPool.remove(i);
        }else{
            i++;
        }
    }
    m_fbobjectPool.reserveForce(m_fbobjectPool.size());
    m_resLock->unlock();
}

bool SVCameraNode::removeLinkFboObject(SVFboObjectPtr _fbo){
    if (_fbo) {
        for (s32 i = 0; i < m_fbobjectPool.size(); i++) {
            if (m_fbobjectPool[i] == _fbo) {
                m_fbobjectPool.removeForce(i);
                return true;
            }
        }
    }
    return false;
}

//
void SVCameraNode::update(f32 _dt) {
    //移除关联fbo
    _removeUnuseLinkFboObject();
    //控制器计算
    if(m_pCtrl) {
        m_pCtrl->run(THIS_TO_SHAREPTR(SVCameraNode),_dt);
    }
    //脏数据更新
    if (m_dirty) {
        m_dirty = false;
        updateProjMat();
        updateCameraMat();
    }
    //关联fbo
    for (s32 i = 0; i < m_fbobjectPool.size(); i++) {
        SVFboObjectPtr t_fbo = m_fbobjectPool[i];
        t_fbo->setLink(true);
        t_fbo->setViewMat(m_mat_view);
        t_fbo->setProjMat(m_mat_proj);
    }
}

void SVCameraNode::resetDefaultCamera() {
    resetCamera(720.0f, 1280.0f);
}

void SVCameraNode::resetCamera(f32 w, f32 h, f32 fovy) {
    //设置默认值
    m_width = w;
    m_height = h;
    m_p_zn = 100.0f;
    m_p_zf = 15000.0f;
    m_fovy = fovy;
    m_postion.set(0.0f, 0.0, 0.5f * m_height / tan(0.5f*m_fovy * DEGTORAD));
    m_targetEx.set(0.0f, 0.0f, 0.0f);
    m_direction = m_targetEx - m_postion;
    m_direction.normalize();
    m_upEx.set(0.0f,1.0f,0.0f);
    m_upEx.normalize();
    //更新
    updateProjMat();
    updateCameraMat();
    //计算角度
    m_angle_yaw = acos(m_direction.x)*RAD2DEG;
    if(m_direction.z<0) {
        m_angle_yaw = 360.0f - m_angle_yaw;
    }
    m_angle_pitch = asin(m_direction.y)*RAD2DEG;
}

//设置远进裁
void SVCameraNode::setZ(f32 _near, f32 _far) {
    m_p_zn = _near;
    m_p_zf = _far;
    updateProjMat();
}

void SVCameraNode::setPosition(FVec3& _pos){
    SVNode::setPosition(_pos);
    updateCameraMat();
}

void SVCameraNode::setTarget(f32 _x, f32 _y, f32 _z) {
    m_targetEx.set(_x,_y,_z);
    updateCameraMat();
}

void SVCameraNode::setDirection(f32 _x, f32 _y, f32 _z) {
    m_direction.set(_x, _y, _z);
    m_direction.normalize();
    updateCameraMat();
}

void SVCameraNode::setUp(f32 _x, f32 _y, f32 _z) {
    m_upEx.set(_x, _y, _z);
    m_upEx.normalize();
    updateCameraMat();
}

void SVCameraNode::setPose(f32 _x, f32 _y, f32 _z){
    FMat4 rotMatX;
    rotMatX.setIdentity();
    rotMatX.setRotateX(_x);
    FMat4 rotMatY;
    rotMatY.setIdentity();
    rotMatY.setRotateY(_y);
    FMat4 rotMatZ;
    rotMatZ.setIdentity();
    rotMatZ.setRotateZ(_z);
    FMat4 rotMat;
    rotMat.setIdentity();
    rotMat = rotMatZ * rotMatY * rotMatX;
    rotMat = inverse(rotMat);
    resetDefaultCamera();
    m_mat_view = rotMat*m_mat_view;
    updateViewProj();
}

void SVCameraNode::syncViewMatrix(FMat4 &_mat){
    m_mat_view = _mat;
    m_mat_vp =m_mat_proj*m_mat_view;
    //反算其他参数
    FMat4 t_camRotInver = m_mat_view;
    t_camRotInver[12] = 0;
    t_camRotInver[13] = 0;
    t_camRotInver[14] = 0;
    t_camRotInver =transpose(t_camRotInver);
    FMat4 tmpMat = t_camRotInver*m_mat_view;
    //获取相机世界位置
    FVec3 t_cameraEye = FVec3(-tmpMat[12], -tmpMat[13], -tmpMat[14]);
    m_postion.set(t_cameraEye);
    //获取up
    m_upEx.set(m_mat_view[0], m_mat_view[4], m_mat_view[8]);
}

void SVCameraNode::syncProjectMatrix(FMat4 &_mat){
    m_mat_proj = _mat;
    m_mat_vp =m_mat_proj*m_mat_view;
    //反算裁剪面
}

FVec3& SVCameraNode::getUp(){
    return m_upEx;
}

FVec3& SVCameraNode::getDirection(){
    return m_direction;
}

f32 *SVCameraNode::getProjectMat() {
    return m_mat_proj.get();
}

f32 *SVCameraNode::getCameraMat() {
    return m_mat_view.get();
}

f32 *SVCameraNode::getVPMat() {
    return m_mat_vp.get();
}

FMat4& SVCameraNode::getProjectMatObj(){
    return m_mat_proj;
}

FMat4& SVCameraNode::getViewMatObj(){
    return m_mat_view;
}

FMat4& SVCameraNode::getVPMatObj(){
    return m_mat_vp;
}

//获取控制器
SVNodeCtrlCameraPtr SVCameraNode::getCtrl(){
    return m_pCtrl;
}

//设置控制器
void SVCameraNode::setCtrl(SVNodeCtrlCameraPtr _ctr) {
    m_pCtrl = _ctr;
}

void SVCameraNode::updateProjMat() {
//    m_mat_proj = perspective(m_fovy,m_width/m_height, m_p_zn, m_p_zf);
//    m_mat_vp = m_mat_proj*m_mat_view;
}

void SVCameraNode::updateCameraMat() {
    m_mat_view = lookAt(FVec3(m_postion.x,m_postion.y,m_postion.z),
                        FVec3(m_targetEx.x,m_targetEx.y,m_targetEx.z),
                        FVec3(m_upEx.x,m_upEx.y,m_upEx.z) );
    m_mat_vp =m_mat_proj*m_mat_view;
    //
    m_mat_viewUI =  lookAt(FVec3(0,0,381), FVec3(0.0,0.0,0.0), FVec3(0.0,1.0,0.0));
    m_mat_vpUI = m_mat_projUI*m_mat_viewUI;
}

void SVCameraNode::updateViewProj() {
    m_mat_vp =m_mat_proj*m_mat_view;
}

//重制
void SVCameraNode::reset() {
}

/*proj camera*/

SVCameraProjNode::SVCameraProjNode(SVInst *_app) {
    m_fovy = 60.0f;
}

SVCameraProjNode::~SVCameraProjNode() {
}

void SVCameraProjNode::setProjectParam(f32 _znear, f32 _zfar, f32 _fovy, f32 _aspect) {
    m_p_zn = _znear;
    m_p_zf = _zfar;
    updateProjMat();
}
//
void SVCameraProjNode::resetCamera(f32 w, f32 h, f32 fovy) {
    //设置默认值
    m_width = w;
    m_height = h;
    m_p_zn = 100.0f;
    m_p_zf = 15000.0f;
    m_fovy = fovy;
    m_postion.set(0.0f, 0.0, 0.5f * m_height / tan(0.5f*m_fovy * DEGTORAD));
    m_targetEx.set(0.0f, 0.0f, 0.0f);
    m_direction = m_targetEx - m_postion;
    m_direction.normalize();
    m_upEx.set(0.0f,1.0f,0.0f);
    m_upEx.normalize();
    //
    updateProjMat();
    updateCameraMat();
    //计算角度
    m_angle_yaw = acos(m_direction.x)*RAD2DEG;
    if(m_direction.z<0) {
        m_angle_yaw = 360.0f - m_angle_yaw;
    }
    m_angle_pitch = asin(m_direction.y)*RAD2DEG;
}

void SVCameraProjNode::updateProjMat(){
    m_mat_proj = perspective(m_fovy,m_width/m_height, m_p_zn, m_p_zf);
    updateViewProj();
}

/*ortho camera*/
SVCameraOrthoNode::SVCameraOrthoNode(SVInst *_app) {
    m_p_zn = 100.0f;
    m_p_zf = 5000.0f;
}

SVCameraOrthoNode::~SVCameraOrthoNode() {
}
//
void SVCameraOrthoNode::resetCamera(f32 w, f32 h) {
    m_width = w;
    m_height = h;
    m_p_zn = 100.0f;
    m_p_zf = 15000.0f;
//    //
//    m_postion.set(0.0f, 0.0, 0.5f * m_height / tan(0.5f*m_fovy * DEGTORAD));
//    m_targetEx.set(0.0f, 0.0f, 0.0f);
//    m_direction = m_targetEx - m_postion;
//    m_direction.normalize();
//    m_upEx.set(0.0f,1.0f,0.0f);
//    m_upEx.normalize();
}

//
void SVCameraOrthoNode::updateProjMat() {
    m_mat_proj = ortho( -m_width/2 , m_width/2 , -m_height/2 , m_height/2  , m_p_zn  , m_p_zf );   //投影矩阵
    m_mat_vp = m_mat_proj*m_mat_view;
}
