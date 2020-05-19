//
// SVCtxBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVCtxBase.h"

SVCtxBase::SVCtxBase()
:m_call_create(nullptr){
}

SVCtxBase::~SVCtxBase() {
}

bool SVCtxBase::activeContext(){
    return false;
}

void* SVCtxBase::getContext(){
    return nullptr;
}
