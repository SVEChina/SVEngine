//
// SVMtlStreak.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLSTREAK_H
#define SV_MTLSTREAK_H

#include "SVMtlADFilterBase.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlStreak : public SVMtlADFilterBase {
        public:
            SVMtlStreak(SVInst *_app);
            
            SVMtlStreak(SVMtlStreak *_mtl);
            
            ~SVMtlStreak();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
            void _resetTime();
            f32 m_lerp;
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLSTREAK_H
