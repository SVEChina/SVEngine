//
// SVFilterShake.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FILTERSHAKE_H
#define SV_FILTERSHAKE_H

#include "SVFilterBase.h"

namespace sv {
    namespace logic {
        
        class SVFilterShake : public SVFilterBase {
        public:
            SVFilterShake(SVInst *_app);
            
            ~SVFilterShake();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            void setTime(f32 _time);
            
            void setAmount(f32 _amount);
            
            void setSpeed(f32 _speed);
        protected:
            SVMtlShakePtr m_mtlShake;
            f32 m_time;
            f32 m_amount;
            f32 m_speed;
        };
        
    };//!namespace logic
}//!namespace sv


#endif /* SV_FILTERSHAKE_H */
