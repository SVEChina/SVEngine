//
// SVUIMoveNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVUIMoveNode.h"

//
SVUIMoveNode::SVUIMoveNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVUIMoveNode";
    m_rsType = RST_PREFILTER;
}

SVUIMoveNode::~SVUIMoveNode() {
}

void SVUIMoveNode::create(s32 _w,s32 _h) {
}

void SVUIMoveNode::destroy() {
}

void SVUIMoveNode::update(f32 dt) {
    SVNode::update(dt);
}

void SVUIMoveNode::render() {
    SVNode::render();
}
