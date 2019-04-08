//
// SVPenStroke.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPenStroke.h"
#include "../SVGameReady.h"
#include "../SVGameRun.h"
#include "../SVGameEnd.h"


SVPenStroke::SVPenStroke(SVInst *_app)
:SVGameBase(_app) {
    m_ptPool.clear();
}

SVPenStroke::~SVPenStroke() {
    m_ptPool.clear();
}

//绘制一笔
void SVPenStroke::update(f32 _dt) {
    
}

void SVPenStroke::begin(f32 _px,f32 _py,f32 _pz) {
    m_ptPool.append(FVec3(_px,_py,_pz));
}

void SVPenStroke::end(f32 _px,f32 _py,f32 _pz) {
    m_ptPool.append(FVec3(_px,_py,_pz));
}

void SVPenStroke::draw(f32 _px,f32 _py,f32 _pz) {
    m_ptPool.append(FVec3(_px,_py,_pz));
}
