//
// SVInst.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVInst.h"
#include "SVGlobalMgr.h"
#include "SVGlobalParam.h"
#include "../work/SVTdCore.h"
#include "../work/SVThreadPool.h"
#include "../file/SVFileMgr.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderState.h"
#include "../rendercore/SVRenderer.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVConfig.h"
#include "../operate/SVOpBase.h"
#include "../operate/SVOpCreate.h"
#include "../operate/SVOpThread.h"

SVInst::SVInst() {
    m_svst = SV_ST_NULL;
    m_engTimeState = ENG_TS_NOR;
}

SVInst::~SVInst() {
}

SVInstPtr SVInst::makeCreate() {
    SVInstPtr t_inst = MakeSharedPtr<SVInst>();
    return t_inst;
}

void SVInst::init() {
    //同步方式
    m_pGlobalMgr = MakeSharedPtr<SVGlobalMgr>(this);
    m_pGlobalParam = MakeSharedPtr<SVGlobalParam>(this);
    //构建线程池
    m_pGlobalMgr->m_pConfig = MakeSharedPtr<SVConfig>(this);
    m_pGlobalMgr->m_pConfig->init();
    //
    m_svst = SV_ST_WAIT;
}

void SVInst::destroy() {
    m_pGlobalMgr = nullptr;
    m_pGlobalParam = nullptr;
    m_svst = SV_ST_NULL;
}

void SVInst::startSVE() {
    //开启一个线程，构建引擎，引擎构建完毕之后，调用返回
    m_pGlobalMgr->init();
    m_svst = SV_ST_RUN;
}

void SVInst::stopSVE() {
    //开启一个线程，销毁引擎，引擎销毁完毕以后，调用返回
    m_svst = SV_ST_WAIT;
    m_pGlobalMgr->destroy();    //引擎各个模块开始销毁(这里 opengl 没有回收 尴尬)
}

void SVInst::updateSVE(f32 _dt) {
    m_pGlobalMgr->update(_dt);
}

void SVInst::renderSVE() {
    m_pGlobalMgr->m_pRenderMgr->render();
}

void SVInst::svSuspend(){
}

void SVInst::svResume(){
}

void SVInst::clearCache(){
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

SVRendererPtr SVInst::getRenderer() {
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
