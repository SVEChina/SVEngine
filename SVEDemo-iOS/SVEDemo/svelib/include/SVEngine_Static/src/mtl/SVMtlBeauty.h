//
// SVMtlBeauty.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLBEAUTY_H
#define SV_MTLBEAUTY_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlBeautyParam : public SVMtlCoreParam {
        public:
            SVMtlBeautyParam();
            
            virtual SVMtlCorePtr genMtl(SVInst *_app);
            
            f32 m_smooth;
        };
        
        class SVMtlBeauty : public SVMtlCore  {
        public:
            SVMtlBeauty(SVInst *_app);
            
            SVMtlBeauty(SVMtlBeauty* _mtl);
            
            ~SVMtlBeauty();
            
            virtual SVMtlCorePtr clone();
            
            void setSmooth(f32 _smooth);
            
            void setBGSize(s32 _width, s32 _height);

        protected:
            virtual void _submitMtl();
            //
            f32 m_lSmoothSize;
            f32 m_lImgW;
            f32 m_lImgH;
            //
            f32 m_rSmoothSize;
            f32 m_rImgW;
            f32 m_rImgH;
        };
        
    }//!namespace mtl
    
}//!namespace sv



#endif //SV_MTLBEAUTY_H
