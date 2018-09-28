//
// SVMtlFace2d.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_FACE2D_H
#define SV_MTL_FACE2D_H


#include "SVMtlCore.h"

//2D面具材质
namespace sv {
    
    namespace mtl{
        
        class SVMtlFace2d : public SVMtlCore {
        public:
            SVMtlFace2d(SVInst *_app);
            
            SVMtlFace2d(SVMtlFace2d *_mtl);
            
            ~SVMtlFace2d();
            
            virtual SVMtlCorePtr clone();
            
            void setMaskTexture(SVTexturePtr texture);
            
            void update(f32 dt);
            
        protected:
            SVTexturePtr m_pMaskTexture;
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_FACE2D_H
