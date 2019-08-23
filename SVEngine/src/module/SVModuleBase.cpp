//
// SVModuleBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVModuleBase.h"

SVModuleBase::SVModuleBase(SVInst *_app)
:SVEventProc(_app)
,m_isOpen(false){
    m_module_name = "";
}

SVModuleBase::~SVModuleBase() {
}

void SVModuleBase::init() {
}

void SVModuleBase::destroy() {
}

void SVModuleBase::update(f32 _dt) {
}

void SVModuleBase::open() {
    m_isOpen = true;
}

void SVModuleBase::close() {
    m_isOpen = false;
}

bool SVModuleBase::isOpen(){
    return m_isOpen;
}

void SVModuleBase::setOpCallBack(cb_func_op _cb, void *_obj){
    m_cb = _cb;
    m_obj = _obj;
}
