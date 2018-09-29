//
// SVSysBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVSysBase.h"

SVSysBase::SVSysBase(SVInst *_app)
:SVEventProc(_app) {
    m_subsysType = "SVSysBase";
}
SVSysBase::~SVSysBase() {
}

