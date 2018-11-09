//
// SVOpEvent.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOpEvent.h"
#include "../base/SVDataSwap.h"
SVTouchEvent::SVTouchEvent() {
    x = 0;
    y = 0;
}

SVAnchorPointEvent::SVAnchorPointEvent() {
    x = 0;
    y = 0;
    z = 0;
}

SVPickEvent::SVPickEvent() {
    colorid = 0;
}

SVCameraOrenEvent::SVCameraOrenEvent() {
    yaw = 0;
    pitch = 0;
    roll = 0;
}

SVDeviceAccelerometerEvent::SVDeviceAccelerometerEvent() {
    x = 0;
    y = 0;
    z = 0;
}

SVCameraMatrixEvent::SVCameraMatrixEvent() {
    m_matData = MakeSharedPtr<SVDataSwap>();
}

SVCameraMatrixEvent::~SVCameraMatrixEvent(){
    m_matData = nullptr;
}

SVProjectMatrixEvent::SVProjectMatrixEvent() {
    m_matData = MakeSharedPtr<SVDataSwap>();
}

SVProjectMatrixEvent::~SVProjectMatrixEvent(){
    m_matData = nullptr;
}

SVReactEvent::SVReactEvent(){
    m_actAniName = "";
    m_actAniLoop = 0;
}

bool SVReactEvent::isEqual(SVEventPtr _event) {
    bool t_flag = true;
//    SVReactEvent *tempEvent = dynamic_cast<SVReactEvent *>(_event);
//    if (!tempEvent || eventName != tempEvent->eventName)
//        t_flag = false;
    if (eventName != _event->eventName)
        t_flag = false;
    return t_flag;
}
