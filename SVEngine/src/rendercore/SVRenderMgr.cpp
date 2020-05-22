//
// SVRenderMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderMgr.h"
#include "SVRenderScene.h"
#include "SVRenderCmd.h"
#include "SVRenderStream.h"
#include "SVRenderTarget.h"
#include "SVRenderPipline.h"
#include "SVRenderer.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlCore.h"
#include "../work/SVTdCore.h"
#include "../basesys/SVConfig.h"
#include "../basesys/SVStaticData.h"
#include "../app/SVInst.h"

SVRenderMgr::SVRenderMgr(SVInst *_app)
:SVGBase(_app) {
    m_RStreamCache =  MakeSharedPtr<SVRenderStream>();      //逻辑全局流
    m_renderLock = MakeSharedPtr<SVLock>();
    m_logicLock = MakeSharedPtr<SVLock>();
    m_pRenderScene = nullptr;
    m_pRenderer = nullptr;
    m_adaptMode = 0;
}

SVRenderMgr::~SVRenderMgr() {
    m_logicLock = nullptr;
    m_renderLock = nullptr;
}

void SVRenderMgr::init() {
}

void SVRenderMgr::destroy() {
    clear();
    if (m_pRenderer) {
        m_pRenderer->destroy();
        m_pRenderer = nullptr;
    }
}

SVRenderMeshPtr SVRenderMgr::createMeshRObj(){
    SVRenderMeshPtr t_rmesh = MakeSharedPtr<SVRenderMesh>(mApp);
    return t_rmesh;
}

//这里相当于准备数据
void SVRenderMgr::swapData(){
    m_logicLock->lock();
    m_renderLock->lock();
    //交换全局(逻辑流,渲染流)
    if(m_pRenderScene && m_pRenderer){
        //准备的cache推送到流中
        for(s32 i=0;i<m_RStreamCache->m_cmdArray.size();i++){
            m_RStreamCache->m_cmdArray[i]->setRenderer(m_pRenderer);
            m_pRenderScene->pushCacheCmd(RST_BEGIN,m_RStreamCache->m_cmdArray[i]);
        }
        m_RStreamCache->clearSVRenderCmd();
        //交换管线
        m_pRenderScene->swapPipline();
    }
    m_renderLock->unlock();
    m_logicLock->unlock();
}

void SVRenderMgr::pushRCmdCreate(SVRObjBasePtr _robj){
    m_logicLock->lock();
    if(_robj){
        SVRCmdCreatePtr t_cmd= MakeSharedPtr<SVRCmdCreate>(_robj);
        m_RStreamCache->addSVRenderCmd(t_cmd);
    }
    m_logicLock->unlock();
}

void SVRenderMgr::setRenderer(SVRendererPtr _renderer){
    m_pRenderer = _renderer;
}

SVRendererPtr SVRenderMgr::getRenderer(){
    return m_pRenderer;
}

void SVRenderMgr::setRenderScene(SVRenderScenePtr _rs){
    m_renderLock->lock();
    m_pRenderScene = _rs;
    m_renderLock->unlock();
}

SVRenderScenePtr SVRenderMgr::getRenderScene() {
    return m_pRenderScene;
}

void SVRenderMgr::setRenderTarget(cptr8 _name,SVRenderTargetPtr _rt) {
    m_renderLock->lock();
    m_renderLock->unlock();
}

SVRenderTargetPtr SVRenderMgr::getRenderTarget(cptr8 _name) {
    return nullptr;
}

//只关心渲染，不应该关心环境的切换 环境放到外面去调用
void SVRenderMgr::render(){
    //
    m_renderLock->lock();
    if(m_pRenderScene ){
        if( m_pRenderer ) {
            m_pRenderer->renderBegin();     //渲染器开始
            _adapt();                       //适配
            m_pRenderScene->render();       //渲染
            m_pRenderer->resetState();      //重置状态
            m_pRenderer->removeUnuseRes();  //资源释放
            m_pRenderer->renderEnd();       //渲染器结束
        }else {
            m_pRenderScene->clearRenderCmd();
        }
    }
    m_renderLock->unlock();
}

void SVRenderMgr::clearScreen(){
    m_renderLock->lock();
    if(m_pRenderer && m_pRenderScene ){
//        SVCtxBasePtr t_context = m_pRenderer->getRenderContext();
//        if( t_context && t_context->activeContext() ){
//            m_pRenderer->renderBegin();
//            SVRenderTargetPtr t_rt = getRenderTarget( m_pRenderScene->getName() );
//            if( t_context->activeRenderTarget( t_rt ) ){
//                m_pRenderer->resetState();
//                t_context->swapRenderTarget( t_rt );   //交换场景
//            }
//            m_pRenderScene->clearRenderCmd();
//            m_pRenderer->removeUnuseRes();  //资源释放
//            m_pRenderer->renderEnd();
//        }
    }
    m_renderLock->unlock();
}

void SVRenderMgr::_adapt() {
    return;
    if(m_pRenderer) {
        if(m_adaptMode == 0) {
            //形变 填充
            SVMtlCorePtr t_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "screennor");
            t_pMtl->setTexture(0,E_TEX_MAIN);    //那第一张纹理
            t_pMtl->setBlendEnable(false);
            t_pMtl->setBlendState(MTL_BLEND_ONE,MTL_BLEND_ZERO);
            bool t_mirror = mApp->getConfig()->mirror;
            if( !t_mirror ) {
                t_pMtl->setTexcoordFlip(-1.0f, 1.0f);
            }else {
                t_pMtl->setTexcoordFlip(1.0f, 1.0f);
            }
            SVRenderCmdAdaptPtr t_cmd = MakeSharedPtr<SVRenderCmdAdapt>();
            t_cmd->mTag = "adaptscene";
            t_cmd->setRenderer(m_pRenderer);
            t_cmd->setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            t_cmd->setWinSize(mApp->m_pGlobalParam->m_inner_width,mApp->m_pGlobalParam->m_inner_height);
            t_cmd->setMesh(mApp->getDataMgr()->m_screenMesh);
            t_cmd->setMaterial(t_pMtl->clone());
            m_pRenderScene->getPiplineRead()->pushRenderCmd(RST_ADAPT_SCENE, t_cmd);

        }else if(m_adaptMode == 1) {
            //非形变 固定
            
        }else if(m_adaptMode == 2) {
            //非形变 固定 内接
            
        }else if(m_adaptMode == 3) {
            //非形变 固定 外接
        }
    }
}

void SVRenderMgr::clear() {
    m_renderLock->lock();
    if (m_RStreamCache) {
        m_RStreamCache->clearSVRenderCmd();
        m_RStreamCache = nullptr;
    }
    if (m_pRenderScene) {
        m_pRenderScene->clearRenderCmd();
        m_pRenderScene = nullptr;
    }
    m_renderLock->unlock();
}

//回收GL资源
void SVRenderMgr::recycleRes() {
    if( m_pRenderer ) {
        m_pRenderer->clearRes();
    }
}
