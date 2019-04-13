//
// SVPenCurve.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PENCURVE_H
#define SV_PENCURVE_H

#include "../../base/SVObject.h"
namespace sv{
    
    class SVPenCurve : public SVObject {
    public:
        SVPenCurve();
        
        ~SVPenCurve();
        
        bool addPoint(f32 x, f32 y, f32 _width, f32 _density);

    };
    
}//!namespace sv

#endif //SV_PENCURVE_H
