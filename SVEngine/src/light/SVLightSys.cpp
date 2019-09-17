//
// SVLightMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVLightSys.h"
#include "../base/SVLock.h"
#include "../node/SVCameraNode.h"
#include "../act/SVCameraCtrl.h"

SVLightSys::SVLightSys(SVInst *_app)
:SVSysBase(_app) {
    m_subsysType = "SVLightSys";
}

SVLightSys::~SVLightSys() {
}

void SVLightSys::init() {
    m_g_ambient = FVec3(0.2f,0.2f,0.2f);
    m_g_diffuse = FVec3(0.6f,0.6f,0.6f);
    m_g_specular = FVec3(1.0f,1.0f,1.0f);
    m_g_direction = FVec3(1.0f,1.0f,0.0f);
    m_g_diffuse_sc = 1.0f;
    
    s32 m_sw = mApp->m_pGlobalParam->m_inner_width;
    s32 m_sh = mApp->m_pGlobalParam->m_inner_height;
    m_pLightCamera = MakeSharedPtr<SVCameraNode>(mApp);
    m_pLightCamera->resetCamera(m_sw,m_sh);
    SVCamCtrlBasePtr t_pCtrl =  MakeSharedPtr<SVCamCtrlBase>(mApp);
    f32 t_pos_z = 0.5f * m_sh / tan(30.0f * DEGTORAD);
    t_pCtrl->setPosition( 228.888855, 753.333312,815.555419);
    t_pCtrl->setTarget(0.0f, 0.0f, 0.0f);
    t_pCtrl->setUp(0.0f,1.f,0.0f);
    m_pLightCamera->setCtrl(t_pCtrl);
    m_pLightCamera->setZ(100, 3000);
    m_pLightCamera->setProject();
   
    
//    m_uiLock = MakeSharedPtr<SVLock>();
//    m_pUIRoot = MakeSharedPtr<SVNode>(mApp);
}

SVCameraNodePtr SVLightSys::getCamera(){
    return m_pLightCamera;
}

void SVLightSys::destroy() {
    //m_pUIRoot = nullptr;
    m_pLightCamera = nullptr;
}

void SVLightSys::update(f32 dt) {
     m_pLightCamera->update(dt);
}
