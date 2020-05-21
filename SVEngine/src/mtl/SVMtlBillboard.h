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
            
            void setObjectPos(FVec3 &_pos);//物体世界位置
            
            void setViewPos(FVec3 &_viewPos);
            
            void setUp(FVec3 &_up);
        protected:
            virtual void _submitUniform(SVRendererPtr _render);
            FVec3 m_objPos;
            FVec3 m_viewPos;
            FVec3 m_up;
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_BILLBOARD_H
