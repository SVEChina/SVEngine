// SVThreadMain.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MAINTHREAD_H
#define SV_MAINTHREAD_H

#include "SVThreadWork.h"

namespace sv {

    //主线程 控制线程
    class SVThreadMain : public SVThreadWork {
    public:
        SVThreadMain(SVInst *_app);
        
        ~SVThreadMain();
        
        void resetTime();
        
        void clearThreadCache();
    protected:
        void _innerUpdate();
        
        void _innerDestroy();
        
        f32 _getDert();
        
        bool m_first;
        
        timeval m_lastT;
    };
    
    class SVThreadSync : public SVThreadMain {
    public:
        SVThreadSync(SVInst *_app);
        
        ~SVThreadSync();
        
        void startThread();
        
        void stopThread();
        
        void syncUpdate(f32 _dt);
    };
    
}//!namespace sv




#endif //SV_MAINTHREAD_H
