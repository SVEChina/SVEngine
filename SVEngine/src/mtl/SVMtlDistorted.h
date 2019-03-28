//
// SVMtlDistorted.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLDISTORTED_H
#define SV_MTLDISTORTED_H

#include "SVMtlADFilterBase.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlDistorted : public SVMtlADFilterBase {
        public:
            SVMtlDistorted(SVInst *_app);
            
            SVMtlDistorted(SVMtlDistorted *_mtl);
            
            ~SVMtlDistorted();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
            void _resetTime();
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLDISTORTED_H
