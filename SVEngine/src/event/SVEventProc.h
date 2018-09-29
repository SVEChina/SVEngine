//
// SVEventProc.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_EVENTPROC_H
#define SV_EVENTPROC_H

#include "../base/SVGBase.h"
#include "SVEventDef.h"

//消息处理器

namespace sv {
    
    class SVEventProc : public SVGBase {
    public:
        SVEventProc(SVInst* _app);
        
        ~SVEventProc();
        
        virtual void startListen();
        
        virtual void stopListen();
        
        virtual bool procEvent(SVEventPtr _event);
    };
    
}//!namespace sv


#endif //SV_EVENTPROC_H
