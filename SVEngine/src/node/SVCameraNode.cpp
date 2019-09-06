//
// SVCameraNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVCameraNode.h"
#include "../basesys/SVConfig.h"
#include "../rendercore/SVFboObject.h"
#include "../act/SVCameraCtrl.h"


SVProjMethod::SVProjMethod() {
    m_width = 720.0f;
    m_height = 1280.0f;
    m_zfar = 1000.0f;
    m_znear = 1.0f;
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
    m_zfar = 10000.0f;
    m_znear = 1.0f;
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
    m_zfar = 10000.0f;
    m_znear = 1.0f;
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
    //m_projMat =
}

//
SVCameraNode::SVCameraNode(SVInst *_app)
: SVEventProc(_app) {
    m_dirty = true;
    m_resLock = MakeSharedPtr<SVLock>();
    //视矩阵
    SVCamCtrlBasePtr t_pCtrl =  MakeSharedPtr<SVCamCtrlBase>(_app);
    f32 t_pos_z = 0.5f * 1280.0f / tan(30.0f * DEGTORAD);
    t_pCtrl->setPosition(0.0f, 0.0,t_pos_z);
    t_pCtrl->setTarget(0.0f, 0.0f, 0.0f);
    t_pCtrl->setUp(0.0f,1.0f,0.0f);
    m_pCtrl = t_pCtrl;
    //投影矩阵
    m_pProjMethod = MakeSharedPtr<SVProject>();
    m_pProjMethod->reset();
    //更新矩阵
    m_pos = m_pCtrl->getPos();
    m_mat_v = m_pCtrl->getMat();
    m_mat_p = m_pProjMethod->getMat();
    //
    updateViewProj();
}

SVCameraNode::~SVCameraNode() {
    m_fbobjectPool.destroy();
    m_resLock = nullptr;
    m_pCtrl = nullptr;
    m_pProjMethod = nullptr;
}

void SVCameraNode::init() {
    if(m_pCtrl) {
        m_pCtrl->bind(THIS_TO_SHAREPTR(SVCameraNode));
    }
}

void SVCameraNode::destroy() {
    if(m_pCtrl) {
        m_pCtrl->unbind();
    }
}

void SVCameraNode::setProject() {
    if(m_pProjMethod) {
        SVProjectPtr tt = MakeSharedPtr<SVProject>();
        tt->setWidth(m_pProjMethod->getWidth());
        tt->setHeight(m_pProjMethod->getHeight());
        tt->setNear(m_pProjMethod->getNear());
        tt->setFar(m_pProjMethod->getFar());
        tt->setFovy(60.0f);
        tt->refresh();
        m_pProjMethod = tt;
    }else {
        m_pProjMethod = MakeSharedPtr<SVProject>();
        m_pProjMethod->reset();
    }
    //
    m_mat_p = m_pProjMethod->getMat();
}

void SVCameraNode::ortho() {
    if(m_pProjMethod) {
        SVOrthoPtr tt = MakeSharedPtr<SVOrtho>();
        tt->setWidth(tt->getWidth());
        tt->setHeight(tt->getHeight());
        tt->setNear(tt->getNear());
        tt->setFar(tt->getFar());
        tt->refresh();
        m_pProjMethod = tt;
    }else {
        m_pProjMethod = MakeSharedPtr<SVOrtho>();
        m_pProjMethod = MakeSharedPtr<SVProject>();
        m_pProjMethod->reset();
    }
    //
    m_mat_p = m_pProjMethod->getMat();
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
    //数据更新
    if(m_pCtrl && m_pProjMethod) {
        //控制器接管
        bool t_flag = m_pCtrl->run(THIS_TO_SHAREPTR(SVCameraNode),_dt);
        if (m_dirty || t_flag) {
            m_pos = m_pCtrl->getPos();
            m_mat_v = m_pCtrl->getMat();
            m_mat_p = m_pProjMethod->getMat();
            updateViewProj();  // 更新vp矩阵
        }
        //关联fbo
        for (s32 i = 0; i < m_fbobjectPool.size(); i++) {
            SVFboObjectPtr t_fbo = m_fbobjectPool[i];
            t_fbo->setLink(true);
            t_fbo->setViewMat(m_mat_v);
            t_fbo->setProjMat(m_mat_p);
        }
    }
}

void SVCameraNode::_updateForce() {
    if(m_pProjMethod) {
        m_pProjMethod->refresh();
    }
    //
    m_pos = m_pCtrl->getPos();
    m_mat_v = m_pCtrl->getMat();
    m_mat_p = m_pProjMethod->getMat();
    //
    m_dirty = false;
}

void SVCameraNode::resetDefaultCamera() {
    //控制重制
    if(m_pCtrl){
        m_pCtrl->reset();
    }
    //投影充值
    if(m_pProjMethod) {
        m_pProjMethod->reset();
    }
    //
    m_pos = m_pCtrl->getPos();
    m_mat_v = m_pCtrl->getMat();
    m_mat_p = m_pProjMethod->getMat();
}

void SVCameraNode::resetCamera(f32 w, f32 h) {
    if(m_pProjMethod) {
        m_pProjMethod->setWidth(w);
        m_pProjMethod->setHeight(h);
    }
    _updateForce();
}

//设置远进裁
void SVCameraNode::setZ(f32 _near, f32 _far) {
    if(m_pProjMethod) {
        m_pProjMethod->setNear(_near);
        m_pProjMethod->setFar(_far);
        m_pProjMethod->refresh();
        m_mat_p = m_pProjMethod->getMat();
    }
}

FVec3& SVCameraNode::getPosition() {
    return m_pos;
}

f32 *SVCameraNode::getProjectMat() {
    return m_mat_p.get();
}

f32 *SVCameraNode::getCameraMat() {
    return m_mat_v.get();
}

f32 *SVCameraNode::getVPMat() {
    return m_mat_vp.get();
}

FMat4& SVCameraNode::getProjectMatObj(){
    return m_mat_p;
}

FMat4& SVCameraNode::getViewMatObj(){
    return m_mat_v;
}

FMat4& SVCameraNode::getVPMatObj(){
    return m_mat_vp;
}

//获取控制器
SVCameraCtrlPtr SVCameraNode::getCtrl(){
    return m_pCtrl;
}

//设置控制器
void SVCameraNode::setCtrl(SVCameraCtrlPtr _ctr) {
    if(!_ctr){
        //error 不允许没有
        return ;
    }
    m_pCtrl->unbind();
    m_pCtrl = _ctr;
    m_pCtrl->bind(THIS_TO_SHAREPTR(SVCameraNode));
    //
    m_pos = m_pCtrl->getPos();
    m_mat_v = m_pCtrl->getMat();
}

SVProjMethodPtr SVCameraNode::getProjMethod() {
    return m_pProjMethod;
}


void SVCameraNode::updateViewProj() {
    m_mat_vp =m_mat_p*m_mat_v;
}


///*proj camera*/
//
//SVCameraProjNode::SVCameraProjNode(SVInst *_app)
//:SVCameraNode(_app) {
//    m_fovy = 60.0f;
//}
//
//SVCameraProjNode::~SVCameraProjNode() {
//}
//
//void SVCameraProjNode::setProjectParam(f32 _znear, f32 _zfar, f32 _fovy, f32 _aspect) {
//    m_p_zn = _znear;
//    m_p_zf = _zfar;
//    m_fovy = _fovy;
//    m_aspect = _aspect;
//    updateProjMat();
//}
//
////
//void SVCameraProjNode::resetCamera(f32 w, f32 h) {
//    //设置默认值
//    m_width = w;
//    m_height = h;
//    m_p_zn = 100.0f;
//    m_p_zf = 15000.0f;
//    m_fovy = 60.0f;
//    //
//    f32 t_pos_z = 0.5f * m_height / tan(0.5f*m_fovy * DEGTORAD);
//    m_pos.set(0.0f, 0.0,t_pos_z);
//    m_targetEx.set(0.0f, 0.0f, 0.0f);
//    m_direction = m_targetEx - m_pos;
//    m_direction.normalize();
//    m_upEx.set(0.0f,1.0f,0.0f);
//    m_upEx.normalize();
//    //
//    _updateForce();
////    //计算角度
////    m_angle_yaw = acos(m_direction.x)*RAD2DEG;
////    if(m_direction.z<0) {
////        m_angle_yaw = 360.0f - m_angle_yaw;
////    }
////    m_angle_pitch = asin(m_direction.y)*RAD2DEG;
//}
//
//void SVCameraProjNode::updateProjMat(){
//    m_mat_proj = perspective(m_fovy,m_width/m_height, m_p_zn, m_p_zf);
//    updateViewProj();
//}
//
///*ortho camera*/
//SVCameraOrthoNode::SVCameraOrthoNode(SVInst *_app)
//:SVCameraNode(_app)  {
//    m_p_zn = 100.0f;
//    m_p_zf = 5000.0f;
//}
//
//SVCameraOrthoNode::~SVCameraOrthoNode() {
//}
//
//void SVCameraOrthoNode::resetCamera(f32 w, f32 h) {
//    //正交
//    m_width = w;
//    m_height = h;
//    m_p_zn = 100.0f;
//    m_p_zf = 15000.0f;
//    f32 t_pos_z = 1000.0f;
//    m_pos.set(0.0f, 0.0,t_pos_z);
//    m_targetEx.set(0.0f, 0.0f, 0.0f);
//    m_direction = m_targetEx - m_pos;
//    m_direction.normalize();
//    m_upEx.set(0.0f,1.0f,0.0f);
//    m_upEx.normalize();
//    //
//    _updateForce();
//}
//
////
//void SVCameraOrthoNode::updateProjMat() {
//    m_mat_proj = ortho( -m_width/2 , m_width/2 , -m_height/2 , m_height/2  , m_p_zn  , m_p_zf );   //投影矩阵
//    updateViewProj();
//}
//
////
//SVCameraARNode::SVCameraARNode(SVInst *_app)
//:SVCameraNode(_app)  {
//}
//
//SVCameraARNode::~SVCameraARNode() {
//}
////void SVCameraARNode::setPose(f32 _x, f32 _y, f32 _z){
////    FMat4 rotMatX;
////    rotMatX.setIdentity();
////    rotMatX.setRotateX(_x);
////    FMat4 rotMatY;
////    rotMatY.setIdentity();
////    rotMatY.setRotateY(_y);
////    FMat4 rotMatZ;
////    rotMatZ.setIdentity();
////    rotMatZ.setRotateZ(_z);
////    FMat4 rotMat;
////    rotMat.setIdentity();
////    rotMat = rotMatZ * rotMatY * rotMatX;
////    rotMat = inverse(rotMat);
////    resetDefaultCamera();
////    m_mat_view = rotMat*m_mat_view;
////    updateViewProj();
////}
////
////void SVCameraARNode::syncViewMatrix(FMat4 &_mat){
////    m_mat_view = _mat;
////    m_mat_vp =m_mat_proj*m_mat_view;
////    //反算其他参数
////    FMat4 t_camRotInver = m_mat_view;
////    t_camRotInver[12] = 0;
////    t_camRotInver[13] = 0;
////    t_camRotInver[14] = 0;
////    t_camRotInver =transpose(t_camRotInver);
////    FMat4 tmpMat = t_camRotInver*m_mat_view;
////    //获取相机世界位置
////    FVec3 t_cameraEye = FVec3(-tmpMat[12], -tmpMat[13], -tmpMat[14]);
////    m_postion.set(t_cameraEye);
////    //获取up
////    m_upEx.set(m_mat_view[0], m_mat_view[4], m_mat_view[8]);
////}
////
////void SVCameraARNode::syncProjectMatrix(FMat4 &_mat){
////    m_mat_proj = _mat;
////    m_mat_vp =m_mat_proj*m_mat_view;
////    //反算裁剪面
////}
