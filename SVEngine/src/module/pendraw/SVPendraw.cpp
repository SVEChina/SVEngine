//
// SVPendraw.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPendraw.h"
#include "../SVGameReady.h"
#include "../SVGameRun.h"
#include "../SVGameEnd.h"


SVPendraw::SVPendraw(SVInst *_app)
:SVGameBase(_app) {
    
}

SVPendraw::~SVPendraw() {
}

void SVPendraw::init(SVGameReadyPtr _ready,SVGameRunPtr _run,SVGameEndPtr _end) {
    SVGameBase::init(_ready,_run,_end);
}

void SVPendraw::destroy() {
    SVGameBase::destroy();
}

void SVPendraw::update(f32 _dt) {
    SVGameBase::update(_dt);
}

void SVPendraw::open() {
    SVGameBase::open();
}

void SVPendraw::close() {
    SVGameBase::close();
}

bool SVPendraw::procEvent(SVEventPtr _event){
    return true;
}


