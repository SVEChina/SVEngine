//
// SVRResMetalTex.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVResMetalTex.h"
#include "SVRendererMetal.h"
#include "../../app/SVInst.h"
#include "../../mtl/SVMtlDef.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../base/SVDataChunk.h"
#include "../../file/SVFileMgr.h"
#include "../../base/SVDataSwap.h"
#include "../../base/SVLock.h"
#include "../../rendercore/SVRenderMgr.h"
#include "../SVRendererBase.h"

//tex资源
SVRResMetalTex::SVRResMetalTex(SVInst* _app)
:SVResTex(_app)
,m_srcTex(nullptr){
    
}

SVRResMetalTex::~SVRResMetalTex(){
}

void SVRResMetalTex:: create(SVRendererBasePtr _renderer) {
    SVResTex::create(_renderer);
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
    if (t_rendeMetalPtr) {
        MTLTextureDescriptor *dsp = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:m_width height:m_height mipmapped:m_enableMipMap];
        m_srcTex = [t_rendeMetalPtr->m_pDevice newTextureWithDescriptor:dsp]; // 创建纹理
        MTLRegion region;
        region.origin.x = 0;
        region.origin.y = 0;
        region.origin.z = 0;
        region.size.width = m_width;
        region.size.height = m_height;
        region.size.depth = 1;
        if(m_pData) {
            m_bLoad = true;
            m_pData->lockData();
            [m_srcTex replaceRegion:region mipmapLevel:0 withBytes:m_pData->getData()
                        bytesPerRow:4*m_width];
            m_pData->unlockData();
            m_pData = nullptr;
        }
    }
    ////        if( m_id == 0 ){
    ////            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    ////            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ////            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ////            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    ////            if (m_enableMipMap) {
    ////                glGenerateMipmap(GL_TEXTURE_2D);
    ////                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    ////                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    ////            }
    ////        }
}

void SVRResMetalTex::destroy(SVRendererBasePtr _renderer) {
    SVResTex::destroy(_renderer);
    if(m_id>0){
        //        glDeleteTextures(1, &m_id);
        //        m_id = 0;
    }
}

void SVRResMetalTex::commit() {
    m_texLock->lock();
    //    if (m_pData) {
    //        if(m_bLoad){
    //            //更新数据
    //            m_pData->lockData();
    //            glBindTexture(GL_TEXTURE_2D, m_id);
    //            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_informate,GL_UNSIGNED_BYTE,m_pData->getData());
    //            m_pData->unlockData();
    //            m_pData = nullptr;
    //        }
    //    }
    m_texLock->unlock();
}

void SVRResMetalTex::setTexData(void *_data, s32 _len){
    m_texLock->lock();
    //    if( _data && _len>0 ) {
    //        SVDataSwapPtr t_pDataSwap = MakeSharedPtr<SVDataSwap>();
    //        t_pDataSwap->writeData(_data, _len);
    //        setData(t_pDataSwap);
    //    }
    m_texLock->unlock();
}


/*
 metal texture
 */

u32 SVRResMetalTex::getTexID(){
    return m_id;
}

u32 SVRResMetalTex::getuid(){
    return m_uid;
}

bool SVRResMetalTex::getbLoad() {
    return m_bLoad;
}


/*
 metal shader
 */

SVRResMetalShader::SVRResMetalShader(SVInst* _app)
:SVResShader(_app){
}

SVRResMetalShader::~SVRResMetalShader() {
}

void SVRResMetalShader::create(SVRendererBasePtr _renderer) {
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
    if (t_rendeMetalPtr && t_rendeMetalPtr->m_pLibrary) {
        NSString* t_vsname = [NSString stringWithFormat:@"%s",m_vs_fname.c_str()];
        NSString* t_fsname = [NSString stringWithFormat:@"%s",m_fs_fname.c_str()];
        m_pVS = [t_rendeMetalPtr->m_pLibrary newFunctionWithName:t_vsname];
        m_pFS = [t_rendeMetalPtr->m_pLibrary newFunctionWithName:t_fsname];
        MTLRenderPipelineDescriptor *t_dsp = [[MTLRenderPipelineDescriptor alloc] init];
        t_dsp.vertexFunction = m_pVS;
        t_dsp.fragmentFunction = m_pFS;
        //t_dsp.colorAttachments[0].pixelFormat = ;
        //t_dsp.colorAttachments[0].pixelFormat = self.mtkView.colorPixelFormat;
        m_pProgram = [t_rendeMetalPtr->m_pDevice newRenderPipelineStateWithDescriptor:t_dsp error:nullptr];
    }
    //创建图形渲染管道，耗性能操作不宜频繁调用
    //self.commandQueue = [self.mtkView.device newCommandQueue]; // CommandQueue是渲染指令队列，保证渲染指令有序地提交到GPU
}

void SVRResMetalShader::destroy(SVRendererBasePtr _renderer) {
    //
}

bool SVRResMetalShader::active(SVRendererBasePtr _render) {
    return false;
}
