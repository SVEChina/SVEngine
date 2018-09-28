//
// SVMtlFilterTime.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLFILTERTIME_H
#define SV_MTLFILTERTIME_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
    
        class SVMtlFilterTimeParam : public SVMtlCoreParam {
        public:
            SVMtlFilterTimeParam();
            
            virtual SVMtlCorePtr genMtl(SVInst *_app);
        };
        
        //基于时间的运动
        class SVMtlFilterTime : public SVMtlCore  {
        public:
            SVMtlFilterTime(SVInst *_app);

            SVMtlFilterTime(SVMtlFilterTime* _mtl);
            
            virtual SVMtlCorePtr clone();
            
            void update(f32 dt);
            
        protected:
            virtual void _submitMtl();
            //
            f32 m_lGlobalTime;
        };
        
    }//!namespace mtl
    
}//!namespace sv



#endif //SV_MTLFILTERTIME_H
