//
// SVOpADFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li，daming Li
//

#ifndef SV_OPADFILTER_H
#define SV_OPADFILTER_H
#include "operate/SVOpBase.h"
namespace sv {
    class SVOpLoadADFilter : public SVOpBase {
    public:
        SVOpLoadADFilter(SVInst *_app);
        
        ~SVOpLoadADFilter();
        
    protected:
        void _process(f32 dt);
        
    };
    
    class SVOpDestroyADFilter : public SVOpBase {
    public:
        SVOpDestroyADFilter(SVInst *_app);
        
        ~SVOpDestroyADFilter();
    protected:
        void _process(f32 dt);
    };
}//!namespace sv



#endif //SV_OPADFILTER_H
