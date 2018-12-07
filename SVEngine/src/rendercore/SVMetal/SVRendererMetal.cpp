//
// SVRendererMetal.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRendererMetal.h"
#include "../../app/SVInst.h"
#include "../../base/SVDataSwap.h"

SVRendererMetal::SVRendererMetal(SVInst* _app)
:SVRendererBase(_app){
#ifdef SV_IOS
    m_pMetalDevice = [[SVMetalDevice alloc] init];
#endif
}

SVRendererMetal::~SVRendererMetal(){
#ifdef SV_IOS
    m_pMetalDevice = nil;
#endif
}

//纹理资源
u32 SVRendererMetal::applyTexID(){
    return 0;
}

void SVRendererMetal::returnTexID(u32 _id){
    
}

//VertexBuf
u32 SVRendererMetal::applyVBO(){
    return 0;
}


void SVRendererMetal::returnVBO(u32 _id){
    
}

//shader-vs
u32 SVRendererMetal::applyVS(){
    return 0;
}

void SVRendererMetal::returnVS(u32 _id){
    
}

//shader-fs
u32 SVRendererMetal::applyFS(){
    return 0;
}

void SVRendererMetal::returnFS(u32 _id){
    
}

//shader-programme
u32 SVRendererMetal::applyProgramme(){
    return 0;
}

void SVRendererMetal::returnProgramme(u32 _id){
    
}
