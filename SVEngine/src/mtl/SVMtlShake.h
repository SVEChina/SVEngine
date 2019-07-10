//
// SVMtlShake.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLSHAKE_H
#define SV_MTLSHAKE_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlShake : public SVMtlCore {
        public:
            SVMtlShake(SVInst *_app);
            
            SVMtlShake(SVMtlShake *_mtl);
            
            ~SVMtlShake();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
            
            void setTime(f32 _time);
            
            void setAmount(f32 _amount);
            
            void setSpeed(f32 _speed);
        protected:
            virtual void _submitUniform(SVRendererBasePtr _render);
            
            f32 m_time;
            f32 m_amount;
            f32 m_speed;
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLSHAKE_H
