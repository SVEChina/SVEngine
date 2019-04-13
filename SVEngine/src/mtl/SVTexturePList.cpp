//
// SVTexturePList.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVTexturePList.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderMgr.h"
#include "../base/SVLock.h"
#include "../rendercore/SVGL/SVRendererGL.h"
#include "../rendercore/SVGL/SVRResGL.h"
#include "../rendercore/SVMetal/SVRendererMetal.h"
#include "../rendercore/SVVulkan/SVRendererVK.h"

SVTexturePList::SVTexturePList(SVInst *_app)
: SVTexture(_app) {
    m_rot = false;
    m_trim = false;
    m_srcw = 1;
    m_srch = 1;
    m_bApply = false;
    m_bApplyData = false;
    m_texset = nullptr;
    m_btexSet = false;
}

SVTexturePList::~SVTexturePList() {
    m_texset = nullptr;
}

void SVTexturePList::init(cptr8 _name, s32 _type, s32 _width, s32 _height, s32 _informate, s32 _dateformate){
    m_name = _name;
    m_width = _width;
    m_height = _height;
    m_type = _type;
    m_informate = _informate;
    m_dataformate = _dateformate;
}


void SVTexturePList::create(SVRendererBasePtr _renderer){
    SVRObjBase::create(_renderer);
    SVRendererBasePtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        m_objTexPtr = MakeSharedPtr<SVRResGLTexPlist>(mApp);
        
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
}

void SVTexturePList::destroy(SVRendererBasePtr _renderer){
    SVRObjBase::destroy(_renderer);
}

void SVTexturePList::refreshParam(){
    SVRResGLTexPlistPtr t_tmp = std::dynamic_pointer_cast<SVRResGLTexPlist>(m_objTexPtr);
    if (t_tmp) {
        t_tmp->refreshParam();
    }
}

void SVTexturePList::apply(){
    m_bApply = true;
    _updateData();
}

void SVTexturePList::apply(void *data){
//    m_pApplyData->writeData(data, m_srcw*m_srch*4);
//    m_bApplyData = true;
    m_bApply = true;
    _updateData();
}

void SVTexturePList::commit(){
    SVRResGLTexPlistPtr t_tmp = std::dynamic_pointer_cast<SVRResGLTexPlist>(m_objTexPtr);
    if (t_tmp) {
        t_tmp->commit();
    }
}

SVRect *SVTexturePList::getSrcRect() {
    SVRResGLTexPlistPtr t_tmp = std::dynamic_pointer_cast<SVRResGLTexPlist>(m_objTexPtr);
    if (t_tmp) {
        return t_tmp->getSrcRect();
    }
    return nullptr;
}

SVRect *SVTexturePList::getDstRect() {
    SVRResGLTexPlistPtr t_tmp = std::dynamic_pointer_cast<SVRResGLTexPlist>(m_objTexPtr);
    if (t_tmp) {
        return t_tmp->getDstRect();
    }
    return nullptr;
}

void SVTexturePList::bindTexset(SVTextureSetPtr _texset){
    if (_texset) {
        m_btexSet = true;
        m_texset = _texset;
        _updateData();
    }
    
}

bool SVTexturePList::getbLoad(){
    SVRResGLTexPlistPtr t_tmp = std::dynamic_pointer_cast<SVRResGLTexPlist>(m_objTexPtr);
    if (t_tmp) {
        return t_tmp->getbLoad();
    }
    return SVTexture::getbLoad();
}

void SVTexturePList::setRot(bool _rot){
    m_rot = _rot;
}

void SVTexturePList::setTrim(bool _trim){
    m_trim = _trim;
}

void SVTexturePList::setSrcw(bool _srcw){
    m_srcw = _srcw;
}

void SVTexturePList::setSrch(bool _srch){
    m_srch = _srch;
}

void SVTexturePList::_updateData(){
    SVTexture::_updateData();
    SVRResGLTexPlistPtr t_tmp = std::dynamic_pointer_cast<SVRResGLTexPlist>(m_objTexPtr);
    if (t_tmp) {
        t_tmp->rot = m_rot;
        t_tmp->trim = m_trim;
        t_tmp->srcw = m_srcw;
        t_tmp->srch = m_srch;
        if (m_bApply) {
//            t_tmp->apply(m_pApplyData->getData());
            m_bApply = false;
        }
        if (m_bApplyData) {
            m_bApplyData = false;
            t_tmp->apply();
        }
        if (m_btexSet) {
            m_btexSet = false;
            SVRObjBasePtr t_tmp_tex = m_texset->getResTex();
            SVRResGLTexSetPtr t_tmp_texset = std::dynamic_pointer_cast<SVRResGLTexSet>(m_objTexPtr);
            if (t_tmp && t_tmp_texset) {
                t_tmp->bindTexset(t_tmp_texset);
            }
        }
        
    }
}


//
SVTextureSet::SVTextureSet(SVInst *_app)
: SVTexture(_app) {
    
    
}

SVTextureSet::~SVTextureSet() {
    
}

void SVTextureSet::init(cptr8 _name, s32 _type, s32 _width, s32 _height, s32 _informate, s32 _dateformate){
    m_name = _name;
    m_width = _width;
    m_height = _height;
    m_type = _type;
    m_informate = _informate;
    m_dataformate = _dateformate;
}

void SVTextureSet::create(SVRendererBasePtr _renderer){
    SVRendererBasePtr t_renderBasePtr = mApp->getRenderer();
    SVRendererGLPtr t_renderGLPtr = std::dynamic_pointer_cast<SVRendererGL>(t_renderBasePtr);
    if (t_renderGLPtr) {
        //渲染器类型E_RENDERER_GLES,
        m_objTexPtr = MakeSharedPtr<SVRResGLTexSet>(mApp);
        
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
}

void SVTextureSet::destroy(SVRendererBasePtr _renderer){
    SVRObjBase::destroy(_renderer);
}










