#include "SVMathLib.h"

namespace sv {
    
    namespace util {
        
        //
        Math::Math() {
            
        }

        //
        f32 Math::exp2Fast(f32 v) {
            s32 i = ftoi(v - 0.5f);
            v = v - itof(i);
            return IntFloat((i + 127) << 23).f * (((((0.0018775767f * v + 0.0089893397f) * v + 0.055826318f) * v + 0.24015361f) * v + 0.69315308f) * v + 0.99999994f);
        }

        //
        f32 Math::log2Fast(f32 v) {
            s32 i = IntFloat(v).i;
            s32 e = ((i >> 23) & 0xff) - 127;
            v = IntFloat((i & 0x007fffff) | 0x3f800000).f;
            return itof(e) + (v - 1.0f) * (((((-0.034436006f * v + 0.31821337f) * v - 1.2315303f) * v + 2.5988452f) * v - 3.3241990f) * v + 3.1157899f);
        }

        //
        f32 Math::sinFast(f32 a) {
            f32 a2 = a * a;
            return a * (((((-2.39e-08f * a2 + 2.7526e-06f) * a2 - 1.98409e-04f) * a2 + 0.0083333315f) * a2 - 0.1666666664f) * a2 + 1.0f);
        }

        //
        f32 Math::cosFast(f32 a) {
            f32 a2 = a * a;
            return (((((-2.605e-07f * a2 + 2.47609e-05f) * a2 - 0.0013888397f) * a2 + 0.0416666418f) * a2 - 0.4999999963f) * a2 + 1.0f);
        }

        //
        f32 Math::asinFast(f32 v) {
            return PI05 - (((((((-0.0012624911f * v + 0.0066700901f) * v - 0.0170881256f) * v + 0.0308918810f) * v - 0.0501743046f) * v + 0.0889789874f) * v - 0.2145988016f) * v + 1.5707963050f) * Math::sqrt(1.0f - v);
        }

        //
        f32 Math::acosFast(f32 v) {
            return (((((((-0.0012624911f * v + 0.0066700901f) * v - 0.0170881256f) * v + 0.0308918810f) * v - 0.0501743046f) * v + 0.0889789874f) * v - 0.2145988016f) * v + 1.5707963050f) * Math::sqrt(1.0f - v);
        }

        //
        void Math::sincos(f32 angle,f32 &s,f32 &c) {
            #if defined(_WIN32) && defined(ARCH_X86)
                __asm {
                    fld angle
                    fsincos
                    mov edx, s
                    mov ecx, c
                    fstp dword ptr [ecx]
                    fstp dword ptr [edx]
                }
            #elif defined(_LINUX) && !defined(ARCH_ARM)
                f64 res_0,res_1;
                asm volatile("fsincos" : "=t"(res_0), "=u"(res_1) : "0"(angle));
                s = (f32)res_1;
                c = (f32)res_0;
            #else
                s = Math::sin(angle);
                c = Math::cos(angle);
            #endif
        }

        void Math::sincos(f64 angle,f64 &s,f64 &c) {
            #if defined(_WIN32) && defined(ARCH_X86)
                __asm {
                    fld angle
                    fsincos
                    mov edx, s
                    mov ecx, c
                    fstp qword ptr [ecx]
                    fstp qword ptr [edx]
                }
            #elif defined(_LINUX) && !defined(ARCH_ARM)
                asm volatile("fsincos" : "=t"(c), "=u"(s) : "0"(angle));
            #else
                s = Math::sin(angle);
                c = Math::cos(angle);
            #endif
        }

        //
        template <class Type>
        static sv_inline Type bezier_func(const Type *curve,f32 k0) {
            f32 k1 = 1.0f - k0;
            f32 k00 = k0 * k0;
            f32 k11 = k1 * k1;
            return curve[0] * (k11 * k1) + 3.0f * (curve[1] * (k11 * k0) + curve[2] * (k00 * k1)) + curve[3] * (k00 * k0);
        }

        f32 Math::bezier(const f32 *times,const f32 *values,f32 time) {
            f32 k0 = 0.0f;
            f32 k1 = 1.0f;
            f32 t1 = bezier_func(times,k1) - time;
            if(abs(t1) < EPSILON) return bezier_func(values,k1);
            f32 t0 = bezier_func(times,k0) - time;
            if(abs(t0) < EPSILON || t0 * t1 > 0.0f) return bezier_func(values,k0);
            for(s32 i = 0; i < 20; i++) {
                f32 k = (k0 + k1) * 0.5f;
                f32 t = bezier_func(times,k) - time;
                f32 v = t0 * t;
                if(v < 0.0f) {
                    k1 = k;
                    t1 = t;
                } else if(v > 0.0f) {
                    k0 = k;
                    t0 = t;
                } else {
                    return bezier_func(values,k);
                }
            }
            return bezier_func(values,k0);
        }

        f64 Math::bezier(const f32 *times,const f64 *values,f32 time) {
            f32 k0 = 0.0f;
            f32 k1 = 1.0f;
            f32 t1 = bezier_func(times,k1) - time;
            if(abs(t1) < EPSILON) return bezier_func(values,k1);
            f32 t0 = bezier_func(times,k0) - time;
            if(abs(t0) < EPSILON || t0 * t1 > 0.0f) return bezier_func(values,k0);
            for(s32 i = 0; i < 20; i++) {
                f32 k = (k0 + k1) * 0.5f;
                f32 t = bezier_func(times,k) - time;
                f32 v = t0 * t;
                if(v < 0.0f) {
                    k1 = k;
                    t1 = t;
                } else if(v > 0.0f) {
                    k0 = k;
                    t0 = t;
                } else {
                    return bezier_func(values,k);
                }
            }
            return bezier_func(values,k0);
        }

        /******************************************************************************\
        *
        * Memory
        *
        \******************************************************************************/

        //
#ifndef _WEBGL

        //
        static sv_inline void math_memset_64(void *dest,u32 src,size_t size) {

            ASSERT_ALIGNED16(dest);
            #if defined(USE_SSE) || defined(USE_ALTIVEC)
                ATTRIBUTE_ALIGNED16(u32 data[4]);
                data[0] = src;
                data[1] = src;
                data[2] = src;
                data[3] = src;
                #ifdef USE_SSE
                    __m128 res = _mm_load_ps((const f32*)data);
                #elif USE_ALTIVEC
                    vec_uchar16 res = vec_ld(0,(const u8*)data);
                #endif
            #endif
            
            u32 *d = (u32*)dest;
            for(size_t i = size >> 6; i > 0; i--) {
                #ifdef USE_SSE
                    _mm_stream_ps((f32*)d + 0,res);
                    _mm_stream_ps((f32*)d + 4,res);
                    _mm_stream_ps((f32*)d + 8,res);
                    _mm_stream_ps((f32*)d + 12,res);
                #elif USE_ALTIVEC
                    vec_st(res,0,(u8*)d);
                    vec_st(res,16,(u8*)d);
                    vec_st(res,32,(u8*)d);
                    vec_st(res,48,(u8*)d);
                #else
                    d[0] = src;
                    d[1] = src;
                    d[2] = src;
                    d[3] = src;
                    d[4] = src;
                    d[5] = src;
                    d[6] = src;
                    d[7] = src;
                    d[8] = src;
                    d[9] = src;
                    d[10] = src;
                    d[11] = src;
                    d[12] = src;
                    d[13] = src;
                    d[14] = src;
                    d[15] = src;
                #endif
                
                d += 16;
            }
        }

        //
        void Math::memset(void *dest,s32 c,size_t size) {
            
            u8 *d = (u8*)dest;
            
            u32 src = (c) | (c << 8) | (c << 16) | (c << 24);
            
            if(size & ~63) {
                
                while(IS_ALIGNED16(d) == 0) {
                    *d++ = c;
                    size--;
                }
                
                if(size & ~63) {
                    math_memset_64(d,src,size);
                    d += (size & ~63);
                    size &= 63;
                }
            }
            
            if(size & ~15) {
                
                for(size_t i = size >> 4; i > 0; i--) {
                    *(u32*)(d + 0) = src;
                    *(u32*)(d + 4) = src;
                    *(u32*)(d + 8) = src;
                    *(u32*)(d + 12) = src;
                    d += 16;
                }
                
                size &= 15;
            }
            
            if(size) {
                
                for(size_t i = size; i > 0; i--) {
                    *d++ = c;
                }
            }
        }

        //
        static sv_inline void math_memcpy_64a(void *dest,const void *src,size_t size) {
            
            ASSERT_ALIGNED16(src);
            ASSERT_ALIGNED16(dest);
            
            u32 *d = (u32*)dest;
            const u32 *s = (const u32*)src;
            for(size_t i = size >> 6; i > 0; i--) {
                
                #ifdef USE_SSE
                
                    _mm_prefetch((const char*)s + 512,_MM_HINT_NTA);
                
                    __m128 res_0 = _mm_load_ps((const f32*)s + 0);
                    __m128 res_1 = _mm_load_ps((const f32*)s + 4);
                    __m128 res_2 = _mm_load_ps((const f32*)s + 8);
                    __m128 res_3 = _mm_load_ps((const f32*)s + 12);
                
                    _mm_stream_ps((f32*)d + 0,res_0);
                    _mm_stream_ps((f32*)d + 4,res_1);
                    _mm_stream_ps((f32*)d + 8,res_2);
                    _mm_stream_ps((f32*)d + 12,res_3);
                
                #elif USE_ALTIVEC
                
                    __dcbt((const u8*)s + 512);
                
                    vec_uchar16 res_0 = vec_ld(0,(const u8*)s);
                    vec_uchar16 res_1 = vec_ld(16,(const u8*)s);
                    vec_uchar16 res_2 = vec_ld(32,(const u8*)s);
                    vec_uchar16 res_3 = vec_ld(48,(const u8*)s);
                
                    vec_st(res_0,0,(u8*)d);
                    vec_st(res_1,16,(u8*)d);
                    vec_st(res_2,32,(u8*)d);
                    vec_st(res_3,48,(u8*)d);
                
                #elif defined(USE_NEON) && (defined(_LINUX) || defined(_ANDROID))
                
                    asm volatile(
                        "pld  [ %r1, #512 ]			\n"
                        "vldm   %r1, { d0 - d7 }	\n"
                        "vstm   %r0, { d0 - d7 }	\n"
                        : : "r"(d), "r"(s)
                        : "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7"
                    );
                
                #elif defined(ARCH_ARM) && (defined(_LINUX) || defined(_ANDROID))
                
                    asm volatile(
                        "mov   r0, %r0				\n"
                        "mov   r1, %r1				\n"
                        "pld [ r1, #512 ]			\n"
                        "ldm   r1!, { r2 - r9 }		\n"
                        "stm   r0!, { r2 - r9 }		\n"
                        "ldm   r1,  { r2 - r9 }		\n"
                        "stm   r0,  { r2 - r9 }		\n"
                        : : "r"(d), "r"(s)
                        : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9"
                    );
                
                #else
                
                    d[0] = s[0];
                    d[1] = s[1];
                    d[2] = s[2];
                    d[3] = s[3];
                    d[4] = s[4];
                    d[5] = s[5];
                    d[6] = s[6];
                    d[7] = s[7];
                    d[8] = s[8];
                    d[9] = s[9];
                    d[10] = s[10];
                    d[11] = s[11];
                    d[12] = s[12];
                    d[13] = s[13];
                    d[14] = s[14];
                    d[15] = s[15];
                
                #endif
                
                s += 16;
                d += 16;
            }
        }

        static sv_inline void math_memcpy_64u(void *dest,const void *src,size_t size) {
            
            ASSERT_ALIGNED16(dest);
            
            #ifdef USE_ALTIVEC
                vec_uchar16 mask = vec_lvsl(0,(const u8*)src);
            #endif
            
            u32 *d = (u32*)dest;
            const u32 *s = (const u32*)src;
            for(size_t i = size >> 6; i > 0; i--) {
                
                #ifdef USE_SSE
                
                    _mm_prefetch((const char*)s + 512,_MM_HINT_NTA);
                
                    __m128 res_0 = _mm_loadu_ps((const f32*)s + 0);
                    __m128 res_1 = _mm_loadu_ps((const f32*)s + 4);
                    __m128 res_2 = _mm_loadu_ps((const f32*)s + 8);
                    __m128 res_3 = _mm_loadu_ps((const f32*)s + 12);
                
                    _mm_stream_ps((f32*)d + 0,res_0);
                    _mm_stream_ps((f32*)d + 4,res_1);
                    _mm_stream_ps((f32*)d + 8,res_2);
                    _mm_stream_ps((f32*)d + 12,res_3);
                
                #elif USE_ALTIVEC
                
                    __dcbt((const u8*)s + 512);
                
                    vec_uchar16 res_0 = vec_ld(0,(const u8*)s);
                    vec_uchar16 res_1 = vec_ld(15,(const u8*)s);
                    vec_uchar16 res_2 = vec_ld(31,(const u8*)s);
                    vec_uchar16 res_3 = vec_ld(47,(const u8*)s);
                    vec_uchar16 res_4 = vec_ld(63,(const u8*)s);
                
                    vec_st(vec_perm(res_0,res_1,mask),0,(u8*)d);
                    vec_st(vec_perm(res_1,res_2,mask),16,(u8*)d);
                    vec_st(vec_perm(res_2,res_3,mask),32,(u8*)d);
                    vec_st(vec_perm(res_3,res_4,mask),48,(u8*)d);
                
                #else
                
                    d[0] = s[0];
                    d[1] = s[1];
                    d[2] = s[2];
                    d[3] = s[3];
                    d[4] = s[4];
                    d[5] = s[5];
                    d[6] = s[6];
                    d[7] = s[7];
                    d[8] = s[8];
                    d[9] = s[9];
                    d[10] = s[10];
                    d[11] = s[11];
                    d[12] = s[12];
                    d[13] = s[13];
                    d[14] = s[14];
                    d[15] = s[15];
                
                #endif
                
                s += 16;
                d += 16;
            }
        }

        //
        void Math::memcpy(void *dest,const void *src,size_t size) {
            
            u8 *d = (u8*)dest;
            const u8 *s = (const u8*)src;
            
            if(size & ~63) {
                
                while(IS_ALIGNED16(d) == 0) {
                    *d++ = *s++;
                    size--;
                }
                
                if(size & ~63) {
                    if(IS_ALIGNED16(s)) math_memcpy_64a(d,s,size);
                    else math_memcpy_64u(d,s,size);
                    s += (size & ~63);
                    d += (size & ~63);
                    size &= 63;
                }
            }
            
            if(size & ~15) {
                
                while(IS_ALIGNED4(d) == 0) {
                    *d++ = *s++;
                    size--;
                }
                
                for(size_t i = size >> 4; i > 0; i--) {
                    *(u32*)(d + 0) = *(const u32*)(s + 0);
                    *(u32*)(d + 4) = *(const u32*)(s + 4);
                    *(u32*)(d + 8) = *(const u32*)(s + 8);
                    *(u32*)(d + 12) = *(const u32*)(s + 12);
                    s += 16;
                    d += 16;
                }
                
                size &= 15;
            }
            
            if(size & ~3) {
                
                for(size_t i = size >> 2; i > 0; i--) {
                    *(u32*)d = *(const u32*)s;
                    s += 4;
                    d += 4;
                }
                
                size &= 3;
            }
            
            if(size) {
                
                for(size_t i = size; i > 0; i--) {
                    *d++ = *s++;
                }
            }
        }

        //
        s32 Math::memcmp(const void *src_0,const void *src_1,size_t size) {
            
            const u8 *s0 = (const u8*)src_0;
            const u8 *s1 = (const u8*)src_1;
            
            if(size & ~3) {
                
                for(size_t i = size >> 2; i > 0; i--) {
                    u32 i0 = *(u32*)s0;
                    u32 i1 = *(u32*)s1;
                    if(i0 != i1) {
                        for(s32 j = 0; j < 4; j++) {
                            u8 c0 = *s0++;
                            u8 c1 = *s1++;
                            if(c0 < c1) return -1;
                            if(c0 > c1) return 1;
                        }
                    }
                    s0 += 4;
                    s1 += 4;
                }
                
                size &= 3;
            }
            
            if(size) {
                
                for(size_t i = size; i > 0; i--) {
                    u8 c0 = *s0++;
                    u8 c1 = *s1++;
                    if(c0 < c1) return -1;
                    if(c0 > c1) return 1;
                }
            }
            
            return 0;
        }

#endif /* _WEBGL */


        //
        void Math::setGL() {
            //hardwareProjection = hardwareProjectionGL;
        }

        void Math::setD3D9() {
            //hardwareProjection = hardwareProjectionD3D9;
        }

        void Math::setD3D10() {
            //hardwareProjection = hardwareProjectionD3D10;
        }

    }//!namesapce util
    
}//!namespace sv
