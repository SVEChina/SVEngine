//
// SVFboObject.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFboObject.h"
#include "SVRenderScene.h"
#include "SVRenderMgr.h"
#include "SVGL/SVRResGL.h"
#if defined(SV_IOS) || defined(SV_OSX)
#include "SVMetal/SVRendererMetal.h"
#endif
#include "SVVulkan/SVRendererVK.h"
#include "SVGL/SVRendererGL.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVCameraMgr.h"
#include "../node/SVScene.h"
#include "../node/SVCameraNode.h"
//

SVFboObject::SVFboObject(SVInst *_app)
:SVRObjBase(_app)
,m_link(false){
}

SVFboObject::~SVFboObject() {
}

void SVFboObject::create(SVRendererPtr _renderer){
    SVRObjBase::create(_renderer);
    SVRendererPtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        m_objFBOPtr = MakeSharedPtr<SVRResGLFBO>(mApp);
        
    }
    SVRendererVKPtr t_rendeVKPtr = std::dynamic_pointer_cast<SVRendererVK>(t_renderBasePtr);
    if (t_rendeVKPtr) {
        //渲染器类型E_RENDERER_VUNKAN,
        
    }
//    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(t_renderBasePtr);
//    if (t_rendeMetalPtr) {
//        //渲染器类型E_RENDERER_METAL,
//
//    }
    if (m_objFBOPtr) {
        m_objFBOPtr->create(_renderer);
    }
}

void SVFboObject::destroy(SVRendererPtr _renderer) {
    if (m_objFBOPtr) {
        m_objFBOPtr->destroy(_renderer);
    }
    SVRObjBase::destroy(_renderer);
}

void SVFboObject::refresh() {
    SVRResGLFBOPtr t_tmp = std::dynamic_pointer_cast<SVRResGLFBO>(m_objFBOPtr);
    if (t_tmp) {
        t_tmp->refresh();
    }
}

void SVFboObject::setLink(bool _link) {
    m_link = _link;
}

void SVFboObject::bind() {
    SVRResGLFBOPtr t_tmp = std::dynamic_pointer_cast<SVRResGLFBO>(m_objFBOPtr);
    if (t_tmp) {
        t_tmp->bind();
        SVRendererPtr t_renderBasePtr = mApp->getRenderer();
        if(m_link && t_renderBasePtr){
            t_renderBasePtr->pushViewMat(m_mat_view);
            t_renderBasePtr->pushProjMat(m_mat_proj);
            t_renderBasePtr->pushVPMat(m_mat_vp);
        }
    }
}

void SVFboObject::clear(){
    SVRResGLFBOPtr t_tmp = std::dynamic_pointer_cast<SVRResGLFBO>(m_objFBOPtr);
    if (t_tmp) {
        t_tmp->clear();
    }
}

void SVFboObject::unbind() {
    SVRResGLFBOPtr t_tmp = std::dynamic_pointer_cast<SVRResGLFBO>(m_objFBOPtr);
    if (t_tmp) {
        t_tmp->unbind();
        SVRendererPtr t_renderBasePtr = mApp->getRenderer();
        if(m_link && t_renderBasePtr){
            t_renderBasePtr->popViewMat();
            t_renderBasePtr->popProjMat();
            t_renderBasePtr->popVPMat();
        }
    }
}

u32 SVFboObject::getFboID(){
    SVRResGLFBOPtr t_tmp = std::dynamic_pointer_cast<SVRResGLFBO>(m_objFBOPtr);
    if (t_tmp) {
        return t_tmp->getFboID();
    }
    return 0;
}

u32 SVFboObject::getWidth(){
    SVRResGLFBOPtr t_tmp = std::dynamic_pointer_cast<SVRResGLFBO>(m_objFBOPtr);
    if (t_tmp) {
        return t_tmp->getWidth();
    }
    return 0;
}

u32 SVFboObject::getHeight(){
    SVRResGLFBOPtr t_tmp = std::dynamic_pointer_cast<SVRResGLFBO>(m_objFBOPtr);
    if (t_tmp) {
        return t_tmp->getHeight();
    }
    return 0;
}

bool SVFboObject::hasDepth(){
    SVRResGLFBOPtr t_tmp = std::dynamic_pointer_cast<SVRResGLFBO>(m_objFBOPtr);
    if (t_tmp) {
        return t_tmp->hasDepth();
    }
    return 0;
}

bool SVFboObject::hasStencil(){
    SVRResGLFBOPtr t_tmp = std::dynamic_pointer_cast<SVRResGLFBO>(m_objFBOPtr);
    if (t_tmp) {
        return t_tmp->hasStencil();
    }
    return 0;
}

void SVFboObject::setProjMat(FMat4 _mat) {
    m_mat_proj = _mat;
    m_mat_vp = m_mat_proj*m_mat_view;
}

void SVFboObject::setViewMat(FMat4 _view) {
    m_mat_view = _view;
    m_mat_vp = m_mat_proj*m_mat_view;
}

//
SVOutFboObject::SVOutFboObject(SVInst *_app,u32 _fboid)
:SVFboObject(_app){
    m_fboID = _fboid;
}

SVOutFboObject::~SVOutFboObject() {
    
}

void SVOutFboObject::create(SVRendererPtr _renderer){
    //外部设置的FBO 直接使用外部的就好
    SVRObjBase::create(_renderer);
    SVRendererPtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        m_objFBOPtr = MakeSharedPtr<SVRResGLOutFBO>(mApp, m_fboID);
        
    }
    SVRendererVKPtr t_rendeVKPtr = std::dynamic_pointer_cast<SVRendererVK>(t_renderBasePtr);
    if (t_rendeVKPtr) {
        //渲染器类型E_RENDERER_VUNKAN,
        
    }
//    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(t_renderBasePtr);
//    if (t_rendeMetalPtr) {
//        //渲染器类型E_RENDERER_METAL,
//        
//    }
    if(m_objFBOPtr){
        m_objFBOPtr->create(_renderer);
    }
}

void SVOutFboObject::destroy(SVRendererPtr _renderer){
    if (m_objFBOPtr) {
        m_objFBOPtr->destroy(_renderer);
    }
    SVRObjBase::destroy(_renderer);
}

