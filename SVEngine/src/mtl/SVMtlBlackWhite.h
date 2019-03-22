//
// SVMtlBlackWhite.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLBLACKWHITE_H
#define SV_MTLBLACKWHITE_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlBlackWhite : public SVMtlCore {
        public:
            SVMtlBlackWhite(SVInst *_app);
            
            SVMtlBlackWhite(SVMtlBlackWhite *_mtl);
            
            ~SVMtlBlackWhite();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
            
            f32 m_mode;
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLBLACKWHITE_H
