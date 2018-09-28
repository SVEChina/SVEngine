//
// SVGameReady.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GAMEREADY_H
#define SV_GAMEREADY_H

#include "SVModuleDef.h"
#include "SVGamePart.h"

namespace sv{
    class SVGameReady : public SVGamePart{
    public:
        SVGameReady(SVInst* _app);
        
        ~SVGameReady();
        
        void init();
        
        void destroy();
        
        void update(f32 _dt);
        
        bool isEnd();
        
    protected:
        
    };
}//!namespace sv





#endif //SV_GAMEREADY_H
