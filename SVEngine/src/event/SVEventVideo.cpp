//
// SVEventVideo.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVEventVideo.h"

SVEventVideoCtrl::SVEventVideoCtrl() {
    eventType = EVN_T_VIDEO_CTRL;
    ctrlmode = VIDEO_CTRL_NONE;
}

SVEventVideoStatus::SVEventVideoStatus() {
    eventType = EVN_T_VIDEO_STATE;
    state = VIDEO_STATE_NONE;
}

//
SVEventMixBegin::SVEventMixBegin() {
    eventType = EVN_T_SAVE_BEGIN;
}

//
SVEventMixEnd::SVEventMixEnd() {
    eventType = EVN_T_SAVE_END;
}
