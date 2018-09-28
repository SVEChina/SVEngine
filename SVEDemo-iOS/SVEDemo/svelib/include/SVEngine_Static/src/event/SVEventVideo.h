//
// SVEventVideo.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_EVENTVIDEO_H
#define SV_EVENTVIDEO_H


#include "SVEvent.h"

namespace sv {
    
    class SVEventVideo : public SVEvent {
    public:
        SVString videoname;
    };
    
    //视频控制
    class SVEventVideoCtrl : public SVEventVideo {
    public:
        SVEventVideoCtrl();
        
        VIDEOCTRL ctrlmode;
    };
    
    //
    class SVEventVideoStatus : public SVEventVideo {
    public:
        SVEventVideoStatus();
        
        VIDEOSTATE state;
    };
    
    //保存开始
    class SVEventMixBegin : public SVEventVideo {
    public:
        SVEventMixBegin();
    };
    
    //保存结束
    class SVEventMixEnd : public SVEventVideo {
    public:
        SVEventMixEnd();
    };

    
}//!namespace sv


#endif //SV_EVENTVIDEO_H
