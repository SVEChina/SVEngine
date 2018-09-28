#ifndef SV_VECBASE_H
#define SV_VECBASE_H

#include "SVMathLib.h"

namespace sv {
    
    namespace util {
        
        //******************************** Vectors ******************************************
        sv_inline f32 dot33(const f32 * sv_restrict v0,const f32 * sv_restrict v1) {
            return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
        }
        
        sv_inline f32 dot34(const f32 * sv_restrict v0,const f32 * sv_restrict v1) {
            return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2] + v1[3];
        }
        
        sv_inline f32 dot43(const f32 * sv_restrict v0,const f32 * sv_restrict v1) {
            return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2] + v0[3];
        }
        
        sv_inline f32 dot44(const f32 * sv_restrict v0,const f32 * sv_restrict v1) {
            return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2] + v0[3] * v1[3];
        }
        
        sv_inline f32 *mul3(f32 * sv_restrict ret,const f32 * sv_restrict v0,f32 v1) {
            ret[0] = v0[0] * v1;
            ret[1] = v0[1] * v1;
            ret[2] = v0[2] * v1;
            return ret;
        }
        
        sv_inline f32 *add3(f32 * sv_restrict ret,const f32 * sv_restrict v0,const f32 * sv_restrict v1) {
            ret[0] = v0[0] + v1[0];
            ret[1] = v0[1] + v1[1];
            ret[2] = v0[2] + v1[2];
            return ret;
        }
        
        sv_inline f32 *sub3(f32 * sv_restrict ret,const f32 * sv_restrict v0,const f32 * sv_restrict v1) {
            ret[0] = v0[0] - v1[0];
            ret[1] = v0[1] - v1[1];
            ret[2] = v0[2] - v1[2];
            return ret;
        }
        
        sv_inline f32 *cross3(f32 * sv_restrict ret,const f32 * sv_restrict v0,const f32 * sv_restrict v1) {
            ret[0] = v0[1] * v1[2] - v0[2] * v1[1];
            ret[1] = v0[2] * v1[0] - v0[0] * v1[2];
            ret[2] = v0[0] * v1[1] - v0[1] * v1[0];
            return ret;
        }
        
        //
#ifdef USE_SSE
        
        //
        sv_inline __m128 _mm_rcp_ss_nr(__m128 v) {
            __m128 iv = _mm_rcp_ss(v);
            iv = _mm_sub_ss(_mm_add_ss(iv,iv),_mm_mul_ss(v,_mm_mul_ss(iv,iv)));
            return _mm_sub_ss(_mm_add_ss(iv,iv),_mm_mul_ss(v,_mm_mul_ss(iv,iv)));
        }
        
        sv_inline __m128 _mm_rcp_ps_nr(__m128 v) {
            __m128 iv = _mm_rcp_ps(v);
            iv = _mm_sub_ps(_mm_add_ps(iv,iv),_mm_mul_ps(v,_mm_mul_ps(iv,iv)));
            return _mm_sub_ps(_mm_add_ps(iv,iv),_mm_mul_ps(v,_mm_mul_ps(iv,iv)));
        }
        
        //
        sv_inline __m128 _mm_rsqrt_ss_nr(__m128 v) {
            __m128 iv = _mm_rsqrt_ss(v);
            __m128 nr = _mm_mul_ss(_mm_mul_ss(v,iv),iv);
            return _mm_mul_ss(_mm_mul_ss(_mm_set1_ps(0.5f),iv),_mm_sub_ss(_mm_set1_ps(3.0f),nr));
        }
        
        sv_inline __m128 _mm_rsqrt_ps_nr(__m128 v) {
            __m128 iv = _mm_rsqrt_ps(v);
            __m128 nr = _mm_mul_ps(_mm_mul_ps(v,iv),iv);
            return _mm_mul_ps(_mm_mul_ps(_mm_set1_ps(0.5f),iv),_mm_sub_ps(_mm_set1_ps(3.0f),nr));
        }
        
        //
        sv_inline __m128 _mm_dot33_ps(__m128 v0,__m128 v1) {
            __m128 v2 = _mm_mul_ps(v0,v1);
            __m128 v3 = _mm_add_ps(v2,_MM_SWIZZLE(v2,Y,X,Y,W));
            return _mm_add_ps(v3,_MM_SWIZZLE(v2,Z,Z,X,W));
        }
        
        sv_inline __m128 _mm_dot44_ps(__m128 v0,__m128 v1) {
            __m128 v2 = _mm_mul_ps(v0,v1);
            v2 = _mm_add_ps(v2,_MM_SWIZZLE(v2,Y,X,W,Z));
            return _mm_add_ps(v2,_MM_SWIZZLE(v2,Z,W,X,Y));
        }
        
        //
        sv_inline __m128 _mm_normalize3_ps(__m128 v) {
            __m128 length2 = _mm_dot33_ps(v,v);
            return _mm_mul_ps(v,_mm_rsqrt_ps_nr(length2));
        }
        
        sv_inline __m128 _mm_normalize4_ps(__m128 v) {
            __m128 length2 = _mm_dot44_ps(v,v);
            return _mm_mul_ps(v,_mm_rsqrt_ps_nr(length2));
        }
        
        //
        sv_inline __m128 _mm_cross_ps(__m128 v0,__m128 v1) {
            __m128 v0_yzxw = _MM_SWIZZLE(v0,Y,Z,X,W);
            __m128 v1_yzxw = _MM_SWIZZLE(v1,Y,Z,X,W);
            __m128 v2 = _mm_sub_ps(_mm_mul_ps(v0,v1_yzxw),_mm_mul_ps(v1,v0_yzxw));
            return _MM_SWIZZLE(v2,Y,Z,X,W);
        }
        
        //
#elif USE_ALTIVEC
        
        //
        sv_inline vec_float4 vec_rcp_nr(vec_float4 v) {
            vec_float4 iv = vec_re(v);
            vec_float4 one = vec_splats(1.0f);
            iv = vec_madd(vec_nmsub(iv,v,one),iv,iv);
            return vec_madd(vec_nmsub(iv,v,one),iv,iv);
        }
        
        //
        sv_inline vec_float4 vec_rsqrt_nr(vec_float4 v) {
            vec_float4 iv = vec_rsqrte(v);
            vec_float4 zero = vec_splats(0.0f);
            vec_float4 nr = vec_madd(vec_madd(v,iv,zero),iv,zero);
            return vec_madd(vec_madd(vec_splats(0.5f),iv,zero),vec_sub(vec_splats(3.0f),nr),zero);
        }
        
        //
        sv_inline vec_float4 vec_dot33(vec_float4 v0,vec_float4 v1) {
            vec_float4 v2 = vec_madd(v0,v1,vec_splats(0.0f));
            vec_float4 v3 = vec_add(v2,VEC_SWIZZLE(v2,Y,X,Y,W));
            return vec_add(v3,VEC_SWIZZLE(v2,Z,Z,X,W));
        }
        
        sv_inline vec_float4 vec_dot44(vec_float4 v0,vec_float4 v1) {
            vec_float4 v2 = vec_madd(v0,v1,vec_splats(0.0f));
            v2 = vec_add(v2,vec_sld(v2,v2,8));
            return vec_add(v2,vec_sld(v2,v2,4));
        }
        
        //
        sv_inline vec_float4 vec_normalize3(vec_float4 v) {
            vec_float4 length2 = vec_dot33(v,v);
            return vec_madd(v,vec_rsqrt_nr(length2),vec_splats(0.0f));
        }
        
        sv_inline vec_float4 vec_normalize4(vec_float4 v) {
            vec_float4 length2 = vec_dot44(v,v);
            return vec_madd(v,vec_rsqrt_nr(length2),vec_splats(0.0f));
        }
        
        //
        sv_inline vec_float4 vec_cross(vec_float4 v0,vec_float4 v1) {
            vec_uchar16 yzxw = VEC_PERM2(Y,Z,X,W);
            vec_float4 v0_yzxw = vec_perm(v0,v0,yzxw);
            vec_float4 v1_yzxw = vec_perm(v1,v1,yzxw);
            vec_float4 v2 = vec_nmsub(v1,v0_yzxw,vec_madd(v0,v1_yzxw,vec_splats(0.0f)));
            return vec_perm(v2,v2,yzxw);
        }
        
        //
#elif USE_NEON
        
        //
        sv_inline s32 vmask_u32(uint32x2_t v) {
            const uint32x2_t mask = NEON_UINT2(1,2);
#if defined(_WINRT) || defined(_IOS)
            uint32x2_t res_0 = vand_u32(v,mask);
            res_0 = vpadd_u32(res_0,res_0);
            return vget_lane_u32(res_0,0);
#else
            s32 ret;
            asm volatile(
                         "vand.u32  %P1, %P1, %P2    \n"
                         "vpadd.u32 %P1, %P1, %P1    \n"
                         "vmov.u32  %r0, %P1[0]        \n"
                         : "=r"(ret) : "w"(v), "w"(mask)
                         );
            return ret;
#endif
        }
        
        sv_inline s32 vmaskq_u32(uint32x4_t v) {
            const uint32x4_t mask = NEON_UINT4(1,2,4,8);
#if defined(_WINRT) || defined(_IOS)
            uint32x4_t res_0 = vandq_u32(v,mask);
            uint32x2_t res_1 = vget_low_u32(res_0);
            uint32x2_t res_2 = vget_high_u32(res_0);
            res_1 = vorr_u32(res_1,res_2);
            res_1 = vpadd_u32(res_1,res_1);
            return vget_lane_u32(res_1,0);
#else
            s32 ret;
            asm volatile(
                         "vand.u32  %q1, %q1, %q2    \n"
                         "vorr.u32  %e1, %e1, %f1    \n"
                         "vpadd.u32 %e1, %e1, %e1    \n"
                         "vmov.u32  %r0, %e1[0]        \n"
                         : "=r"(ret) : "w"(v), "w"(mask)
                         );
            return ret;
#endif
        }
        
        //
        sv_inline float32x2_t vrcp_nr_f32(float32x2_t v) {
#if defined(_WINRT) || defined(_IOS)
            float32x2_t res_0 = vrecpe_f32(v);
            float32x2_t res_1 = vrecps_f32(res_0,v);
            res_0 = vmul_f32(res_0,res_1);
            res_1 = vrecps_f32(res_0,v);
            return vmul_f32(res_0,res_1);
#else
            float32x2_t ret;
            asm volatile(
                         "vrecpe.f32  d0, %P1        \n"
                         "vrecps.f32  d1,  d0, %P1    \n"
                         "vmul.f32    d0,  d0,  d1    \n"
                         "vrecps.f32  d1,  d0, %P1    \n"
                         "vmul.f32   %P0,  d0,  d1    \n"
                         : "=w"(ret) : "w"(v) : "d0", "d1"
                         );
            return ret;
#endif
        }
        
        sv_inline float32x4_t vrcpq_nr_f32(float32x4_t v) {
#if defined(_WINRT) || defined(_IOS)
            float32x4_t res_0 = vrecpeq_f32(v);
            float32x4_t res_1 = vrecpsq_f32(res_0,v);
            res_0 = vmulq_f32(res_0,res_1);
            res_1 = vrecpsq_f32(res_0,v);
            return vmulq_f32(res_0,res_1);
#else
            float32x4_t ret;
            asm volatile(
                         "vrecpe.f32  q0, %q1        \n"
                         "vrecps.f32  q1,  q0, %q1    \n"
                         "vmul.f32    q0,  q0,  q1    \n"
                         "vrecps.f32  q1,  q0, %q1    \n"
                         "vmul.f32   %q0,  q0,  q1    \n"
                         : "=w"(ret) : "w"(v) : "q0", "q1"
                         );
            return ret;
#endif
        }
        
        //
        sv_inline float32x2_t vrsqrt_nr_f32(float32x2_t v) {
#if defined(_WINRT) || defined(_IOS)
            float32x2_t res_0 = vrsqrte_f32(v);
            float32x2_t res_1 = vmul_f32(res_0,v);
            float32x2_t res_2 = vrsqrts_f32(res_0,res_1);
            return vmul_f32(res_0,res_2);
#else
            float32x2_t ret;
            asm volatile(
                         "vrsqrte.f32  d0, %P1        \n"
                         "vmul.f32     d1,  d0, %P1    \n"
                         "vrsqrts.f32  d2,  d0,  d1    \n"
                         "vmul.f32    %P0,  d0,  d2    \n"
                         : "=w"(ret) : "w"(v) : "d0", "d1", "d2"
                         );
            return ret;
#endif
        }
        
        sv_inline float32x4_t vrsqrtq_nr_f32(float32x4_t v) {
#if defined(_WINRT) || defined(_IOS)
            float32x4_t res_0 = vrsqrteq_f32(v);
            float32x4_t res_1 = vmulq_f32(res_0,v);
            float32x4_t res_2 = vrsqrtsq_f32(res_0,res_1);
            return vmulq_f32(res_0,res_2);
#else
            float32x4_t ret;
            asm volatile(
                         "vrsqrte.f32  q0, %q1        \n"
                         "vmul.f32     q1,  q0, %q1    \n"
                         "vrsqrts.f32  q2,  q0,  q1    \n"
                         "vmul.f32    %q0,  q0,  q2    \n"
                         : "=w"(ret) : "w"(v) : "q0", "q1", "q2"
                         );
            return ret;
#endif
        }
        
        //
        sv_inline float32x4_t vdot33q_f32(float32x4_t v0,float32x4_t v1) {
#if defined(_WINRT) || defined(_IOS)
            float32x4_t res_0 = vmulq_f32(v0,v1);
            float32x2_t res_1 = vget_low_f32(res_0);
            float32x2_t res_2 = vget_high_f32(res_0);
            float32x2_t res_3 = vpadd_f32(res_1,res_1);
            res_3 = vadd_f32(res_3,res_2);
            return vdupq_lane_f32(res_3,0);
#else
            float32x4_t ret;
            asm volatile(
                         "vmul.f32   q0, %q1,   %q2    \n"
                         "vpadd.f32  d0,  d0,    d0    \n"
                         "vadd.f32   d0,  d0,    d1    \n"
                         "vdup.32   %q0,  d0[0]        \n"
                         : "=w"(ret) : "w"(v0), "w"(v1) : "q0"
                         );
            return ret;
#endif
        }
        
        sv_inline float32x4_t vdot44q_f32(float32x4_t v0,float32x4_t v1) {
#if defined(_WINRT) || defined(_IOS)
            float32x4_t res_0 = vmulq_f32(v0,v1);
            float32x4_t res_1 = vextq_f32(res_0,res_0,2);
            res_0 = vaddq_f32(res_0,res_1);
            res_1 = vextq_f32(res_0,res_0,1);
            return vaddq_f32(res_0,res_1);
#else
            float32x4_t ret;
            asm volatile(
                         "vmul.f32  q0, %q1, %q2        \n"
                         "vext.32   q1,  q0,  q0, #2    \n"
                         "vadd.f32  q0,  q0,  q1        \n"
                         "vext.32   q1,  q0,  q0, #1    \n"
                         "vadd.f32 %q0,  q0,  q1        \n"
                         : "=w"(ret) : "w"(v0), "w"(v1) : "q0", "q1"
                         );
            return ret;
#endif
        }
        
        //
        sv_inline float32x4_t vnormalize3q_f32(float32x4_t v) {
            float32x4_t length2 = vdot33q_f32(v,v);
            return vmulq_f32(v,vrsqrtq_nr_f32(length2));
        }
        
        sv_inline float32x4_t vnormalize4q_f32(float32x4_t v) {
            float32x4_t length2 = vdot44q_f32(v,v);
            return vmulq_f32(v,vrsqrtq_nr_f32(length2));
        }
        
        //
        sv_inline float32x4_t vcrossq_f32(float32x4_t v0,float32x4_t v1) {
#if defined(_WINRT) || defined(_IOS)
            float32x2_t low_0 = vget_low_f32(v0);
            float32x2_t low_1 = vget_low_f32(v1);
            float32x2_t high_0 = vget_high_f32(v0);
            float32x2_t high_1 = vget_high_f32(v1);
            float32x2_t res_0 = vext_f32(low_0,high_0,1);
            float32x2_t res_1 = vext_f32(low_1,high_1,1);
            float32x2_t res_2 = vmul_f32(high_0,low_1);
            float32x2_t res_3 = vmul_f32(low_0,res_1);
            res_2 = vmls_f32(res_2,high_1,low_0);
            res_3 = vmls_f32(res_3,low_1,res_0);
            res_0 = vext_f32(res_3,res_2,1);
            return vcombine_f32(res_0,res_3);
#else
            float32x4_t ret;
            asm volatile(
                         "vext.32   d0, %e1, %f1, #1    \n"
                         "vext.32   d1, %e2, %f2, #1    \n"
                         "vmul.f32  d2, %f1, %e2        \n"
                         "vmul.f32  d3, %e1,  d1        \n"
                         "vmls.f32  d2, %f2, %e1        \n"
                         "vmls.f32  d3, %e2,  d0        \n"
                         "vext.32  %e0,  d3,  d2, #1    \n"
                         "vmov     %f0,  d3            \n"
                         : "=w"(ret) : "w"(v0), "w"(v1) : "q0", "q1"
                         );
            return ret;
#endif
        }
        
#endif
    
    }//!namespace util
    
}//!namespace sv


#endif /* SV_VECBASE_H */
