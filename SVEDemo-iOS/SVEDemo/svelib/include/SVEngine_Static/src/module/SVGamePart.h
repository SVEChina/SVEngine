//
// SVGamePart.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GAMEPART_H
#define SV_GAMEPART_H

#include "SVModuleDef.h"
#include "../event/SVEventProc.h"

namespace sv{
    
    class SVGamePart : public SVGBase {
    public:
        SVGamePart(SVInst* _app);
        
        virtual ~SVGamePart();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void reset();
        
        virtual void enter();
        
        virtual void exit();
        
        virtual void update(f32 _dt);
        
        virtual bool isEnd();
        
    protected:
        
    };
    
}//!namespace sv



#endif //SV_GAMEPART_H
