//
// SVUIMovieNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVUIMovieNode.h"

//
SVUIMovieNode::SVUIMovieNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVUIMovieNode";
    m_rsType = RST_PREFILTER;
}

SVUIMovieNode::~SVUIMovieNode() {
}

void SVUIMovieNode::create(s32 _w,s32 _h) {
}

void SVUIMovieNode::destroy() {
}

void SVUIMovieNode::update(f32 dt) {
    SVNode::update(dt);
}

void SVUIMovieNode::render() {
    SVNode::render();
}
