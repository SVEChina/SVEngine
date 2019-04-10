//
// SVGameEnd.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GAMEEND_H
#define SV_GAMEEND_H

#include "SVModuleDef.h"
#include "SVGamePart.h"

namespace sv{
    class SVGameEnd : public SVGamePart {
    public:
        SVGameEnd(SVInst* _app);
        
        ~SVGameEnd();
        
        void init();
        
        void destroy();
        
        void update(f32 _dt);
        
        bool isEnd();
        
    protected:
        
    };
    
}//!namespace sv





#endif //SV_GAMEREADY_H
