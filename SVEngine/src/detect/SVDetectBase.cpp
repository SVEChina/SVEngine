//
// SVDetectBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVDetectBase.h"
#include "SVPerson.h"

SVDetectBase::SVDetectBase(SVInst *_app)
:SVListenBase(_app) {
}

SVDetectBase::~SVDetectBase() {
}

void SVDetectBase::update(f32 _dt){
}

void SVDetectBase::pushData(void *_faceData){
}

s32 SVDetectBase::transformIndex(s32 index) {
    return 0;
}

BINDREGION SVDetectBase::getIndexRegion(s32 index) {
    return BD_REGION_CENTER;
}

