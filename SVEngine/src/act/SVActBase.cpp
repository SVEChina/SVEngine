//
// SVActBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVActBase.h"
#include "../node/SVNode.h"
SVActParam::SVActParam(){
    
}

SVActBase::SVActBase(SVInst* _app):SVGBase(_app) {
    
}

SVActBase::~SVActBase() {
}

void SVActBase::run(SVNodePtr _nodePtr, f32 dt) {
}

void SVActBase::enter(SVNodePtr _nodePtr) {
}

void SVActBase::exit(SVNodePtr _nodePtr) {
}

bool SVActBase::isEnd() {
    return false;
}

void SVActBase::reset(){
    
}
