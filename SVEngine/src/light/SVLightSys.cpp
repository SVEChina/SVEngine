//
// SVLightMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVLightSys.h"
#include "../base/SVLock.h"

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
//    m_uiLock = MakeSharedPtr<SVLock>();
//    m_pUIRoot = MakeSharedPtr<SVNode>(mApp);
}

void SVLightSys::destroy() {
    //m_pUIRoot = nullptr;
}

void SVLightSys::update(f32 dt) {
}
