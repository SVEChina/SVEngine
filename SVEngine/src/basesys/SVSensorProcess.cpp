//
// SVSensorProcess.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVSensorProcess.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../event/SVEventMgr.h"
#include "../event/SVOpEvent.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVCameraMgr.h"
#include "../node/SVScene.h"
#include "../node/SVCameraNode.h"
#include "../node/SV3DBox.h"
#include "../node/SVBillboardNode.h"
#include "../node/SVSpriteNode.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVPickProcess.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderMgr.h"
#include <sys/time.h>
//
SVSensorProcess::SVSensorProcess(SVInst *_app)
:SVProcess(_app) {
    m_pARCamera = MakeSharedPtr<SVCameraNode>(mApp);
    s32 m_sw = mApp->m_pGlobalParam->m_inner_width;
    s32 m_sh = mApp->m_pGlobalParam->m_inner_height;
    m_pARCamera->resetCamera(m_sw,m_sh);
    m_acc0.set(0.0f, 0.0f, 0.0f);
    m_acc1.set(0.0f, 0.0f, 0.0f);
    m_velocity0.set(0.0f, 0.0f, 0.0f);
    m_velocity1.set(0.0f, 0.0f, 0.0f);
    m_distance0.set(0.0f, 0.0f, 0.0f);
    m_distance1.set(0.0f, 0.0f, 0.0f);
    m_isFitst = false;
}

SVSensorProcess::~SVSensorProcess() {
    m_pARCamera = nullptr;
}

void SVSensorProcess::update(f32 _dt){
    if(m_pARCamera) {
        m_pARCamera->update(_dt);
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        SVRendererBasePtr t_renderer = mApp->getRenderer();
        if(t_rs && t_renderer) {
            //这不知道要写到哪，先写这了. 晓帆
            FMat4 t_vm = m_pARCamera->getViewMatObj();
            FMat4 t_pm = m_pARCamera->getProjectMatObj();
            SVRenderCmdPushVPMatPtr t_cmd0 = MakeSharedPtr<SVRenderCmdPushVPMat>(t_vm,t_pm);
            t_cmd0->setRenderer(t_renderer);
            t_cmd0->mTag = "ar_camera_begin";
            t_rs->pushRenderCmd(RST_AR_BEGIN, t_cmd0);
            SVRenderCmdPopVPMatPtr t_cmd1 = MakeSharedPtr<SVRenderCmdPopVPMat>();
            t_cmd1->setRenderer(t_renderer);
            t_cmd1->mTag = "ar_camera_end";
            t_rs->pushRenderCmd(RST_AR_END, t_cmd1);
        }
    }
}

FVec3& SVSensorProcess::getVelocity(){
    return m_velocity0;
}

FVec3& SVSensorProcess::getDistance(){
    return m_distance0;
}

bool SVSensorProcess::procEvent(SVEventPtr _event){
    if (_event->eventType == SV_EVENT_TYPE::EVN_T_CAMERA_OREN){
        SVCameraOrenEventPtr oren = std::dynamic_pointer_cast<SVCameraOrenEvent>(_event);
        if(m_pARCamera) {
            m_pARCamera->setPose(oren->pitch, oren->roll, oren->yaw);
        }
    }else if (_event->eventType == SV_EVENT_TYPE::EVN_T_CAMERA_MATRIX){
        SVCameraMatrixEventPtr cameraMatrix = std::dynamic_pointer_cast<SVCameraMatrixEvent>(_event);
        if(m_pARCamera) {
            FMat4 t_viewMat = FMat4((f32 *)cameraMatrix->m_matData->getData());
            m_pARCamera->syncViewMatrix(t_viewMat);
        }
    }else if (_event->eventType == SV_EVENT_TYPE::EVN_T_PROJECT_MATRIX){
        SVProjectMatrixEventPtr projectMatrix = std::dynamic_pointer_cast<SVProjectMatrixEvent>(_event);
                SVCameraNodePtr mainCamera = mApp->getCameraMgr()->getMainCamera();
        if(m_pARCamera) {
            FMat4 t_projectMat = FMat4((f32 *)projectMatrix->m_matData->getData());
            m_pARCamera->syncProjectMatrix(t_projectMat);
        }
    }else if (_event->eventType == SV_EVENT_TYPE::EVN_T_ANCHOR_AR){
        SVARAnchorEventPtr anchor = std::dynamic_pointer_cast<SVARAnchorEvent>(_event);
        if (anchor) {
            
        }
        
    }else if (_event->eventType == SV_EVENT_TYPE::EVN_T_ANCHORPOINT_AR){
        SVARAnchorProjPosEventPtr anchorPoint = std::dynamic_pointer_cast<SVARAnchorProjPosEvent>(_event);
        if (anchorPoint) {
            
            
        }
        
    }else if (_event->eventType == SV_EVENT_TYPE::EVN_T_DEVICE_ACCELEROMETER){
        if (!m_isFitst) {
            m_isFitst = true;
            gettimeofday(&m_timTagLast, NULL);
            m_deltaTime = 0.0f;
        }else{
            //控制帧率使用
            timeval timTagCur;
            gettimeofday(&timTagCur, NULL);
            s32 dt = s32((timTagCur.tv_sec - m_timTagLast.tv_sec) * 1000.0f + (timTagCur.tv_usec - m_timTagLast.tv_usec) * 0.001f);
            m_deltaTime = dt*0.01f;
            m_timTagLast = timTagCur;
        }
        SVDeviceAccelerometerEventPtr accelerometer = std::dynamic_pointer_cast<SVDeviceAccelerometerEvent>(_event);
        bool isDirty = false;
        m_acc0.set(accelerometer->x, accelerometer->y, accelerometer->z);
        if (fabsf(m_acc0.x - m_acc1.x) > 0.01f) {
            m_acc1.x = m_acc0.x;
            isDirty = true;
        }
        if (fabsf(m_acc0.y - m_acc1.y) > 0.01f) {
            m_acc1.y = m_acc0.y;
            isDirty = true;
        }
        if (fabsf(m_acc0.z - m_acc1.z) > 0.01f) {
            m_acc1.z = m_acc0.z;
            isDirty = true;
        }
        if (isDirty) {
            _computePosition();
        }
    }
    return true;
}

void SVSensorProcess::_computePosition(){
//    m_deltaTime = 1.0f;
    //x
    m_velocity1.x = m_velocity0.x + m_acc0.x + ((m_acc1.x - m_acc0.x)/2)*m_deltaTime;
    m_distance1.x = m_distance0.x + m_velocity0.x + ((m_velocity1.x - m_velocity0.x)/2)*m_deltaTime;
    //y
    m_velocity1.y = m_velocity0.y + m_acc0.y + ((m_acc1.y - m_acc0.y)/2)*m_deltaTime;
    m_distance1.y = m_distance0.y + m_velocity0.y + ((m_velocity1.y - m_velocity0.y)/2)*m_deltaTime;
    //z
    m_velocity1.z = m_velocity0.z + m_acc0.z + ((m_acc1.z - m_acc0.z)/2)*m_deltaTime;
    m_distance1.z = m_distance0.z + m_velocity0.z + ((m_velocity1.z - m_velocity0.z)/2)*m_deltaTime;
    //
    m_velocity0 = m_velocity1;
    m_distance0 = m_distance1;
}
