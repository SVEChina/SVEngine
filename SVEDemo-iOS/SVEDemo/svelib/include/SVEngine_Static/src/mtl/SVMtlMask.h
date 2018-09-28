//
// SVMtlMask.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLMASK_H
#define SV_MTLMASK_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlMaskParam : public SVMtlCoreParam {
        public:
            SVMtlMaskParam();
            
            virtual SVMtlCorePtr genMtl(SVInst *_app);
            
            SVString m_masktex_name; //遮挡纹理
            
        };
        
        //
        class SVMtlMask : public SVMtlCore {
        public:
            SVMtlMask(SVInst *_app);
            
            SVMtlMask(SVMtlMask *_mtl);
            
            ~SVMtlMask();
            
            virtual SVMtlCorePtr clone();
            
            virtual void update(f32 dt);
            
            void setMaskTex(SVTexturePtr _masktex);
            
        protected:
            
            SVTexturePtr m_pMaskTex;  //
        };
        
    }//!namespace mtl
    
}//!namespace sv




#endif //SV_MTLMASK_H
