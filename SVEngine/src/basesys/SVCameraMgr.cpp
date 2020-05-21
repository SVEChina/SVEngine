//
// SVCameraMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVCameraMgr.h"
#include "../basesys/SVBasicSys.h"
#include "../node/SVCameraNode.h"
#include "../base/SVLock.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderer.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderCmd.h"

SVCameraMgr::SVCameraMgr(SVInst *_app)
:SVSysBase(_app) {
    m_subsysType = "SVCameraMgr";
    m_cameraLock = MakeSharedPtr<SVLock>();
}

SVCameraMgr::~SVCameraMgr() {
    m_cameraLock = nullptr;
}

void SVCameraMgr::init() {
    //主相机
    m_mainCamera = MakeSharedPtr<SVCameraNode>(mApp);
    m_mainCamera->init();
    m_mainCamera->project();
    //ui相机
    m_uiCamera = MakeSharedPtr<SVCameraNode>(mApp);
    m_uiCamera->init();
    m_uiCamera->ortho();
}

//
void SVCameraMgr::destroy() {
    if(m_mainCamera) {
        m_mainCamera->destroy();
        m_mainCamera = nullptr;
    }
    if(m_uiCamera) {
        m_uiCamera->destroy();
        m_uiCamera = nullptr;
    }    
}

//更新
void SVCameraMgr::update(f32 dt) {
     //主相机更新
    if(m_mainCamera){
        m_mainCamera->update(dt);
        //
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        SVRendererPtr t_renderer = mApp->getRenderer();
        if(t_renderer && t_renderer->getRenderTexture() ) {
            m_mainCamera->addLinkFboObject( t_renderer->getRenderTexture() );
        }
        if(t_rs && t_renderer) {
            //这不知道要写到哪，先写这了. 晓帆
            FMat4 t_vm = m_mainCamera->getViewMatObj();
            FMat4 t_pm = m_mainCamera->getProjectMatObj();
            SVRenderCmdPushVPMatPtr t_cmd = MakeSharedPtr<SVRenderCmdPushVPMat>(t_vm,t_pm);
            t_cmd->setRenderer(t_renderer);
            t_cmd->mTag = "main_camera_begin";
            t_rs->pushRenderCmd(RST_SCENE_BEGIN, t_cmd);
        }
    }
    //更新ui相机
    if(m_uiCamera) {
        m_uiCamera->update(dt);
        //
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        SVRendererPtr t_renderer = mApp->getRenderer();
        if(t_rs && t_renderer) {
            FMat4 t_vm = m_uiCamera->getViewMatObj();
            FMat4 t_pm = m_uiCamera->getProjectMatObj();
            SVRenderCmdPushVPMatPtr t_cmd_push_ui = MakeSharedPtr<SVRenderCmdPushVPMat>(t_vm,t_pm);
            t_cmd_push_ui->setRenderer(t_renderer);
            t_cmd_push_ui->mTag = "ui_camera_begin";
            t_rs->pushRenderCmd(RST_UI_BEGIN, t_cmd_push_ui);
            //
            SVRenderCmdPopVPMatPtr t_cmd_pop_ui = MakeSharedPtr<SVRenderCmdPopVPMat>();
            t_cmd_pop_ui->setRenderer(t_renderer);
            t_cmd_pop_ui->mTag = "ui_camera_end";
            t_rs->pushRenderCmd(RST_UI_END, t_cmd_pop_ui);
        }
    }
}

SVCameraNodePtr SVCameraMgr::getMainCamera(){
    return m_mainCamera;
}

SVCameraNodePtr SVCameraMgr::getUICamera() {
    return m_uiCamera;
}

void SVCameraMgr::resize(f32 _w,f32 _h) {
    if(m_mainCamera){
        m_mainCamera->resetCamera(_w,_h);
    }
    if(m_uiCamera){
        m_uiCamera->resetCamera(_w,_h);
    }
}
