//
// SVMtlBeautyFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_MTLBEAUTYFILTER_H
#define SV_MTLBEAUTYFILTER_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlBeautyFilterParam : public SVMtlCoreParam {
        public:
            SVMtlBeautyFilterParam();
            
            virtual SVMtlCorePtr genMtl(SVInst *_app);
        };
    
        //美颜滤镜
        class SVMtlBeautyFilter : public SVMtlCore  {
        public:
            SVMtlBeautyFilter(SVInst *_app);
            
            SVMtlBeautyFilter(SVMtlBeautyFilter* _mtl);
            
            virtual SVMtlCorePtr clone();
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTLBEAUTYFILTER_H
