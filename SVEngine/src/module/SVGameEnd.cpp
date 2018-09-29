//
// SVGameEnd.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVGameEnd.h"

SVGameEnd::SVGameEnd(SVInst *_app)
:SVGamePart(_app) {
}

SVGameEnd::~SVGameEnd() {
}

void SVGameEnd::init() {
}

void SVGameEnd::destroy() {
}

void SVGameEnd::update(f32 _dt) {
}

bool SVGameEnd::isEnd(){
    return true;
}
