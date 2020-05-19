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
#include "../SVRendererBase.h"

/*
SVResMetalBuf
 */

SVResMetalBuf::SVResMetalBuf(SVInst* _app)
:SVResVBO(_app){
    m_pVerBuf = nullptr;
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
    m_indexNum = 0;
    m_pointNum = 0;
    m_vftype = E_VF_BASE;
    m_drawmethod = E_DM_TRIANGLES;
    m_pDataVertex = nullptr;
    m_pDataIndex = nullptr;
    m_pDataV2 = nullptr;
    m_pDataV3 = nullptr;
    m_pDataC0 = nullptr;
    m_pDataC1 = nullptr;
    m_pDataT0 = nullptr;
    m_pDataT1 = nullptr;
    m_pDataT2 = nullptr;
    m_pDataNor = nullptr;
    m_pDataTag = nullptr;
    m_pDataBTor = nullptr;
    
    m_pIndexBuf = nullptr;
    m_pV2Buf = nullptr;
    m_pV3Buf = nullptr;
    m_pC0Buf = nullptr;
    m_pC1Buf = nullptr;
    m_pT0Buf = nullptr;
    m_pT1Buf = nullptr;
    m_pT2Buf = nullptr;
    m_pNorBuf = nullptr;
    m_pTagBuf = nullptr;
    m_pBTorBuf = nullptr;
}

SVResMetalRenderMesh::~SVResMetalRenderMesh() {
    
}

void SVResMetalRenderMesh::create(SVRendererBasePtr _renderer) {
    SVRendererMetalPtr t_rendeMetalPtr = std::dynamic_pointer_cast<SVRendererMetal>(_renderer);
    if (t_rendeMetalPtr) {
        if(m_pDataVertex) {
            m_pDataVertex->lockData();
            m_pVerBuf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataVertex->getData()
                                                                length:m_pDataVertex->getSize()
                                                               options:0];
            m_pDataVertex->unlockData();
            m_pDataVertex = nullptr;
        }else if(m_pDataIndex) {
            m_pDataIndex->lockData();
            m_pIndexBuf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataIndex->getData()
                                                                length:m_pDataIndex->getSize()
                                                               options:0];
            m_pDataIndex->unlockData();
            m_pDataIndex = nullptr;
        }else if(m_pDataV2) {
            m_pDataV2->lockData();
            m_pV2Buf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataV2->getData()
                                                                length:m_pDataV2->getSize()
                                                               options:0];
            m_pDataV2->unlockData();
            m_pDataV2 = nullptr;
        }else if(m_pDataV3) {
            m_pDataV3->lockData();
            m_pV3Buf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataV3->getData()
                                                                length:m_pDataV3->getSize()
                                                               options:0];
            m_pDataV3->unlockData();
            m_pDataV3 = nullptr;
        }else if(m_pDataC0) {
            m_pDataC0->lockData();
            m_pC0Buf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataC0->getData()
                                                                length:m_pDataC0->getSize()
                                                               options:0];
            m_pDataC0->unlockData();
            m_pDataC0 = nullptr;
        }else if(m_pDataC1) {
            m_pDataC1->lockData();
            m_pC1Buf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataC1->getData()
                                                                length:m_pDataC1->getSize()
                                                               options:0];
            m_pDataC1->unlockData();
            m_pDataC1 = nullptr;
        }else if(m_pDataT0) {
            m_pDataT0->lockData();
            m_pT0Buf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataT0->getData()
                                                                length:m_pDataT0->getSize()
                                                               options:0];
            m_pDataT0->unlockData();
            m_pDataT0 = nullptr;
        }else if(m_pDataT1) {
            m_pDataT1->lockData();
            m_pT1Buf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataT1->getData()
                                                                length:m_pDataT1->getSize()
                                                               options:0];
            m_pDataT1->unlockData();
            m_pDataT1 = nullptr;
        }else if(m_pDataT2) {
            m_pDataT2->lockData();
            m_pT2Buf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataT2->getData()
                                                                length:m_pDataT2->getSize()
                                                               options:0];
            m_pDataT2->unlockData();
            m_pDataT2 = nullptr;
        }else if(m_pDataNor) {
            m_pDataNor->lockData();
            m_pNorBuf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataNor->getData()
                                                                length:m_pDataNor->getSize()
                                                               options:0];
            m_pDataNor->unlockData();
            m_pDataNor = nullptr;
        }else if(m_pDataTag) {
            m_pDataTag->lockData();
            m_pTagBuf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataTag->getData()
                                                                length:m_pDataTag->getSize()
                                                               options:0];
            m_pDataTag->unlockData();
            m_pDataTag = nullptr;
        }else if(m_pDataBTor) {
            m_pDataBTor->lockData();
            m_pBTorBuf = [t_rendeMetalPtr->m_pDevice newBufferWithBytes:m_pDataBTor->getData()
                                                                length:m_pDataBTor->getSize()
                                                               options:0];
            m_pDataBTor->unlockData();
            m_pDataBTor = nullptr;
        }
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
        //设置顶点
        s32 t_offset = 0;
        s32 t_index = 0;
        [t_rendeMetalPtr->m_pCurEncoder setVertexBuffer:m_pVerBuf offset:t_offset atIndex:t_index];
        //
        if(1){
            //混合流
        }else{
            //单一流
        }

        //
        if(m_indexNum>0) {
            if( E_DM_TRIANGLES == m_drawmethod) {
                //        - (void)drawIndexedPrimitives:(MTLPrimitiveType)primitiveType indexCount:(NSUInteger)indexCount indexType:(MTLIndexType)indexType indexBuffer:(id <MTLBuffer>)indexBuffer indexBufferOffset:(NSUInteger)indexBufferOffset;
                
//                [t_rendeMetalPtr->m_pCurEncoder drawPrimitives:MTLPrimitiveTypeTriangle
//                                                   vertexStart:0
//                                                   vertexCount:m_pointNum];
            }else if( E_DM_TRIANGLE_STRIP == m_drawmethod) {
//                [t_rendeMetalPtr->m_pCurEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip
//                                                   vertexStart:0
//                                                   vertexCount:m_pointNum];
            }else if( E_DM_LINES == m_drawmethod) {
//                [t_rendeMetalPtr->m_pCurEncoder drawPrimitives:MTLPrimitiveTypeLine
//                                                   vertexStart:0
//                                                   vertexCount:m_pointNum];
            }else if( E_DM_LINE_STRIP == m_drawmethod) {
//                [t_rendeMetalPtr->m_pCurEncoder drawPrimitives:MTLPrimitiveTypeLineStrip
//                                                   vertexStart:0
//                                                   vertexCount:m_pointNum];
            }else if( E_DM_POINTS == m_drawmethod) {
//                [t_rendeMetalPtr->m_pCurEncoder drawPrimitives:MTLPrimitiveTypePoint
//                                                   vertexStart:0
//                                                   vertexCount:m_pointNum];
            }
        }else{
            //传递数据
            //
            if( E_DM_TRIANGLES == m_drawmethod) {
                [t_rendeMetalPtr->m_pCurEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                                                   vertexStart:0
                                                   vertexCount:m_pointNum];
            }else if( E_DM_TRIANGLE_STRIP == m_drawmethod) {
                [t_rendeMetalPtr->m_pCurEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip
                                                   vertexStart:0
                                                   vertexCount:m_pointNum];
            }else if( E_DM_LINES == m_drawmethod) {
                [t_rendeMetalPtr->m_pCurEncoder drawPrimitives:MTLPrimitiveTypeLine
                                                   vertexStart:0
                                                   vertexCount:m_pointNum];
            }else if( E_DM_LINE_STRIP == m_drawmethod) {
                [t_rendeMetalPtr->m_pCurEncoder drawPrimitives:MTLPrimitiveTypeLineStrip
                                                   vertexStart:0
                                                   vertexCount:m_pointNum];
            }else if( E_DM_POINTS == m_drawmethod) {
                [t_rendeMetalPtr->m_pCurEncoder drawPrimitives:MTLPrimitiveTypePoint
                                                   vertexStart:0
                                                   vertexCount:m_pointNum];
            }
        }
    }
//    id<MTLRenderCommandEncoder> encoder = [commandBuffer renderCommandEncoderWithDescriptor:renderDes];
//    [encoder setCullMode:MTLCullModeNone];
//    [encoder setFrontFacingWinding:MTLWindingCounterClockwise];
//    [encoder setRenderPipelineState:self.pipelineState];
//    [encoder setFragmentTexture:textture atIndex:0];
//    //set render vertex
//    [encoder drawPrimitives:MTLPrimitiveTypeTriangle
//                vertexStart:0
//                vertexCount:3];
//    [encoder endEncoding];
}

void SVResMetalRenderMesh::updateConf(RENDERMESHCONF& _conf) {
    m_drawmethod = _conf.drawmethod;
    m_vftype = _conf.vftype;
    m_seqMode = _conf.seqMode;
}

void SVResMetalRenderMesh::updateData(RENDERMESHDATA& _data) {
    m_indexNum = _data.indexNum;
    m_pointNum = _data.pointNum;
    m_pDataVertex = _data.pDataVertex;
    m_pDataIndex = _data.pDataIndex;
    m_pDataV2 = _data.pDataV2;
    m_pDataV3 = _data.pDataV3;
    m_pDataC0 = _data.pDataC0;
    m_pDataC1 = _data.pDataC1;
    m_pDataT0 = _data.pDataT0;
    m_pDataT1 = _data.pDataT1;
    m_pDataT2 = _data.pDataT2;
    m_pDataNor = _data.pDataNor;
    m_pDataTag = _data.pDataTag;
    m_pDataBTor = _data.pDataBTor;
}
