//
// SVARAnchor.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ARANCHOR_H
#define SV_ARANCHOR_H


#include "SVModuleBase.h"

namespace sv{
    class SVARAnchor : public SVModuleBase {
    public:
        SVARAnchor(SVInst* _app);
        
        ~SVARAnchor();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool isOpen();
        
        virtual bool procEvent(SVEventPtr _event);
        
    protected:
        
    };
    
    
}//!namespace sv

#endif /* SV_ARANCHOR_H */
