//
// SVTextureIOS.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVTextureIOS.h"
#include "../rendercore/SVGL/SVRResGL.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVGL/SVRendererGL.h"
SVTextureIOS::SVTextureIOS(SVInst *_app)
:SVTexture(_app) {
    
}

SVTextureIOS::~SVTextureIOS() {
    mApp->m_IDPool.returnUID(m_uid);
    m_pData = nullptr;
    m_objTexPtr = nullptr;
    m_bCreated = false;
}

void SVTextureIOS::init(cptr8 _name, s32 _type, s32 _width, s32 _height, s32 _informate, s32 _dateformate, bool _enableMipMap){
    m_name = _name;
    m_width = _width;
    m_height = _height;
    m_type = _type;
    m_informate = _informate;
    m_dataformate = _dateformate;
}


void SVTextureIOS::create(SVRendererPtr _renderer){
    SVRObjBase::create(_renderer);
    if (!m_bCreated) {
        m_bCreated = true;
        SVRendererPtr t_renderBasePtr = mApp->getRenderer();
        SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
        if (t_renderGLPtr) {
            //渲染器类型E_RENDERER_GLES,
            m_objTexPtr = MakeSharedPtr<SVRResGLTexiOS>(mApp)  ;
        }
        
        if (m_objTexPtr) {
            m_objTexPtr->setname(m_name);
            m_objTexPtr->settype(m_type);
            m_objTexPtr->setwidth(m_width);
            m_objTexPtr->setheight(m_height);
            m_objTexPtr->setinformate(m_informate);
            m_objTexPtr->setdataformate(m_dataformate);
            _updateData();
            m_objTexPtr->create(_renderer);
        }
    }
}

void SVTextureIOS::destroy(SVRendererPtr _renderer){
    if (m_objTexPtr) {
        m_objTexPtr->destroy(_renderer);
    }
    SVRObjBase::destroy(_renderer);
}

void SVTextureIOS::pushData(u8* _srcPtr,s32 _w,s32 _h,s32 _pixelformate){
#ifdef SV_IOS
    SVRResGLTexiOSPtr t_tmp = std::dynamic_pointer_cast<SVRResGLTexiOS>(m_objTexPtr);
    if (t_tmp) {
        t_tmp->pushData(_srcPtr, _w, _h, _pixelformate);
    }
#endif
}

void SVTextureIOS::fetchData(u8* _dstPtr,s32 _w,s32 _h) {
#ifdef SV_IOS
    SVRResGLTexiOSPtr t_tmp = std::dynamic_pointer_cast<SVRResGLTexiOS>(m_objTexPtr);
    if (t_tmp) {
        t_tmp->fetchData(_dstPtr, _w, _h);
    }
#endif
}

void SVTextureIOS::_updateData(){
    SVTexture::_updateData();
}
