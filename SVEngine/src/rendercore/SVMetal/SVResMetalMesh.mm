//
// SVRResMetal.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVResMetal.h"
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

/*
SVResMetalBuf
 */

SVResMetalBuf::SVResMetalBuf(SVInst* _app)
:SVResVBO(_app){
}

SVResMetalBuf::~SVResMetalBuf() {
}

void SVResMetalBuf::create(SVRendererBasePtr _renderer) {
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
    if (t_rendeMetalPtr && m_pData) {
        m_pData->lockData();
        m_pVerBuf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pData->getData() length:m_pData->getSize() options:0];
        m_pData->unlockData();
        m_pData = nullptr;
    }
}

void SVResMetalBuf::destroy(SVRendererBasePtr _renderer) {
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
    if (t_rendeMetalPtr) {
        
    }
}

/*
SVResMetalRenderMesh
 */

SVResMetalRenderMesh::SVResMetalRenderMesh(SVInst* _app)
:SVResMetalBuf(_app){
    
}

SVResMetalRenderMesh::~SVResMetalRenderMesh() {
    
}

void SVResMetalRenderMesh::create(SVRendererBasePtr _renderer) {
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
    if (t_rendeMetalPtr) {
        m_pData->lockData();
        m_pVerBuf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pData->getData() length:m_pData->getSize() options:0];
        m_pData->unlockData();
        m_pData = nullptr;
    }
}

void SVResMetalRenderMesh::destroy(SVRendererBasePtr _renderer) {
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
    if (t_rendeMetalPtr) {
        
    }
}

void SVResMetalRenderMesh::render(SVRendererBasePtr _renderer) {
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
    if (t_rendeMetalPtr) {
        
    }
//    id<MTLRenderCommandEncoder> encoder = [commandBuffer renderCommandEncoderWithDescriptor:renderDes];
//    [encoder setCullMode:MTLCullModeNone];
//    [encoder setFrontFacingWinding:MTLWindingCounterClockwise];
//    [encoder setRenderPipelineState:self.pipelineState];
//    [encoder setVertexBuffer:self.vertexBuffer offset:0 atIndex:0];
//    [encoder setFragmentTexture:textture atIndex:0];
//    //set render vertex
//    [encoder drawPrimitives:MTLPrimitiveTypeTriangle
//                vertexStart:0
//                vertexCount:3];
//    [encoder endEncoding];
}

void SVResMetalRenderMesh::updateConf(RENDERMESHCONF& _conf) {
//    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
//    if (t_rendeMetalPtr) {
//
//    }
}

void SVResMetalRenderMesh::updateData(RENDERMESHDATA& _data) {
//    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
//    if (t_rendeMetalPtr) {
//        
//    }
}
