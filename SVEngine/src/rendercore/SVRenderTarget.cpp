//
// SVRenderTarget.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderTarget.h"
#include "SVRenderScene.h"
#include "../app/SVInst.h"
#include "SVRenderMgr.h"
#include "SVGL/SVRResGL.h"
#include "SVGL/SVRendererGL.h"
#include "SVMetal/SVRendererMetal.h"
#include "SVVulkan/SVRendererVK.h"

//
SVRenderTarget::SVRenderTarget(SVInst *_app, s32 _w, s32 _h, bool _depth, bool _stencil)
        : SVFboObject(_app) {
    m_w = _w;
    m_h = _h;
    m_depth = _depth;
    m_stencil = _stencil;

}

SVRenderTarget::~SVRenderTarget() {
}

int SVRenderTarget::getColorID() {
    SVResGLRenderTargetPtr t_tmp = std::dynamic_pointer_cast<SVResGLRenderTarget>(m_objFBOPtr);
    if (t_tmp) {
        return t_tmp->getColorID();
    }
    return 0;
}

void SVRenderTarget::create(SVRendererBasePtr _renderer) {
    SVRObjBase::create(_renderer);
    SVRendererBasePtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        m_objFBOPtr = MakeSharedPtr<SVResGLRenderTarget>(mApp, m_w, m_h, m_depth, m_stencil);

    }
    SVRendererVKPtr t_rendeVKPtr = std::dynamic_pointer_cast<SVRendererVK>(t_renderBasePtr);
    if (t_rendeVKPtr) {
        //渲染器类型E_RENDERER_VUNKAN,

    }
//    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(
//            t_renderBasePtr);
//    if (t_rendeMetalPtr) {
//        //渲染器类型E_RENDERER_METAL,
//    }
    if (m_objFBOPtr) {
        m_objFBOPtr->create(_renderer);
    }
}

void SVRenderTarget::destroy(SVRendererBasePtr _renderer) {
    if (m_objFBOPtr) {
        m_objFBOPtr->destroy(_renderer);
    }
    SVRObjBase::destroy(_renderer);
}

//默认RenderTarget
SVRenderTargetDefault::SVRenderTargetDefault(SVInst *_app,s32 _w, s32 _h)
: SVRenderTarget(_app,_w,_h,false,false) {
    m_w = _w;
    m_h = _h;
}

SVRenderTargetDefault::~SVRenderTargetDefault() {
}

void SVRenderTargetDefault::create(SVRendererBasePtr _renderer) {
    SVRObjBase::create(_renderer);
    SVRendererBasePtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        m_objFBOPtr = MakeSharedPtr<SVRResGLOutFBO>(mApp,0);
        
    }
    SVRendererVKPtr t_rendeVKPtr = std::dynamic_pointer_cast<SVRendererVK>(t_renderBasePtr);
    if (t_rendeVKPtr) {
        //渲染器类型E_RENDERER_VUNKAN,
        
    }
//    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(
//                                                                                    t_renderBasePtr);
//    if (t_rendeMetalPtr) {
//        //渲染器类型E_RENDERER_METAL,
//    }
    if (m_objFBOPtr) {
        m_objFBOPtr->create(_renderer);
    }
}

void SVRenderTargetDefault::destroy(SVRendererBasePtr _renderer) {
    if (m_objFBOPtr) {
        m_objFBOPtr->destroy(_renderer);
    }
    SVRObjBase::destroy(_renderer);
}

//环境
SVRenderTargetOut::SVRenderTargetOut(SVInst *_app, s32 _w, s32 _h, u32 _fboid, u32 _colorID)
        : SVRenderTarget(_app, _w, _h, false, false) {
    m_w = _w;
    m_h = _h;
    m_fboid = _fboid;
    m_colorID = _colorID;

}

SVRenderTargetOut::~SVRenderTargetOut() {

}

void SVRenderTargetOut::create(SVRendererBasePtr _renderer) {
    SVRObjBase::create(_renderer);
    SVRendererBasePtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        m_objFBOPtr = MakeSharedPtr<SVResGLRenderTargetOut>(mApp, m_w, m_h, m_fboid, m_colorID);

    }else{
        SVRendererVKPtr t_rendeVKPtr = std::dynamic_pointer_cast<SVRendererVK>(t_renderBasePtr);
        if (t_rendeVKPtr) {
            //渲染器类型E_RENDERER_VUNKAN,
            
        }else{
//            SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(t_renderBasePtr);
//            if (t_rendeMetalPtr) {
//                //渲染器类型E_RENDERER_METAL,
//            }
        }
    }
    if (m_objFBOPtr) {
        m_objFBOPtr->create(_renderer);
    }
}

void SVRenderTargetOut::destroy(SVRendererBasePtr _renderer) {
    if (m_objFBOPtr) {
        m_objFBOPtr->destroy(_renderer);
    }
    SVRObjBase::destroy(_renderer);
}

SVRenderTargetOutTexture::SVRenderTargetOutTexture(SVInst *_app, s32 _w, s32 _h, u32 _texId)
        : SVRenderTarget(_app, _w, _h, false, false) {
    m_w = _w;
    m_h = _h;
    m_texid = _texId;
}

SVRenderTargetOutTexture::~SVRenderTargetOutTexture() {

}

void SVRenderTargetOutTexture::create(SVRendererBasePtr _renderer) {
    SVRenderTarget::create(_renderer);
    SVRObjBase::create(_renderer);
    SVRendererBasePtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        //  SVInst *_app,SVRResGLTexPtr _tex,s32 _w, s32 _h,bool _depth,bool _stencil
        SVRResGLTexWithTexIDPtr t_texture = MakeSharedPtr<SVRResGLTexWithTexID>(mApp, m_texid);
        t_texture->setwidth(m_w);
        t_texture->setheight(m_h);
        t_texture->setdataformate(GL_RGBA);
        t_texture->setdataformate(GL_RGBA);
        t_texture->settype(GL_TEXTURE_2D);
        m_objFBOPtr = MakeSharedPtr<SVResGLRenderTexture>(mApp, t_texture, false, false);

    }
    SVRendererVKPtr t_rendeVKPtr = std::dynamic_pointer_cast<SVRendererVK>(t_renderBasePtr);
    if (t_rendeVKPtr) {
        //渲染器类型E_RENDERER_VUNKAN,

    }
//    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(
//            t_renderBasePtr);
//    if (t_rendeMetalPtr) {
//        //渲染器类型E_RENDERER_METAL,
//
//    }
    if (m_objFBOPtr) {
        m_objFBOPtr->create(_renderer);
    }
}

void SVRenderTargetOutTexture::destroy(SVRendererBasePtr _renderer) {
    SVRenderTarget::destroy(_renderer);
    if (m_objFBOPtr) {
        m_objFBOPtr->destroy(_renderer);
    }
}
