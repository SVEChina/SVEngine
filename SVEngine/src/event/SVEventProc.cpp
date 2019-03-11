//
// SVEventProc.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVEventProc.h"
#include "SVEventMgr.h"
#include "../app/SVInst.h"

SVEventProc::SVEventProc(SVInst* _app)
:SVGBase(_app) {
}

SVEventProc::~SVEventProc() {
}

void SVEventProc::startListen(){
    mApp->getEventMgr()->registProcer(THIS_TO_SHAREPTR(SVEventProc));
}

void SVEventProc::stopListen(){
    mApp->getEventMgr()->unregistProcer(THIS_TO_SHAREPTR(SVEventProc));
}

//返回true 表示消息继续传递 否者 表示消息不在传递
bool SVEventProc::procEvent(SVEventPtr _event) {
    return true;
}
