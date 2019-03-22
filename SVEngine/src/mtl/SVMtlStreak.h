//
// SVMtlStreak.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLSTREAK_H
#define SV_MTLSTREAK_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlStreak : public SVMtlCore {
        public:
            SVMtlStreak(SVInst *_app);
            
            SVMtlStreak(SVMtlStreak *_mtl);
            
            ~SVMtlStreak();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLSTREAK_H
