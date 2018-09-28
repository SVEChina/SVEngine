//
// SVPosition2d.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
// copy from irr
//

#ifndef SV_POSITION_H
#define SV_POSITION_H

#include "SVVector2d.h"

namespace sv{
    
    namespace util{
    
    typedef SVVector2d<f32> position2df;
        
    typedef SVVector2d<s32> position2di;
    
    //!namespace util
    }
    
//!namespace sv
};

// ...and use a #define to catch the rest, for (e.g.) position2d<f64>
#define SVPosition2d SVVector2d

#endif // SV_POSITION_H

