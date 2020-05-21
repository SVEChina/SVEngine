//
// SVMtlDark.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLDARK_H
#define SV_MTLDARK_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlDark : public SVMtlCore {
        public:
            SVMtlDark(SVInst *_app);
            
            SVMtlDark(SVMtlDark *_mtl);
            
            ~SVMtlDark();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
            
            void setDarkIntensity(f32 _intensity);
        protected:
            virtual void _submitUniform(SVRendererPtr _render);
            
            f32 m_intensity;
            
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLDARK_H
