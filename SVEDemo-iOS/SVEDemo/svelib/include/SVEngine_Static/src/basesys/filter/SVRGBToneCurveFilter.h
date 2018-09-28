//
// SVRGBToneCurveFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_RGBTONECURVEFILTER_H
#define SV_RGBTONECURVEFILTER_H

#include "../../base/SVGBase.h"
#include "../../mtl/SVMtlDeclare.h"

namespace sv {

    namespace logic {
        
        class SVRGBToneCurveFilter : public SVGBase {
        public:
            SVRGBToneCurveFilter(SVInst *_app);
            
            ~SVRGBToneCurveFilter();

            void update(f32 dt);

            SVMultPassNodePtr m_toneCurverNode;
            
            SVMtlCorePtr m_mtl_rgba;
            
            SVTexturePtr texrgb;
        };
        
    };//!namespace logic
    
}//!namespace sv

#endif /* SV_RGBTONECURVEFILTER_H */
