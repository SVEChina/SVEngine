//
// SVMtlDistorted.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLDISTORTED_H
#define SV_MTLDISTORTED_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlDistorted : public SVMtlCore {
        public:
            SVMtlDistorted(SVInst *_app);
            
            SVMtlDistorted(SVMtlDistorted *_mtl);
            
            ~SVMtlDistorted();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
            f32 m_time;
            f32 m_accTime;
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLDISTORTED_H
