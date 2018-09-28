//
// SVMtlMirror.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_MIRROR_H
#define SV_MTL_MIRROR_H

#include "SVMtlCore.h"

namespace sv {
    namespace mtl{
        
        class SVMtlMirror : public SVMtlCore {
        public:
            SVMtlMirror(SVInst *_app);
            
            ~SVMtlMirror();
            
        public:
            
            virtual void update(f32 dt);
            
        protected:
            virtual void _submitMtl();
            f32 m_xMulti;
            f32 m_yMulti;
        };

        
    }//!namespace mtl
}//!namespace sv



#endif //SV_MTL_MIRROR_H
