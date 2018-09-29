//
// SVOpDetect.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOpDetect.h"
#include "../app/SVInst.h"
#include "../detect/SVDetectMgr.h"

//
SVOpOpenDetect::SVOpOpenDetect(SVInst *_app, s32 detecttype)
:SVOpBase(_app) {
    m_detectType = (DETECTTYPE)detecttype;
}

void SVOpOpenDetect::_process(float dt) {
    mApp->getDetectMgr()->init(m_detectType);
}

//
SVOpCloseDetect::SVOpCloseDetect(SVInst *_app)
:SVOpBase(_app) {
}

void SVOpCloseDetect::_process(float dt) {
    mApp->getDetectMgr()->init(DETECT_T_NULL);
}
