//
// SVCameraNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVCameraNode.h"
#include "../basesys/SVConfig.h"
#include "../rendercore/SVFboObject.h"
//
SVCameraNode::SVCameraNode(SVInst *_app)
: SVNode(_app) {
    ntype = "SVCameraNode";
    m_fovy = 60.0f;
    m_mat_proj.setIdentity();
    m_mat_view.setIdentity();
    m_mat_vp.setIdentity();
    m_resLock = MakeSharedPtr<SVLock>();
}

SVCameraNode::~SVCameraNode() {
    m_fbobjectPool.destroy();
    m_resLock = nullptr;
}

void SVCameraNode::update(f32 _dt) {
    _removeUnuseLinkFboObject();
    if (m_dirty) {
        //更新本地矩阵
        m_dirty = false;
        updateProjMat();
        updateCameraMat();
    }
    //
    for (s32 i = 0; i < m_fbobjectPool.size(); i++) {
        SVFboObjectPtr t_fbo = m_fbobjectPool[i];
        t_fbo->setLink(true);
        t_fbo->setViewMat(m_mat_view);
        t_fbo->setProjMat(m_mat_proj);
    }
}

void SVCameraNode::active() {
    m_active = true;
}

void SVCameraNode::unactive() {
    m_active = false;
}

void SVCameraNode::resetDefaultCamera() {
    resetCamera(720.0f, 1280.0f);
}

void SVCameraNode::resetCamera(f32 w, f32 h, f32 fovy) {
    //设置默认值
    m_width = w;
    m_height = h;
    m_p_zn = 100.0f;
    m_p_zf = 5000.0f;
    m_fovy = fovy;
    //
    m_postion.set(0.0f, 0.0f, 0.5f * m_height / tan(0.5f*m_fovy * DEGTORAD));
    //
    m_targetEx.set(0.0f, 0.0f, 0.0f);
    //
    m_direction = m_targetEx - m_postion;
    m_direction.normalize();
    //
    m_upEx.set(0.0f,1.0f,0.0f);
    m_upEx.normalize();
    //
    updateProjMat();
    updateCameraMat();
}

void SVCameraNode::setProjectParam(f32 _znear, f32 _zfar, f32 _fovy, f32 _aspect) {
    m_p_zn = _znear;
    m_p_zf = _zfar;
    updateProjMat();
}

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

void SVCameraNode::updateProjMat() {
    m_mat_proj = perspective(m_fovy,m_width/m_height, m_p_zn, m_p_zf);
    m_mat_vp =m_mat_proj*m_mat_view;
}

void SVCameraNode::updateCameraMat() {
    m_mat_view = lookAt(FVec3(m_postion.x,m_postion.y,m_postion.z),
                        FVec3(m_targetEx.x,m_targetEx.y,m_targetEx.z),
                        FVec3(m_upEx.x,m_upEx.y,m_upEx.z) );
    m_mat_vp =m_mat_proj*m_mat_view;
}

void SVCameraNode::updateViewProj() {
    m_mat_vp =m_mat_proj*m_mat_view;
}

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


//推进，推远
void SVCameraNode::ctrlZoom(f32 _dis) {
    f32 t_min_dis = m_p_zn + 1.0f;
    f32 t_max_dis = m_p_zf - 1.0f;
    f32 t_real_dis = (m_postion-m_targetEx).length();
    if(_dis>0) {
        //推进
        if(_dis>t_min_dis) {
            m_postion = m_targetEx - m_direction*t_min_dis;
        } else {
            m_postion = m_targetEx - m_direction*(t_real_dis-_dis);
        }
    }else {
        //拉远
        if(fabs(_dis)>t_max_dis) {
            m_postion = m_targetEx - m_direction*t_max_dis;
        }else{
            m_postion = m_targetEx - m_direction*(t_real_dis-_dis);
        }
    }
    updateCameraMat();
}

//航向xoz
void SVCameraNode::ctrlYaw(f32 _angle) {
    
}

//俯仰y
void SVCameraNode::ctrlPitch(f32 _angle) {
    
}

//前进 后退
void SVCameraNode::ctrlForward(f32 _dis) {
    
}

//平移左右
void SVCameraNode::ctrlMoveLR(f32 _dis) {
    
}

//平移前后
void SVCameraNode::ctrlMoveFB(f32 _dis) {
    
}

//重制
void SVCameraNode::reset() {
    
}
