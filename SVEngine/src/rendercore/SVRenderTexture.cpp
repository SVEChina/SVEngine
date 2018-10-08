//
// SVRenderTexture.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderTexture.h"
#include "SVRenderScene.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"
#include "SVRenderMgr.h"
#include "SVGL/SVRResGL.h"
#include "SVGL/SVRendererGL.h"
#include "SVMetal/SVRendererMetal.h"
#include "SVVulkan/SVRendererVK.h"
#include <iostream>
#include <memory>
//
SVRenderTexture::SVRenderTexture(SVInst *_app,SVTexturePtr _tex ,bool _depth,bool _stencil)
:SVFboObject(_app){
    m_tex = _tex;
    m_depth = _depth;
    m_stencil = _stencil;
}

SVRenderTexture::~SVRenderTexture() {
}

void SVRenderTexture::create(SVRendererBasePtr _renderer) {
    SVRObjBase::create(_renderer);
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(_renderer);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        SVRResGLTexPtr t_tex = nullptr;
        if (m_tex) {
            t_tex = std::dynamic_pointer_cast<SVRResGLTex>(m_tex->getResTex());
        }
        m_objFBOPtr = MakeSharedPtr<SVResGLRenderTexture>(mApp, t_tex, m_depth, m_stencil);
        m_objFBOPtr->create(_renderer);
        return ;
    }
    SVRendererVKPtr t_rendeVKPtr = std::dynamic_pointer_cast<SVRendererVK>(_renderer);
    if (t_rendeVKPtr) {
        //渲染器类型E_RENDERER_VUNKAN,
        
    }
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
    if (t_rendeMetalPtr) {
        //渲染器类型E_RENDERER_METAL,
        
    }
}

void SVRenderTexture::destroy(SVRendererBasePtr _renderer) {
    if (m_objFBOPtr) {
        m_objFBOPtr->destroy(_renderer);
    }
    SVRObjBase::destroy(_renderer);
}

void SVRenderTexture::setTexture(SVTexturePtr _tex) {
    SVResGLRenderTexturePtr t_tmp = std::dynamic_pointer_cast<SVResGLRenderTexture>(m_objFBOPtr);
    SVRResGLTexPtr t_texGL = nullptr;
    if (_tex) {
        t_texGL = std::dynamic_pointer_cast<SVRResGLTex>(_tex->getResTex());
    }
    if (t_tmp && t_texGL) {
        t_tmp->setTexture(t_texGL);
    }
}

void SVRenderTexture::refresh() {
    SVResGLRenderTexturePtr t_tmp = std::dynamic_pointer_cast<SVResGLRenderTexture>(m_objFBOPtr);
    if (t_tmp) {
        t_tmp->refresh();
    }
}
