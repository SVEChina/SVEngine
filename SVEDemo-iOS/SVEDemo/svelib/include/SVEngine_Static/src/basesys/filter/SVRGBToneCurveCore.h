//
// SVRGBToneCurveCore.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RGBTONECURVECORE_H
#define SV_RGBTONECURVECORE_H

#include "../../base/SVGBase.h"
#include "../../mtl/SVMtlDeclare.h"

typedef struct filterpoint{
        f32 x;                ///< 坐标点x轴的值
        f32 y;                ///< 坐标点y轴的值s
} FILTERPOINT, *SVFILTERPOINT;

void getPreparedSplineCurve(SVArray<FILTERPOINT>  mRgbCompositeControlPoints,
                            SVArray<FILTERPOINT>  mRedControlPoints,
                            SVArray<FILTERPOINT>  mGreenControlPoints,
                            SVArray<FILTERPOINT>  mBlueControlPoints);

#endif /* SV_RGBTONECURVECORE_H */
