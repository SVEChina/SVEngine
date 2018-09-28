#ifndef __AMATH_H__
#define __AMATH_H__

#ifdef USE_SSE2
	#include <emmintrin.h>
#elif USE_SSE
	#include <xmmintrin.h>
#elif USE_ALTIVEC
	#include <altivec.h>
	#include <vec_types.h>
#elif USE_NEON
	#include <arm_neon.h>
#endif

#ifdef __PPU__
	#include <ppu_intrinsics.h>
#endif

#include "SVBase.h"
#include "SVHalf.h"

//
#ifdef PI
	#undef PI
#endif

#ifdef DEG2RAD
	#undef DEG2RAD
#endif

#ifdef RAD2DEG
	#undef RAD2DEG
#endif

#ifdef EPSILON
	#undef EPSILON
#endif

#ifdef INFINITY
	#undef INFINITY
#endif

#ifdef min
	#undef min
#endif

#ifdef max
	#undef max
#endif

//
#define PI			3.141592654f
#define PI2			6.283185308f
#define PI05		1.570796327f
#define LOG2		0.693147181f
#define LOG10		2.302585093f
#define SQRT2		1.414213562f
#define DEG2RAD		(PI / 180.0f)
#define RAD2DEG		(180.0f / PI)
#define EPSILON		1e-6f
#define INFINITY	1e+9f

//
#ifdef USE_SSE
	#define _MM_PERM2_X		0
	#define _MM_PERM2_Y		1
	#define _MM_PERM2_Z		2
	#define _MM_PERM2_W		3
	#define _MM_PERM2(X,Y,Z,W) _MM_SHUFFLE(_MM_PERM2_ ## W,_MM_PERM2_ ## Z,_MM_PERM2_ ## Y,_MM_PERM2_ ## X)
	#define _MM_SWIZZLE(V,X,Y,Z,W) _mm_shuffle_ps(V,V,_MM_PERM2(X,Y,Z,W))
	#ifdef USE_SSE2
		#define _MM_PERM22(X,Y) _MM_SHUFFLE2(_MM_PERM2_ ## Y,_MM_PERM2_ ## X)
		#define _MM_SWIZZLE2(V,X,Y) _mm_shuffle_pd(V,V,_MM_PERM22(X,Y))
	#endif
#elif USE_ALTIVEC
	#define VEC_PERM4_LX 	0x00010203
	#define VEC_PERM4_LY 	0x04050607
	#define VEC_PERM4_LZ 	0x08090a0b
	#define VEC_PERM4_LW 	0x0c0d0e0f
	#define VEC_PERM4_RX 	0x10111213
	#define VEC_PERM4_RY 	0x14151617
	#define VEC_PERM4_RZ 	0x18191a1b
	#define VEC_PERM4_RW 	0x1c1d1e1f
	#define VEC_PERM4_LB0	0x0004080c
	#define VEC_PERM4_LB1	0x0105090d
	#define VEC_PERM4_LB2	0x02060a0e
	#define VEC_PERM4_LB3	0x03070b0f
	#define VEC_PERM4_RB0	0x1014181c
	#define VEC_PERM4_RB1	0x1115191d
	#define VEC_PERM4_RB2	0x12161a1e
	#define VEC_PERM4_RB3	0x13171b1f
	#define VEC_PERM4(X,Y,Z,W) (vec_uchar16) (vec_uint4) { VEC_PERM4_ ## X, VEC_PERM4_ ## Y, VEC_PERM4_ ## Z, VEC_PERM4_ ## W }
	#define VEC_PERM2(X,Y,Z,W) (vec_uchar16) (vec_uint4) { VEC_PERM4_L ## X, VEC_PERM4_L ## Y, VEC_PERM4_R ## Z, VEC_PERM4_R ## W }
	#define VEC_SWIZZLE(V,X,Y,Z,W) vec_perm(V,V,VEC_PERM2(X,Y,Z,W))
	#define VEC_FLOAT4(X,Y,Z,W) (vec_float4) { X, Y, Z, W }
	#define VEC_UINT4(X,Y,Z,W) (vec_uint4) { X, Y, Z, W }
#elif USE_NEON
	#if defined(_WINRT) || defined(_IOS)
		#define NEON_UINT2(X,Y) vset_lane_u32(Y,vdup_n_u32(X),1)
		#define NEON_UINT4(X,Y,Z,W) vsetq_lane_u32(W,vsetq_lane_u32(Z,vsetq_lane_u32(Y,vdupq_n_u32(X),1),2),3)
	#else
		#define NEON_UINT2(X,Y) { X, Y }
		#define NEON_UINT4(X,Y,Z,W) { X, Y, Z, W }
	#endif
#endif

//
#ifdef USE_DOUBLE
	#define Scalar			f64
	#define Vec2			DVec2
	#define Vec3			DVec3
	#define Vec4			DVec4
	#define Mat4			DMat4
	#define Vec2_zero		DVec2_zero
	#define Vec3_zero		DVec3_zero
	#define Vec4_zero		DVec4_zero
	#define Vec2_one		DVec2_one
	#define Vec3_one		DVec3_one
	#define Vec4_one		DVec4_one
	#define Vec2_epsilon	DVec2_epsilon
	#define Vec3_epsilon	DVec3_epsilon
	#define Vec4_epsilon	DVec4_epsilon
	#define Vec2_infinity	DVec2_infinity
	#define Vec3_infinity	DVec3_infinity
	#define Vec4_infinity	DVec4_infinity
	#define Mat4_identity	DMat4_identity
#else
	#define Scalar			f32
	#define Vec2			FVec2
	#define Vec3			FVec3
	#define Vec4			FVec4
	#define Mat4			FMat4
	#define Vec2_zero		FVec2_zero
	#define Vec3_zero		FVec3_zero
	#define Vec4_zero		FVec4_zero
	#define Vec2_one		FVec2_one
	#define Vec3_one		FVec3_one
	#define Vec4_one		FVec4_one
	#define Vec2_epsilon	FVec2_epsilon
	#define Vec3_epsilon	FVec3_epsilon
	#define Vec4_epsilon	FVec4_epsilon
	#define Vec2_infinity	FVec2_infinity
	#define Vec3_infinity	FVec3_infinity
	#define Vec4_infinity	FVec4_infinity
	#define Mat4_identity	FMat4_identity
#endif


namespace sv {
    
    namespace util {

        //************************************ Type conversion ******************************************
        //
        union IntFloat {
            IntFloat() { }
            IntFloat(s32 i) : i(i) { }
            IntFloat(f32 f) : f(f) { }
            IntFloat(u32 ui) : ui(ui) { }
            s32 i;
            f32 f;
            u32 ui;
        };
        
        //
        union IntDouble {
            IntDouble() { }
            IntDouble(f64 d) : d(d) { }
            s32 i[2];
            f64 d;
            u32 ui[2];
        };
        
    }//!namespace util
    
}//!namespace sv


#endif /* __MATH_LIB_H__ */
