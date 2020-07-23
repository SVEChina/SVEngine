//
// SVMtlWindowLight.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLWINDOWLIGHT_H
#define SV_MTLWINDOWLIGHT_H

#include "SVMtlADFilterBase.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlWindowLight : public SVMtlADFilterBase {
        public:
            SVMtlWindowLight(SVInst *_app);
            
            SVMtlWindowLight(SVMtlWindowLight *_mtl);
            
            ~SVMtlWindowLight();
            
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

#endif //SV_MTLWINDOWLIGHT_H
