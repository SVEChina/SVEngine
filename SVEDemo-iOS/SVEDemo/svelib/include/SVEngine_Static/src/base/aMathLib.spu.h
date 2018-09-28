#ifndef __AMATH_LIB_SPU_H__
#define __AMATH_LIB_SPU_H__

#include "Base.spu.h"
#include "SVHalf.h"

//
#ifdef INFINITY
	#undef INFINITY
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
#define SPU_PERM4_LX 	0x00010203
#define SPU_PERM4_LY 	0x04050607
#define SPU_PERM4_LZ 	0x08090a0b
#define SPU_PERM4_LW 	0x0c0d0e0f
#define SPU_PERM4_RX 	0x10111213
#define SPU_PERM4_RY 	0x14151617
#define SPU_PERM4_RZ 	0x18191a1b
#define SPU_PERM4_RW 	0x1c1d1e1f
#define SPU_PERM4_LB0	0x0004080c
#define SPU_PERM4_LB1	0x0105090d
#define SPU_PERM4_LB2	0x02060a0e
#define SPU_PERM4_LB3	0x03070b0f
#define SPU_PERM4_RB0	0x1014181c
#define SPU_PERM4_RB1	0x1115191d
#define SPU_PERM4_RB2	0x12161a1e
#define SPU_PERM4_RB3	0x13171b1f
#define SPU_PERM4_LPXY	0x02030607
#define SPU_PERM4_LPZW	0x0a0b0e0f
#define SPU_PERM4_RPXY	0x12131617
#define SPU_PERM4_RPZW	0x1a1b1e1f
#define SPU_PERM4_LULX	0x80800001
#define SPU_PERM4_LULY	0x80800203
#define SPU_PERM4_LULZ	0x80800405
#define SPU_PERM4_LULW	0x80800607
#define SPU_PERM4_RULX	0x80801011
#define SPU_PERM4_RULY	0x80801213
#define SPU_PERM4_RULZ	0x80801415
#define SPU_PERM4_RULW	0x80801617
#define SPU_PERM4_LURX	0x80800809
#define SPU_PERM4_LURY	0x80800a0b
#define SPU_PERM4_LURZ	0x80800c0d
#define SPU_PERM4_LURW	0x80800e0f
#define SPU_PERM4_RURX	0x80801819
#define SPU_PERM4_RURY	0x80801a1b
#define SPU_PERM4_RURZ	0x80801c1d
#define SPU_PERM4_RURW	0x80801e1f
#define SPU_PERM4(X,Y,Z,W) (vec_uchar16) (vec_uint4) { SPU_PERM4_ ## X, SPU_PERM4_ ## Y, SPU_PERM4_ ## Z, SPU_PERM4_ ## W }
#define SPU_PERM2(X,Y,Z,W) (vec_uchar16) (vec_uint4) { SPU_PERM4_L ## X, SPU_PERM4_L ## Y, SPU_PERM4_R ## Z, SPU_PERM4_R ## W }
#define SPU_SWIZZLE(V,X,Y,Z,W) spu_shuffle(V,V,SPU_PERM2(X,Y,Z,W))
#define SPU_FLOAT4(X,Y,Z,W) (vec_float4) { X, Y, Z, W }
#define SPU_UINT4(X,Y,Z,W) (vec_uint4) { X, Y, Z, W }

namespace sv {
    
    namespace util {
        
        //
        struct FVec3;
        struct FVec4;
        struct FMat3;
        struct FMat4;
        struct SVQuat;

        //
        sv_inline vec_float4 spu_rcp_nr(vec_float4 v);
        sv_inline vec_float4 spu_rsqrt_nr(vec_float4 v);

        /******************************************************************************\
        *
        * Type conversion
        *
        \******************************************************************************/

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

        /******************************************************************************\
        *
        * MathLib
        *
        \******************************************************************************/

        //
        class Math {
            
                Math() { }
            
            public:
            
                // functions
                static s32 signMask(s32 v);
                static f32 sign(f32 v);
                static vec_float4 sign4(vec_float4 v);
            
                static s32 abs(s32 v);
                static f32 abs(f32 v);
                static vec_float4 abs4(vec_float4 v);
            
                static f32 ceil(f32 v);
                static vec_float4 ceil4(vec_float4 v);
            
                static f32 floor(f32 v);
                static vec_float4 floor4(vec_float4 v);
            
                static f32 frac(f32 v);
                static vec_float4 frac4(vec_float4 v);
            
                static f32 sqrt(f32 v);
                static vec_float4 sqrt4(vec_float4 v);
            
                static f32 rcp(f32 v);
                static vec_float4 rcp4(vec_float4 v);
            
                static f32 rsqrt(f32 v);
                static f32 rsqrtFast(f32 v);
                static vec_float4 rsqrt4(vec_float4 v);
                static vec_float4 rsqrtFast4(vec_float4 v);
            
                static f32 mod(f32 x,f32 y);
                static vec_float4 mod(vec_float4 x,vec_float4 y);
            
                static f32 pow(f32 x,f32 y);
                static f32 powFast(f32 x,f32 y);
                static vec_float4 pow4(vec_float4 x,vec_float4 y);
            
                static f32 exp(f32 v);
                static f32 expFast(f32 v);
                static vec_float4 exp4(vec_float4 v);
            
                static f32 exp2(f32 v);
                static f32 exp2Fast(f32 v);
                static vec_float4 exp24(vec_float4 v);
            
                static f32 log(f32 v);
                static f32 logFast(f32 v);
                static vec_float4 log4(vec_float4 v);
            
                static f32 log2(f32 v);
                static f32 log2Fast(f32 v);
                static vec_float4 log24(vec_float4 v);
            
                static f32 log10(f32 v);
                static vec_float4 log104(vec_float4 v);
            
                // trigonometry
                static f32 sin(f32 a);
                static vec_float4 sin4(vec_float4 a);
            
                static f32 cos(f32 a);
                static vec_float4 cos4(vec_float4 a);
            
                static f32 tan(f32 a);
                static vec_float4 tan4(vec_float4 a);
            
                static f32 asin(f32 v);
                static vec_float4 asin4(vec_float4 v);
            
                static f32 acos(f32 v);
                static vec_float4 acos4(vec_float4 v);
            
                static f32 atan(f32 v);
                static vec_float4 atan4(vec_float4 v);
            
                static f32 atan2(f32 y,f32 x);
                static vec_float4 atan24(vec_float4 y,vec_float4 x);
            
                static void sincos(f32 a,f32 &s,f32 &c);
                static void sincosFast(f32 a,f32 &s,f32 &c);
                static void sincos4(vec_float4 a,vec_float4 &s,vec_float4 &c);
                static void sincosFast4(vec_float4 a,vec_float4 &s,vec_float4 &c);
            
                // branching
                static s32 select(s32 c,s32 v0,s32 v1);
                static f32 select(s32 c,f32 v0,f32 v1);
                static f32 select(f32 c,f32 v0,f32 v1);
            
                // conversion
                static f32 itof(s32 v);
                static s32 ftoi(f32 v);
                static s32 round(f32 v);
        };

        //
        sv_inline s32 Math::signMask(s32 v) {
            return (v >> 31);
        }

        sv_inline f32 Math::sign(f32 v) {
            return IntFloat((IntFloat(v).ui & 0x80000000) | 0x3f800000).f;
        }

        sv_inline vec_float4 Math::sign4(vec_float4 v) {
            return (vec_float4)spu_or(spu_and((vec_uint4)v,0x80000000),0x3f800000);
        }

        //
        sv_inline s32 Math::abs(s32 v) {
            return ::abs(v);
        }

        sv_inline f32 Math::abs(f32 v) {
            return IntFloat(IntFloat(v).ui & 0x7fffffff).f;
        }

        sv_inline vec_float4 Math::abs4(vec_float4 v) {
            return ::fabsf4(v);
        }

        //
        sv_inline f32 Math::ceil(f32 v) {
            return ::ceilf(v);
        }

        sv_inline vec_float4 Math::ceil4(vec_float4 v) {
            return ::ceilf4(v);
        }

        //
        sv_inline f32 Math::floor(f32 v) {
            return ::floorf(v);
        }

        sv_inline vec_float4 Math::floor4(vec_float4 v) {
            return ::floorf4(v);
        }

        //
        sv_inline f32 Math::frac(f32 v) {
            return v - ::floorf(v);
        }

        sv_inline vec_float4 Math::frac4(vec_float4 v) {
            return spu_sub(v,::floorf4(v));
        }

        //
        sv_inline f32 Math::sqrt(f32 v) {
            return ::sqrtf(v);
        }

        sv_inline vec_float4 Math::sqrt4(vec_float4 v) {
            return ::sqrtf4(v);
        }

        //
        sv_inline f32 Math::rcp(f32 v) {
            return 1.0f / v;
        }

        sv_inline vec_float4 Math::rcp4(vec_float4 v) {
            return spu_rcp_nr(v);
        }

        //
        sv_inline f32 Math::rsqrt(f32 v) {
            if(v < 1e-8f) return INFINITY;
            return 1.0f / ::sqrtf(v);
        }

        sv_inline f32 Math::rsqrtFast(f32 v) {
            IntFloat i = v;
            i.i = 0x5f3759df - (i.i >> 1);
            return i.f * (1.5f - (i.f * i.f * v * 0.5f));
        }

        sv_inline vec_float4 Math::rsqrt4(vec_float4 v) {
            return spu_rsqrt_nr(v);
        }

        sv_inline vec_float4 Math::rsqrtFast4(vec_float4 v) {
            return spu_rsqrte(v);
        }

        //
        sv_inline f32 Math::pow(f32 x,f32 y) {
            return ::powf(x,y);
        }

        sv_inline f32 Math::powFast(f32 x,f32 y) {
            return Math::exp2Fast(Math::log2Fast(x) * y);
        }

        sv_inline vec_float4 Math::pow4(vec_float4 x,vec_float4 y) {
            return ::powf4(x,y);
        }

        //
        sv_inline f32 Math::mod(f32 x,f32 y) {
            return ::fmodf(x,y);
        }

        sv_inline vec_float4 Math::mod(vec_float4 x,vec_float4 y) {
            return ::fmodf4(x,y);
        }

        //
        sv_inline f32 Math::exp(f32 v) {
            return ::expf(v);
        }

        sv_inline f32 Math::expFast(f32 v) {
            return exp2Fast(v * (1.0f / LOG2));
        }

        sv_inline vec_float4 Math::exp4(vec_float4 v) {
            return ::expf4(v);
        }

        //
        sv_inline f32 Math::exp2(f32 v) {
            return ::exp2f(v);
        }

        sv_inline f32 Math::exp2Fast(f32 v) {
            s32 i = ftoi(v - 0.5f);
            v = v - itof(i);
            return IntFloat((i + 127) << 23).f * (((((0.0018775767f * v + 0.0089893397f) * v + 0.055826318f) * v + 0.24015361f) * v + 0.69315308f) * v + 0.99999994f);
        }

        sv_inline vec_float4 Math::exp24(vec_float4 v) {
            return ::exp2f4(v);
        }

        //
        sv_inline f32 Math::log(f32 v) {
            return ::logf(v);
        }

        sv_inline f32 Math::logFast(f32 v) {
            return log2Fast(v) * LOG2;
        }

        sv_inline vec_float4 Math::log4(vec_float4 v) {
            return ::logf4(v);
        }

        //
        sv_inline f32 Math::log2(f32 v) {
            return ::log2f(v);
        }

        sv_inline f32 Math::log2Fast(f32 v) {
            s32 i = IntFloat(v).i;
            s32 e = ((i >> 23) & 0xff) - 127;
            v = IntFloat((i & 0x007fffff) | 0x3f800000).f;
            return itof(e) + (v - 1.0f) * (((((-0.034436006f * v + 0.31821337f) * v - 1.2315303f) * v + 2.5988452f) * v - 3.3241990f) * v + 3.1157899f);
        }

        sv_inline vec_float4 Math::log24(vec_float4 v) {
            return ::log2f4(v);
        }

        //
        sv_inline f32 Math::log10(f32 v) {
            return ::log10f(v);
        }

        sv_inline vec_float4 Math::log104(vec_float4 v) {
            return ::log10f4(v);
        }

        //
        sv_inline f32 Math::sin(f32 a) {
            return ::sinf(a);
        }

        sv_inline vec_float4 Math::sin4(vec_float4 a) {
            return ::sinf4(a);
        }

        //
        sv_inline f32 Math::cos(f32 a) {
            return ::cosf(a);
        }

        sv_inline vec_float4 Math::cos4(vec_float4 a) {
            return ::cosf4(a);
        }

        //
        sv_inline f32 Math::tan(f32 a) {
            return ::tanf(a);
        }

        sv_inline vec_float4 Math::tan4(vec_float4 a) {
            return ::tanf4(a);
        }

        //
        sv_inline f32 Math::asin(f32 v) {
            return ::asinf(v);
        }

        sv_inline vec_float4 Math::asin4(vec_float4 v) {
            return ::asinf4(v);
        }

        //
        sv_inline f32 Math::acos(f32 v) {
            return ::acosf(v);
        }

        sv_inline vec_float4 Math::acos4(vec_float4 v) {
            return ::acosf4(v);
        }

        //
        sv_inline f32 Math::atan(f32 v) {
            return ::atanf(v);
        }

        sv_inline vec_float4 Math::atan4(vec_float4 v) {
            return ::atanf4(v);
        }

        //
        sv_inline f32 Math::atan2(f32 y,f32 x) {
            return ::atan2f(y,x);
        }

        sv_inline vec_float4 Math::atan24(vec_float4 y,vec_float4 x) {
            return ::atan2f4(y,x);
        }

        //
        sv_inline void Math::sincos(f32 a,f32 &s,f32 &c) {
            s = Math::sin(a);
            c = Math::cos(a);
        }

        sv_inline void Math::sincosFast(f32 a,f32 &s,f32 &c) {
            if(a < 0.0f) a -= Math::ftoi(a * (1.0f / PI2)) * PI2 - PI2;
            else if(a >= PI2) a -= Math::ftoi(a * (1.0f / PI2)) * PI2;
            c = 1.0f;
            s = PI - a;
            if(s < -PI05) s = -PI - s;
            else if(s > PI05) s = PI - s;
            else c = -1.0f;
            f32 a2 = s * s;
            s *= ((0.00761f * a2 - 0.16605f) * a2 + 1.0f);
            c *= ((0.03705f * a2 - 0.49670f) * a2 + 1.0f);
        }

        sv_inline void Math::sincos4(vec_float4 a,vec_float4 &s,vec_float4 &c) {
            ::sincosf4(a,&s,&c);
        }

        sv_inline void Math::sincosFast4(vec_float4 a,vec_float4 &s,vec_float4 &c) {
            vec_float4 one = spu_splats(1.0f);
            vec_float4 ia = floorf4(spu_mul(a,spu_splats(1.0f / PI2)));
            a = spu_sub(a,spu_mul(ia,spu_splats(PI2)));
            s = spu_sub(spu_splats(PI),a);
            vec_uint4 sign = spu_and((vec_uint4)s,0x80000000);
            vec_uint4 mask = spu_cmpabsgt(s,spu_splats(PI05));
            vec_float4 is = spu_sub((vec_float4)spu_or((vec_uint4)spu_splats(PI),sign),s);
            s = spu_sel(s,is,mask);
            c = spu_sel(spu_splats(-1.0f),one,mask);
            vec_float4 a2 = spu_mul(s,s);
            s = spu_mul(s,spu_madd(spu_madd(spu_splats(7.610e-03f),a2,spu_splats(-1.6605e-01f)),a2,one));
            c = spu_mul(c,spu_madd(spu_madd(spu_splats(3.705e-02f),a2,spu_splats(-4.9670e-01f)),a2,one));
        }

        //
        sv_inline s32 Math::select(s32 c,s32 v0,s32 v1) {
            s32 mask = Math::signMask(c | -c);
            return (v0 & mask) | (v1 & ~mask);
        }

        sv_inline f32 Math::select(s32 c,f32 v0,f32 v1) {
            s32 mask = Math::signMask(c | -c);
            return IntFloat((IntFloat(v0).i & mask) | (IntFloat(v1).i & ~mask)).f;
        }

        sv_inline f32 Math::select(f32 c,f32 v0,f32 v1) {
            s32 mask = Math::signMask(IntFloat(c).i);
            return IntFloat((IntFloat(v0).i & mask) | (IntFloat(v1).i & ~mask)).f;
        }

        //
        sv_inline f32 Math::itof(s32 v) {
            return static_cast<f32>(v);
        }

        sv_inline s32 Math::ftoi(f32 v) {
            return static_cast<s32>(v);
        }

        sv_inline s32 Math::round(f32 v) {
            return static_cast<s32>(v + 0.5f);
        }

        /******************************************************************************\
        *
        * Scalars
        *
        \******************************************************************************/

        //
        sv_inline s32 compare(f32 v0,f32 v1) {
            f32 v = Math::abs(v0 - v1);
            return (v < EPSILON);
        }

        sv_inline s32 compare(f32 v0,f32 v1,f32 epsilon) {
            f32 v = Math::abs(v0 - v1);
            return (v < (Math::abs(v0) + Math::abs(v1) + 1.0f) * epsilon);
        }

        //
        sv_inline f32 min(f32 v0,f32 v1) {
            return (v0 < v1) ? v0 : v1;
        }

        sv_inline f32 max(f32 v0,f32 v1) {
            return (v0 > v1) ? v0 : v1;
        }

        sv_inline f32 clamp(f32 v,f32 v0,f32 v1) {
            if(v < v0) return v0;
            if(v > v1) return v1;
            return v;
        }

        sv_inline f32 saturate(f32 v) {
            if(v < 0.0f) return 0.0f;
            if(v > 1.0f) return 1.0f;
            return v;
        }

        sv_inline f32 lerp(f32 v0,f32 v1,f32 k) {
            return v0 + (v1 - v0) * k;
        }

        //
        sv_inline s32 min(s32 v0,s32 v1) {
            s32 i = v1 - v0;
            i &= ~Math::signMask(i);
            return v1 - i;
        }

        sv_inline s32 max(s32 v0,s32 v1) {
            s32 i = v1 - v0;
            i &= ~Math::signMask(i);
            return v0 + i;
        }

        sv_inline s32 clamp(s32 v,s32 v0,s32 v1) {
            s32 i = v0 - v;
            i &= ~Math::signMask(i);
            i = v1 - v - i;
            i &= ~Math::signMask(i);
            return v1 - i;
        }

        /******************************************************************************\
        *
        * Vectors
        *
        \******************************************************************************/

        //
        sv_inline vec_float4 spu_rcp_nr(vec_float4 v) {
            vec_float4 iv = spu_re(v);
            vec_float4 one = spu_splats(1.0f);
            iv = spu_madd(spu_nmsub(iv,v,one),iv,iv);
            return spu_madd(spu_nmsub(iv,v,one),iv,iv);
        }

        //
        sv_inline vec_float4 spu_rsqrt_nr(vec_float4 v) {
            vec_float4 iv = spu_rsqrte(v);
            vec_float4 nr = spu_mul(spu_mul(v,iv),iv);
            return spu_mul(spu_mul(spu_splats(0.5f),iv),spu_sub(spu_splats(3.0f),nr));
        }

        //
        sv_inline vec_float4 spu_dot33(vec_float4 v0,vec_float4 v1) {
            vec_float4 v2 = spu_mul(v0,v1);
            vec_float4 v3 = spu_add(v2,SPU_SWIZZLE(v2,Y,X,Y,W));
            return spu_add(v3,SPU_SWIZZLE(v2,Z,Z,X,W));
        }

        sv_inline vec_float4 spu_dot44(vec_float4 v0,vec_float4 v1) {
            vec_float4 v2 = spu_mul(v0,v1);
            v2 = spu_add(v2,spu_rlqwbyte(v2,8));
            return spu_add(v2,spu_rlqwbyte(v2,4));
        }

        //
        sv_inline vec_float4 spu_normalize3(vec_float4 v) {
            vec_float4 ilength = spu_rsqrt_nr(spu_dot33(v,v));
            return spu_mul(v,ilength);
        }

        sv_inline vec_float4 spu_normalize4(vec_float4 v) {
            vec_float4 ilength = spu_rsqrt_nr(spu_dot44(v,v));
            return spu_mul(v,ilength);
        }

        //
        sv_inline vec_float4 spu_cross(vec_float4 v0,vec_float4 v1) {
            vec_uchar16 yzxw = SPU_PERM2(Y,Z,X,W);
            vec_float4 v0_yzxw = spu_shuffle(v0,v0,yzxw);
            vec_float4 v1_yzxw = spu_shuffle(v1,v1,yzxw);
            vec_float4 v2 = spu_nmsub(v1,v0_yzxw,spu_mul(v0,v1_yzxw));
            return spu_shuffle(v2,v2,yzxw);
        }

        //
        sv_inline vec_float4 spu_half_to_float(vec_uint4 v) {
            vec_uint4 em = spu_and(v,0x7fff);
            vec_uint4 fn = spu_add(spu_rl(em,13),(127 - 15) << 23);
            vec_uint4 fd = (vec_uint4)spu_convtf(em,24);
            vec_uint4 f = spu_sel(fd,fn,spu_cmpgt(em,spu_splats((u32)0x03ff)));
            return (vec_float4)spu_sel(f,spu_rl(v,16),spu_splats((u32)0x80000000));
        }

        sv_inline vec_uint4 spu_float_to_half(vec_float4 v) {
            vec_uint4 i = (vec_uint4)v;
            vec_uint4 e = spu_and(spu_rl(i,32 - 23),0x00ff);
            vec_uint4 m = spu_and(i,0x007fffff);
            vec_uint4 hn = spu_and(spu_rl(i,32 - 13),0x3fff);
            vec_uint4 hd = spu_rlmask(spu_or(m,0x00800000),spu_sub((vec_int4)e,spu_splats(127 - 1)));
            vec_uint4 h = spu_sel(hn,hd,spu_cmpgt(spu_splats((u32)(127 - 14)),e));
            return spu_sel(h,spu_rl(i,16),spu_splats((u32)0xc000));
        }

        /******************************************************************************\
        *
        * FVec3
        *
        \******************************************************************************/

        //
        ATTRIBUTE_ALIGNED16(struct) FVec3 {
            
            sv_inline FVec3() { }
            sv_inline FVec3(const FVec3 &v) : vec(v.vec) { }
            sv_inline FVec3(f32 x,f32 y,f32 z) : x(x), y(y), z(z), w(0.0f) { }
            explicit sv_inline FVec3(f32 v) : x(v), y(v), z(v), w(0.0f) { }
            explicit sv_inline FVec3(const FVec4 &v);
            explicit sv_inline FVec3(vec_float4 v) : vec(v) { }
            
            sv_inline FVec3 &operator=(const FVec3 &v) {
                vec = v.vec;
                return *this;
            }
            sv_inline FVec3 operator-() const {
                return FVec3((vec_float4)spu_xor((vec_uint4)vec,0x80000000));
            }
            sv_inline FVec3 &operator*=(f32 v) {
                vec = spu_mul(vec,spu_splats(v));
                return *this;
            }
            sv_inline FVec3 &operator*=(const FVec3 &v) {
                vec = spu_mul(vec,v.vec);
                return *this;
            }
            sv_inline FVec3 &operator/=(f32 v) {
                vec = spu_mul(vec,spu_splats(Math::rcp(v)));
                return *this;
            }
            sv_inline FVec3 &operator/=(const FVec3 &v) {
                vec = spu_mul(vec,Math::rcp4(v.vec));
                return *this;
            }
            sv_inline FVec3 &operator+=(const FVec3 &v) {
                vec = spu_add(vec,v.vec);
                return *this;
            }
            sv_inline FVec3 &operator-=(const FVec3 &v) {
                vec = spu_sub(vec,v.vec);
                return *this;
            }
            
            sv_inline f32 &operator[](s16 i) {
                assert((u16)i < 3 && "FVec3::operator[](): bad index");
                return v[i];
            }
            sv_inline f32 operator[](s16 i) const {
                assert((u16)i < 3 && "FVec3::operator[](): bad index");
                return v[i];
            }
            
            sv_inline f32 length2() const {
                return spu_extract(spu_dot33(vec,vec),0);
            }
            sv_inline f32 length() const {
                return Math::sqrt(spu_extract(spu_dot33(vec,vec),0));
            }
            sv_inline FVec3 &normalize() {
                vec = spu_normalize3(vec);
                return *this;
            }
            
            union {
                struct {
                    f32 x,y,z,w;
                };
                f32 v[4];
                vec_float4 vec;
            };
        };

        //
        sv_inline s32 operator==(const FVec3 &v0,const FVec3 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z));
        }

        sv_inline s32 operator!=(const FVec3 &v0,const FVec3 &v1) {
            return !(compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z));
        }

        sv_inline FVec3 operator*(const FVec3 &v0,f32 v1) {
            return FVec3(spu_mul(v0.vec,spu_splats(v1)));
        }

        sv_inline FVec3 operator*(const FVec3 &v0,const FVec3 &v1) {
            return FVec3(spu_mul(v0.vec,v1.vec));
        }

        sv_inline FVec3 operator/(const FVec3 &v0,f32 v1) {
            return FVec3(spu_mul(v0.vec,spu_splats(Math::rcp(v1))));
        }

        sv_inline FVec3 operator/(const FVec3 &v0,const FVec3 &v1) {
            return FVec3(spu_mul(v0.vec,Math::rcp4(v1.vec)));
        }

        sv_inline FVec3 operator+(const FVec3 &v0,const FVec3 &v1) {
            return FVec3(spu_add(v0.vec,v1.vec));
        }

        sv_inline FVec3 operator-(const FVec3 &v0,const FVec3 &v1) {
            return FVec3(spu_sub(v0.vec,v1.vec));
        }

        //
        sv_inline s32 compare(const FVec3 &v0,const FVec3 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z));
        }

        sv_inline s32 compare(const FVec3 &v0,const FVec3 &v1,f32 epsilon) {
            return (compare(v0.x,v1.x,epsilon) && compare(v0.y,v1.y,epsilon) && compare(v0.z,v1.z,epsilon));
        }

        sv_inline f32 dot(const FVec3 &v0,const FVec3 &v1) {
            return spu_extract(spu_dot33(v0.vec,v1.vec),0);
        }

        sv_inline FVec3 &mul(FVec3 &ret,const FVec3 &v0,f32 v1) {
            ret.vec = spu_mul(v0.vec,spu_splats(v1));
            return ret;
        }

        sv_inline FVec3 &mul(FVec3 &ret,const FVec3 &v0,const FVec3 &v1) {
            ret.vec = spu_mul(v0.vec,v1.vec);
            return ret;
        }

        sv_inline FVec3 &mad(FVec3 &ret,const FVec3 &v0,f32 v1,const FVec3 &v2) {
            ret.vec = spu_madd(v0.vec,spu_splats(v1),v2.vec);
            return ret;
        }

        sv_inline FVec3 &mad(FVec3 &ret,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2) {
            ret.vec = spu_madd(v0.vec,v1.vec,v2.vec);
            return ret;
        }

        sv_inline FVec3 &add(FVec3 &ret,const FVec3 &v0,const FVec3 &v1) {
            ret.vec = spu_add(v0.vec,v1.vec);
            return ret;
        }

        sv_inline FVec3 &sub(FVec3 &ret,const FVec3 &v0,const FVec3 &v1) {
            ret.vec = spu_sub(v0.vec,v1.vec);
            return ret;
        }

        sv_inline FVec3 &lerp(FVec3 &ret,const FVec3 &v0,const FVec3 &v1,f32 k) {
            ret.vec = spu_madd(spu_sub(v1.vec,v0.vec),spu_splats(k),v0.vec);
            return ret;
        }

        sv_inline FVec3 &cross(FVec3 &ret,const FVec3 &v0,const FVec3 &v1) {
            ret.vec = spu_cross(v0.vec,v1.vec);
            return ret;
        }

        //
        sv_inline f32 length(const FVec3 &v) {
            return v.length();
        }

        sv_inline f32 length2(const FVec3 &v) {
            return v.length2();
        }

        sv_inline FVec3 normalize(const FVec3 &v) {
            FVec3 ret = v;
            return ret.normalize();
        }

        sv_inline FVec3 lerp(const FVec3 &v0,const FVec3 &v1,f32 k) {
            FVec3 ret;
            return lerp(ret,v0,v1,k);
        }

        sv_inline FVec3 cross(const FVec3 &v0,const FVec3 &v1) {
            FVec3 ret;
            return cross(ret,v0,v1);
        }

        //
        sv_inline FVec3 min(const FVec3 &v0,const FVec3 &v1) {
            return FVec3(spu_sel(v0.vec,v1.vec,spu_cmpgt(v0.vec,v1.vec)));
        }

        sv_inline FVec3 max(const FVec3 &v0,const FVec3 &v1) {
            return FVec3(spu_sel(v0.vec,v1.vec,spu_cmpgt(v1.vec,v0.vec)));
        }

        sv_inline FVec3 clamp(const FVec3 &v,const FVec3 &v0,const FVec3 &v1) {
            vec_uint4 mask0 = spu_cmpgt(v.vec,v0.vec);
            vec_uint4 mask1 = spu_cmpgt(v.vec,v1.vec);
            return FVec3(spu_sel(v0.vec,spu_sel(v.vec,v1.vec,mask1),mask0));
        }

        sv_inline FVec3 saturate(const FVec3 &v) {
            vec_float4 one = spu_splats(1.0f);
            vec_float4 zero = spu_splats(0.0f);
            vec_uint4 mask_0 = spu_cmpgt(v.vec,one);
            vec_uint4 mask_1 = spu_cmpgt(v.vec,zero);
            return FVec3(spu_sel(zero,spu_sel(v.vec,one,mask_0),mask_1));
        }

        /******************************************************************************\
        *
        * FVec4
        *
        \******************************************************************************/

        //
        ATTRIBUTE_ALIGNED16(struct) FVec4 {
            
            sv_inline FVec4() { }
            sv_inline FVec4(const FVec4 &v) : vec(v.vec) { }
            sv_inline FVec4(const FVec3 &v,f32 w) : x(v.x), y(v.y), z(v.z), w(w) { }
            sv_inline FVec4(f32 x,f32 y,f32 z,f32 w) : x(x), y(y), z(z), w(w) { }
            explicit sv_inline FVec4(f32 v) : x(v), y(v), z(v), w(v) { }
            explicit sv_inline FVec4(const FVec3 &v) : x(v.x), y(v.y), z(v.z), w(1.0f) { }
            explicit sv_inline FVec4(vec_float4 v) : vec(v) { }
            
            sv_inline FVec4 &operator=(const FVec4 &v) {
                vec = v.vec;
                return *this;
            }
            sv_inline FVec4 operator-() const {
                return FVec4((vec_float4)spu_xor((vec_uint4)vec,0x80000000));
            }
            sv_inline FVec4 &operator*=(f32 v) {
                vec = spu_mul(vec,spu_splats(v));
                return *this;
            }
            sv_inline FVec4 &operator*=(const FVec4 &v) {
                vec = spu_mul(vec,v.vec);
                return *this;
            }
            sv_inline FVec4 &operator/=(f32 v) {
                vec = spu_mul(vec,spu_splats(Math::rcp(v)));
                return *this;
            }
            sv_inline FVec4 &operator/=(const FVec4 &v) {
                vec = spu_mul(vec,Math::rcp4(v.vec));
                return *this;
            }
            sv_inline FVec4 &operator+=(const FVec4 &v) {
                vec = spu_add(vec,v.vec);
                return *this;
            }
            sv_inline FVec4 &operator-=(const FVec4 &v) {
                vec = spu_sub(vec,v.vec);
                return *this;
            }
            
            sv_inline f32 &operator[](s16 i) {
                assert((u16)i < 4 && "FVec4::operator[](): bad index");
                return v[i];
            }
            sv_inline f32 operator[](s16 i) const {
                assert((u16)i < 4 && "FVec4::operator[](): bad index");
                return v[i];
            }
            
            sv_inline f32 length2() const {
                return spu_extract(spu_dot44(vec,vec),0);
            }
            sv_inline f32 length() const {
                return Math::sqrt(spu_extract(spu_dot44(vec,vec),0));
            }
            sv_inline FVec4 &normalize() {
                vec = spu_normalize4(vec);
                return *this;
            }
            
            union {
                struct {
                    f32 x,y,z,w;
                };
                f32 v[4];
                vec_float4 vec;
            };
        };

        //
        sv_inline FVec3::FVec3(const FVec4 &v) : x(v.x), y(v.y), z(v.z), w(0.0f) { }

        //
        sv_inline s32 operator==(const FVec4 &v0,const FVec4 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z) && compare(v0.w,v1.w));
        }

        sv_inline s32 operator!=(const FVec4 &v0,const FVec4 &v1) {
            return !(compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z) && compare(v0.w,v1.w));
        }

        sv_inline FVec4 operator*(const FVec4 &v0,f32 v1) {
            return FVec4(spu_mul(v0.vec,spu_splats(v1)));
        }

        sv_inline FVec4 operator*(const FVec4 &v0,const FVec4 &v1) {
            return FVec4(spu_mul(v0.vec,v1.vec));
        }

        sv_inline FVec4 operator/(const FVec4 &v0,f32 v1) {
            return FVec4(spu_mul(v0.vec,spu_splats(Math::rcp(v1))));
        }

        sv_inline FVec4 operator/(const FVec4 &v0,const FVec4 &v1) {
            return FVec4(spu_mul(v0.vec,Math::rcp4(v1.vec)));
        }

        sv_inline FVec4 operator+(const FVec4 &v0,const FVec4 &v1) {
            return FVec4(spu_add(v0.vec,v1.vec));
        }

        sv_inline FVec4 operator-(const FVec4 &v0,const FVec4 &v1) {
            return FVec4(spu_sub(v0.vec,v1.vec));
        }

        //
        sv_inline s32 compare(const FVec4 &v0,const FVec4 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z) && compare(v0.w,v1.w));
        }

        sv_inline s32 compare(const FVec4 &v0,const FVec4 &v1,f32 epsilon) {
            return (compare(v0.x,v1.x,epsilon) && compare(v0.y,v1.y,epsilon) && compare(v0.z,v1.z,epsilon) && compare(v0.w,v1.w,epsilon));
        }

        sv_inline f32 dot(const FVec3 &v0,const FVec4 &v1) {
            return spu_extract(spu_dot33(v0.vec,v1.vec),0) + v1.w;
        }

        sv_inline f32 dot(const FVec4 &v0,const FVec3 &v1) {
            return spu_extract(spu_dot33(v0.vec,v1.vec),0) + v0.w;
        }

        sv_inline f32 dot(const FVec4 &v0,const FVec4 &v1) {
            return spu_extract(spu_dot44(v0.vec,v1.vec),0);
        }

        sv_inline f32 dot3(const FVec3 &v0,const FVec4 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }

        sv_inline f32 dot3(const FVec4 &v0,const FVec3 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }

        sv_inline f32 dot3(const FVec4 &v0,const FVec4 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }

        sv_inline FVec4 &mul(FVec4 &ret,const FVec4 &v0,f32 v1) {
            ret.vec = spu_mul(v0.vec,spu_splats(v1));
            return ret;
        }

        sv_inline FVec4 &mul(FVec4 &ret,const FVec4 &v0,const FVec4 &v1) {
            ret.vec = spu_mul(v0.vec,v1.vec);
            return ret;
        }

        sv_inline FVec4 &mad(FVec4 &ret,const FVec4 &v0,f32 v1,const FVec4 &v2) {
            ret.vec = spu_madd(v0.vec,spu_splats(v1),v2.vec);
            return ret;
        }

        sv_inline FVec4 &mad(FVec4 &ret,const FVec4 &v0,const FVec4 &v1,const FVec4 &v2) {
            ret.vec = spu_madd(v0.vec,v1.vec,v2.vec);
            return ret;
        }

        sv_inline FVec4 &add(FVec4 &ret,const FVec4 &v0,const FVec4 &v1) {
            ret.vec = spu_add(v0.vec,v1.vec);
            return ret;
        }

        sv_inline FVec4 &sub(FVec4 &ret,const FVec4 &v0,const FVec4 &v1) {
            ret.vec = spu_sub(v0.vec,v1.vec);
            return ret;
        }

        sv_inline FVec4 &lerp(FVec4 &ret,const FVec4 &v0,const FVec4 &v1,f32 k) {
            ret.vec = spu_madd(spu_sub(v1.vec,v0.vec),spu_splats(k),v0.vec);
            return ret;
        }

        sv_inline FVec4 &cross(FVec4 &ret,const FVec3 &v0,const FVec3 &v1) {
            ret.vec = spu_cross(v0.vec,v1.vec);
            return ret;
        }

        //
        sv_inline f32 length(const FVec4 &v) {
            return v.length();
        }

        sv_inline f32 length2(const FVec4 &v) {
            return v.length2();
        }

        sv_inline FVec4 normalize(const FVec4 &v) {
            FVec4 ret = v;
            return ret.normalize();
        }

        sv_inline FVec4 lerp(const FVec4 &v0,const FVec4 &v1,f32 k) {
            FVec4 ret;
            return lerp(ret,v0,v1,k);
        }

        //
        sv_inline FVec4 min(const FVec4 &v0,const FVec4 &v1) {
            return FVec4(spu_sel(v0.vec,v1.vec,spu_cmpgt(v0.vec,v1.vec)));
        }

        sv_inline FVec4 max(const FVec4 &v0,const FVec4 &v1) {
            return FVec4(spu_sel(v0.vec,v1.vec,spu_cmpgt(v1.vec,v0.vec)));
        }

        sv_inline FVec4 clamp(const FVec4 &v,const FVec4 &v0,const FVec4 &v1) {
            vec_uint4 mask0 = spu_cmpgt(v.vec,v0.vec);
            vec_uint4 mask1 = spu_cmpgt(v.vec,v1.vec);
            return FVec4(spu_sel(v0.vec,spu_sel(v.vec,v1.vec,mask1),mask0));
        }

        sv_inline FVec4 saturate(const FVec4 &v) {
            vec_float4 one = spu_splats(1.0f);
            vec_float4 zero = spu_splats(0.0f);
            vec_uint4 mask0 = spu_cmpgt(v.vec,zero);
            vec_uint4 mask1 = spu_cmpgt(v.vec,one);
            return FVec4(spu_sel(zero,spu_sel(v.vec,one,mask1),mask0));
        }

        /******************************************************************************\
        *
        * FMat3
        *
        \******************************************************************************/

        //
        ATTRIBUTE_ALIGNED16(struct) FMat3 {
            
            sv_inline FMat3() { }
            FMat3(const FMat3 &m);
            explicit FMat3(f32 v);
            explicit FMat3(const FMat4 &m);
            explicit FMat3(const quat &q);
            
            sv_inline FMat3 &operator=(const FMat3 &v) {
                col0 = v.col0;
                col1 = v.col1;
                col2 = v.col2;
                return *this;
            }
            
            FMat3 operator-() const;
            FMat3 &operator*=(const FMat3 &m);
            FMat3 &operator+=(const FMat3 &m);
            FMat3 &operator-=(const FMat3 &m);
            
            sv_inline f32 &operator[](s16 i) {
                assert((u16)i < 12 && "FMat3::operator[](): bad index");
                return mat[i];
            }
            sv_inline f32 operator[](s16 i) const {
                assert((u16)i < 12 && "FMat3::operator[](): bad index");
                return mat[i];
            }
            
            void setRow(s16 row,const FVec3 &v);
            FVec3 getRow(s16 row) const;
            
            void setColumn(s16 column,const FVec3 &v);
            FVec3 getColumn(s16 column) const;
            
            union {
                struct {
                    f32 m00,m10,m20,m30;
                    f32 m01,m11,m21,m31;
                    f32 m02,m12,m22,m32;
                };
                f32 mat[12];
                struct {
                    vec_float4 col0;
                    vec_float4 col1;
                    vec_float4 col2;
                };
            };
        };

        //
        sv_inline FMat3::FMat3(f32 v) {
            m00 = v;    m01 = v;    m02 = v;
            m10 = v;    m11 = v;    m12 = v;
            m20 = v;    m21 = v;    m22 = v;
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f;
        }

        sv_inline FMat3::FMat3(const FMat3 &m) {
            col0 = m.col0;
            col1 = m.col1;
            col2 = m.col2;
        }

        //
        sv_inline FVec3 &mul(FVec3 &ret,const FMat3 &m,const FVec3 &v) {
            vec_float4 res_0 = spu_mul(m.col0,SPU_SWIZZLE(v.vec,X,X,X,W));
            vec_float4 res_1 = spu_madd(m.col1,SPU_SWIZZLE(v.vec,Y,Y,Y,W),res_0);
            ret.vec = spu_madd(m.col2,SPU_SWIZZLE(v.vec,Z,Z,Z,W),res_1);
            return ret;
        }

        sv_inline FMat3 &mul(FMat3 &ret,const FMat3 &m0,const FMat3 &m1) {
            vec_uchar16 xxxx = SPU_PERM2(X,X,X,X);
            vec_uchar16 yyyy = SPU_PERM2(Y,Y,Y,Y);
            vec_uchar16 zzzz = SPU_PERM2(Z,Z,Z,Z);
            ret.col0 = spu_mul(m0.col0,spu_shuffle(m1.col0,m1.col0,xxxx));
            ret.col1 = spu_mul(m0.col0,spu_shuffle(m1.col1,m1.col1,xxxx));
            ret.col2 = spu_mul(m0.col0,spu_shuffle(m1.col2,m1.col2,xxxx));
            ret.col0 = spu_madd(m0.col1,spu_shuffle(m1.col0,m1.col0,yyyy),ret.col0);
            ret.col1 = spu_madd(m0.col1,spu_shuffle(m1.col1,m1.col1,yyyy),ret.col1);
            ret.col2 = spu_madd(m0.col1,spu_shuffle(m1.col2,m1.col2,yyyy),ret.col2);
            ret.col0 = spu_madd(m0.col2,spu_shuffle(m1.col0,m1.col0,zzzz),ret.col0);
            ret.col1 = spu_madd(m0.col2,spu_shuffle(m1.col1,m1.col1,zzzz),ret.col1);
            ret.col2 = spu_madd(m0.col2,spu_shuffle(m1.col2,m1.col2,zzzz),ret.col2);
            return ret;
        }

        sv_inline FMat3 &add(FMat3 &ret,const FMat3 &m0,const FMat3 &m1) {
            ret.col0 = spu_add(m0.col0,m1.col0);
            ret.col1 = spu_add(m0.col1,m1.col1);
            ret.col2 = spu_add(m0.col2,m1.col2);
            return ret;
        }

        sv_inline FMat3 &sub(FMat3 &ret,const FMat3 &m0,const FMat3 &m1) {
            ret.col0 = spu_sub(m0.col0,m1.col0);
            ret.col1 = spu_sub(m0.col1,m1.col1);
            ret.col2 = spu_sub(m0.col2,m1.col2);
            return ret;
        }

        //
        sv_inline FMat3 FMat3::operator-() const {
            FMat3 ret;
            ret.col0 = (vec_float4)spu_xor((vec_uint4)col0,spu_splats(0x80000000));
            ret.col1 = (vec_float4)spu_xor((vec_uint4)col1,spu_splats(0x80000000));
            ret.col2 = (vec_float4)spu_xor((vec_uint4)col2,spu_splats(0x80000000));
            return ret;
        }

        sv_inline FMat3 &FMat3::operator*=(const FMat3 &m) {
            return mul(*this,FMat3(*this),m);
        }

        sv_inline FMat3 &FMat3::operator+=(const FMat3 &m) {
            return add(*this,*this,m);
        }

        sv_inline FMat3 &FMat3::operator-=(const FMat3 &m) {
            return sub(*this,*this,m);
        }

        //
        sv_inline void FMat3::setRow(s16 row,const FVec3 &v) {
            assert((u16)row < 3 && "FMat3::setRow(): bad row");
            mat[row + 0] = v.x;
            mat[row + 4] = v.y;
            mat[row + 8] = v.z;
        }

        sv_inline FVec3 FMat3::getRow(s16 row) const {
            assert((u16)row < 3 && "FMat3::getRow(): bad row");
            return FVec3(mat[row + 0],mat[row + 4],mat[row + 8]);
        }

        //
        sv_inline void FMat3::setColumn(s16 column,const FVec3 &v) {
            assert((u16)column < 3 && "FMat3::setColumn(): bad column");
            mat[column * 4 + 0] = v.x;
            mat[column * 4 + 1] = v.y;
            mat[column * 4 + 2] = v.z;
        }

        sv_inline FVec3 FMat3::getColumn(s16 column) const {
            assert((u16)column < 3 && "FMat3::getColumn(): bad column");
            return FVec3(mat[column * 4 + 0],mat[column * 4 + 1],mat[column * 4 + 2]);
        }

        /******************************************************************************\
        *
        * FMat4
        *
        \******************************************************************************/

        //
        ATTRIBUTE_ALIGNED16(struct) FMat4 {
            
            sv_inline FMat4() { }
            FMat4(const FMat4 &m);
            explicit FMat4(f32 v);
            explicit FMat4(const FMat3 &m);
            explicit FMat4(const quat &q);
            explicit FMat4(const FMat3 &m,const FVec3 &v);
            explicit FMat4(const quat &q,const FVec3 &v);
            
            sv_inline FMat4 &operator=(const FMat4 &v) {
                col0 = v.col0;
                col1 = v.col1;
                col2 = v.col2;
                col3 = v.col3;
                return *this;
            }
            
            FMat4 operator-() const;
            FMat4 &operator*=(const FMat4 &m);
            FMat4 &operator+=(const FMat4 &m);
            FMat4 &operator-=(const FMat4 &m);
            
            sv_inline f32 &operator[](s16 i) {
                assert((u16)i < 16 && "FMat4::operator[](): bad index");
                return mat[i];
            }
            sv_inline f32 operator[](s16 i) const {
                assert((u16)i < 16 && "FMat4::operator[](): bad index");
                return mat[i];
            }
            
            void setRow(s16 row,const FVec4 &v);
            FVec4 getRow(s16 row) const;
            void setRow3(s16 row,const FVec3 &v);
            FVec3 getRow3(s16 row) const;
            
            void setColumn(s16 column,const FVec4 &v);
            FVec4 getColumn(s16 column) const;
            void setColumn3(s16 column,const FVec3 &v);
            FVec3 getColumn3(s16 column) const;
            
            void setZero();
            void setIdentity();
            void setTranslate(const FVec3 &v);
            void setScale(const FVec3 &v);
            
            union {
                struct {
                    f32 m00,m10,m20,m30;
                    f32 m01,m11,m21,m31;
                    f32 m02,m12,m22,m32;
                    f32 m03,m13,m23,m33;
                };
                f32 mat[16];
                struct {
                    vec_float4 col0;
                    vec_float4 col1;
                    vec_float4 col2;
                    vec_float4 col3;
                };
            };
        };

        //
        sv_inline FMat3::FMat3(const FMat4 &m) {
            col0 = m.col0;
            col1 = m.col1;
            col2 = m.col2;
        }

        //
        sv_inline FMat4::FMat4(f32 v) {
            vec_float4 temp = spu_splats(v);
            col0 = temp;
            col1 = temp;
            col2 = temp;
            col3 = temp;
        }

        sv_inline FMat4::FMat4(const FMat3 &m) {
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = 0.0f;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = 0.0f;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = 0.0f;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;  m33 = 1.0f;
        }

        sv_inline FMat4::FMat4(const FMat4 &m) {
            col0 = m.col0;
            col1 = m.col1;
            col2 = m.col2;
            col3 = m.col3;
        }

        //
        sv_inline FVec3 &mul(FVec3 &ret,const FMat4 &m,const FVec3 &v) {
            vec_float4 res_0 = spu_madd(m.col0,SPU_SWIZZLE(v.vec,X,X,X,W),m.col3);
            vec_float4 res_1 = spu_madd(m.col1,SPU_SWIZZLE(v.vec,Y,Y,Y,W),res_0);
            ret.vec = spu_madd(m.col2,SPU_SWIZZLE(v.vec,Z,Z,Z,W),res_1);
            return ret;
        }

        sv_inline FVec4 &mul(FVec4 &ret,const FMat4 &m,const FVec4 &v) {
            vec_float4 res_0 = spu_mul(m.col0,SPU_SWIZZLE(v.vec,X,X,X,X));
            vec_float4 res_1 = spu_madd(m.col1,SPU_SWIZZLE(v.vec,Y,Y,Y,Y),res_0);
            vec_float4 res_2 = spu_madd(m.col2,SPU_SWIZZLE(v.vec,Z,Z,Z,Z),res_1);
            ret.vec = spu_madd(m.col3,SPU_SWIZZLE(v.vec,W,W,W,W),res_2);
            return ret;
        }

        sv_inline FVec3 &mul3(FVec3 &ret,const FMat4 &m,const FVec3 &v) {
            vec_float4 res_0 = spu_mul(m.col0,SPU_SWIZZLE(v.vec,X,X,X,W));
            vec_float4 res_1 = spu_madd(m.col1,SPU_SWIZZLE(v.vec,Y,Y,Y,W),res_0);
            ret.vec = spu_madd(m.col2,SPU_SWIZZLE(v.vec,Z,Z,Z,W),res_1);
            return ret;
        }

        sv_inline FVec4 &mul3(FVec4 &ret,const FMat4 &m,const FVec4 &v) {
            vec_float4 res_0 = spu_mul(m.col0,SPU_SWIZZLE(v.vec,X,X,X,W));
            vec_float4 res_1 = spu_madd(m.col1,SPU_SWIZZLE(v.vec,Y,Y,Y,W),res_0);
            ret.vec = spu_madd(m.col2,SPU_SWIZZLE(v.vec,Z,Z,Z,W),res_1);
            ret.vec = spu_insert(spu_extract(v.vec,3),ret.vec,3);
            return ret;
        }

        sv_inline FMat4 &mul(FMat4 &ret,const FMat4 &m0,const FMat4 &m1) {
            vec_uchar16 xxxx = SPU_PERM2(X,X,X,X);
            vec_uchar16 yyyy = SPU_PERM2(Y,Y,Y,Y);
            vec_uchar16 zzzz = SPU_PERM2(Z,Z,Z,Z);
            vec_uchar16 wwww = SPU_PERM2(W,W,W,W);
            ret.col0 = spu_mul(m0.col0,spu_shuffle(m1.col0,m1.col0,xxxx));
            ret.col1 = spu_mul(m0.col0,spu_shuffle(m1.col1,m1.col1,xxxx));
            ret.col2 = spu_mul(m0.col0,spu_shuffle(m1.col2,m1.col2,xxxx));
            ret.col3 = spu_mul(m0.col0,spu_shuffle(m1.col3,m1.col3,xxxx));
            ret.col0 = spu_madd(m0.col1,spu_shuffle(m1.col0,m1.col0,yyyy),ret.col0);
            ret.col1 = spu_madd(m0.col1,spu_shuffle(m1.col1,m1.col1,yyyy),ret.col1);
            ret.col2 = spu_madd(m0.col1,spu_shuffle(m1.col2,m1.col2,yyyy),ret.col2);
            ret.col3 = spu_madd(m0.col1,spu_shuffle(m1.col3,m1.col3,yyyy),ret.col3);
            ret.col0 = spu_madd(m0.col2,spu_shuffle(m1.col0,m1.col0,zzzz),ret.col0);
            ret.col1 = spu_madd(m0.col2,spu_shuffle(m1.col1,m1.col1,zzzz),ret.col1);
            ret.col2 = spu_madd(m0.col2,spu_shuffle(m1.col2,m1.col2,zzzz),ret.col2);
            ret.col3 = spu_madd(m0.col2,spu_shuffle(m1.col3,m1.col3,zzzz),ret.col3);
            ret.col0 = spu_madd(m0.col3,spu_shuffle(m1.col0,m1.col0,wwww),ret.col0);
            ret.col1 = spu_madd(m0.col3,spu_shuffle(m1.col1,m1.col1,wwww),ret.col1);
            ret.col2 = spu_madd(m0.col3,spu_shuffle(m1.col2,m1.col2,wwww),ret.col2);
            ret.col3 = spu_madd(m0.col3,spu_shuffle(m1.col3,m1.col3,wwww),ret.col3);
            return ret;
        }

        sv_inline FMat4 &add(FMat4 &ret,const FMat4 &m0,const FMat4 &m1) {
            ret.col0 = spu_add(m0.col0,m1.col0);
            ret.col1 = spu_add(m0.col1,m1.col1);
            ret.col2 = spu_add(m0.col2,m1.col2);
            ret.col3 = spu_add(m0.col3,m1.col3);
            return ret;
        }

        sv_inline FMat4 &sub(FMat4 &ret,const FMat4 &m0,const FMat4 &m1) {
            ret.col0 = spu_sub(m0.col0,m1.col0);
            ret.col1 = spu_sub(m0.col1,m1.col1);
            ret.col2 = spu_sub(m0.col2,m1.col2);
            ret.col3 = spu_sub(m0.col3,m1.col3);
            return ret;
        }

        //
        sv_inline FMat4 FMat4::operator-() const {
            FMat4 ret;
            ret.col0 = (vec_float4)spu_xor((vec_uint4)col0,spu_splats(0x80000000));
            ret.col1 = (vec_float4)spu_xor((vec_uint4)col1,spu_splats(0x80000000));
            ret.col2 = (vec_float4)spu_xor((vec_uint4)col2,spu_splats(0x80000000));
            ret.col3 = (vec_float4)spu_xor((vec_uint4)col3,spu_splats(0x80000000));
            return ret;
        }

        sv_inline FMat4 &FMat4::operator*=(const FMat4 &m) {
            return mul(*this,FMat4(*this),m);
        }

        sv_inline FMat4 &FMat4::operator+=(const FMat4 &m) {
            return add(*this,*this,m);
        }

        sv_inline FMat4 &FMat4::operator-=(const FMat4 &m) {
            return sub(*this,*this,m);
        }

        //
        sv_inline void FMat4::setRow(s16 row,const FVec4 &v) {
            assert((u16)row < 4 && "FMat4::setRow(): bad row");
            mat[row + 0] = v.x;
            mat[row + 4] = v.y;
            mat[row + 8] = v.z;
            mat[row + 12] = v.w;
        }

        sv_inline FVec4 FMat4::getRow(s16 row) const {
            assert((u16)row < 4 && "FMat4::getRow(): bad row");
            return FVec4(mat[row + 0],mat[row + 4],mat[row + 8],mat[row + 12]);
        }

        sv_inline void FMat4::setRow3(s16 row,const FVec3 &v) {
            assert((u16)row < 4 && "FMat4::setRow3(): bad row");
            mat[row + 0] = v.x;
            mat[row + 4] = v.y;
            mat[row + 8] = v.z;
        }

        sv_inline FVec3 FMat4::getRow3(s16 row) const {
            assert((u16)row < 4 && "FMat4::getRow(): bad row");
            return FVec3(mat[row + 0],mat[row + 4],mat[row + 8]);
        }

        //
        sv_inline void FMat4::setColumn(s16 column,const FVec4 &v) {
            assert((u16)column < 4 && "FMat4::setColumn(): bad column");
            mat[column * 4 + 0] = v.x;
            mat[column * 4 + 1] = v.y;
            mat[column * 4 + 2] = v.z;
            mat[column * 4 + 3] = v.w;
        }

        sv_inline FVec4 FMat4::getColumn(s16 column) const {
            assert((u16)column < 4 && "FMat4::getColumn(): bad column");
            return FVec4(mat[column * 4 + 0],mat[column * 4 + 1],mat[column * 4 + 2],mat[column * 4 + 3]);
        }

        sv_inline void FMat4::setColumn3(s16 column,const FVec3 &v) {
            assert((u16)column < 4 && "FMat4::setColumn3(): bad column");
            mat[column * 4 + 0] = v.x;
            mat[column * 4 + 1] = v.y;
            mat[column * 4 + 2] = v.z;
        }

        sv_inline FVec3 FMat4::getColumn3(s16 column) const {
            assert((u16)column < 4 && "FMat4::getColumn3(): bad column");
            return FVec3(mat[column * 4 + 0],mat[column * 4 + 1],mat[column * 4 + 2]);
        }

        //
        sv_inline void FMat4::setZero() {
            vec_float4 zero = spu_splats(0.0f);
            col0 = zero;
            col1 = zero;
            col2 = zero;
            col3 = zero;
        }

        sv_inline void FMat4::setIdentity() {
            col0 = SPU_FLOAT4(1.0f,0.0f,0.0f,0.0f);
            col1 = SPU_FLOAT4(0.0f,1.0f,0.0f,0.0f);
            col2 = SPU_FLOAT4(0.0f,0.0f,1.0f,0.0f);
            col3 = SPU_FLOAT4(0.0f,0.0f,0.0f,1.0f);
        }

        sv_inline void FMat4::setTranslate(const FVec3 &v) {
            col0 = SPU_FLOAT4(1.0f,0.0f,0.0f,0.0f);
            col1 = SPU_FLOAT4(0.0f,1.0f,0.0f,0.0f);
            col2 = SPU_FLOAT4(0.0f,0.0f,1.0f,0.0f);
            col3 = spu_insert(1.0f,v.vec,3);
        }

        sv_inline void FMat4::setScale(const FVec3 &v) {
            vec_float4 zero = spu_splats(0.0f);
            col0 = spu_insert(spu_extract(v.vec,0),zero,0);
            col1 = spu_insert(spu_extract(v.vec,1),zero,1);
            col2 = spu_insert(spu_extract(v.vec,2),zero,2);
            col3 = SPU_FLOAT4(0.0f,0.0f,0.0f,1.0f);
        }

        //
        sv_inline FVec3 operator*(const FMat4 &m,const FVec3 &v) {
            FVec3 ret;
            return mul(ret,m,v);
        }

        sv_inline FVec4 operator*(const FMat4 &m,const FVec4 &v) {
            FVec4 ret;
            return mul(ret,m,v);
        }

        sv_inline FMat4 operator*(const FMat4 &m0,const FMat4 &m1) {
            FMat4 ret;
            return mul(ret,m0,m1);
        }

        sv_inline FMat4 operator+(const FMat4 &m0,const FMat4 &m1) {
            FMat4 ret;
            return add(ret,m0,m1);
        }

        sv_inline FMat4 operator-(const FMat4 &m0,const FMat4 &m1) {
            FMat4 ret;
            return sub(ret,m0,m1);
        }

        //
        sv_inline FMat4 &transpose(FMat4 &ret,const FMat4 &m) {
            vec_float4 res_0 = spu_shuffle(m.col0,m.col1,SPU_PERM2(X,Y,X,Y));
            vec_float4 res_1 = spu_shuffle(m.col0,m.col1,SPU_PERM2(Z,W,Z,W));
            vec_float4 res_2 = spu_shuffle(m.col2,m.col3,SPU_PERM2(X,Y,X,Y));
            vec_float4 res_3 = spu_shuffle(m.col2,m.col3,SPU_PERM2(Z,W,Z,W));
            ret.col0 = spu_shuffle(res_0,res_2,SPU_PERM2(X,Z,X,Z));
            ret.col1 = spu_shuffle(res_0,res_2,SPU_PERM2(Y,W,Y,W));
            ret.col2 = spu_shuffle(res_1,res_3,SPU_PERM2(X,Z,X,Z));
            ret.col3 = spu_shuffle(res_1,res_3,SPU_PERM2(Y,W,Y,W));
            return ret;
        }

        sv_inline FMat4 &inverse(FMat4 &ret,const FMat4 &m) {
            vec_uchar16 yxwz = SPU_PERM2(Y,X,W,Z);
            vec_uchar16 zwxy = SPU_PERM2(Z,W,X,Y);
            vec_float4 res_0 = spu_shuffle(m.col0,m.col1,SPU_PERM2(X,Y,X,Y));
            vec_float4 res_1 = spu_shuffle(m.col0,m.col1,SPU_PERM2(Z,W,Z,W));
            vec_float4 res_2 = spu_shuffle(m.col2,m.col3,SPU_PERM2(X,Y,X,Y));
            vec_float4 res_3 = spu_shuffle(m.col2,m.col3,SPU_PERM2(Z,W,Z,W));
            vec_float4 row_0 = spu_shuffle(res_0,res_2,SPU_PERM2(X,Z,X,Z));
            vec_float4 row_1 = spu_shuffle(res_2,res_0,SPU_PERM2(Y,W,Y,W));
            vec_float4 row_2 = spu_shuffle(res_1,res_3,SPU_PERM2(X,Z,X,Z));
            vec_float4 row_3 = spu_shuffle(res_3,res_1,SPU_PERM2(Y,W,Y,W));
            vec_float4 temp = spu_mul(row_2,row_3);
            temp = spu_shuffle(temp,temp,yxwz);
            res_0 = spu_mul(row_1,temp);
            res_1 = spu_mul(row_0,temp);
            temp = spu_shuffle(temp,temp,zwxy);
            res_0 = spu_sub(spu_mul(row_1,temp),res_0);
            res_1 = spu_sub(spu_mul(row_0,temp),res_1);
            res_1 = spu_shuffle(res_1,res_1,zwxy);
            temp = spu_mul(row_1,row_2);
            temp = spu_shuffle(temp,temp,yxwz);
            res_0 = spu_madd(row_3,temp,res_0);
            res_3 = spu_mul(row_0,temp);
            temp = spu_shuffle(temp,temp,zwxy);
            res_0 = spu_nmsub(row_3,temp,res_0);
            res_3 = spu_sub(spu_mul(row_0,temp),res_3);
            res_3 = spu_shuffle(res_3,res_3,zwxy);
            temp = spu_mul(spu_shuffle(row_1,row_1,zwxy),row_3);
            temp = spu_shuffle(temp,temp,yxwz);
            row_2 = spu_shuffle(row_2,row_2,zwxy);
            res_0 = spu_madd(row_2,temp,res_0);
            res_2 = spu_mul(row_0,temp);
            temp = spu_shuffle(temp,temp,zwxy);
            res_0 = spu_nmsub(row_2,temp,res_0);
            res_2 = spu_sub(spu_mul(row_0,temp),res_2);
            res_2 = spu_shuffle(res_2,res_2,zwxy);
            temp = spu_mul(row_0,row_1);
            temp = spu_shuffle(temp,temp,yxwz);
            res_2 = spu_madd(row_3,temp,res_2);
            res_3 = spu_sub(spu_mul(row_2,temp),res_3);
            temp = spu_shuffle(temp,temp,zwxy);
            res_2 = spu_sub(spu_mul(row_3,temp),res_2);
            res_3 = spu_nmsub(row_2,temp,res_3);
            temp = spu_mul(row_0,row_3);
            temp = spu_shuffle(temp,temp,yxwz);
            res_1 = spu_nmsub(row_2,temp,res_1);
            res_2 = spu_madd(row_1,temp,res_2);
            temp = spu_shuffle(temp,temp,zwxy);
            res_1 = spu_madd(row_2,temp,res_1);
            res_2 = spu_nmsub(row_1,temp,res_2);
            temp = spu_mul(row_0,row_2);
            temp = spu_shuffle(temp,temp,yxwz);
            res_1 = spu_madd(row_3,temp,res_1);
            res_3 = spu_nmsub(row_1,temp,res_3);
            temp = spu_shuffle(temp,temp,zwxy);
            res_1 = spu_nmsub(row_3,temp,res_1);
            res_3 = spu_madd(row_1,temp,res_3);
            vec_float4 det = spu_mul(row_0,res_0);
            det = spu_add(det,spu_rlqwbyte(det,8));
            det = spu_add(det,spu_rlqwbyte(det,4));
            temp = spu_rcp_nr(det);
            ret.col0 = spu_mul(res_0,temp);
            ret.col1 = spu_mul(res_1,temp);
            ret.col2 = spu_mul(res_2,temp);
            ret.col3 = spu_mul(res_3,temp);
            return ret;
        }

        //
        sv_inline FMat4 transpose(const FMat4 &m) {
            FMat4 ret;
            return transpose(ret,m);
        }

        sv_inline FMat4 inverse(const FMat4 &m) {
            FMat4 ret;
            return inverse(ret,m);
        }

        //
        sv_inline FMat4 translate(const FVec3 &v) {
            FMat4 ret;
            ret.setTranslate(v);
            return ret;
        }

        sv_inline FMat4 translate(f32 x,f32 y,f32 z) {
            return translate(FVec3(x,y,z));
        }

        sv_inline FMat4 scale(const FVec3 &v) {
            FMat4 ret;
            ret.setScale(v);
            return ret;
        }

        sv_inline FMat4 scale(f32 x,f32 y,f32 z) {
            return scale(FVec3(x,y,z));
        }

        /******************************************************************************\
        *
        * quat
        *
        \******************************************************************************/

        //
        ATTRIBUTE_ALIGNED16(struct) quat {
            
            sv_inline quat() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) { }
            sv_inline quat(const quat &q) : x(q.x), y(q.y), z(q.z), w(q.w) { }
            explicit sv_inline quat(const FVec3 &v) : x(v.x), y(v.y), z(v.z), w(0.0f) { }
            explicit sv_inline quat(const FVec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
            explicit sv_inline quat(vec_float4 vec) : vec(vec) { }
            explicit quat(const FMat3 &m);
            explicit quat(const FMat4 &m);
            
            sv_inline f32 &operator[](s16 i) {
                assert((u16)i < 4 && "quat::operator[](): bad index");
                return q[i];
            }
            sv_inline f32 operator[](s16 i) const {
                assert((u16)i < 4 && "quat::operator[](): bad index");
                return q[i];
            }
            
            union {
                struct {
                    f32 x,y,z,w;
                };
                f32 q[4];
                vec_float4 vec;
            };
        };

    }//!namespace util

}//!namespace sv

#endif /* __AMATH_LIB_SPU_H__ */
