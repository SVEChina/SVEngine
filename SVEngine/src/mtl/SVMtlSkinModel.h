//
// SVMtlSkinModel.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_SKINMODEL_H
#define SV_MTL_SKINMODEL_H

#include "SVMtlCore.h"
#include "../base/SVColor.h"

#ifdef CONFIG_IS_LOAD_ASSIMP

#include <assimp/FMat4x4.h>
//

namespace sv {
    
    namespace mtl{
        
        class SVMtlSkinModel : public SVMtlCore {
        public:
            SVMtlSkinModel(SVInst *_app);
            
            SVMtlSkinModel(SVMtlSkinModel* _mtl);
            
            ~SVMtlSkinModel();
            
            virtual SVMtlCorePtr clone();
            
            static const u32 MAX_BONES = 30;
            
            void setBoneTransform(s32 iIndex, const aiMatrix4x4 &Transform);
            
            void update(f32 dt);
            
            virtual bool submitMtl();
            
        private:
            s32 m_boneLocation[MAX_BONES];
            SVArray<aiMatrix4x4> m_vecBoneMatrix;
        };
        
        //
        class SVMatSkinModelSelect : public SVMtlSkinModel {
        public:
            SVMatSkinModelSelect(SVInst *_app);
            
            SVMatSkinModelSelect(SVMatSkinModelSelect *_mtl);
            
            virtual SVMtlCorePtr clone();

            void setColor(u32 colorid);
            
            void _submitMtl(SVRendererBasePtr _render); 
            
        protected:
            SVColor mColor;
        };
        
    }//!namespace mtl
}//!namespace sv



#endif //CONFIG_IS_LOAD_ASSIMP

#endif //SV_MTL_SKINMODEL_H
