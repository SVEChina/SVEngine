#include "SVVec3.h"

namespace sv {
    
    namespace util {

        /******************************************************************************\
        *
        * FVec3
        *
        \******************************************************************************/

        //
        const FVec3 FVec3_zero(0.0f);
        const FVec3 FVec3_one(1.0f);
        const FVec3 FVec3_epsilon(EPSILON);
        const FVec3 FVec3_infinity(INFINITY);

        //
        FVec3 min(const FVec3 &v0,const FVec3 &v1) {
            FVec3 ret;
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
            #endif
            return ret;
        }

        FVec3 max(const FVec3 &v0,const FVec3 &v1) {
            FVec3 ret;
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
            #endif
            return ret;
        }

        FVec3 clamp(const FVec3 &v,const FVec3 &v0,const FVec3 &v1) {
            FVec3 ret;
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
            #endif
            return ret;
        }

        FVec3 saturate(const FVec3 &v) {
            FVec3 ret;
            #ifdef USE_SSE
                ret.vec = _mm_min_ps(_mm_max_ps(v.vec,FVec3_zero.vec),FVec3_one.vec);
            #elif USE_ALTIVEC
                ret.vec = vec_min(vec_max(v.vec,FVec3_zero.vec),FVec3_one.vec);
            #elif USE_NEON
                ret.vec = vminq_f32(vmaxq_f32(v.vec,FVec3_zero.vec),FVec3_one.vec);
            #else
                ret.x = saturate(v.x);
                ret.y = saturate(v.y);
                ret.z = saturate(v.z);
            #endif
            return ret;
        }

        FVec3 lerp(const FVec3 &v0,const FVec3 &v1,f32 k) {
            FVec3 ret;
            return lerp(ret,v0,v1,k);
        }

        /******************************************************************************\
        *
        * DVec3
        *
        \******************************************************************************/

        //
        const DVec3 DVec3_zero(0.0);
        const DVec3 DVec3_one(1.0);
        const DVec3 DVec3_epsilon(EPSILON);
        const DVec3 DVec3_infinity(INFINITY);

        //
        DVec3 min(const DVec3 &v0,const DVec3 &v1) {
            DVec3 ret;
            #ifdef USE_SSE2
                ret.vec0 = _mm_min_pd(v0.vec0,v1.vec0);
                ret.vec1 = _mm_min_sd(v0.vec1,v1.vec1);
            #else
                ret.x = min(v0.x,v1.x);
                ret.y = min(v0.y,v1.y);
                ret.z = min(v0.z,v1.z);
            #endif
            return ret;
        }

        DVec3 max(const DVec3 &v0,const DVec3 &v1) {
            DVec3 ret;
            #ifdef USE_SSE2
                ret.vec0 = _mm_max_pd(v0.vec0,v1.vec0);
                ret.vec1 = _mm_max_sd(v0.vec1,v1.vec1);
            #else
                ret.x = max(v0.x,v1.x);
                ret.y = max(v0.y,v1.y);
                ret.z = max(v0.z,v1.z);
            #endif
            return ret;
        }

        DVec3 clamp(const DVec3 &v,const DVec3 &v0,const DVec3 &v1) {
            DVec3 ret;
            #ifdef USE_SSE2
                ret.vec0 = _mm_min_pd(_mm_max_pd(v.vec0,v0.vec0),v1.vec0);
                ret.vec1 = _mm_min_sd(_mm_max_sd(v.vec1,v0.vec1),v1.vec1);
            #else
                ret.x = clamp(v.x,v0.x,v1.x);
                ret.y = clamp(v.y,v0.y,v1.y);
                ret.z = clamp(v.z,v0.z,v1.z);
            #endif
            return ret;
        }

        DVec3 saturate(const DVec3 &v) {
            DVec3 ret;
            #ifdef USE_SSE2
                ret.vec0 = _mm_min_pd(_mm_max_pd(v.vec0,DVec2_zero.vec),DVec2_one.vec);
                ret.vec1 = _mm_min_sd(_mm_max_sd(v.vec1,DVec2_zero.vec),DVec2_one.vec);
            #else
                ret.x = saturate(v.x);
                ret.y = saturate(v.y);
                ret.z = saturate(v.z);
            #endif
            return ret;
        }

        DVec3 lerp(const DVec3 &v0,const DVec3 &v1,f64 k) {
            DVec3 ret;
            return lerp(ret,v0,v1,k);
        }

        /******************************************************************************\
        *
        * HVec3
        *
        \******************************************************************************/

        //
        const HVec3 HVec3_zero(0.0f);
        const HVec3 HVec3_one(1.0f);

        /******************************************************************************\
        *
        * IVec3
        *
        \******************************************************************************/

        //
        const IVec3 IVec3_zero(0);
        const IVec3 IVec3_one(1);

        //
        IVec3 min(const IVec3 &v0,const IVec3 &v1) {
            IVec3 ret;
            #ifdef USE_NEON
                ret.vec = vminq_s32(v0.vec,v1.vec);
            #else
                ret.x = min(v0.x,v1.x);
                ret.y = min(v0.y,v1.y);
                ret.z = min(v0.z,v1.z);
            #endif
            return ret;
        }

        IVec3 max(const IVec3 &v0,const IVec3 &v1) {
            IVec3 ret;
            #ifdef USE_NEON
                ret.vec = vmaxq_s32(v0.vec,v1.vec);
            #else
                ret.x = max(v0.x,v1.x);
                ret.y = max(v0.y,v1.y);
                ret.z = max(v0.z,v1.z);
            #endif
            return ret;
        }

        IVec3 clamp(const IVec3 &v,const IVec3 &v0,const IVec3 &v1) {
            IVec3 ret;
            #ifdef USE_NEON
                ret.vec = vminq_s32(vmaxq_s32(v.vec,v0.vec),v1.vec);
            #else
                ret.x = clamp(v.x,v0.x,v1.x);
                ret.y = clamp(v.y,v0.y,v1.y);
                ret.z = clamp(v.z,v0.z,v1.z);
            #endif
            return ret;
        }

        IVec3 lerp(const IVec3 &v0,const IVec3 &v1,s32 k) {
            IVec3 ret;
            return lerp(ret,v0,v1,k);
        }

    }//!namesapce util
    
}//!namespace sv
