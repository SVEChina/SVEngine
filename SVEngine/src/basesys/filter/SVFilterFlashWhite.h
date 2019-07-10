//
// SVFilterFlashWhite.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FILTERFLASHWHITE_H
#define SV_FILTERFLASHWHITE_H

#include "SVFilterBase.h"

namespace sv {
    namespace logic {
        
        class SVFilterFlashWhite : public SVFilterBase {
        public:
            SVFilterFlashWhite(SVInst *_app);
            
            ~SVFilterFlashWhite();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            void setTime(f32 _time);
            
            void setCycle(f32 _cycle);
        protected:
            SVMtlFlashWhitePtr m_mtlFlashWhite;
            f32 m_time;
            f32 m_cycle;
        };
        
    };//!namespace logic
}//!namespace sv


#endif /* SV_FILTERFLASHWHITE_H */
