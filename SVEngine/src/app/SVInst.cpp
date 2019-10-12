//
// SVInst.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVInst.h"
#include "SVGlobalMgr.h"
#include "SVGlobalParam.h"
#include "../base/SVLock.h"
#include "../work/SVThreadPool.h"
#include "../work/SVThreadMain.h"
#include "../file/SVFileMgr.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderState.h"
#include "../rendercore/SVRendererBase.h"
#include "../rendercore/SVContextBase.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVConfig.h"
#include "../operate/SVOpBase.h"
#include "../operate/SVOpCreate.h"
#include "../operate/SVOpThread.h"

SVInst::SVInst() {
    m_svst = SV_ST_NULL;
    m_sync = 0;
    m_engTimeState = ENG_TS_NOR;
}

SVInst::~SVInst() {
}

void SVInst::init(s32 _sync) {
    //同步方式
    m_sync = _sync;
    m_pGlobalMgr = MakeSharedPtr<SVGlobalMgr>(this);
    m_pGlobalParam = MakeSharedPtr<SVGlobalParam>(this);
    m_pTPool = MakeSharedPtr<SVThreadPool>(this);
    m_pGlobalMgr->m_pConfig = MakeSharedPtr<SVConfig>(this);
    m_pGlobalMgr->m_pConfig->init();
    m_svst = SV_ST_WAIT;
}

void SVInst::destroy() {
    m_pTPool = nullptr;
    m_pGlobalMgr = nullptr;
    m_pGlobalParam = nullptr;
    m_svst = SV_ST_NULL;
}

void SVInst::startSVE() {
    m_pGlobalMgr->init();
    m_pTPool->init(m_sync);
    m_pTPool->start();
    m_svst = SV_ST_RUN;
}

void SVInst::stopSVE() {
    m_svst = SV_ST_WAIT;
    m_pTPool->stop();
    m_pTPool->destroy();        //跑一边线程 该干掉的都干掉了
    m_pGlobalMgr->destroy();    //引擎各个模块开始销毁(这里 opengl 没有回收 尴尬)
}

void SVInst::updateSVE(f32 _dt) {
    if(m_sync && m_pTPool) {
        SVThreadSyncPtr threadSync = DYN_TO_SHAREPTR(SVThreadSync, m_pTPool->getMainThread());
        if (threadSync) {
            threadSync->syncUpdate(_dt);
        }
    }
}

void SVInst::clearCache(){
    if((!m_sync) && m_pTPool && m_pTPool->getMainThread()){
        m_pTPool->getMainThread()->clearThreadCache();
    }
}

void SVInst::svSuspend(){
    if( (!m_sync) && m_pTPool && m_pTPool->getMainThread()){
        m_svst = SV_ST_SUSPEND;
        m_pTPool->getMainThread()->suspend();
    }
}

void SVInst::svWillSuspend(){
    if( (!m_sync) && m_pTPool && m_pTPool->getMainThread()){
        m_svst = SV_ST_WILLSUSPEND;
    }
}

void SVInst::svResume(){
    if( (!m_sync) && m_pTPool && m_pTPool->getMainThread()){
        m_pTPool->getMainThread()->resetTime();//重新开始的时候要重置下时间，否则第一帧有问题
        m_pTPool->getMainThread()->resume();
        m_svst = SV_ST_RUN;
    }
}

////同步模式
//void SVInst::svSync() {
//    if(m_pTPool && m_pTPool->getMainThread()){
//        m_pTPool->getMainThread()->setAuoWait(true);
//    }
//}

////异步模式
//void SVInst::syASync() {
//    if(m_pTPool && m_pTPool->getMainThread()){
//        m_pTPool->getMainThread()->setAuoWait(false);
//        m_pTPool->getMainThread()->notice();
//    }
//}

//
SVContextBase* SVInst::getContext(){
    if(m_pGlobalMgr->m_pRenderMgr){
        SVRendererBasePtr t_renderer = m_pGlobalMgr->m_pRenderMgr->getRenderer();
        if(t_renderer && t_renderer->getRenderContext() ){
            return t_renderer->getRenderContext().get();
        }
    }
    return nullptr;
}

//
void SVInst::addRespath(cptr8 path) {
    m_pGlobalMgr->m_pFileMgr->addRespath(path);
}

void SVInst::setTimeState(SV_ENG_TIMESTATE _mode){
    m_engTimeState = _mode;
}

SV_ENG_TIMESTATE SVInst::getTimeState(){
    return m_engTimeState;
}

//
SVFileMgrPtr SVInst::getFileMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pFileMgr;
}

SVConfigPtr SVInst::getConfig(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pConfig;
}

SVEventMgrPtr SVInst::getEventMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pEventMgr;
}

SVBasicSysPtr SVInst::getBasicSys(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pBasicSys;
}

SVCameraMgrPtr SVInst::getCameraMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pCameraMgr;
}

SVSceneMgrPtr SVInst::getSceneMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pSceneMgr;
}

SVUIMgrPtr SVInst::getUIMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pUIMgr;
}

SVLightSysPtr SVInst::getLightSys() {
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pLightSys;
}

SVModuleSysPtr SVInst::getModuleSys(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pModuleSys;
}

SVShaderMgrPtr SVInst::getShaderMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pShaderMgr;
}

SVTexMgrPtr SVInst::getTexMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pTexMgr;
}

SVRenderMgrPtr SVInst::getRenderMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pRenderMgr;
}

SVDetectMgrPtr SVInst::getDetectMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pDetectMgr;
}

SVDeformMgrPtr SVInst::getDeformMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pDeformSys;
}

SVStaticDataPtr SVInst::getDataMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pStaticData;
}

SVActionMgrPtr SVInst::getActionMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pActionMgr;
}

SVModelMgrPtr SVInst::getModelMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pModelMgr;
}

SVPhysicsWorldMgrPtr SVInst::getPhysicsWorldMgr(){
    if(!m_pGlobalMgr)
        return nullptr;
    return m_pGlobalMgr->m_pPhysicSys;
}

SVRendererBasePtr SVInst::getRenderer() {
    if(!m_pGlobalMgr)
        return nullptr;
    if(!m_pGlobalMgr->m_pRenderMgr)
        return nullptr;
    return m_pGlobalMgr->m_pRenderMgr->getRenderer();
}

SVRenderStatePtr SVInst::getRenderState() {
    if(!m_pGlobalMgr)
        return nullptr;
    if(!m_pGlobalMgr->m_pRenderMgr)
        return nullptr;
    if(!m_pGlobalMgr->m_pRenderMgr->getRenderer())
        return nullptr;
    return m_pGlobalMgr->m_pRenderMgr->getRenderer()->getState();
}
