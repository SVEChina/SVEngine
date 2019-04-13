//
// SVTexture.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"
#include "../basesys/SVConfig.h"
#include "../rendercore/SVRenderMgr.h"
#include "../base/SVLock.h"
#include "../base/SVDataSwap.h"
//
#include "../rendercore/SVGL/SVRResGL.h"
#include "../rendercore/SVGL/SVRendererGL.h"
//
#include "../rendercore/SVMetal/SVRendererMetal.h"
#include "../rendercore/SVMetal/SVRResMetal.h"
//
#include "../rendercore/SVVulkan/SVRendererVK.h"

SVTexture::SVTexture(SVInst*_app)
: SVRObjBase(_app) {
    m_uid = mApp->m_IDPool.applyUID();
    m_name = "";
    m_bData = false;
    m_bEnableMipMap = false;
    m_width = 0;
    m_height = 0;
    m_type = GL_TEXTURE_2D;
    m_informate = GL_RGBA;
    m_dataformate = GL_RGBA;
    m_pData = MakeSharedPtr<SVDataSwap>();
}

SVTexture::~SVTexture() {
    mApp->m_IDPool.returnUID(m_uid);
    m_pData = nullptr;
}

void SVTexture::create(SVRendererBasePtr _renderer){
    SVRObjBase::create(_renderer);
    SVRendererBasePtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        m_objTexPtr = MakeSharedPtr<SVRResGLTex>(mApp);
        
    }
    SVRendererVKPtr t_rendeVKPtr = std::dynamic_pointer_cast<SVRendererVK>(t_renderBasePtr);
    if (t_rendeVKPtr) {
        //渲染器类型E_RENDERER_VUNKAN,
        //m_objTexPtr = MakeSharedPtr<SVRResGLTex>(mApp);
    }
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(t_renderBasePtr);
    if (t_rendeMetalPtr) {
        //渲染器类型E_RENDERER_METAL,
        m_objTexPtr = MakeSharedPtr<SVRResMetalTex>(mApp);
    }
    if (m_objTexPtr) {
        m_objTexPtr->setname(m_name);
        m_objTexPtr->settype(m_type);
        m_objTexPtr->setwidth(m_width);
        m_objTexPtr->setheight(m_height);
        m_objTexPtr->setinformate(m_informate);
        m_objTexPtr->setdataformate(m_dataformate);
        m_objTexPtr->setEnableMipMap(m_bEnableMipMap);
        _updateData();
        m_objTexPtr->create(_renderer);
    }
}

void SVTexture::init(cptr8 _name, s32 _type, s32 _width, s32 _height, s32 _informate, s32 _dateformate, bool _enableMipMap){
    m_name = _name;
    m_width = _width;
    m_height = _height;
    m_type = _type;
    m_informate = _informate;
    m_dataformate = _dateformate;
    m_bEnableMipMap = _enableMipMap;
}

void SVTexture::destroy(SVRendererBasePtr _renderer){
    if (m_objTexPtr) {
        m_objTexPtr->destroy(_renderer);
    }
    SVRObjBase::destroy(_renderer);
}

void SVTexture::setTexData(void *_data, s32 _len){
    if (_data && _len > 0) {
        m_bData = true;
        m_pData->writeData(_data, _len);
        _updateData();
    }
    
}

void SVTexture::commit(){
    if (m_objTexPtr) {
        m_objTexPtr->commit();
    }
}

u32  SVTexture::getTexID(){
    if (m_objTexPtr) {
        return m_objTexPtr->getTexID();
    }
    return 0;
    
}

bool SVTexture::getbLoad(){
    if (m_objTexPtr) {
        return m_objTexPtr->getbLoad();
    }
    return 0;
}

u32 SVTexture::getuid(){
   
    return m_uid;
}

cptr8 SVTexture::getname(){
    return m_name.c_str();
}

s32 SVTexture::gettype(){
    if (m_objTexPtr) {
        return m_objTexPtr->gettype();
    }
    return 0;
}

s32 SVTexture::getwidth(){
    if (m_objTexPtr) {
        return m_objTexPtr->getwidth();
    }
    return m_width;
}

s32 SVTexture::getheight(){
    if (m_objTexPtr) {
        return m_objTexPtr->getheight();
    }
    return m_height;
}

s32 SVTexture::getinformate(){
    if (m_objTexPtr) {
        return m_objTexPtr->getinformate();
    }
    return 0;
}

s32 SVTexture::getdataformate(){
    if (m_objTexPtr) {
        return m_objTexPtr->getdataformate();
    }
    return 0;
}

void SVTexture::_updateData(){
    if (m_objTexPtr) {
        if (m_bData) {
            m_objTexPtr->setTexData(m_pData->getData(), m_pData->getSize());
            m_bData = false;
        }
    }
}

SVResTexPtr SVTexture::getResTex(){
    return m_objTexPtr;
}
//
SVTextureInputTexID::SVTextureInputTexID(SVInst *_app, s32 _texID):SVTexture(_app){
    m_uid = mApp->m_IDPool.applyUID();
    m_name = "";
    m_bData = false;
    m_width = 1;
    m_height = 1;
    m_type = GL_TEXTURE_2D;
    m_informate = GL_RGBA;
    m_dataformate = GL_RGBA;
    m_texID = _texID;
}

SVTextureInputTexID::~SVTextureInputTexID(){
    mApp->m_IDPool.returnUID(m_uid);
    m_pData = nullptr;
    m_texID = 0;
}

void SVTextureInputTexID::init(cptr8 _name, s32 _type, s32 _width, s32 _height, s32 _informate, s32 _dateformate, bool _enableMipMap){
    m_name = _name;
    m_width = _width;
    m_height = _height;
    m_type = _type;
    m_informate = _informate;
    m_dataformate = _dateformate;
}

void SVTextureInputTexID::create(SVRendererBasePtr _renderer){
    SVRObjBase::create(_renderer);
    SVRendererBasePtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        m_objTexPtr = MakeSharedPtr<SVRResGLTexWithTexID>(mApp, m_texID);
        
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
    if (m_objTexPtr) {
        SVRResGLTexWithTexIDPtr t_tmp = std::dynamic_pointer_cast<SVRResGLTexWithTexID>(m_objTexPtr);
        t_tmp->setname(m_name);
        t_tmp->settype(m_type);
        t_tmp->setwidth(m_width);
        t_tmp->setheight(m_height);
        t_tmp->setinformate(m_informate);
        t_tmp->setdataformate(m_dataformate);
        m_objTexPtr->create(_renderer);
    }
}

void SVTextureInputTexID::destroy(SVRendererBasePtr _renderer){
    if (m_objTexPtr) {
        m_objTexPtr->destroy(_renderer);
    }
    SVRObjBase::destroy(_renderer);
}
