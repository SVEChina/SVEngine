//
// SVMtlFace2D.h
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
        
        class SVMtlFace2D : public SVMtlCore {
        public:
            SVMtlFace2D(SVInst *_app);
            
            SVMtlFace2D(SVMtlFace2D *_mtl);
            
            ~SVMtlFace2D();
            
            virtual SVMtlCorePtr clone();
            
            void setMaskTexture(SVTexturePtr texture);
            
            SVTexturePtr getMaskTexture();
            
            void update(f32 dt);
            
        protected:
            SVTexturePtr m_pMaskTexture;
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_FACE2D_H
