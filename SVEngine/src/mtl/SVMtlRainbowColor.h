//
// SVMtlRainbowColor.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLRAINBOWCOLOR_H
#define SV_MTLRAINBOWCOLOR_H

#include "SVMtlADFilterBase.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlRainbowColor : public SVMtlADFilterBase {
        public:
            SVMtlRainbowColor(SVInst *_app);
            
            SVMtlRainbowColor(SVMtlRainbowColor *_mtl);
            
            ~SVMtlRainbowColor();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
        protected:
            virtual void _submitUniform(SVRendererPtr _render);
            void _resetTime();
            f32 m_accTime;
            f32 m_time;//周期
            f32 m_lerp;
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLRAINBOWCOLOR_H
