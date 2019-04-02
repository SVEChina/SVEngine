//
// SVOpEvent.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OPEVENT_H
#define SV_OPEVENT_H


#include "SVEvent.h"

namespace sv {
    
    class SVTouchEvent : public SVEvent {
    public:
        SVTouchEvent();
        s32 x;
        s32 y;
    };
    
    class SVARAnchorEvent : public SVEvent {
    public:
        SVARAnchorEvent();
        f32 m_p_x;
        f32 m_p_y;
        f32 m_p_z;
        f32 m_s_x;
        f32 m_s_y;
        f32 m_s_z;
        f32 m_r_x;
        f32 m_r_y;
        f32 m_r_z;
        f32 m_index;
        SVDataSwapPtr m_matData;
    };
    
    //
    class SVPickEvent : public SVEvent {
    public:
        SVPickEvent();
        u32 colorid;
    };
    
    //
    class SVCameraOrenEvent : public SVEvent {
    public:
        SVCameraOrenEvent();
        f32 yaw;
        f32 pitch;
        f32 roll;
    };
    
    class SVDeviceAccelerometerEvent : public SVEvent {
    public:
        SVDeviceAccelerometerEvent();
        f32 x;
        f32 y;
        f32 z;
    };
    
    class SVCameraMatrixEvent : public SVEvent {
    public:
        SVCameraMatrixEvent();
        
        ~SVCameraMatrixEvent();
        
        SVDataSwapPtr m_matData;
    };
    
    class SVProjectMatrixEvent : public SVEvent {
    public:
        SVProjectMatrixEvent();
        
        ~SVProjectMatrixEvent();
        
        SVDataSwapPtr m_matData;
    };
    
    class SVReactEvent : public SVEvent {
    public:
        SVReactEvent();
        bool isEqual(SVEventPtr _event);
        SVString     m_actAniName;//要切换的spine动画名称
        s32         m_actAniLoop;//要切换的spine动画是否循环播放
    };

    
}//!namespace sv



#endif //SV_OPEVENT_H
