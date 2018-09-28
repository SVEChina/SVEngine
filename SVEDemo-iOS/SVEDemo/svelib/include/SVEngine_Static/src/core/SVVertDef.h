//
// SVVertDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_VERTDEF_H
#define SV_VERTDEF_H

#include "../base/SVObject.h"

typedef struct V2 {
    f32 x;
    f32 y;
} V2;

typedef struct V2_T0 {
    f32 x;
    f32 y;
    f32 t0x;
    f32 t0y;
} V2_T0;

typedef struct V2_T0_T1 {
    f32 x;
    f32 y;
    f32 t0x;
    f32 t0y;
    f32 t1x;
    f32 t1y;
} V2_T0_T1;

typedef struct V2_C {
    f32 x;
    f32 y;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} V2_C;

typedef struct V2_C_T0 {
    f32 x;
    f32 y;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    f32 t0x;
    f32 t0y;
} V2_C_T0;

typedef struct V2_C_T0_T1 {
    f32 x;
    f32 y;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    f32 t0x;
    f32 t0y;
    f32 t1x;
    f32 t1y;
} V2_C_T0_T1;

//
typedef struct V3 {
    f32 x;
    f32 y;
    f32 z;
} V3;

typedef struct V3_T0 {
    f32 x;
    f32 y;
    f32 z;
    f32 t0x;
    f32 t0y;
} V3_T0;

typedef struct V3_T0_T1 {
    f32 x;
    f32 y;
    f32 z;
    f32 t0x;
    f32 t0y;
    f32 t1x;
    f32 t1y;
} V3_T0_T1;

typedef struct V3_N {
    f32 x;
    f32 y;
    f32 z;
    f32 nx;
    f32 ny;
    f32 nz;
} V3_N;

typedef struct V3_N_T0 {
    f32 x;
    f32 y;
    f32 z;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 t0x;
    f32 t0y;
} V3_N_T0;

typedef struct V3_N_T0_T1 {
    f32 x;
    f32 y;
    f32 z;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 t0x;
    f32 t0y;
    f32 t1x;
    f32 t1y;
} V3_N_T0_T1;


typedef struct V3_C {
    f32 x;
    f32 y;
    f32 z;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} V3_C;

typedef struct V3_C_T0 {
    f32 x;
    f32 y;
    f32 z;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    f32 t0x;
    f32 t0y;
} V3_C_T0;

typedef struct V3_C_T0_T1 {
    f32 x;
    f32 y;
    f32 z;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    f32 t0x;
    f32 t0y;
    f32 t1x;
    f32 t1y;
} V3_C_T0_T1;

typedef struct V3_N_C {
    f32 x;
    f32 y;
    f32 z;
    f32 nx;
    f32 ny;
    f32 nz;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} V3_N_C;

typedef struct V3_N_C_T0 {
    f32 x;
    f32 y;
    f32 z;
    f32 nx;
    f32 ny;
    f32 nz;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    f32 t0x;
    f32 t0y;
} V3_N_C_T0;

typedef struct V3_N_C_T0_T1 {
    f32 x;
    f32 y;
    f32 z;
    f32 nx;
    f32 ny;
    f32 nz;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    f32 t0x;
    f32 t0y;
    f32 t1x;
    f32 t1y;
} V3_N_C_T0_T1;

typedef struct V3_N_T0_BONE4 {
    f32 x;
    f32 y;
    f32 z;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 t0x;
    f32 t0y;
    f32 bone0;
    f32 bone1;
    f32 bone2;
    f32 bone3;
    f32 weight0;
    f32 weight1;
    f32 weight2;
    f32 weight3;
} V3_N_T0_BONE4;

//顶点格式索引
#define VERTEX_FORMATE_V2       0x0001
#define VERTEX_FORMATE_V3       0x0002
#define VERTEX_FORMATE_NORMAL   0x0010
#define VERTEX_FORMATE_TANGENT  0x0020
#define VERTEX_FORMATE_BTANGENT 0x0040
#define VERTEX_FORMATE_COLOR0   0x0100
#define VERTEX_FORMATE_COLOR1   0x0200
#define VERTEX_FORMATE_COLOR2   0x0400
#define VERTEX_FORMATE_COLOR3   0x0800
#define VERTEX_FORMATE_T0       0x1000
#define VERTEX_FORMATE_T1       0x2000
#define VERTEX_FORMATE_T2       0x4000
#define VERTEX_FORMATE_T3       0x8000
#define VERTEX_FORMATE_BONE             0x00010000
#define VERTEX_FORMATE_BONE_WEIGHT      0x00020000

enum VFTYPE {
    E_VF_BASE = 0,
    E_VF_V2 = VERTEX_FORMATE_V2,
    E_VF_V2_T0 = VERTEX_FORMATE_V2 | VERTEX_FORMATE_T0,
    E_VF_V2_T0_T1 = VERTEX_FORMATE_V2 | VERTEX_FORMATE_T0 | VERTEX_FORMATE_T1,
    E_VF_V2_T0_T1_T2 =
    VERTEX_FORMATE_V2 | VERTEX_FORMATE_T0 | VERTEX_FORMATE_T1 | VERTEX_FORMATE_T2,
    E_VF_V2_C = VERTEX_FORMATE_V2 | VERTEX_FORMATE_COLOR0,
    E_VF_V2_C_T0 = VERTEX_FORMATE_V2 | VERTEX_FORMATE_COLOR0 | VERTEX_FORMATE_T0,
    E_VF_V2_C_T0_T1 =
    VERTEX_FORMATE_V2 | VERTEX_FORMATE_COLOR0 | VERTEX_FORMATE_T0 | VERTEX_FORMATE_T1,
    E_VF_V3 = VERTEX_FORMATE_V3,
    E_VF_V3_T0 = VERTEX_FORMATE_V3 | VERTEX_FORMATE_T0,
    E_VF_V3_T0_T1 = VERTEX_FORMATE_V3 | VERTEX_FORMATE_T0 | VERTEX_FORMATE_T1,
    E_VF_V3_N = VERTEX_FORMATE_V3 | VERTEX_FORMATE_NORMAL,
    E_VF_V3_N_T0 = VERTEX_FORMATE_V3 | VERTEX_FORMATE_NORMAL | VERTEX_FORMATE_T0,
    E_VF_V3_N_T0_T1 =
    VERTEX_FORMATE_V3 | VERTEX_FORMATE_NORMAL | VERTEX_FORMATE_T0 | VERTEX_FORMATE_T1,
    E_VF_V3_C = VERTEX_FORMATE_V3 | VERTEX_FORMATE_COLOR0,
    E_VF_V3_C_T0 = VERTEX_FORMATE_V3 | VERTEX_FORMATE_COLOR0 | VERTEX_FORMATE_T0,
    E_VF_V3_C_T0_T1 =
    VERTEX_FORMATE_V3 | VERTEX_FORMATE_COLOR0 | VERTEX_FORMATE_T0 | VERTEX_FORMATE_T1,
    E_VF_V3_N_C = VERTEX_FORMATE_V3 | VERTEX_FORMATE_NORMAL | VERTEX_FORMATE_COLOR0,
    E_VF_V3_N_C_T0 =
    VERTEX_FORMATE_V3 | VERTEX_FORMATE_NORMAL | VERTEX_FORMATE_COLOR0 | VERTEX_FORMATE_T0,
    E_VF_V3_N_C_T0_T1 =
    VERTEX_FORMATE_V3 | VERTEX_FORMATE_NORMAL | VERTEX_FORMATE_COLOR0 | VERTEX_FORMATE_T0 |
    VERTEX_FORMATE_T1,
    E_VF_V3_N_T0_BONE_WEIGHT =
    VERTEX_FORMATE_V3 | VERTEX_FORMATE_NORMAL | VERTEX_FORMATE_T0 | VERTEX_FORMATE_BONE |
    VERTEX_FORMATE_BONE_WEIGHT
};

enum DRAWMETHOD {
    E_DM_POINTS = 0x0000,
    E_DM_LINES = 0x0001,
    E_DM_LINE_LOOP = 0x0002,
    E_DM_LINE_STRIP = 0x0003,
    E_DM_TRIANGLES = 0x0004,
    E_DM_TRIANGLE_STRIP = 0x0005,
    E_DM_TRIANGLE_FAN = 0x0006,
};





//
#endif //SV_VERTDEF_H
