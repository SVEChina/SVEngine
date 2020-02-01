//
// SVARAnchor.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVARAnchor.h"
#include "../node/SVNode.h"
#include "../base/SVPreDeclare.h"
#include "../basesys/SVSceneMgr.h"
#include "../node/SVScene.h"
#include "../node/SVNode.h"
SVARAnchor::SVARAnchor(SVInst* _app)
:SVModuleBase(_app){
    
}

SVARAnchor::~SVARAnchor(){
    
}

void SVARAnchor::init(){
    SVModuleBase::init();
    
}

void SVARAnchor::destroy(){
    SVModuleBase::destroy();
}

void SVARAnchor::open(){
    SVModuleBase::open();
    
}

void SVARAnchor::close() {
    SVModuleBase::close();
    
}

bool SVARAnchor::isOpen(){
    return SVModuleBase::isOpen();
}

void SVARAnchor::update(f32 _dt) {
    SVModuleBase::update(_dt);
    
}

bool SVARAnchor::procEvent(SVEventPtr _event) {
    return  true;
}


