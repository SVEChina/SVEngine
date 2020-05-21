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
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVPickProcess.h"
#include "../node/SVScene.h"
#include "../node/SVCameraNode.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderMgr.h"
#include "../act/SVCameraCtrl.h"
#include <sys/time.h>
//
SVSensorProcess::SVSensorProcess(SVInst *_app)
:SVProcess(_app) {
    s32 m_sw = mApp->m_pGlobalParam->m_inner_width;
    s32 m_sh = mApp->m_pGlobalParam->m_inner_height;
    //
    m_pARCtrl =  MakeSharedPtr<SVCtrlCamereAR>(mApp);
    //
    m_pARProj = MakeSharedPtr<SVARProj>();
    //
    m_pARCamera = MakeSharedPtr<SVCameraNode>(mApp);
    m_pARCamera->init();
    m_pARCamera->resetCamera(m_sw,m_sh);
    m_pARCamera->setCtrl(m_pARCtrl);
    m_pARCamera->setProjMethod(m_pARProj);
    
}

SVSensorProcess::~SVSensorProcess() {
    m_pARCtrl = nullptr;
    m_pARProj = nullptr;
    if(m_pARCamera) {
        m_pARCamera->destroy();
        m_pARCamera = nullptr;
    }
}

void SVSensorProcess::update(f32 _dt){
    if(m_pARCamera) {
        m_pARCamera->update(_dt);
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        SVRendererPtr t_renderer = mApp->getRenderer();
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

bool SVSensorProcess::procEvent(SVEventPtr _event){
    if (_event->eventType == SV_EVENT_TYPE::EVN_T_CAMERA_OREN){
        SVCameraOrenEventPtr oren = std::dynamic_pointer_cast<SVCameraOrenEvent>(_event);
        if(m_pARCtrl) {
            m_pARCtrl->setEur( oren->yaw,oren->pitch,oren->roll);
        }
    }if (_event->eventType == SV_EVENT_TYPE::EVN_T_CAMERA_POSITION){
        SVCameraPosEventPtr t_cameraPos = std::dynamic_pointer_cast<SVCameraPosEvent>(_event);
        if(t_cameraPos) {
            FVec3 t_camPos = FVec3(t_cameraPos->m_px, t_cameraPos->m_py, t_cameraPos->m_pz);
            m_pARCtrl->setPos(t_camPos);
        }
    }else if (_event->eventType == SV_EVENT_TYPE::EVN_T_CAMERA_MATRIX){
        SVCameraMatrixEventPtr cameraMatrix = std::dynamic_pointer_cast<SVCameraMatrixEvent>(_event);
        if(m_pARCtrl) {
            FMat4 t_viewMat = FMat4((f32 *)cameraMatrix->m_matData->getData());
            m_pARCtrl->setViewMat(t_viewMat);
        }
    }else if (_event->eventType == SV_EVENT_TYPE::EVN_T_PROJECT_MATRIX){
        SVProjectMatrixEventPtr projectMatrix = std::dynamic_pointer_cast<SVProjectMatrixEvent>(_event);
        if(m_pARProj) {
            FMat4 t_projectMat = FMat4((f32 *)projectMatrix->m_matData->getData());
            m_pARProj->setProjMat(t_projectMat);
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
        SVDeviceAccelerometerEventPtr accelerometer = std::dynamic_pointer_cast<SVDeviceAccelerometerEvent>(_event);
    }
    return true;
}
