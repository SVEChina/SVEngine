//
// SVMathHelper.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include <math.h>
#include <cstdlib>
#include "SVMathHelper.h"

f32 SVMathHelper::vec2Length(f32 _x1, f32 _y1, f32 _x2, f32 _y2) {
    return sqrtf((_x1 - _x2) * (_x1 - _x2) + (_y1 - _y2) * (_y1 - _y2));
}

f32 SVMathHelper::atan2fV2(V2 p1, V2 p2) {
    return atan2f(p1.y - p2.y, p1.x - p2.x);
}

V2 SVMathHelper::translateToUp(s32 x, s32 y, f32 roll, f32 len) {
    V2 t_point;
    t_point.x = x + len * cosf(roll);
    t_point.y = y + len * sinf(roll);
    return t_point;
}

V2 SVMathHelper::translateToLeft(s32 x, s32 y, f32 roll, f32 len) {
    V2 t_point;
    t_point.x = x - len * sinf(roll);
    t_point.y = y + len * cosf(roll);
    return t_point;
}

V2 SVMathHelper::translateToRight(s32 x, s32 y, f32 roll, f32 len) {
    V2 t_point;
    t_point.x = x + len * sinf(roll);;
    t_point.y = y - len * cosf(roll);;
    return t_point;
}

//三次贝塞尔曲线
static f32 bezier3funcX(f32 uu, V2 *controlP) {
    f32 part0 = controlP[0].x * uu * uu * uu;
    f32 part1 = 3 * controlP[1].x * uu * uu * (1 - uu);
    f32 part2 = 3 * controlP[2].x * uu * (1 - uu) * (1 - uu);
    f32 part3 = controlP[3].x * (1 - uu) * (1 - uu) * (1 - uu);
    return part0 + part1 + part2 + part3;
}

static f32 bezier3funcY(f32 uu, V2 *controlP) {
    f32 part0 = controlP[0].y * uu * uu * uu;
    f32 part1 = 3 * controlP[1].y * uu * uu * (1 - uu);
    f32 part2 = 3 * controlP[2].y * uu * (1 - uu) * (1 - uu);
    f32 part3 = controlP[3].y * (1 - uu) * (1 - uu) * (1 - uu);
    return part0 + part1 + part2 + part3;
}

void SVMathHelper::createNormalCurve(V2 *originpoint, int origincount, int step, SVArray<V2> &outcurve) {
    //控制点收缩系数 ，经调试0.6较好
    float scale = 0.6;
    V2 midpoints[origincount];
    //生成中点
    for (int i = 0; i < origincount; i++) {
        int nexti = (i + 1) % origincount;
        midpoints[i].x = (originpoint[i].x + originpoint[nexti].x) / 2.0;
        midpoints[i].y = (originpoint[i].y + originpoint[nexti].y) / 2.0;
    }
    //平移中点
    V2 extrapoints[2 * origincount];
    for (int i = 0; i < origincount; i++) {
        //        int nexti = (i + 1) % origincount;
        int backi = (i + origincount - 1) % origincount;
        V2 midinmid;
        midinmid.x = (midpoints[i].x + midpoints[backi].x) / 2.0;
        midinmid.y = (midpoints[i].y + midpoints[backi].y) / 2.0;
        int offsetx = originpoint[i].x - midinmid.x;
        int offsety = originpoint[i].y - midinmid.y;
        int extraindex = 2 * i;
        extrapoints[extraindex].x = midpoints[backi].x + offsetx;
        extrapoints[extraindex].y = midpoints[backi].y + offsety;
        //朝 originpoint[i]方向收缩
        int addx = (extrapoints[extraindex].x - originpoint[i].x) * scale;
        int addy = (extrapoints[extraindex].y - originpoint[i].y) * scale;
        extrapoints[extraindex].x = originpoint[i].x + addx;
        extrapoints[extraindex].y = originpoint[i].y + addy;
        
        int extranexti = (extraindex + 1) % (2 * origincount);
        extrapoints[extranexti].x = midpoints[i].x + offsetx;
        extrapoints[extranexti].y = midpoints[i].y + offsety;
        //朝 originpoint[i]方向收缩
        addx = (extrapoints[extranexti].x - originpoint[i].x) * scale;
        addy = (extrapoints[extranexti].y - originpoint[i].y) * scale;
        extrapoints[extranexti].x = originpoint[i].x + addx;
        extrapoints[extranexti].y = originpoint[i].y + addy;
    }
    
    V2 controlPoint[4];
    //生成4控制点，产生贝塞尔曲线
    for (int i = 0; i < origincount; i++) {
        controlPoint[0] = originpoint[i];
        int extraindex = 2 * i;
        controlPoint[1] = extrapoints[extraindex + 1];
        int extranexti = (extraindex + 2) % (2 * origincount);
        controlPoint[2] = extrapoints[extranexti];
        int nexti = (i + 1) % origincount;
        controlPoint[3] = originpoint[nexti];
        float u = 1;
        while (u > 0) {
            //u的步长决定曲线的疏密
            u -= 1.0 / (step+1);
            if (u == 0) {
                break;
            }
            int px = bezier3funcX(u, controlPoint);
            int py = bezier3funcY(u, controlPoint);
            V2 tempP;
            tempP.x = px;
            tempP.y = py;
            //存入曲线点
            outcurve.append(tempP);
        }
    }
}


void
SVMathHelper::createCurve(V2 *originpoint, s32 origincount, s32 step, SVArray<V2>& outcurve) {
    //控制点收缩系数 ，经调试0.6较好
    f32 scale = 0.6;
    V2 midpoints[origincount];
    //生成中点
    for (s32 i = 0; i < origincount; i++) {
        s32 nexti = (i + 1) % origincount;
        midpoints[i].x = (originpoint[i].x + originpoint[nexti].x) / 2.0;
        midpoints[i].y = (originpoint[i].y + originpoint[nexti].y) / 2.0;
    }
    //平移中点
    V2 extrapoints[2 * origincount];
    for (s32 i = 0; i < origincount; i++) {
        //        s32 nexti = (i + 1) % origincount;
        s32 backi = (i + origincount - 1) % origincount;
        V2 midinmid;
        midinmid.x = (midpoints[i].x + midpoints[backi].x) / 2.0;
        midinmid.y = (midpoints[i].y + midpoints[backi].y) / 2.0;
        s32 offsetx = originpoint[i].x - midinmid.x;
        s32 offsety = originpoint[i].y - midinmid.y;
        s32 extraindex = 2 * i;
        extrapoints[extraindex].x = midpoints[backi].x + offsetx;
        extrapoints[extraindex].y = midpoints[backi].y + offsety;
        //朝 originpoint[i]方向收缩
        s32 addx = (extrapoints[extraindex].x - originpoint[i].x) * scale;
        s32 addy = (extrapoints[extraindex].y - originpoint[i].y) * scale;
        extrapoints[extraindex].x = originpoint[i].x + addx;
        extrapoints[extraindex].y = originpoint[i].y + addy;
        
        s32 extranexti = (extraindex + 1) % (2 * origincount);
        extrapoints[extranexti].x = midpoints[i].x + offsetx;
        extrapoints[extranexti].y = midpoints[i].y + offsety;
        //朝 originpoint[i]方向收缩
        addx = (extrapoints[extranexti].x - originpoint[i].x) * scale;
        addy = (extrapoints[extranexti].y - originpoint[i].y) * scale;
        extrapoints[extranexti].x = originpoint[i].x + addx;
        extrapoints[extranexti].y = originpoint[i].y + addy;
    }
    
    V2 controlPoint[4];
    //生成4控制点，产生贝塞尔曲线
    for (s32 i = 0; i < origincount; i++) {
        controlPoint[0] = originpoint[i];
        s32 extraindex = 2 * i;
        controlPoint[1] = extrapoints[extraindex + 1];
        s32 extranexti = (extraindex + 2) % (2 * origincount);
        controlPoint[2] = extrapoints[extranexti];
        s32 nexti = (i + 1) % origincount;
        controlPoint[3] = originpoint[nexti];
        f32 u = 1;
        while (u > 0) {
            //u的步长决定曲线的疏密
            u -= 1.0 / (step+1);
            if (u == 0) {
                break;
            }
            s32 px = bezier3funcX(u, controlPoint);
            s32 py = bezier3funcY(u, controlPoint);
            V2 tempP;
            tempP.x = px;
            tempP.y = py;
            //存入曲线点
            outcurve.append(tempP);
        }
    }
}

f32 SVMathHelper::getEllipseY(f32 ellipse_a, f32 ellipse_b, f32 ellipse_x) {
    return ellipse_b * sqrtf((1 - (ellipse_x * ellipse_x) / (ellipse_a * ellipse_a)));
}


f32 SVMathHelper::fit_size(f32 srcw, f32 srch, f32 dstw, f32 dsth, s32 type) {
    f32 src_w_h_rate = srcw / srch;
    f32 dst_w_h_rate = dstw / dsth;
    f32 t_s_factor = 1.0f;
    if (type == 1) {
        //最小内接矩形
        if (src_w_h_rate > dst_w_h_rate) {
            t_s_factor = dstw / srcw;
        } else {
            t_s_factor = dsth / srch;
        }
    } else if (type == 2) {
        //最大外接矩形
        if (src_w_h_rate > dst_w_h_rate) {
            t_s_factor = dstw / srcw;
        } else {
            t_s_factor = dsth / srch;
        }
    }
    return t_s_factor;
}
