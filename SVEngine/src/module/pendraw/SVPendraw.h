//
// SVPendraw.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PENDRAW_H
#define SV_PENDRAW_H

#include "../SVGameBase.h"
#include "../../event/SVEvent.h"

namespace sv{
    
    //绘制事件

    class SVPendraw : public SVGameBase {
    public:
        SVPendraw(SVInst* _app);
        
        ~SVPendraw();
        
        virtual void init(SVGameReadyPtr _ready,SVGameRunPtr _run,SVGameEndPtr _end);
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool procEvent(SVEventPtr _event);
        
    protected:
        typedef SVArray<SVPenStrokePtr> STROKEPOOL;
        STROKEPOOL m_strokes;
    };
    
}//!namespace sv





#endif //SV_PENDRAW_H
