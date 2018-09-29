//
// SVRendererVK.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRendererVK.h"
#include "../../app/SVInst.h"
#include "../../base/SVDataSwap.h"

SVRendererVK::SVRendererVK(SVInst* _app)
:SVRendererBase(_app){
    
}

SVRendererVK::~SVRendererVK(){
}

//纹理资源
u32 SVRendererVK::applyTexID(){
    return 0;
}

void SVRendererVK::returnTexID(u32 _id){
    
}

//VertexBuf
u32 SVRendererVK::applyVBO(){
    return 0;
}


void SVRendererVK::returnVBO(u32 _id){
    
}

//shader-vs
u32 SVRendererVK::applyVS(){
    return 0;
}

void SVRendererVK::returnVS(u32 _id){
    
}

//shader-fs
u32 SVRendererVK::applyFS(){
    return 0;
}

void SVRendererVK::returnFS(u32 _id){
    
}

//shader-programme
u32 SVRendererVK::applyProgramme(){
    return 0;
}

void SVRendererVK::returnProgramme(u32 _id){
    
}
