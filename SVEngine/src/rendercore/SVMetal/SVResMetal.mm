//
// SVRResMetal.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVResMetal.h"


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

////render des
//MTLRenderPassDescriptor *renderDes = [MTLRenderPassDescriptor new];
//renderDes.colorAttachments[0].texture = drawable.texture;
//renderDes.colorAttachments[0].loadAction = MTLLoadActionClear;
//renderDes.colorAttachments[0].storeAction = MTLStoreActionStore;
//renderDes.colorAttachments[0].clearColor = MTLClearColorMake(0.5, 0.65, 0.8, 1); //background color
//
//
////command encoder
//id<MTLRenderCommandEncoder> encoder = [commandBuffer renderCommandEncoderWithDescriptor:renderDes];
//[encoder setCullMode:MTLCullModeNone];
//[encoder setFrontFacingWinding:MTLWindingCounterClockwise];
//[encoder setRenderPipelineState:self.pipelineState];
//[encoder setVertexBuffer:self.vertexBuffer offset:0 atIndex:0];
//[encoder setFragmentTexture:textture atIndex:0];
//
////set render vertex
//[encoder drawPrimitives:MTLPrimitiveTypeTriangle
//            vertexStart:0
//            vertexCount:3];
//
//[encoder endEncoding];
