//
// SVUIMovieNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVUIMovieNode.h"
#include "../act/SVMovie.h"
#include "../act/SVDragma.h"
#include "../act/SVKeyFrame.h"

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
    SVMoviePtr t_movie = nullptr;
}

void SVUIMovieNode::render() {
    SVNode::render();
    //渲染move背景
    //SVMoviePtr t_movie = nullptr;
    //渲染dragma
    
    //渲染timeline
    
    //渲染key
    
}

void SVUIMovieNode::_renderMoview() {
    
}

void SVUIMovieNode::_renderMovieHead() {
    
}

void SVUIMovieNode::_renderMovieBody() {
    
}

bool SVUIMovieNode::_renderDragma() {
    return true;
}

bool SVUIMovieNode::_renderTimeLine() {
    return true;
}

bool SVUIMovieNode::_renderKey() {
    return true;
}
