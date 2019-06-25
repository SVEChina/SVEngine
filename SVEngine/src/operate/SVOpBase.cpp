//
// SVOpBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOpBase.h"
#include "../app/SVInst.h"
#include "../callback/SVOpCallback.h"

SVOpBase::SVOpBase(SVInst *_app)
:SVGBase(_app)
,m_pCB(nullptr){
}

SVOpBase::~SVOpBase() {
}

void SVOpBase::setCallBack(cb_func_op _cb,cptr8 _info){
    m_pCB = _cb;
    m_info = _info;
}

void SVOpBase::process(f32 dt) {
    _process(dt);
    if(m_pCB){
        (*m_pCB)(m_info.c_str(), mApp);
    }
}

void SVOpBase::_process(f32 dt){
}


