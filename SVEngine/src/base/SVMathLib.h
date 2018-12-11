#ifndef __AMATH_LIB_H__
#define __AMATH_LIB_H__

#include "aMath.h"

namespace sv {
    
    namespace util {

        /******************************************************************************\
         *
         * MathLib
         *
         \******************************************************************************/
        
        //
        class Math {
            
            Math();
            
        public:
            
            // projection
            static void setGL();
            static void setD3D9();
            static void setD3D10();
            
            // functions
            static s32 signMask(s32 v);
            static f32 sign(f32 v);
            static f64 sign(f64 v);
            
            static s32 abs(s32 v);
            static s64 abs(s64 v);
            static f32 abs(f32 v);
            static f64 abs(f64 v);
            
            static f32 ceil(f32 v);
            static f64 ceil(f64 v);
            
            static f32 floor(f32 v);
            static f64 floor(f64 v);
            
            static f32 frac(f32 v);
            static f64 frac(f64 v);
            
            static f32 sqrt(f32 v);
            static f64 sqrt(f64 v);
            static f32 sqrtFast(f32 v);
            
            static f32 rcp(f32 v);
            static f64 rcp(f64 v);
            static f32 rcpFast(f32 v);
            
            static f32 rsqrt(f32 v);
            static f64 rsqrt(f64 v);
            static f32 rsqrtFast(f32 v);
            
            static f32 mod(f32 x,f32 y);
            static f64 mod(f64 x,f64 y);
            
            static f32 pow(f32 x,f32 y);
            static f64 pow(f64 x,f64 y);
            static f32 powFast(f32 x,f32 y);
            
            static f32 exp(f32 v);
            static f64 exp(f64 v);
            static f32 expFast(f32 v);
            
            static f32 exp2(f32 v);
            static f64 exp2(f64 v);
            static f32 exp2Fast(f32 v);
            
            static f32 log(f32 v);
            static f64 log(f64 v);
            static f32 logFast(f32 v);
            
            static s32 log2(s32 v);
            static f32 log2(f32 v);
            static f64 log2(f64 v);
            static f32 log2Fast(f32 v);
            
            static f32 log10(f32 v);
            static f64 log10(f64 v);
            
            // trigonometry
            static f32 sin(f32 a);
            static f64 sin(f64 a);
            static f32 sinFast(f32 a);
            
            static f32 cos(f32 a);
            static f64 cos(f64 a);
            static f32 cosFast(f32 a);
            
            static f32 tan(f32 a);
            static f64 tan(f64 a);
            
            static f32 asin(f32 v);
            static f64 asin(f64 v);
            static f32 asinFast(f32 v);
            
            static f32 acos(f32 v);
            static f64 acos(f64 v);
            static f32 acosFast(f32 v);
            
            static f32 atan(f32 v);
            static f64 atan(f64 v);
            
            static f32 atan2(f32 y,f32 x);
            static f64 atan2(f64 y,f64 x);
            
            static void sincos(f32 a,f32 &s,f32 &c);
            static void sincos(f64 a,f64 &s,f64 &c);
            static void sincosFast(f32 a,f32 &s,f32 &c);
            
            // branching
            static s32 select(s32 c,s32 v0,s32 v1);
            static f32 select(s32 c,f32 v0,f32 v1);
            static f32 select(f32 c,f32 v0,f32 v1);
            
            // conversion
            static f32 itof(s32 v);
            static s32 ftoi(f32 v);
            static s32 round(f32 v);
            
            static f64 itod(s32 v);
            static s32 dtoi(f64 v);
            
            static f32 ltof(s64 v);
            static s64 ftol(f32 v);
            
            static f64 ltod(s64 v);
            static s64 dtol(f64 v);
            
            // nearest power of two
            static s32 npot(s32 v);
            
            // rounding up division
            static s32 udiv(s32 x,s32 y);
            
            // bezier curve solver
            static f32 bezier(const f32 *t,const f32 *v,f32 time);
            static f64 bezier(const f32 *t,const f64 *v,f32 time);
            
            // memory
            static void prefetch(const void *ptr);
            static void memset(void *dest,s32 c,size_t size);
            static void memcpy(void *dest,const void *src,size_t size);
            static s32 memcmp(const void *src_0,const void *src_1,size_t size);
        };
        
        //
        sv_inline s32 Math::signMask(s32 v) {
            return (v >> 31);
        }
        
        sv_inline f32 Math::sign(f32 v) {
            return IntFloat((IntFloat(v).ui & 0x80000000) | 0x3f800000).f;
        }
        
        sv_inline f64 Math::sign(f64 v) {
            if(v >= 0.0) return 1.0;
            return -1.0;
        }
        
        //
        sv_inline s32 Math::abs(s32 v) {
            if(v >= 0) return v;
            return -v;
        }
        
        sv_inline s64 Math::abs(s64 v) {
            if(v >= 0) return v;
            return -v;
        }
        
        sv_inline f32 Math::abs(f32 v) {
#ifdef _CELLOS_LV2
            return __fabsf(v);
#else
            if(v >= 0.0f) return v;
            return -v;
#endif
        }
        
        sv_inline f64 Math::abs(f64 v) {
#ifdef _CELLOS_LV2
            return __fabs(v);
#else
            if(v >= 0.0) return v;
            return -v;
#endif
        }
        
        //
        sv_inline f32 Math::ceil(f32 v) {
            return ::ceilf(v);
        }
        
        sv_inline f64 Math::ceil(f64 v) {
            return ::ceil(v);
        }
        
        //
        sv_inline f32 Math::floor(f32 v) {
            return ::floorf(v);
        }
        
        sv_inline f64 Math::floor(f64 v) {
            return ::floor(v);
        }
        
        //
        sv_inline f32 Math::frac(f32 v) {
            return v - ::floorf(v);
        }
        
        sv_inline f64 Math::frac(f64 v) {
            return v - ::floor(v);
        }
        
        //
        sv_inline f32 Math::sqrt(f32 v) {
#if defined(_CELLOS_LV2) && !defined(__SNC__)
            return __fsqrts(v);
#elif USE_SSE
            _mm_store_ss(&v,_mm_sqrt_ss(_mm_set_ss(v)));
            return v;
#else
            return ::sqrtf(v);
#endif
        }
        
        sv_inline f64 Math::sqrt(f64 v) {
#if defined(_CELLOS_LV2) && !defined(__SNC__)
            return __fsqrt(v);
#else
            return ::sqrt(v);
#endif
        }
        
        sv_inline f32 Math::sqrtFast(f32 v) {
#ifdef ARCH_ARM
            return ::sqrtf(v);
#else
            IntFloat i = v;
            i.i = 0x5f3759df - (i.i >> 1);
            v = i.f * v;
            return v * (1.5f - (i.f * v * 0.5f));
#endif
        }
        
        //
        sv_inline f32 Math::rcp(f32 v) {
            return 1.0f / v;
        }
        
        sv_inline f64 Math::rcp(f64 v) {
            return 1.0 / v;
        }
        
        sv_inline f32 Math::rcpFast(f32 v) {
#if defined(_CELLOS_LV2) && !defined(__SNC__)
            return __fres(v);
#elif USE_SSE
            _mm_store_ss(&v,_mm_rcp_ss(_mm_set_ss(v)));
            return v;
#elif ARCH_ARM
            return 1.0f / v;
#else
            IntFloat i = v;
            i.i = 0x7f000000 - i.i;
            return i.f * (2.0f - v * i.f);
#endif
        }
        
        //
        sv_inline f32 Math::rsqrt(f32 v) {
            if(v < 1e-18f) {
                return INFINITY;
            }
            return 1.0f / ::sqrtf(v);
        }
        
        sv_inline f64 Math::rsqrt(f64 v) {
            if(v < 1e-18) return INFINITY;
            return 1.0 / ::sqrt(v);
        }
        
        sv_inline f32 Math::rsqrtFast(f32 v) {
#ifdef _CELLOS_LV2
            return (f32)__frsqrte(v);
#elif USE_SSE
            _mm_store_ss(&v,_mm_rsqrt_ss(_mm_set_ss(v)));
            return v;
#else
            IntFloat i = v;
            i.i = 0x5f3759df - (i.i >> 1);
            return i.f * (1.5f - (i.f * i.f * v * 0.5f));
#endif
        }
        
        //
        sv_inline f32 Math::mod(f32 x,f32 y) {
            return ::fmodf(x,y);
        }
        
        sv_inline f64 Math::mod(f64 x,f64 y) {
            return ::fmod(x,y);
        }
        
        //
        sv_inline f32 Math::pow(f32 x,f32 y) {
            return ::powf(x,y);
        }
        
        sv_inline f64 Math::pow(f64 x,f64 y) {
            return ::pow(x,y);
        }
        
        sv_inline f32 Math::powFast(f32 x,f32 y) {
            return Math::exp2Fast(Math::log2Fast(x) * y);
        }
        
        //
        sv_inline f32 Math::exp(f32 v) {
            return ::expf(v);
        }
        
        sv_inline f64 Math::exp(f64 v) {
            return ::exp(v);
        }
        
        sv_inline f32 Math::expFast(f32 v) {
            return exp2Fast(v * (1.0f / LOG2));
        }
        
        //
        sv_inline f32 Math::exp2(f32 v) {
            return ::expf(v * LOG2);
        }
        
        sv_inline f64 Math::exp2(f64 v) {
            return ::exp(v * LOG2);
        }
        
        //
        sv_inline f32 Math::log(f32 v) {
            return ::logf(v);
        }
        
        sv_inline f64 Math::log(f64 v) {
            return ::log(v);
        }
        
        sv_inline f32 Math::logFast(f32 v) {
            return log2Fast(v) * LOG2;
        }
        
        //
        sv_inline s32 Math::log2(s32 v) {
            s32 ret = 0;
            if(v >= 1 << 16) { v >>= 16; ret |= 16; }
            if(v >= 1 << 8) { v >>= 8; ret |= 8; }
            if(v >= 1 << 4) { v >>= 4; ret |= 4; }
            if(v >= 1 << 2) { v >>= 2; ret |= 2; }
            if(v >= 1 << 1) { ret |= 1; }
            return ret;
        }
        
        sv_inline f32 Math::log2(f32 v) {
            return ::logf(v) * (1.0f / LOG2);
        }
        
        sv_inline f64 Math::log2(f64 v) {
            return ::log(v) * (1.0 / LOG2);
        }
        
        //
        sv_inline f32 Math::log10(f32 v) {
            return ::logf(v) * (1.0f / LOG10);
        }
        
        sv_inline f64 Math::log10(f64 v) {
            return ::log(v) * (1.0 / LOG10);
        }
        
        //
        sv_inline f32 Math::sin(f32 a) {
            return ::sinf(a);
        }
        
        sv_inline f64 Math::sin(f64 a) {
            return ::sin(a);
        }
        
        //
        sv_inline f32 Math::cos(f32 a) {
            return ::cosf(a);
        }
        
        sv_inline f64 Math::cos(f64 a) {
            return ::cos(a);
        }
        
        //
        sv_inline f32 Math::tan(f32 a) {
            return ::tanf(a);
        }
        
        sv_inline f64 Math::tan(f64 a) {
            return ::tan(a);
        }
        
        //
        sv_inline f32 Math::asin(f32 v) {
            return ::asinf(v);
        }
        
        sv_inline f64 Math::asin(f64 v) {
            return ::asin(v);
        }
        
        //
        sv_inline f32 Math::acos(f32 v) {
            return ::acosf(v);
        }
        
        sv_inline f64 Math::acos(f64 v) {
            return ::acos(v);
        }
        
        //
        sv_inline f32 Math::atan(f32 v) {
            return ::atanf(v);
        }
        
        sv_inline f64 Math::atan(f64 v) {
            return ::atan(v);
        }
        
        //
        sv_inline f32 Math::atan2(f32 y,f32 x) {
            return ::atan2f(y,x);
        }
        
        sv_inline f64 Math::atan2(f64 y,f64 x) {
            return ::atan2(y,x);
        }
        
        //
        sv_inline void Math::sincosFast(f32 a,f32 &s,f32 &c) {
            if(a < 0.0f) {
                a -= Math::ftoi(a * (1.0f / PI2)) * PI2 - PI2;
            } else if(a >= PI2) {
              a -= Math::ftoi(a * (1.0f / PI2)) * PI2;
            }
            c = 1.0f;
            s = PI - a;
            if(s < -PI05) {
                s = -PI - s;
            } else if(s > PI05) {
                s = PI - s;
            } else {
                c = -1.0f;
            }
            f32 a2 = s * s;
            s *= ((0.00761f * a2 - 0.16605f) * a2 + 1.0f);
            c *= ((0.03705f * a2 - 0.49670f) * a2 + 1.0f);
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
#ifdef _CELLOS_LV2
            return __fsels(c,v1,v0);
#else
            s32 mask = Math::signMask(IntFloat(c).i);
            return IntFloat((IntFloat(v0).i & mask) | (IntFloat(v1).i & ~mask)).f;
#endif
        }
        
        //
        sv_inline f32 Math::itof(s32 v) {
            return static_cast<f32>(v);
        }
        
        sv_inline s32 Math::ftoi(f32 v) {
#ifdef _CELLOS_LV2
            return __fctiwz(v);
#else
            return static_cast<s32>(v);
#endif
        }
        //四舍五入
        sv_inline s32 Math::round(f32 v) {
#ifdef _CELLOS_LV2
            return __fctiw(v);
#elif defined(_WIN32) && defined(ARCH_X86)
            s32 i;
            __asm {
                fld v
                fistp i
            }
            return i;
#elif defined(_LINUX) && !defined(ARCH_ARM)
            s32 i;
            asm volatile("fistpl %0" : "=m"(i) : "t"(v) : "st");
            return i;
#elif USE_SSE
            return _mm_cvt_ss2si(_mm_load_ss(&v));
#else
            return static_cast<s32>(v + 0.5f);
#endif
        }
        
        //
        sv_inline f64 Math::itod(s32 v) {
            return static_cast<f64>(v);
        }
        
        sv_inline s32 Math::dtoi(f64 v) {
            return static_cast<s32>(v);
        }
        
        //
        sv_inline f32 Math::ltof(s64 v) {
            return static_cast<f32>(v);
        }
        
        sv_inline s64 Math::ftol(f32 v) {
            return static_cast<s64>(v);
        }
        
        //
        sv_inline f64 Math::ltod(s64 v) {
            return static_cast<f64>(v);
        }
        
        sv_inline s64 Math::dtol(f64 v) {
            return static_cast<s64>(v);
        }
        
        //
        sv_inline s32 Math::npot(s32 v) {
            s32 i = 1;
            while(i < v) i += i;
            return i;
        }
        
        sv_inline s32 Math::udiv(s32 x,s32 y) {
            return x / y + (x % y != 0);
        }
        
        //
        sv_inline void Math::prefetch(const void *ptr) {
#ifdef USE_SSE
            _mm_prefetch((const char*)ptr,_MM_HINT_NTA);
#elif USE_ALTIVEC
            __dcbt(ptr);
#elif defined(ARCH_ARM) && (defined(_LINUX) || defined(_ANDROID))
            asm volatile("pld [%r0,#0]" : "=r"(ptr));
#endif
        }
        
        //
#ifdef _WEBGL
        
        //
        sv_inline void Math::memset(void *dest,s32 c,size_t size) {
            ::memset(dest,c,size);
        }
        
        sv_inline void Math::memcpy(void *dest,const void *src,size_t size) {
            ::memcpy(dest,src,size);
        }
        
        sv_inline s32 Math::memcmp(const void *src_0,const void *src_1,size_t size) {
            return ::memcmp(src_0,src_1,size);
        }
        
#endif
        
        //***************************************Scalars***************************************
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
#ifdef _CELLOS_LV2
            return __fsels(v1 - v0,v0,v1);
#else
            return (v0 < v1) ? v0 : v1;
#endif
        }
        
        sv_inline f32 max(f32 v0,f32 v1) {
#ifdef _CELLOS_LV2
            return __fsels(v0 - v1,v0,v1);
#else
            return (v0 > v1) ? v0 : v1;
#endif
        }
        
        sv_inline f32 clamp(f32 v,f32 v0,f32 v1) {
#ifdef _CELLOS_LV2
            v = __fsels(v - v0,v,v0);
            return __fsels(v1 - v,v,v1);
#else
            if(v < v0) return v0;
            if(v > v1) return v1;
            return v;
#endif
        }
        
        sv_inline f32 saturate(f32 v) {
#ifdef _CELLOS_LV2
            v = __fsels(v,v,0.0f);
            return __fsels(1.0f - v,v,1.0f);
#else
            if(v < 0.0f) return 0.0f;
            if(v > 1.0f) return 1.0f;
            return v;
#endif
        }
        
        sv_inline f32 lerp(f32 v0,f32 v1,f32 k) {
            return v0 + (v1 - v0) * k;
        }
        
        //
        sv_inline s32 compare(f64 v0,f64 v1) {
            f64 v = Math::abs(v0 - v1);
            return (v < EPSILON);
        }
        
        sv_inline s32 compare(f64 v0,f64 v1,f64 epsilon) {
            f64 v = Math::abs(v0 - v1);
            return (v < (Math::abs(v0) + Math::abs(v1) + 1.0) * epsilon);
        }
        
        //
        sv_inline f64 min(f64 v0,f64 v1) {
#ifdef _CELLOS_LV2
            return __fsel(v1 - v0,v0,v1);
#else
            return (v0 < v1) ? v0 : v1;
#endif
        }
        
        sv_inline f64 max(f64 v0,f64 v1) {
#ifdef _CELLOS_LV2
            return __fsel(v0 - v1,v0,v1);
#else
            return (v0 > v1) ? v0 : v1;
#endif
        }
        
        sv_inline f64 clamp(f64 v,f64 v0,f64 v1) {
#ifdef _CELLOS_LV2
            v = __fsel(v - v0,v,v0);
            return __fsel(v1 - v,v,v1);
#else
            if(v < v0) return v0;
            if(v > v1) return v1;
            return v;
#endif
        }
        
        sv_inline f64 saturate(f64 v) {
#ifdef _CELLOS_LV2
            v = __fsel(v,v,0.0);
            return __fsel(1.0 - v,v,1.0);
#else
            if(v < 0.0) return 0.0;
            if(v > 1.0) return 1.0;
            return v;
#endif
        }
        
        sv_inline f64 lerp(f64 v0,f64 v1,f64 k) {
            return v0 + (v1 - v0) * k;
        }
        
        //
        sv_inline s32 min(s32 v0,s32 v1) {
            return (v0 < v1) ? v0 : v1;
        }
        
        sv_inline s32 max(s32 v0,s32 v1) {
            return (v0 > v1) ? v0 : v1;
        }
        
        sv_inline s32 clamp(s32 v,s32 v0,s32 v1) {
            if(v < v0) return v0;
            if(v > v1) return v1;
            return v;
        }
        
        sv_inline s32 lerp(s32 v0,s32 v1,s32 k) {
            return v0 + (((v1 - v0) * k) >> 16);
        }

    }//!namespace util
    
}//!namespace sv


#endif /* __MATH_LIB_H__ */
