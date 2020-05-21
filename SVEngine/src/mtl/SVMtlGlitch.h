//
// SVMtlGlitch.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLGLITCH_H
#define SV_MTLGLITCH_H

#include "SVMtlADFilterBase.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlGlitch : public SVMtlADFilterBase {
        public:
            
            SVMtlGlitch(SVInst *_app);
            
            SVMtlGlitch(SVMtlGlitch *_mtl);
            
            ~SVMtlGlitch();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
        protected:
            virtual void _submitUniform(SVRendererPtr _render);
            void _initializeParameter();
            void _destroyParameter();
            void _resetTime();
            s32 m_maxFrame;
            s32 m_curFrame;
            f32 *m_drift;
            f32 *m_jitter;
            f32 *m_threshHold;
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLGLITCH_H
