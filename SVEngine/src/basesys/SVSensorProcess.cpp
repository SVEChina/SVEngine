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
#include "../base/SVPreDeclare.h"
#include <sys/time.h>
//
SVSensorProcess::SVSensorProcess(SVInst *_app)
:SVProcess(_app) {
    m_acc0.set(0.0f, 0.0f, 0.0f);
    m_acc1.set(0.0f, 0.0f, 0.0f);
    m_velocity0.set(0.0f, 0.0f, 0.0f);
    m_velocity1.set(0.0f, 0.0f, 0.0f);
    m_distance0.set(0.0f, 0.0f, 0.0f);
    m_distance1.set(0.0f, 0.0f, 0.0f);
    m_isFitst = false;
    m_isEnable= true;
}

SVSensorProcess::~SVSensorProcess() {
    
}

void SVSensorProcess::startSensor(){
    m_isEnable = true;
}

void SVSensorProcess::disableSensor(){
    m_isEnable = false;
}

void SVSensorProcess::update(f32 _dt){
    
}

FVec3& SVSensorProcess::getVelocity(){
    return m_velocity0;
}

FVec3& SVSensorProcess::getDistance(){
    return m_distance0;
}

bool SVSensorProcess::procEvent(SVEventPtr _event){
    if (!m_isEnable) {
        return false;
    }
    if (_event->eventType == SV_EVENT_TYPE::EVN_T_CAMERA_OREN){
        SVCameraOrenEventPtr oren = std::dynamic_pointer_cast<SVCameraOrenEvent>(_event);
//        SVCameraNodePtr mainCamera = mApp->getCameraMgr()->getMainCamera();
//        mainCamera->setDirection(oren->pitch, oren->yaw, oren->roll);
        
    }else if (_event->eventType == SV_EVENT_TYPE::EVN_T_CAMERA_MATRIX){
        SVCameraMatrixEventPtr cameraMatrix = std::dynamic_pointer_cast<SVCameraMatrixEvent>(_event);
        SVCameraNodePtr mainCamera = mApp->getCameraMgr()->getMainCamera();
        memcpy(mainCamera->getCameraMat(), cameraMatrix->m_matData->getData(), cameraMatrix->m_matData->getSize());
    }else if (_event->eventType == SV_EVENT_TYPE::EVN_T_PROJECT_MATRIX){
        SVProjectMatrixEventPtr projectMatrix = std::dynamic_pointer_cast<SVProjectMatrixEvent>(_event);
        //        SVCameraNodePtr mainCamera = mApp->getCameraMgr()->getMainCamera();
        //        memcpy(mainCamera->getProjectMat(), projectMatrix->m_matData->getData(), projectMatrix->m_matData->getSize());
    }else if (_event->eventType == SV_EVENT_TYPE::EVN_T_ANCHOR_POINT){
        //        SVAnchorPointEventPtr anchorPoint = std::dynamic_pointer_cast<SVAnchorPointEvent>(_event);
        //        if (m_stage == E_G_STAGE_RUN) {
        //            SVLightStickUnitPtr unit = m_run->getUnitMgr()->generateUnit();
        //            if (unit) {
        //                unit->enter();
        //                unit->setPos(FVec3(anchorPoint->x, anchorPoint->y, anchorPoint->z));
        //            }
        //        }
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
