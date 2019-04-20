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
#include "SVRendererBase.h"
#include "SVContextBase.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlCore.h"
#include "../base/SVLock.h"
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

void SVRenderMgr::setRenderer(SVRendererBasePtr _renderer){
    m_pRenderer = _renderer;
}

SVRendererBasePtr SVRenderMgr::getRenderer(){
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
    if(!_rt) {
        m_targetPool.remove(_name);
    } else {
        m_targetPool[_name] = _rt;    //没有就增加，有就替换
    }
    m_renderLock->unlock();
}

SVRenderTargetPtr SVRenderMgr::getRenderTarget(cptr8 _name) {
    TARGETPOOL::Iterator it = m_targetPool.find(_name);
    if(it!=m_targetPool.end()){
        return it->data;
    }
    return nullptr;
}

//渲染目标(这是要搞走的啊)
void SVRenderMgr::render(){
    m_renderLock->lock();
    if(m_pRenderer && m_pRenderScene ){
        SVContextBasePtr t_context = m_pRenderer->getRenderContext();
        if( t_context && t_context->activeContext() ){
            _adapt();
            _pushMatStack();
            SVRenderTargetPtr t_rt = getRenderTarget( m_pRenderScene->getName() );
            if( t_context->activeRenderTarget( t_rt ) ){
                m_pRenderScene->render();
                m_pRenderer->resetState();
                t_context->swapRenderTarget( t_rt );   //交换场景
            }else{
                m_pRenderScene->clearRenderCmd();
            }
            m_pRenderer->removeUnuseRes();  //资源释放
            _clearMatStack();
        }
    }
    m_renderLock->unlock();
}

void SVRenderMgr::_adapt() {
    if(m_pRenderer) {
        if(m_adaptMode == 0) {
            //形变 填充
            SVMtlCorePtr t_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "screennor");
            t_pMtl->setTexture(0,E_TEX_MAIN);    //那第一张纹理
            t_pMtl->setBlendEnable(false);
            t_pMtl->setBlendState(GL_ONE,GL_ZERO);
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
    m_targetPool.clear();
    m_stack_proj.destroy();
    m_stack_view.destroy();
    m_stack_vp.destroy();
    m_renderLock->unlock();
}

//回收GL资源
void SVRenderMgr::recycleRes() {
    if( m_pRenderer ) {
        m_pRenderer->clearRes();
    }
}

void SVRenderMgr::refreshDefMat(FMat4 _viewMat, FMat4 _projMat, FMat4 _vpMat){
    m_viewMat = _vpMat;
    m_projMat = _projMat;
    m_vpMat = _vpMat;
}
//
void SVRenderMgr::pushProjMat(FMat4 _mat){
    FMat4 mat4 = _mat;
    m_stack_proj.push(mat4);
}
FMat4 SVRenderMgr::getProjMat(){
    FMat4 mat4Proj = m_stack_proj.top();
    return mat4Proj;
}
void SVRenderMgr::popProjMat(){
    m_stack_proj.pop();
}
//
void SVRenderMgr::pushViewMat(FMat4 _mat){
    FMat4 mat4 = _mat;
    m_stack_view.push(mat4);
}
FMat4 SVRenderMgr::getViewMat(){
    FMat4 mat4View = m_stack_view.top();;
    return mat4View;
}
void SVRenderMgr::popViewMat(){
    m_stack_view.pop();
}
//
void SVRenderMgr::pushVPMat(FMat4 _mat){
    FMat4 mat4 = _mat;
    m_stack_vp.push(mat4);
}
FMat4 SVRenderMgr::getVPMat(){
    FMat4 mat4VP = m_stack_vp.top();;
    return mat4VP;
}
void SVRenderMgr::popVPMat(){
    m_stack_vp.pop();
}

void SVRenderMgr::_clearMatStack(){
    m_stack_proj.clear();
    m_stack_view.clear();
    m_stack_vp.clear();
}

void SVRenderMgr::_pushMatStack(){
    pushProjMat(m_projMat);
    pushViewMat(m_viewMat);
    pushVPMat(m_vpMat);
}
