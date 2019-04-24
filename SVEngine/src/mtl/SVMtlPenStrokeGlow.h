//
// SVMtlPenStrokeGlow.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_PENSTROKEGLOW_H
#define SV_MTL_PENSTROKEGLOW_H


#include "SVMtlBillboard.h"
namespace sv {
    
    namespace mtl{
        
        class SVMtlPenStrokeGlow : public SVMtlBillboard {
        public:
            SVMtlPenStrokeGlow(SVInst *_app);
            
            SVMtlPenStrokeGlow(SVMtlPenStrokeGlow *_mtl);
            
            ~SVMtlPenStrokeGlow();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
    
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
            FVec3 m_objPos;
            FVec3 m_viewPos;
            FVec3 m_up;
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_PENSTROKEGLOW_H
