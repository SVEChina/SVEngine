//
// SVColor.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
// copy from irr

#ifndef SV_COLOR_H
#define SV_COLOR_H

//#include <math.h>
#include "SVDef.h"

namespace sv {

    class SVColor {
    public:
        SVColor();
        
        SVColor(f32 r, f32 g, f32 b, f32 a);
        
        void operator=(SVColor& _color);
        
        void setColor(f32 r, f32 g, f32 b, f32 a);
        
        void setColorARGB(u32 color);
        
        u32 getColorARGB();
        
        f32 r;
        f32 g;
        f32 b;
        f32 a;
        
        static u32 transRgbaToID(u8 r, u8 g, u8 b, u8 a);
    };
    
}//!namespace sv


#endif //SV_COLOR_H
