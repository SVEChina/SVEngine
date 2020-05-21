//
// SVMtl2D.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_2D_H
#define SV_MTL_2D_H


#include "SVMtlCore.h"

//2D动画材质
namespace sv {
    
    namespace mtl{
        
        class SVMtl2D : public SVMtlCore {
        public:
            SVMtl2D(SVInst *_app, cptr8 _shader);
            
            SVMtl2D(SVMtl2D *_mtl);
            
            ~SVMtl2D();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void setAlpha(f32 _alpha);
            
        protected:
            virtual void _submitUniform(SVRendererPtr _render);
            f32  m_alpha;
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_2D_H
