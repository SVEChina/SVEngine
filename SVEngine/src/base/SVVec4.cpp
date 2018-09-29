#include "SVVec4.h"

namespace sv {
    
    namespace util {

        //*********************************FVec4*********************************************
        //
        const FVec4 FVec4_zero(0.0f);
        const FVec4 FVec4_one(1.0f);
        const FVec4 FVec4_epsilon(EPSILON);
        const FVec4 FVec4_infinity(INFINITY);

        //
        FVec4 min(const FVec4 &v0,const FVec4 &v1) {
            FVec4 ret;
            #ifdef USE_SSE
                ret.vec = _mm_min_ps(v0.vec,v1.vec);
            #elif USE_ALTIVEC
                ret.vec = vec_min(v0.vec,v1.vec);
            #elif USE_NEON
                ret.vec = vminq_f32(v0.vec,v1.vec);
            #else
                ret.x = min(v0.x,v1.x);
                ret.y = min(v0.y,v1.y);
                ret.z = min(v0.z,v1.z);
                ret.w = min(v0.w,v1.w);
            #endif
            return ret;
        }

        FVec4 max(const FVec4 &v0,const FVec4 &v1) {
            FVec4 ret;
            #ifdef USE_SSE
                ret.vec = _mm_max_ps(v0.vec,v1.vec);
            #elif USE_ALTIVEC
                ret.vec = vec_max(v0.vec,v1.vec);
            #elif USE_NEON
                ret.vec = vmaxq_f32(v0.vec,v1.vec);
            #else
                ret.x = max(v0.x,v1.x);
                ret.y = max(v0.y,v1.y);
                ret.z = max(v0.z,v1.z);
                ret.w = max(v0.w,v1.w);
            #endif
            return ret;
        }

        FVec4 clamp(const FVec4 &v,const FVec4 &v0,const FVec4 &v1) {
            FVec4 ret;
            #ifdef USE_SSE
                ret.vec = _mm_min_ps(_mm_max_ps(v.vec,v0.vec),v1.vec);
            #elif USE_ALTIVEC
                ret.vec = vec_min(vec_max(v.vec,v0.vec),v1.vec);
            #elif USE_NEON
                ret.vec = vminq_f32(vmaxq_f32(v.vec,v0.vec),v1.vec);
            #else
                ret.x = clamp(v.x,v0.x,v1.x);
                ret.y = clamp(v.y,v0.y,v1.y);
                ret.z = clamp(v.z,v0.z,v1.z);
                ret.w = clamp(v.w,v0.w,v1.w);
            #endif
            return ret;
        }

        FVec4 saturate(const FVec4 &v) {
            FVec4 ret;
            #ifdef USE_SSE
                ret.vec = _mm_min_ps(_mm_max_ps(v.vec,FVec4_zero.vec),FVec4_one.vec);
            #elif USE_ALTIVEC
                ret.vec = vec_min(vec_max(v.vec,FVec4_zero.vec),FVec4_one.vec);
            #elif USE_NEON
                ret.vec = vminq_f32(vmaxq_f32(v.vec,FVec4_zero.vec),FVec4_one.vec);
            #else
                ret.x = saturate(v.x);
                ret.y = saturate(v.y);
                ret.z = saturate(v.z);
                ret.w = saturate(v.w);
            #endif
            return ret;
        }

        FVec4 lerp(const FVec4 &v0,const FVec4 &v1,f32 k) {
            FVec4 ret;
            return lerp(ret,v0,v1,k);
        }

        //*********************************DVec4*********************************************
        //
        const DVec4 DVec4_zero(0.0);
        const DVec4 DVec4_one(1.0);
        const DVec4 DVec4_epsilon(EPSILON);
        const DVec4 DVec4_infinity(INFINITY);

        //
        DVec4 min(const DVec4 &v0,const DVec4 &v1) {
            DVec4 ret;
            #ifdef USE_SSE2
                ret.vec0 = _mm_min_pd(v0.vec0,v1.vec0);
                ret.vec1 = _mm_min_pd(v0.vec1,v1.vec1);
            #else
                ret.x = min(v0.x,v1.x);
                ret.y = min(v0.y,v1.y);
                ret.z = min(v0.z,v1.z);
                ret.w = min(v0.w,v1.w);
            #endif
            return ret;
        }

        DVec4 max(const DVec4 &v0,const DVec4 &v1) {
            DVec4 ret;
            #ifdef USE_SSE2
                ret.vec0 = _mm_max_pd(v0.vec0,v1.vec0);
                ret.vec1 = _mm_max_pd(v0.vec1,v1.vec1);
            #else
                ret.x = max(v0.x,v1.x);
                ret.y = max(v0.y,v1.y);
                ret.z = max(v0.z,v1.z);
                ret.w = max(v0.w,v1.w);
            #endif
            return ret;
        }

        DVec4 clamp(const DVec4 &v,const DVec4 &v0,const DVec4 &v1) {
            DVec4 ret;
            #ifdef USE_SSE2
                ret.vec0 = _mm_min_pd(_mm_max_pd(v.vec0,v0.vec0),v1.vec0);
                ret.vec1 = _mm_min_pd(_mm_max_pd(v.vec1,v0.vec1),v1.vec1);
            #else
                ret.x = clamp(v.x,v0.x,v1.x);
                ret.y = clamp(v.y,v0.y,v1.y);
                ret.z = clamp(v.z,v0.z,v1.z);
                ret.w = clamp(v.w,v0.w,v1.w);
            #endif
            return ret;
        }

        DVec4 saturate(const DVec4 &v) {
            DVec4 ret;
            #ifdef USE_SSE2
                ret.vec0 = _mm_min_pd(_mm_max_pd(v.vec0,DVec2_zero.vec),DVec2_one.vec);
                ret.vec1 = _mm_min_pd(_mm_max_pd(v.vec1,DVec2_zero.vec),DVec2_one.vec);
            #else
                ret.x = saturate(v.x);
                ret.y = saturate(v.y);
                ret.z = saturate(v.z);
                ret.w = saturate(v.w);
            #endif
            return ret;
        }

        DVec4 lerp(const DVec4 &v0,const DVec4 &v1,f64 k) {
            DVec4 ret;
            return lerp(ret,v0,v1,k);
        }

        //*********************************HVec4*********************************************
        //
        const HVec4 HVec4_zero(0.0f);
        const HVec4 HVec4_one(1.0f);

        //*********************************IVec4*********************************************
        //
        const IVec4 IVec4_zero(0);
        const IVec4 IVec4_one(1);

        //
        IVec4 min(const IVec4 &v0,const IVec4 &v1) {
            IVec4 ret;
            #ifdef USE_NEON
                ret.vec = vminq_s32(v0.vec,v1.vec);
            #else
                ret.x = min(v0.x,v1.x);
                ret.y = min(v0.y,v1.y);
                ret.z = min(v0.z,v1.z);
                ret.w = min(v0.w,v1.w);
            #endif
            return ret;
        }

        IVec4 max(const IVec4 &v0,const IVec4 &v1) {
            IVec4 ret;
            #ifdef USE_NEON
                ret.vec = vmaxq_s32(v0.vec,v1.vec);
            #else
                ret.x = max(v0.x,v1.x);
                ret.y = max(v0.y,v1.y);
                ret.z = max(v0.z,v1.z);
                ret.w = max(v0.w,v1.w);
            #endif
            return ret;
        }

        IVec4 clamp(const IVec4 &v,const IVec4 &v0,const IVec4 &v1) {
            IVec4 ret;
            #ifdef USE_NEON
                ret.vec = vminq_s32(vmaxq_s32(v.vec,v0.vec),v1.vec);
            #else
                ret.x = clamp(v.x,v0.x,v1.x);
                ret.y = clamp(v.y,v0.y,v1.y);
                ret.z = clamp(v.z,v0.z,v1.z);
                ret.w = clamp(v.w,v0.w,v1.w);
            #endif
            return ret;
        }

        IVec4 lerp(const IVec4 &v0,const IVec4 &v1,s32 k) {
            IVec4 ret;
            return lerp(ret,v0,v1,k);
        }

        //*********************************BVec4*********************************************
        //
        const BVec4 BVec4_zero((u8)0);
        const BVec4 BVec4_one((u8)255);

        //
        BVec4 min(const BVec4 &v0,const BVec4 &v1) {
            BVec4 ret;
            ret.x = min(v0.x,v1.x);
            ret.y = min(v0.y,v1.y);
            ret.z = min(v0.z,v1.z);
            ret.w = min(v0.w,v1.w);
            return ret;
        }

        BVec4 max(const BVec4 &v0,const BVec4 &v1) {
            BVec4 ret;
            ret.x = max(v0.x,v1.x);
            ret.y = max(v0.y,v1.y);
            ret.z = max(v0.z,v1.z);
            ret.w = max(v0.w,v1.w);
            return ret;
        }

        BVec4 clamp(const BVec4 &v,const BVec4 &v0,const BVec4 &v1) {
            BVec4 ret;
            ret.x = clamp(v.x,v0.x,v1.x);
            ret.y = clamp(v.y,v0.y,v1.y);
            ret.z = clamp(v.z,v0.z,v1.z);
            ret.w = clamp(v.w,v0.w,v1.w);
            return ret;
        }

    }//!namesapce util
    
}//!namespace sv
