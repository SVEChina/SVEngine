//
// SVRResGL.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRResGL.h"
#include "../../third/rapidjson/document.h"
#include "../../app/SVInst.h"
#include "../../base/SVPreDeclare.h"
#include "../../base/SVDataSwap.h"
#include "../../work/SVTdCore.h"
#include "../../base/SVDataChunk.h"
#include "../../mtl/SVMtlDef.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../file/SVFileMgr.h"
#include "../../rendercore/SVRenderMgr.h"
#include "../SVRenderer.h"
#include "../SVGL/SVRendererGL.h"

SVRResGLTex::SVRResGLTex(SVInst* _app)
        :SVResTex(_app){
    m_bLoad = false;
    m_enableMipMap = false;
    m_name = "";
    m_id = 0;
    m_width = 1;
    m_height = 1;
    m_type = GL_TEXTURE_2D;
    m_informate = GL_RGBA;
    m_dataformate = GL_RGBA;
}

SVRResGLTex::~SVRResGLTex(){
    SV_LOG_INFO("SVRResGLTex destroy %d ",m_uid);
}

void SVRResGLTex:: create(SVRendererPtr _renderer) {
    SVRObjBase::create(_renderer);
    SV_LOG_INFO("SVRResGLTex create %d ",m_uid);
    if( m_id == 0 ){
        m_bLoad = true;
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        if(m_pData){
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         m_informate,
                         m_width,
                         m_height,
                         0,
                         m_dataformate,
                         GL_UNSIGNED_BYTE,
                         m_pData->getData());
            
        }else{
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             m_informate,
                             m_width,
                             m_height,
                             0,
                             m_dataformate,
                             GL_UNSIGNED_BYTE,
                             nullptr);
        }
        m_pData = nullptr;
        if (m_enableMipMap) {
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            
        }else{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

void SVRResGLTex::destroy(SVRendererPtr _renderer) {
    SVRObjBase::destroy(_renderer);
    if(m_id>0){
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }
}

void SVRResGLTex::commit() {
    m_texLock->lock();
    if (m_pData && m_bLoad) {
        //更新数据
        s32 bpp = 1;
        if ( m_informate == GL_RGBA ){
            bpp = 4;
        }else if ( m_informate == GL_RGB ){
            bpp = 3;
        }else if ( m_informate == GL_LUMINANCE_ALPHA ){
            bpp = 2;
        }
        s32 t_dataLen = m_width*m_height*bpp;
        if (m_pData->getSize() == t_dataLen) {
            m_pData->lockData();
            glBindTexture(GL_TEXTURE_2D, m_id);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_informate,GL_UNSIGNED_BYTE,m_pData->getData());
            m_pData->unlockData();
        }else{
            SV_LOG_ERROR("SVRResGLTex: commit pixel data error");
        }
        m_pData = nullptr;
    }
    m_texLock->unlock();
}

void SVRResGLTex::setTexData(void *_data, s32 _len){
    m_texLock->lock();
    if( _data && _len>0 ) {
        SVDataSwapPtr t_pDataSwap = MakeSharedPtr<SVDataSwap>();
        t_pDataSwap->writeData(_data, _len);
        setData(t_pDataSwap);
    }
    m_texLock->unlock();
}

//////
SVRResGLTexWithTexID::SVRResGLTexWithTexID(SVInst* _app, s32 _id):SVRResGLTex(_app){
    m_id = _id;
}

SVRResGLTexWithTexID::~SVRResGLTexWithTexID(){
    
}

void SVRResGLTexWithTexID:: create(SVRendererPtr _renderer) {
    SVRObjBase::create(_renderer);
    m_bLoad = true;
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void SVRResGLTexWithTexID::destroy(SVRendererPtr _renderer) {
    SVRObjBase::destroy(_renderer);
    if(m_id>0){
        m_id = 0;
    }
}

void SVRResGLTexWithTexID::setTexID(u32 _texID){
    m_id = _texID;
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

//////
SVRResGLTexPlist::SVRResGLTexPlist(SVInst *mApp)
        : SVRResGLTex(mApp) {
    m_type = 3;
    rot = false;    //是否旋转
    trim = false;   //是否旋转
    srcw = 128;     //原有纹理的宽度
    srch = 128;     //原有纹理的高度
    m_pTexset = nullptr;
}

SVRResGLTexPlist::~SVRResGLTexPlist() {
    unbindTexset();
}

void SVRResGLTexPlist::refreshParam(){
    if(m_pTexset){
        m_id = m_pTexset->getTexID();  //同步纹理ID
        m_width = m_pTexset->getwidth();
        m_height = m_pTexset->getheight();
    }
}

void SVRResGLTexPlist::apply(){
    if(m_pTexset){
        //m_pTexset->apply();
        refreshParam();
    }
}

void SVRResGLTexPlist::apply(void *data){
    if(m_pTexset){
        //m_pTexset->apply(data);
        refreshParam();
    }
}

void SVRResGLTexPlist::commit() {
    if(m_pTexset){
        m_pTexset->commit();
        refreshParam();
    }
}

bool SVRResGLTexPlist::getbLoad(){
    if(m_pTexset){
        return m_pTexset->getbLoad();
    }
    return SVRResGLTex::getbLoad();
}

void SVRResGLTexPlist::bindTexset(SVRResGLTexSetPtr _texset){
    if(m_pTexset!=_texset){
        m_pTexset = _texset;
    }
}

void SVRResGLTexPlist::unbindTexset(){
    if(m_pTexset){
        m_pTexset = nullptr;
    }
}

SVRect *SVRResGLTexPlist::getSrcRect() {
    return &m_srcRect;
}

SVRect *SVRResGLTexPlist::getDstRect() {
    return &m_dstRect;
}

//
SVRResGLTexSet::SVRResGLTexSet(SVInst *mApp)
        : SVRResGLTex(mApp) {
    m_type = 2;
}

SVRResGLTexSet::~SVRResGLTexSet() {
}

SVRResGLTexiOS::SVRResGLTexiOS(SVInst *_app): SVRResGLTex(_app)
{
#ifdef SV_IOS
    m_pTexCacheRef = nullptr;
    m_pTexRef = nullptr;
    m_pPixelBuf = nullptr;
#endif
}

SVRResGLTexiOS::~SVRResGLTexiOS() {
}

void SVRResGLTexiOS::create(SVRendererPtr _renderer){
    if(!_renderer)
        return ;
    SVRObjBase::create(_renderer);
#ifdef SV_IOS
//    SVCtxBasePtr t_contextbase = _renderer->getRenderContext();
//    CVEAGLContext t_glcontext = CVEAGLContext((__bridge id)t_contextbase->getContext());
//    CVReturn t_flag = CVOpenGLESTextureCacheCreate(nullptr,
//                                                   nullptr,
//                                                   t_glcontext,
//                                                   nullptr,
//                                                   &m_pTexCacheRef);
//    if (t_flag) {
//        SV_LOG_ERROR("SVTextureIOS create texture cache failed");
//    }
//    const void *keys[] = {
//        kCVPixelBufferOpenGLESCompatibilityKey,
//        kCVPixelBufferIOSurfacePropertiesKey,
//        kCVPixelBufferBytesPerRowAlignmentKey,
//    };
//    const void *values[] = {
//        (__bridge const void *)([NSNumber numberWithBool:YES]),
//        (__bridge const void *)([NSDictionary dictionary]),
//        (__bridge const void *)([NSNumber numberWithInteger:16])
//    };
//    CFDictionaryRef opDic = CFDictionaryCreate(NULL, keys,
//                                               values, 2,
//                                               NULL, NULL);
//    t_flag = CVPixelBufferCreate(kCFAllocatorDefault,
//                                 m_width,
//                                 m_height,
//                                 kCVPixelFormatType_32BGRA,
//                                 opDic,
//                                 &m_pPixelBuf);
//    if( t_flag == 0) {
//
//    }
//    t_flag = CVOpenGLESTextureCacheCreateTextureFromImage(nullptr,
//                                                          m_pTexCacheRef,
//                                                          m_pPixelBuf,
//                                                          nullptr,
//                                                          GL_TEXTURE_2D,
//                                                          m_informate,
//                                                          m_width,
//                                                          m_height,
//                                                          m_dataformate,
//                                                          GL_UNSIGNED_BYTE,
//                                                          0,
//                                                          &m_pTexRef);
//    if(t_flag == 0){
//        m_id = CVOpenGLESTextureGetName(m_pTexRef);
//        glBindTexture(GL_TEXTURE_2D , m_id);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glBindTexture(GL_TEXTURE_2D, 0);
//    }
//    CVOpenGLESTextureCacheFlush(m_pTexCacheRef,0);
//    CFRelease(opDic);
//    m_bLoad = true;
#endif
}

void SVRResGLTexiOS::destroy(SVRendererPtr _renderer){
#ifdef SV_IOS
    if(m_pPixelBuf){
        CFRelease(m_pPixelBuf);
        m_pPixelBuf = nullptr;
    }
    if(m_pTexRef){
        CFRelease(m_pTexRef);
        m_pTexRef = nullptr;
    }
    if(m_pTexCacheRef){
        CVOpenGLESTextureCacheFlush(m_pTexCacheRef,0);
        CFRelease(m_pTexCacheRef);
        m_pTexCacheRef = nullptr;
    }
#endif
    SVRObjBase::destroy(_renderer);
}

void SVRResGLTexiOS::pushData(u8* _srcPtr,s32 _w,s32 _h,s32 _pixelformate){
#ifdef SV_IOS
    if( (!m_pPixelBuf) || (!m_pTexRef) || (!m_pTexCacheRef) || (_srcPtr == nullptr) )
        return;
    if(CVPixelBufferLockBaseAddress(m_pPixelBuf, 0) == kCVReturnSuccess){
        s32 width = (s32)CVPixelBufferGetWidth(m_pPixelBuf);
        s32 height = (s32)CVPixelBufferGetHeight(m_pPixelBuf);
        s32 bytesPerRow = (s32)CVPixelBufferGetBytesPerRowOfPlane(m_pPixelBuf, 0);
        u8* _dstPtr = (u8*)CVPixelBufferGetBaseAddress(m_pPixelBuf);
        //判断从 CVImageBufferRef 里取出来的数据是否带panding
        if (bytesPerRow/4 - width !=0) {
            //带panding，要处理
            s32 a = 0;
            for (s32 i = 0; i < height; i++) {
                memcpy(_dstPtr +  a, _srcPtr + i * width * 4, width*4);
                a += bytesPerRow;
            }
        }else{
            memcpy(_dstPtr,_srcPtr,_w*_h*4);
        }
        
        CVPixelBufferUnlockBaseAddress(m_pPixelBuf, 0);
    }
#endif
}

void SVRResGLTexiOS::fetchData(u8* _dstPtr,s32 _w,s32 _h) {
#ifdef SV_IOS
    if( (!m_pPixelBuf) || (!m_pTexRef) || (!m_pTexCacheRef) || (_dstPtr == nullptr))
        return;
    if(CVPixelBufferLockBaseAddress(m_pPixelBuf, 0) == kCVReturnSuccess){
        s32 width = (s32)CVPixelBufferGetWidth(m_pPixelBuf);
        s32 height = (s32)CVPixelBufferGetHeight(m_pPixelBuf);
        s32 bytesPerRow = (s32)CVPixelBufferGetBytesPerRowOfPlane(m_pPixelBuf, 0);
        u8* t_srtPtr = (u8*)CVPixelBufferGetBaseAddress(m_pPixelBuf);
        //判断从 CVImageBufferRef 里取出来的数据是否带panding
        if (bytesPerRow/4 - width !=0) {
            //带panding，要处理
            s32 a = 0;
            for (s32 i = 0; i < height; i++) {
                memcpy(_dstPtr + i * width * 4 , t_srtPtr + a, width*4);
                a += bytesPerRow;
            }
        }else{
            memcpy(_dstPtr,t_srtPtr,_w*_h*4);
        }

        CVPixelBufferUnlockBaseAddress(m_pPixelBuf, 0);
    }
#endif
}

//FBO资源
SVRResGLFBO::SVRResGLFBO(SVInst* _app)
:SVResFBO(_app)
,m_fboID(0)
,m_depthID(0)
,m_width(0)
,m_height(0)
,m_depth(false)
,m_stencil(false)
,m_dirty(false){

}
SVRResGLFBO::~SVRResGLFBO(){
}

void SVRResGLFBO::create(SVRendererPtr _renderer){
    SVRObjBase::create(_renderer);
    glGenFramebuffers(1,&m_fboID);
    _bindColor();
    _bindDepth();
    s32 t_error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(t_error){
        case GL_FRAMEBUFFER_COMPLETE:
            SV_LOG_INFO("GL_FRAMEBUFFER_COMPLETE");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            SV_LOG_INFO("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT    :
            SV_LOG_INFO("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            SV_LOG_INFO("GL_FRAMEBUFFER_UNSUPPORTED");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            SV_LOG_INFO("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
            break;
        case GL_FRAMEBUFFER_UNDEFINED :
            SV_LOG_INFO("GL_FRAMEBUFFER_UNDEFINED");
            break;
#if defined SV_OSX
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            SV_LOG_INFO("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            SV_LOG_INFO("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
            break;
#endif
    }
}

void SVRResGLFBO::destroy(SVRendererPtr _renderer){
    if(m_fboID>0) {
        glBindFramebuffer(GL_FRAMEBUFFER,m_fboID);
        if(m_stencil) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,0);
        }
        if(m_depth) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,0);
        }
        glDeleteRenderbuffers(1, &m_depthID);
        glDeleteFramebuffers(1,&m_fboID);
        m_depthID = 0;
        m_fboID = 0;
    }
    SVRObjBase::destroy(_renderer);
}

void SVRResGLFBO::_bindColor() {
}

void SVRResGLFBO::_bindDepth() {
    if(m_depth && m_width>0 && m_height>0) {
        if (m_depthID == 0) {
            glGenRenderbuffers(1, &m_depthID);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthID);
        if( m_stencil ) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthID);
        }
    }else{
        if (m_depthID > 0) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,0);
            glDeleteRenderbuffers(1, &m_depthID);
            m_depthID = 0;
        }
    }
}

void SVRResGLFBO::refresh() {
    _bindColor();
    _bindDepth();
}

void SVRResGLFBO::bind() {
    //保存下当前的fbo
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_lastFboID);
    if(m_dirty){
        m_dirty = false;
        refresh();
    }
    SVRendererPtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        t_renderer->svBindFrameBuffer(m_fboID);
        t_renderer->svPushViewPort(0,0,m_width,m_height);
    }
}

void  SVRResGLFBO::clear(){
    SVRendererPtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        t_renderer->svBindClearColor(m_fboID);
    }
}

void SVRResGLFBO::unbind() {
    SVRendererPtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        t_renderer->svPopViewPort();
        t_renderer->svBindFrameBuffer(m_lastFboID);
    }
    m_lastFboID = 0;
}

//
SVRResGLOutFBO::SVRResGLOutFBO(SVInst *_app,u32 _fboid)
:SVRResGLFBO(_app){
    m_fboID = _fboid;
}

SVRResGLOutFBO::~SVRResGLOutFBO() {
    m_fboID = 0;
}

void SVRResGLOutFBO::create(SVRendererPtr _renderer){
    //外部设置的FBO 直接使用外部的就好
}

void SVRResGLOutFBO::destroy(SVRendererPtr _renderer){
    m_fboID = 0;
}

//RenderTarget
//
SVResGLRenderTarget::SVResGLRenderTarget(SVInst *_app,s32 _w, s32 _h,bool _depth,bool _stencil)
        :SVRResGLFBO(_app){
    m_width = _w;
    m_height = _h;
    m_depth = _depth;
    m_stencil = _stencil;
    m_colorID = 0;
}

SVResGLRenderTarget::~SVResGLRenderTarget() {

}

void SVResGLRenderTarget::create(SVRendererPtr _renderer) {
    SVRResGLFBO::create(_renderer);
    if( m_fboID >0 ) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        glGenRenderbuffers(1, &m_colorID);
        glBindRenderbuffer(GL_RENDERBUFFER, m_colorID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorID);
    }
}

void SVResGLRenderTarget::destroy(SVRendererPtr _renderer) {
    if( m_fboID >0 ) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1,&m_colorID);
        m_colorID = 0;
    }
    SVRResGLFBO::destroy(_renderer);
}
//
SVResGLRenderTargetOut::SVResGLRenderTargetOut(SVInst *_app,s32 _w, s32 _h,u32 _fboid,u32 _colorID)
        :SVResGLRenderTarget(_app,_w,_h,false,false){
    m_fboID = _fboid;
    m_colorID = _colorID;
}

SVResGLRenderTargetOut::~SVResGLRenderTargetOut(){
    m_width = 0;
    m_height = 0;
    m_fboID = 0;
    m_colorID = 0;
}

void SVResGLRenderTargetOut::create(SVRendererPtr _renderer) {
}

void SVResGLRenderTargetOut::destroy(SVRendererPtr _renderer) {
    m_fboID = 0;
    m_colorID = 0;
}
//
//RenderTexture
SVResGLRenderTexture::SVResGLRenderTexture(SVInst *_app,SVRResGLTexPtr _tex, bool _depth,bool _stencil)
:SVRResGLFBO(_app){
    if(_tex){
        m_tex = _tex;
        m_width = m_tex->getwidth();
        m_height = m_tex->getheight();
    }else{
        m_tex = nullptr;
        m_width = 0;
        m_height = 0;
    }
    m_depth = _depth;
    m_stencil = _stencil;
}

SVResGLRenderTexture::~SVResGLRenderTexture() {
}

void SVResGLRenderTexture::create(SVRendererPtr _renderer) {
    SVRResGLFBO::create(_renderer);
    m_dirty = true;
}

void SVResGLRenderTexture::_bindColor() {
    if( m_fboID>0 && m_tex) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        if(m_tex->getinformate()==GL_DEPTH_COMPONENT){
            glDrawBuffers(0, GL_NONE);
            glReadBuffer(GL_NONE); glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,m_tex->getTexID(),0);
        }else{
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_tex->getTexID(),0);
        }
    }
}

void SVResGLRenderTexture::destroy(SVRendererPtr _renderer) {
    if(m_fboID>0) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,0,0);
    }
    m_tex = nullptr;
    SVRResGLFBO::destroy(_renderer);
}

void SVResGLRenderTexture::setTexture(SVRResGLTexPtr _tex) {
    if(m_tex != _tex){
        m_tex = _tex;
        if(m_tex) {
            //正常应该 reCreate才对
            m_width = m_tex->getwidth();
            m_height = m_tex->getheight();
        }
        m_dirty = true;
    }
}

void SVResGLRenderTexture::refresh() {
    SVRResGLFBO::refresh();
}
