//
// SVMtlShinning.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLSHINNING_H
#define SV_MTLSHINNING_H

#include "SVMtlADFilterBase.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlShinning : public SVMtlADFilterBase {
        public:
            SVMtlShinning(SVInst *_app);
            
            SVMtlShinning(SVMtlShinning *_mtl);
            
            ~SVMtlShinning();
            
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

#endif //SV_MTLSHINNING_H
