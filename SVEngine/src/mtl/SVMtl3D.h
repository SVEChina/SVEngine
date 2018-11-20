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
            SVMtl3D(SVInst *_app, cptr8 _shader);
            
            SVMtl3D(SVMtl3D *_mtl);
            
            ~SVMtl3D();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            //最多支持8个反射光源，channel 最大为7
            void setDiffuseLight(u32 _channel, FVec3 _lightPos, FVec3 _lightColor);
            
            void setAmbientLight(f32 _strength, FVec3 _ambientColor);
        protected:
            void _submitMtl(SVRendererBasePtr _render);
            FVec3 m_ambient_color;
            f32   m_ambientStrength;
            SVArray<f32> m_diffuseLightPosPool;
            SVArray<f32> m_diffuseLightColorPool;
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_3D_H
