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

typedef struct V3_N_T_T0 {
    f32 x;
    f32 y;
    f32 z;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 tx;
    f32 ty;
    f32 tz;
    f32 t0x;
    f32 t0y;
} V3_N_T_T0;

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
    s16 bone0;
    s16 bone1;
    s16 bone2;
    s16 bone3;
    f32 weight0;
    f32 weight1;
    f32 weight2;
    f32 weight3;
} V3_N_T0_BONE4;

//GLTF
typedef struct V3_N_C_T0_B_W {
    f32 x;
    f32 y;
    f32 z;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 t0x;
    f32 t0y;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    s16 bone0;
    s16 bone1;
    s16 bone2;
    s16 bone3;
    f32 weight0;
    f32 weight1;
    f32 weight2;
    f32 weight3;
} V3_N_C_T0_B_W;

typedef struct C {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} C;

//
struct V3_PARTICLE {
    union {
        struct {
            f32 xyz[3];
            u32 parameters;
            f32 rgba[4];
        };
#ifdef USE_SSE
        __m128 vec;
#elif USE_ALTIVEC
        vec_float4 vec;
#elif USE_NEON
        float32x4_t vec;
#endif
    };
};

//
//顶点格式索引
#define D_VF_NULL 0x0000
#define D_VF_V2   0x0001
#define D_VF_V3   0x0002
#define D_VF_NOR  0x0010
#define D_VF_TAG  0x0020
#define D_VF_BTAG 0x0040
#define D_VF_C0   0x0100
#define D_VF_C1   0x0200
#define D_VF_C2   0x0400
#define D_VF_C3   0x0800
#define D_VF_T0       0x1000
#define D_VF_T1       0x2000
#define D_VF_T2       0x4000
#define D_VF_T3       0x8000
#define D_VF_BONE     0x00010000
#define D_VF_BONE_W   0x00020000
#define D_VF_PARTICLE 0x00100000
#define D_VF_INSOFFSET 0x00200000

enum VFTYPE {
    E_VF_BASE = 0,
    E_VF_V2 = D_VF_V2,
    E_VF_V2_T0 = D_VF_V2 | D_VF_T0,
    E_VF_V2_T0_T1 = D_VF_V2 | D_VF_T0 | D_VF_T1,
    E_VF_V2_T0_T1_T2 = D_VF_V2 | D_VF_T0 | D_VF_T1 | D_VF_T2,
    E_VF_V2_C = D_VF_V2 | D_VF_C0,
    E_VF_V2_C_T0 = D_VF_V2 | D_VF_C0 | D_VF_T0,
    E_VF_V2_C_T0_T1 = D_VF_V2 | D_VF_C0 | D_VF_T0 | D_VF_T1,
    E_VF_V3 = D_VF_V3,
    E_VF_V3_T0 = D_VF_V3 | D_VF_T0,
    E_VF_V3_T0_T1 = D_VF_V3 | D_VF_T0 | D_VF_T1,
    E_VF_V3_N = D_VF_V3 | D_VF_NOR,
    E_VF_V3_N_T0 = D_VF_V3 | D_VF_NOR | D_VF_T0,
    E_VF_V3_N_T0_T1 = D_VF_V3 | D_VF_NOR | D_VF_T0 | D_VF_T1,
    E_VF_V3_C = D_VF_V3 | D_VF_C0,
    E_VF_V3_C_T0 = D_VF_V3 | D_VF_C0 | D_VF_T0,
    E_VF_V3_C_T0_T1 = D_VF_V3 | D_VF_C0 | D_VF_T0 | D_VF_T1,
    E_VF_V3_N_C = D_VF_V3 | D_VF_NOR | D_VF_C0,
    E_VF_V3_N_C_T0 = D_VF_V3 | D_VF_NOR | D_VF_C0 | D_VF_T0,
    E_VF_V3_N_C_T0_T1 = D_VF_V3 | D_VF_NOR | D_VF_C0 | D_VF_T0 | D_VF_T1,
    E_VF_V3_N_T0_BONE_W = D_VF_V3 | D_VF_NOR | D_VF_T0 | D_VF_BONE |D_VF_BONE_W,
    E_VF_V3_N_C_T0_BONE_W = D_VF_V3 | D_VF_NOR | D_VF_C0 | D_VF_T0 | D_VF_BONE | D_VF_BONE_W,
    E_VF_V3_C_BTAG = D_VF_V3 | D_VF_C0 | D_VF_BTAG,
    E_VF_V3_N_T_T0 = D_VF_V3 | D_VF_NOR | D_VF_TAG | D_VF_T0,
    E_VF_V3_PARTICLE = D_VF_PARTICLE
};

//
enum DRAWMETHOD {
    E_DM_POINTS = 0x0000,
    E_DM_LINES = 0x0001,
    E_DM_LINE_LOOP = 0x0002,
    E_DM_LINE_STRIP = 0x0003,
    E_DM_TRIANGLES = 0x0004,
    E_DM_TRIANGLE_STRIP = 0x0005,
    E_DM_TRIANGLE_FAN = 0x0006
};

//
#endif //SV_VERTDEF_H
