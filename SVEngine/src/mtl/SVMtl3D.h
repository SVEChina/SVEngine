//
// SVMtl3D.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_3D_H
#define SV_MTL_3D_H


#include "SVMtlCore.h"

//2D动画材质
namespace sv {
    
    namespace mtl{
        
        class SVMtl3D : public SVMtlCore {
        public:
            SVMtl3D(SVInst *_app);
            
            SVMtl3D(SVMtl3D *_mtl);
            
            ~SVMtl3D();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void _submitMtl(SVRendererBasePtr _render);
        public:
            FVec4 m_ambient_color;
            f32   m_ambientStrength;
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_3D_H
