//
// SVMtlGLTF.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_GLTF_H
#define SV_MTL_GLTF_H


#include "SVMtlCore.h"

//GLTF材质
namespace sv {
    
    namespace mtl{
        
        class SVMtlGLTF : public SVMtlCore {
        public:
            SVMtlGLTF(SVInst *_app);
            
            SVMtlGLTF(SVMtlGLTF *_mtl);
            
            ~SVMtlGLTF();
            
            virtual SVMtlCorePtr clone();
            
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_GLTF_H
