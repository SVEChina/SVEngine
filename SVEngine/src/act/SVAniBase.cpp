//
// SVAniBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVAniBase.h"
#include "SVActBase.h"

SVAniBase::SVAniBase(SVInst *_app)
:SVGBase(_app) {
}

SVAniBase::~SVAniBase() {
}

void SVAniBase::init() {
}

void SVAniBase::destroy() {
}

void SVAniBase::enter(){
}

void SVAniBase::exit(){
}

void SVAniBase::update(f32 _dt) {
}

bool SVAniBase::isEnd(){
    return true;
}
