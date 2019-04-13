//
// SVRResMetal.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRResMetal.h"
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
#include "../renderer/SVContextBase.h"
#include "../renderer/SVRendererBase.h"

//Buf资源
SVRResMetalBuf::SVRResMetalBuf(SVInst* _app)
:SVRObjBase(_app){
}

SVRResMetalBuf::~SVRResMetalBuf() {
}

void SVRResMetalBuf::create(SVRendererBasePtr _renderer) {
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
    if (t_rendeMetalPtr && m_pData) {
        m_pData->lockData();
        m_pVerBuf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pData->getData() length:m_pData->getSize() options:0];
        m_pData->unlockData();
        m_pData = nullptr;
    }
}

void SVRResMetalBuf::destroy(SVRendererBasePtr _renderer) {
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
    if (t_rendeMetalPtr) {
        
    }
}

//Metal RenderMesh

//// 每次渲染都要单独创建一个CommandBuffer
//id<MTLCommandBuffer> commandBuffer = [self.commandQueue commandBuffer];
//MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;
//// MTLRenderPassDescriptor描述一系列attachments的值，类似GL的FrameBuffer；同时也用来创建MTLRenderCommandEncoder
//if(renderPassDescriptor != nil)
//{
//    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.5, 0.5, 1.0f); // 设置默认颜色
//    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor]; //编码绘制指令的Encoder
//    [renderEncoder setViewport:(MTLViewport){0.0, 0.0, self.viewportSize.x, self.viewportSize.y, -1.0, 1.0 }]; // 设置显示区域
//    [renderEncoder setRenderPipelineState:self.pipelineState]; // 设置渲染管道，以保证顶点和片元两个shader会被调用
//
//    [renderEncoder setVertexBuffer:self.vertices
//                            offset:0
//                           atIndex:0]; // 设置顶点缓存
//
//    [renderEncoder setFragmentTexture:self.texture
//                              atIndex:0]; // 设置纹理
//
//    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
//                      vertexStart:0
//                      vertexCount:self.numVertices]; // 绘制
//
//    [renderEncoder endEncoding]; // 结束
//
//    [commandBuffer presentDrawable:view.currentDrawable]; // 显示
//}
//
//[commandBuffer commit]; // 提交；


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
