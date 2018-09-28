//
// SVMathHelper.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MATHHELPER_H
#define SV_MATHHELPER_H

#include "SVVertDef.h"
#include "../base/SVArray.h"

class SVMathHelper{
public:
    static f32 vec2Length(f32 _x1, f32 _x2, f32 _y1, f32 _y2);

    static f32 atan2fV2(V2 p1, V2 p2);

    static V2 translateToUp(s32 x, s32 y, f32 roll, f32 len);

    static V2 translateToLeft(s32 x, s32 y, f32 roll, f32 len);

    static V2 translateToRight(s32 x, s32 y, f32 roll, f32 len);

    static void createCurve(V2 *originpoint, s32 origincount, s32 step, SVArray<V2>& outcurve);

    static f32 getEllipseY(f32 ellipse_a, f32 ellipse_b, f32 ellipse_x);

    static f32 fit_size(f32 srcw, f32 srch, f32 dstw, f32 dsth, s32 type);

    static f32 getDistanceFromPointToLine();
};


#endif //SV_MATHHELPER_H
