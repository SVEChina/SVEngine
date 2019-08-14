//
// SVMtlGLTF.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_GLTF_H
#define SV_MTL_GLTF_H


#include "SVMtlCore.h"
#include "../base/SVVec3.h"
#include "../base/SVVec4.h"

//GLTF材质
namespace sv {
    
    namespace mtl{
        
        class SVMtlGLTF : public SVMtlCore {
        public:
            SVMtlGLTF(SVInst *_app);
            
            SVMtlGLTF(SVMtlGLTF *_mtl);
            
            ~SVMtlGLTF();
            
        protected:
            SVMtlGLTF(SVInst *_app,cptr8 _name);
        
        public:
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
            
            void refresh();
            
        public:
            SVTexturePtr m_pBaseColorTex;
            SVTexturePtr m_pMetallicRoughnessTex;
            SVTexturePtr m_pNormalTex;
            SVTexturePtr m_pOcclusionTex;
            SVTexturePtr m_pEmissiveTex;
            FVec4 m_baseColorFactor;
            f32 m_metallicFactor;
            f32 m_roughtnessFactor;
            f32 m_normalScale;
            f32 m_occlusionStrength;
            FVec3 m_emissiveFactor;
        };
        
#define MAX_BONES 30
#define MAX_BONES_DATA 30*16
#define MAX_BONES_DATA_SIZE 30*16*4
        
        class SVMtlGLTFSkin : public SVMtlGLTF {
        public:
            SVMtlGLTFSkin(SVInst *_app);
            
            SVMtlGLTFSkin(SVMtlGLTFSkin *_mtl);
            
            ~SVMtlGLTFSkin();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
            
            void refresh();
            
            void bindSke(SVSkeletonPtr _ske);
            
            void unbindSke();
            
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
            //
            f32 m_vecBoneMatrix[MAX_BONES_DATA];
            //关联骨架
            SVSkeletonPtr m_pSke;
        };


    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_GLTF_H
