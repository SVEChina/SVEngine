//
// SVMtlRepeat.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_MTL_REPEAT_H
#define SV_MTL_REPEAT_H

#include "SVMtlCore.h"
#include "../base/SVColor.h"


namespace sv {
    namespace mtl{
        
        class SVMtlRepeat : public SVMtlCore {
        public:
            SVMtlRepeat(SVInst *_app);
            
            ~SVMtlRepeat();
            
        public:
            
            virtual void update(f32 dt);
            
            void setRepeat(f32 _x, f32 _y);
            
        protected:
            virtual void _submitMtl();
            
            f32 m_xMulti;
            f32 m_yMulti;
        };
        
    }//!namespace mtl
}//!namespace sv




#endif //SV_MTL_REPEAT_H
