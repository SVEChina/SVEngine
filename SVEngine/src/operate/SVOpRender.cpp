//
// SVOpRender.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOpRender.h"
#include "../app/SVInst.h"
#include "../callback/SVOpCallback.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVCameraMgr.h"
#include "../basesys/SVConfig.h"
#include "../node/SVScene.h"
#include "../node/SVCameraNode.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderTarget.h"
#include "../rendercore/SVGL/SVRendererGL.h"
#include "../rendercore/SVMetal/SVRendererMetal.h"
#include "../rendercore/SVVulkan/SVRendererVK.h"
#include "../rendercore/renderer/SVContextBase.h"

//创建渲染器
SVOpCreateRenderder::SVOpCreateRenderder(SVInst *_app)
: SVOpBase(_app) {
    m_type = E_RENDERER_GLES;
    m_glContext = nullptr;
    m_windows = nullptr;
    m_pixelFormate = nullptr;
    m_glVersion = 2;
    m_w = 720;
    m_h = 1280;
}

#if defined SV_IOS

void SVOpCreateRenderder::setGLParam(s32 _ver, void *_context, s32 _w, s32 _h) {
    m_type = E_RENDERER_GLES;
    m_glVersion = _ver;
    m_glContext = _context;
    m_w = _w;
    m_h = _h;
}

#elif defined SV_ANDROID

void SVOpCreateRenderder::setGLParam(s32 _ver, void *_context, void *_windows, s32 _w, s32 _h) {
    m_type = E_RENDERER_GLES;
    m_glVersion = _ver;
    m_glContext = _context;
    m_windows = _windows;
    m_w = _w;
    m_h = _h;
}

#elif defined SV_OSX

void SVOpCreateRenderder::setGLParam(s32 _ver,void*_context,void* _pixelFormate,s32 _w,s32 _h) {
    m_type = E_RENDERER_GLES;
    m_glVersion = _ver;
    m_glContext = _context;
    m_pixelFormate = _pixelFormate;
    m_w = _w;
    m_h = _h;
}

#endif

void SVOpCreateRenderder::setVunkanParam() {
    m_type = E_RENDERER_VUNKAN;
}

void SVOpCreateRenderder::setMetalParam() {
    m_type = E_RENDERER_METAL;
}

void SVOpCreateRenderder::_process(f32 dt) {
    if (m_type == E_RENDERER_GLES) {
        SVRendererGLPtr t_renderer = MakeSharedPtr<SVRendererGL>(mApp);
        mApp->getRenderMgr()->setRenderer(t_renderer);
#if defined SV_IOS
        t_renderer->init(m_glVersion, m_glContext, m_w, m_h);
        mApp->getConfig()->setCameraDsp(m_w, m_h, 0);
#elif defined SV_ANDROID
        t_renderer->init(m_glVersion, m_windows, m_glContext, m_w, m_h);
        mApp->getConfig()->setCameraDsp(m_w, m_h, 0);
#elif defined SV_OSX
        t_renderer->init(m_glVersion,m_glContext,m_pixelFormate,m_w, m_h);
#endif
    } else if (m_type == E_RENDERER_VUNKAN) {
        SVRendererVKPtr t_renderer = MakeSharedPtr<SVRendererVK>(mApp);
        mApp->getRenderMgr()->setRenderer(t_renderer);
    } else if (m_type == E_RENDERER_METAL) {
//        SVRendererMetalPtr t_renderer = MakeSharedPtr<SVRendererMetal>(mApp);
//        mApp->getRenderMgr()->setRenderer(t_renderer);
    }
}

//删除渲染器
SVOpDestroyRenderder::SVOpDestroyRenderder(SVInst *_app)
        : SVOpBase(_app) {
}

void SVOpDestroyRenderder::_process(float dt) {
    //need write
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer){
        mApp->getRenderMgr()->setRenderer(nullptr);
        t_renderer->destroy();
        t_renderer = nullptr;
    }
}

//重置渲染器内部大小
SVOpResizeRenderder::SVOpResizeRenderder(SVInst *_app,s32 _w,s32 _h)
: SVOpBase(_app)
, m_width(_w)
, m_height(_h){
}

void SVOpResizeRenderder::_process(float dt) {
    //need write
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer){
        t_renderer->resize(m_width,m_height);
        SVCameraNodePtr t_camera = mApp->m_pGlobalMgr->m_pCameraMgr->getMainCamera();
        if(t_camera){
            t_camera->resetCamera(m_width, m_height);
        }
    }
}

//设置渲染目标
SVOpSetRenderTarget::SVOpSetRenderTarget(SVInst *_app)
        : SVOpBase(_app) {
}

void SVOpSetRenderTarget::setTargetParam(s32 _w, s32 _h, u32 _frameID, u32 _colorid, bool _mirror) {
    m_width = _w;
    m_height = _h;
    m_mirror = _mirror;
    m_frameID = _frameID;
    m_colorID = _colorid;
}

void SVOpSetRenderTarget::_process(f32 dt) {
    SVRenderTargetOutPtr t_renderTargetEnv = MakeSharedPtr<SVRenderTargetOut>(mApp,
                                                                              m_width,
                                                                              m_height,
                                                                              m_frameID,
                                                                              m_colorID);
    mApp->getRenderMgr()->pushRCmdCreate(t_renderTargetEnv);
    mApp->getRenderMgr()->setRenderTarget("sve", t_renderTargetEnv);
    mApp->m_pGlobalParam->m_inner_width = m_width;
    mApp->m_pGlobalParam->m_inner_height = m_height;
    mApp->getConfig()->mirror = m_mirror;
}

//
SVOpSetRenderTextureTarget::SVOpSetRenderTextureTarget(SVInst *_app)
        : SVOpBase(_app) {
}

void SVOpSetRenderTextureTarget::setTargetParam(s32 _w, s32 _h, u32 _textureId, bool _mirror) {
    m_width=_w;
    m_height=_h;
    m_mirror = _mirror;
    m_texId=_textureId;
}

void SVOpSetRenderTextureTarget::_process(f32 dt) {
    SVRenderTargetOutTexturePtr t_renderTargetEnv = MakeSharedPtr<SVRenderTargetOutTexture>(mApp, m_width,
                                                                              m_height, m_texId);
    mApp->getRenderMgr()->pushRCmdCreate(t_renderTargetEnv);
    mApp->getRenderMgr()->setRenderTarget("sve", t_renderTargetEnv);
    mApp->getConfig()->mirror = m_mirror;
}

//
SVOpDestroyRenderTarget::SVOpDestroyRenderTarget(SVInst *_app)
        : SVOpBase(_app) {
}

void SVOpDestroyRenderTarget::_process(f32 dt) {
    mApp->getRenderMgr()->setRenderTarget("sve", nullptr);
}

SVOpSetRenderMirror::SVOpSetRenderMirror(SVInst *_app)
: SVOpBase(_app){
    
}

void SVOpSetRenderMirror::setTargetParam(bool _mirror){
    m_mirror = _mirror;
}

void SVOpSetRenderMirror::_process(f32 dt){
    mApp->getConfig()->mirror = m_mirror;
}
//
SVOpRefreshDefMat::SVOpRefreshDefMat(SVInst *_app, FMat4 _viewMat, FMat4 _projMat, FMat4 _vpMat)
: SVOpBase(_app){
    m_viewMat = _viewMat;
    m_projMat = _projMat;
    m_vpMat = _vpMat;
}

void SVOpRefreshDefMat::_process(f32 dt){
    SVCameraNodePtr mainCamera = mApp->getCameraMgr()->getMainCamera();
    if (mainCamera) {
        mApp->getRenderMgr()->refreshDefMat(mainCamera->getViewMatObj(), mainCamera->getProjectMatObj(), mainCamera->getVPMatObj());
    }
}
