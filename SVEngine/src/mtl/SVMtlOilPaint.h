//
// SVMtlOilPaint.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLOILPAINT_H
#define SV_MTLOILPAINT_H

#include "SVMtlADFilterBase.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlOilPaint : public SVMtlADFilterBase {
        public:
            SVMtlOilPaint(SVInst *_app);
            
            SVMtlOilPaint(SVMtlOilPaint *_mtl);
            
            ~SVMtlOilPaint();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
            
            void setTime(f32 _time);
            
            void setAmount(f32 _amount);
            
            void setSpeed(f32 _speed);
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
            
            f32 m_time;
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLOILPAINT_H
