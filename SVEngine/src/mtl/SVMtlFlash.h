//
// SVMtlFlash.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLFLASH_H
#define SV_MTLFLASH_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlFlash : public SVMtlCore {
        public:
            SVMtlFlash(SVInst *_app);
            
            SVMtlFlash(SVMtlFlash *_mtl);
            
            ~SVMtlFlash();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
            f32 m_accTime;
            f32 m_time;//周期
            f32 m_lerp;
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLFLASH_H
