//
// SVOpThread.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOpThread.h"
#include "../app/SVInst.h"
#include "../base/SVThread.h"
#include "../work/SVThreadPool.h"
#include "../work/SVThreadWork.h"

//
SVOpAppMode::SVOpAppMode(SVInst* _app,SV_STATE _mode)
: SVOpBase(_app) {
    m_EngMode = _mode;
}

SVOpAppMode::~SVOpAppMode() {
}

void SVOpAppMode::_process(f32 dt) {
//    if(mApp){
//        SV_LOG_INFO("SVOpAppMode set sv mode %d \n",m_EngMode);
//        mApp->setSVMode(m_EngMode);
//    }
}
