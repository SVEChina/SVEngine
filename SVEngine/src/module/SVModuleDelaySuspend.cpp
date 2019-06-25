//
// SVModuleDelaySuspend.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVModuleDelaySuspend.h"
#include "../app/SVInst.h"
SVModuleDelaySuspend::SVModuleDelaySuspend(SVInst *_app)
:SVModuleBase(_app)
,m_isOpen(false){
    m_module_name = "";
    m_accTime = 0.0f;
}

SVModuleDelaySuspend::~SVModuleDelaySuspend() {
}

void SVModuleDelaySuspend::init() {
    
}

void SVModuleDelaySuspend::destroy() {
}

void SVModuleDelaySuspend::update(f32 _dt) {
    if (m_isOpen && mApp->getState() == SV_ST_WILLSUSPEND) {
        m_accTime -= _dt;
        if (m_accTime <= 0) {
            //计时结束了
            close();
            mApp->svSuspend();
            if (m_cb) {
                (*m_cb)("sv_suspend", mApp);
            }
        }
    }
}

void SVModuleDelaySuspend::open() {
    m_isOpen = true;
}

void SVModuleDelaySuspend::close() {
    m_isOpen = false;
}

bool SVModuleDelaySuspend::isOpen(){
    return m_isOpen;
}

void SVModuleDelaySuspend::setOpCallBack(cb_func_op _cb){
    m_cb = _cb;
}

void SVModuleDelaySuspend::setAccTime(f32 _time){
    m_accTime = _time;
}
