//
// SVMtlColor.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLCOLOR_H
#define SV_MTLCOLOR_H

#include "SVMtlCore.h"
#include "../base/SVColor.h"
namespace sv {
    
    namespace mtl{
        
        class SVMtlColor : public SVMtlCore {
        public:
            SVMtlColor(SVInst *_app);
            
            SVMtlColor(SVMtlColor *_mtl);
            
            ~SVMtlColor();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
            
            void setColor(f32 _r, f32 _g, f32 _b, f32 _a);
        protected:
            virtual void _submitUniform(SVRendererPtr _render);
            
            SVColor m_color;
            
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLCOLOR_H
