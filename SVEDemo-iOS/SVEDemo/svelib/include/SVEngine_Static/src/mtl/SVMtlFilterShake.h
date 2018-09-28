//
// SVMtlFilterShake.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_FILTERSHAKEMATERIAL_H
#define SV_FILTERSHAKEMATERIAL_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
    
        class SVMtlFilterShake : public SVMtlCore {
        public:
            SVMtlFilterShake(SVInst *_app);
            
            SVMtlFilterShake(SVMtlFilterShake* _mtl);
            
            virtual SVMtlCorePtr clone();
            
        protected:
            void _submitMtl();
            
        public:
            f32 dertTime;
            f32 curTime;  //当前震动时间
            f32 lifeTime;  //震动一次的时间
        };
        
    }//!namespace mtl
    
}//!namespace sv



#endif //SV_FILTERSHAKEMATERIAL_H
