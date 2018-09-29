//
// SVRGBToneCurveFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_RGBTONECURVEFILTER_H
#define SV_RGBTONECURVEFILTER_H

#include "../../mtl/SVMtlDeclare.h"
#include "../../node/SVNodeDeclare.h"
#include "SVFilterBase.h"

namespace sv {

    namespace logic {
        
        class SVRGBToneCurveFilter : public SVFilterBase {
        public:
            SVRGBToneCurveFilter(SVInst *_app);
            
            ~SVRGBToneCurveFilter();
            
           virtual bool create();
            
           virtual void destroy();

           virtual void update(f32 dt);
            
            SVMtlCorePtr m_mtl_rgba;
            
            SVMtlCorePtr m_mtl_back;
            
            SVTexturePtr texrgb;
        };
        
    };//!namespace logic
    
}//!namespace sv

#endif /* SV_RGBTONECURVEFILTER_H */
