//
// SVSceneMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVSceneMgr.h"
#include "SVStaticData.h"
#include "../basesys/SVBasicSys.h"
#include "../node/SVScene.h"
#include "../node/SVCameraNode.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtlBeauty.h"
#include "../work/SVThreadPool.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderer.h"
#include "../basesys/SVConfig.h"


SVSceneMgr::SVSceneMgr(SVInst *_app)
:SVSysBase(_app) {
    m_subsysType = "SVSceneMgr";
    m_pMainScene = nullptr;
    m_sceneLock = MakeSharedPtr<SVLock>();
}

SVSceneMgr::~SVSceneMgr() {
    m_sceneLock = nullptr;
}

void SVSceneMgr::init() {
    m_pMainScene = nullptr;
}

void SVSceneMgr::destroy() {
    m_pMainScene = nullptr;
}

void SVSceneMgr::setScene(SVScenePtr _scene){
    m_sceneLock->lock();
    m_pMainScene = _scene;
    m_sceneLock->unlock();
    if(m_pMainScene){
        mApp->getRenderMgr()->setRenderScene( m_pMainScene->getRenderRS() );
    }
}

SVScenePtr SVSceneMgr::getScene(){
    return m_pMainScene;
}

//反适配
void SVSceneMgr::uiToScene(f32& _x,f32& _y) {
    //invert Y
    _y = mApp->m_pGlobalParam->m_inner_height - _y;
}

//更新
void SVSceneMgr::update(f32 dt) {
    m_sceneLock->lock();
    if(m_pMainScene){
        //激活场景
        mApp->m_pGlobalParam->m_curScene = m_pMainScene;
        m_pMainScene->active();
        m_pMainScene->update(dt);
        m_pMainScene->unactive();
        mApp->m_pGlobalParam->m_curScene = nullptr;
    }
    m_sceneLock->unlock();
}

