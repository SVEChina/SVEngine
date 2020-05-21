//
// SVMtlFlashWhite.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLFLASHWHITE_H
#define SV_MTLFLASHWHITE_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlFlashWhite : public SVMtlCore {
        public:
            SVMtlFlashWhite(SVInst *_app);
            
            SVMtlFlashWhite(SVMtlFlashWhite *_mtl);
            
            ~SVMtlFlashWhite();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
            
            void setTime(f32 _time);
            
            void setCycle(f32 _cycle);
        protected:
            virtual void _submitUniform(SVRendererPtr _render);
            
            f32 m_time;
            
            f32 m_cycle;
            
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLFLASHWHITE_H
