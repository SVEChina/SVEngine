//
// SVGameRun.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GAMERUN_H
#define SV_GAMERUN_H

#include "SVModuleDef.h"
#include "SVGamePart.h"

namespace sv{
    class SVGameRun : public SVGamePart {
    public:
        SVGameRun(SVInst* _app);
        
        ~SVGameRun();
        
        void init();
        
        void destroy();
        
        void update(f32 _dt);
        
        bool isEnd();
        
    protected:
        
    };
}//!namespace sv





#endif //SV_GAMEREADY_H
