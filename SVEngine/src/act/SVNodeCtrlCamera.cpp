//
// SVNodeCtrlCamera.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVNodeCtrlCamera.h"
#include "../node/SVCameraNode.h"

SVNodeCtrlCamera::SVNodeCtrlCamera(SVInst* _app)
:SVNodeCtrl(_app) {
    
}

SVNodeCtrlCamera::~SVNodeCtrlCamera() {
}

//相机控制
void SVNodeCtrlCamera::run(SVCameraNodePtr _nodePtr, f32 dt) {
    //
    
}

//平移
void SVNodeCtrlCamera::move(FVec3 _off) {
    
}

//角度旋转
void SVNodeCtrlCamera::angle(f32 _yaw,f32 _pitch) {
    
}

//推拉
void SVNodeCtrlCamera::zoom(f32 _dis) {
    
}
