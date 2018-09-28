//
// SVMtlBlendColor.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLBLENDCOLOR_H
#define SV_MTLBLENDCOLOR_H

#include "SVMtlCore.h"
#include "../base/SVColor.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlBlendColor : public SVMtlCore {
        public:
            SVMtlBlendColor(SVInst *_app);
            
            SVMtlBlendColor(SVMtlBlendColor* _mtl);
            
            ~SVMtlBlendColor();
            
            virtual SVMtlCorePtr clone();
            
            virtual void update(f32 dt);
            
        protected:
            virtual void _submitMtl();
            
            f32 m_lerp;
            SVColor m_color;
        };
        
    }//!namespace mtl
    
}//!namespace sv




#endif //SV_MTLBLENDCOLOR_H
