// SVThreadRender.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERTHREAD_H
#define SV_RENDERTHREAD_H

#include "SVThreadWork.h"

namespace sv {

    //渲染线程
    class SVThreadRender : public SVThreadWork {
    public:
        SVThreadRender(SVInst *_app);
        
        ~SVThreadRender();
        
    protected:
        void _innerUpdate();
        
        void _innerDestroy();
        
        f32 _getDert();
        
        bool m_first;
        
        timeval m_lastT;
    };
    
}//!namespace sv




#endif //SV_MAINTHREAD_H
