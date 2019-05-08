//
// SVMorphNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMorphNode.h"
#include "SVCameraNode.h"
#include "SVScene.h"

SVMorphNode::SVMorphNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVMorphNode";
}

SVMorphNode::~SVMorphNode() {
}
