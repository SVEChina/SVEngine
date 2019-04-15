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
SVRResMetalBuf
 */

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

/*
SVResMetalRenderMesh
 */

SVResMetalRenderMesh::SVResMetalRenderMesh(SVInst* _app)
:SVRResMetalBuf(_app){
    
}

SVResMetalRenderMesh::~SVResMetalRenderMesh() {
    
}

void SVResMetalRenderMesh::create(SVRendererBasePtr _renderer) {
    
}

void SVResMetalRenderMesh::destroy(SVRendererBasePtr _renderer) {
}

void SVResMetalRenderMesh::render() {
//    id<MTLRenderCommandEncoder> encoder = [commandBuffer renderCommandEncoderWithDescriptor:renderDes];
//    [encoder setCullMode:MTLCullModeNone];
//    [encoder setFrontFacingWinding:MTLWindingCounterClockwise];
//    [encoder setRenderPipelineState:self.pipelineState];
//    [encoder setVertexBuffer:self.vertexBuffer offset:0 atIndex:0];
//    [encoder setFragmentTexture:textture atIndex:0];
//
//    //set render vertex
//    [encoder drawPrimitives:MTLPrimitiveTypeTriangle
//                vertexStart:0
//                vertexCount:3];
//    [encoder endEncoding];
}
