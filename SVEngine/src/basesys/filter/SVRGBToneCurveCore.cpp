//
// SVRGBToneCurveCore.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRGBToneCurveCore.h"


u16 intSwap16(u16 var){
    return (u16)(((var << 8) & 0xFF00) | ((var >> 8) & 0xFF));
}

void createSecondDerivative(SVArray<V2> points ,SVArray<f64> *outarray){
    int n = points.size();
    if (n <= 1) {
        return ;
    }
    double matrix[n][3];
    double result[n];
    matrix[0][1] = 1;
    // What about matrix[0][1] and matrix[0][0]? Assuming 0 for now (Brad L.)
    matrix[0][0] = 0;
    matrix[0][2] = 0;
    for (s32 i = 1; i < n - 1; i++) {
        V2 P1 = points[i - 1];
        V2 P2 = points[i];
        V2 P3 = points[i + 1];
        matrix[i][0] = (f64) (P2.x - P1.x) / 6;
        matrix[i][1] = (f64) (P3.x - P1.x) / 3;
        matrix[i][2] = (f64) (P3.x - P2.x) / 6;
        result[i] = (f64) (P3.y - P2.y) / (P3.x - P2.x) - (f64) (P2.y - P1.y) / (P2.x - P1.x);
    }
    // What about result[0] and result[n-1]? Assuming 0 for now (Brad L.)
    result[0] = 0;
    result[n - 1] = 0;
    matrix[n - 1][1] = 1;
    // What about matrix[n-1][0] and matrix[n-1][2]? For now, assuming they are 0 (Brad L.)
    matrix[n - 1][0] = 0;
    matrix[n - 1][2] = 0;
    // solving pass1 (up->down)
    for (int i = 1; i < n; i++) {
        double k = matrix[i][0] / matrix[i - 1][1];
        matrix[i][1] -= k * matrix[i - 1][2];
        matrix[i][0] = 0;
        result[i] -= k * result[i - 1];
    }
    // solving pass2 (down->up)
    for (int i = n - 2; i >= 0; i--) {
        double k = matrix[i][2] / matrix[i + 1][1];
        matrix[i][1] -= k * matrix[i + 1][0];
        matrix[i][2] = 0;
        result[i] -= k * result[i + 1];
    }
    for (int i = 0; i < n; i++)
        outarray->append(result[i] / matrix[i][1]);
    }


void createSplineCurve2(SVArray<V2> points,SVArray<V2> *output ) {
    SVArray<f64> sdA;
    createSecondDerivative(points,&sdA);
    // Is [points count] equal to [sdA count]?
    //    int n = [points count];
    int n = sdA.size();
    if (n < 1) {
        return ;
    }
    f64 sd[n] ;
    // From NSMutableArray to sd[n];
    for (int i = 0; i < n; i++) {
        sd[i] = sdA[i];
    }
    for (int i = 0; i < n - 1; i++) {
        V2 cur = points[i];
        V2 next = points[i + 1];
        for (int x = cur.x; x < (int)next.x; x++) {
            f64 t = (f64) (x - cur.x) / (next.x - cur.x);
            f64 a = 1 - t;
            f64 b = t;
            f64 h = next.x - cur.x;
            f64 y = a * cur.y + b * next.y + (h * h / 6) * ((a * a * a - a) * sd[i] + (b * b * b - b) * sd[i + 1]);
            if (y > 255.0) {
                y = 255.0;
            } else if (y < 0.0) {
                y = 0.0;
            }
            V2 t_point;
            t_point.x=x;
            t_point.y=(s32)round(y);
            output->append(t_point);
        }
    }
    // If the last point is (255, 255) it doesn't get added.
    if (output->size() == 255) {
        output->append(points[points.size() - 1]);
    }
}

void createSplineCurve(SVArray<V2> points,SVArray<f32> *preparedSplinePoints) {
    if (points.size() <= 0) {
        return ;
    }

    SVArray<V2> pointsSorted = points;
    SVArray<V2> convertedPoints;
    for (s32 i = 0; i < points.size(); i++) {
        V2 point = pointsSorted[i];
        V2 point01;
        point01.x=(s32) (point.x) ;
        point01.y=(s32) (point.y );
        convertedPoints.append(point01);
    }
    SVArray<V2> splinePoints ;
    createSplineCurve2(convertedPoints,&splinePoints);
    V2 firstSplinePoint = splinePoints[0];
    if (firstSplinePoint.x > 0) {
        for (s32 i = firstSplinePoint.x; i >= 0; i--) {
            V2 t_point;
            t_point.x=i;
            t_point.y=0;
            splinePoints.append(t_point);
        }
    }
    V2 lastSplinePoint = splinePoints[splinePoints.size() - 1];
    if (lastSplinePoint.x < 255) {
        for (s32 i = lastSplinePoint.x + 1; i <= 255; i++) {
            V2 t_point;
            t_point.x=i;
            t_point.y=255;
            splinePoints.append(t_point);
        }
    }
    for(s32 i=0;i<splinePoints.size();i++){
        V2 origPoint;
        origPoint.x=splinePoints[i].x;
        origPoint.y=splinePoints[i].x;
        f32 distance = (f32)sqrt(pow((origPoint.x - splinePoints[i].x), 2.0) + pow((origPoint.y - splinePoints[i].y), 2.0));
        if (origPoint.y > splinePoints[i].y) {
            distance = -distance;
        }
        preparedSplinePoints->append(distance);
    }
}

void getPreparedSplineCurve(SVArray<V2>  mRgbCompositeControlPoints,
                            SVArray<V2>  mRedControlPoints,
                            SVArray<V2>  mGreenControlPoints,
                            SVArray<V2>  mBlueControlPoints){
    SVArray<f32> outPointsRGB;
    SVArray<f32> outPointsR;
    SVArray<f32> outPointsG;
    SVArray<f32> outPointsB;
    createSplineCurve(mRgbCompositeControlPoints, &outPointsRGB);
    createSplineCurve(mRedControlPoints, &outPointsR);
    createSplineCurve(mGreenControlPoints, &outPointsG);
    createSplineCurve(mBlueControlPoints, &outPointsB);
    unsigned char* toneCurveByteArray = (unsigned char*)malloc(256*4);
    if ( (outPointsRGB.size() >= 256) && (outPointsR.size() >= 256) && (outPointsG.size() >= 256) && (outPointsB.size() >= 256)){
        for (unsigned int currentCurveIndex = 0; currentCurveIndex < 256; currentCurveIndex++)
        {
            // BGRA for upload to texture
            unsigned   char r = fmin(fmax(currentCurveIndex + outPointsR[currentCurveIndex], 0), 255);
            toneCurveByteArray[currentCurveIndex * 4 ] = fmin(fmax(r +outPointsRGB[r], 0), 255);
            unsigned char g = fmin(fmax(currentCurveIndex + outPointsG[currentCurveIndex], 0), 255);
            toneCurveByteArray[currentCurveIndex * 4+1] = fmin(fmax(g +outPointsRGB[g], 0), 255);
            unsigned char b = fmin(fmax(currentCurveIndex +outPointsB[currentCurveIndex], 0), 255);
            toneCurveByteArray[currentCurveIndex * 4+2] = fmin(fmax(b + outPointsRGB[b], 0), 255);
            toneCurveByteArray[currentCurveIndex * 4 + 3] = 255;
            SV_LOG_INFO("rgba %d\n",(unsigned char)toneCurveByteArray[currentCurveIndex * 4 ]);
        }
    }
}


