//
// SVMtlStrokeBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_STROKEBASE_H
#define SV_MTL_STROKEBASE_H


#include "SVMtlCore.h"
namespace sv {
    
    namespace mtl{
        
        class SVMtlStrokeBase : public SVMtlCore {
        public:
            SVMtlStrokeBase(SVInst *_app, cptr8 _shader);
            
            SVMtlStrokeBase(SVMtlStrokeBase *_mtl);
            
            ~SVMtlStrokeBase();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void setQuadPosW(FVec3 &_quadPosW);
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
            FVec3 m_quadPosW;
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_STROKEBASE_H
