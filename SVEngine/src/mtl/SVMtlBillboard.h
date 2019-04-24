//
// SVMtlBillboard.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_BILLBOARD_H
#define SV_MTL_BILLBOARD_H


#include "SVMtlCore.h"
namespace sv {
    
    namespace mtl{
        
        class SVMtlBillboard : public SVMtlCore {
        public:
            SVMtlBillboard(SVInst *_app);
            
            SVMtlBillboard(SVMtlBillboard *_mtl);
            
            ~SVMtlBillboard();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void setQuadPosW(FVec3 &_quadPosW);//物体世界位置
            
            void setViewPosW(FVec3 &_viewPosW);
            
            void setUp(FVec3 &_up);
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
            FVec3 m_quadPosW;
            FVec3 m_viewPosW;
            FVec3 m_up;
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_BILLBOARD_H
