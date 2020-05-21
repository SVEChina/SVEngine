//
// SVMtlADFilterBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLADFILTER_H
#define SV_MTLADFILTER_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlADFilterBase : public SVMtlCore {
        public:
            enum MTLADFILTERSTATE{
                MTL_ADFILTER_BEGIN = 1,
                MTL_ADFILTER_RUNNING,
                MTL_ADFILTER_END,
            };
            SVMtlADFilterBase(SVInst *_app, cptr8 _shader);
            
            SVMtlADFilterBase(SVMtlADFilterBase *_mtl);
            
            ~SVMtlADFilterBase();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
        protected:
            virtual void _submitUniform(SVRendererPtr _render);
            void _resetTime();
            MTLADFILTERSTATE m_state;
            f32 m_time;
            f32 m_accTime;
            f32 m_intervalTime;
        };

    }//!namespace mtl
    
}//!namespace sv

#endif //SV_MTLADFILTER_H
