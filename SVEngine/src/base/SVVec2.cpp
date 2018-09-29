#include "SVVec2.h"

namespace sv {
    
    namespace util {

        /******************************************************************************\
        *
        * FVec2
        *
        \******************************************************************************/

        //
        const FVec2 FVec2_zero(0.0f);
        const FVec2 FVec2_one(1.0f);
        const FVec2 FVec2_epsilon(EPSILON);
        const FVec2 FVec2_infinity(INFINITY);

        //
        FVec2 min(const FVec2 &v0,const FVec2 &v1) {
            FVec2 ret;
            #ifdef USE_NEON
                ret.vec = vmin_f32(v0.vec,v1.vec);
            #else
                ret.x = min(v0.x,v1.x);
                ret.y = min(v0.y,v1.y);
            #endif
            return ret;
        }

        FVec2 max(const FVec2 &v0,const FVec2 &v1) {
            FVec2 ret;
            #ifdef USE_NEON
                ret.vec = vmax_f32(v0.vec,v1.vec);
            #else
                ret.x = max(v0.x,v1.x);
                ret.y = max(v0.y,v1.y);
            #endif
            return ret;
        }

        FVec2 clamp(const FVec2 &v,const FVec2 &v0,const FVec2 &v1) {
            FVec2 ret;
            #ifdef USE_NEON
                ret.vec = vmin_f32(vmax_f32(v.vec,v0.vec),v1.vec);
            #else
                ret.x = clamp(v.x,v0.x,v1.x);
                ret.y = clamp(v.y,v0.y,v1.y);
            #endif
            return ret;
        }

        FVec2 saturate(const FVec2 &v) {
            FVec2 ret;
            #ifdef USE_NEON
                ret.vec = vmin_f32(vmax_f32(v.vec,FVec2_zero.vec),FVec2_one.vec);
            #else
                ret.x = saturate(v.x);
                ret.y = saturate(v.y);
            #endif
            return ret;
        }

        FVec2 lerp(const FVec2 &v0,const FVec2 &v1,f32 k) {
            FVec2 ret;
            return lerp(ret,v0,v1,k);
        }

        /******************************************************************************\
        *
        * DVec2
        *
        \******************************************************************************/

        //
        const DVec2 DVec2_zero(0.0);
        const DVec2 DVec2_one(1.0);
        const DVec2 DVec2_epsilon(EPSILON);
        const DVec2 DVec2_infinity(INFINITY);

        //
        DVec2 min(const DVec2 &v0,const DVec2 &v1) {
            DVec2 ret;
            #ifdef USE_SSE2
                ret.vec = _mm_min_pd(v0.vec,v1.vec);
            #else
                ret.x = min(v0.x,v1.x);
                ret.y = min(v0.y,v1.y);
            #endif
            return ret;
        }

        DVec2 max(const DVec2 &v0,const DVec2 &v1) {
            DVec2 ret;
            #ifdef USE_SSE2
                ret.vec = _mm_max_pd(v0.vec,v1.vec);
            #else
                ret.x = max(v0.x,v1.x);
                ret.y = max(v0.y,v1.y);
            #endif
            return ret;
        }

        DVec2 clamp(const DVec2 &v,const DVec2 &v0,const DVec2 &v1) {
            DVec2 ret;
            #ifdef USE_SSE2
                ret.vec = _mm_min_pd(_mm_max_pd(v.vec,v0.vec),v1.vec);
            #else
                ret.x = clamp(v.x,v0.x,v1.x);
                ret.y = clamp(v.y,v0.y,v1.y);
            #endif
            return ret;
        }

        DVec2 saturate(const DVec2 &v) {
            DVec2 ret;
            #ifdef USE_SSE2
                ret.vec = _mm_min_pd(_mm_max_pd(v.vec,DVec2_zero.vec),DVec2_one.vec);
            #else
                ret.x = saturate(v.x);
                ret.y = saturate(v.y);
            #endif
            return ret;
        }

        DVec2 lerp(const DVec2 &v0,const DVec2 &v1,f64 k) {
            DVec2 ret;
            return lerp(ret,v0,v1,k);
        }

        /******************************************************************************\
        *
        * HVec2
        *
        \******************************************************************************/

        //
        const HVec2 HVec2_zero(0.0f);
        const HVec2 HVec2_one(1.0f);

        /******************************************************************************\
        *
        * IVec2
        *
        \******************************************************************************/

        //
        const IVec2 IVec2_zero(0);
        const IVec2 IVec2_one(1);

        //
        IVec2 min(const IVec2 &v0,const IVec2 &v1) {
            IVec2 ret;
            #ifdef USE_NEON
                ret.vec = vmin_s32(v0.vec,v1.vec);
            #else
                ret.x = min(v0.x,v1.x);
                ret.y = min(v0.y,v1.y);
            #endif
            return ret;
        }

        IVec2 max(const IVec2 &v0,const IVec2 &v1) {
            IVec2 ret;
            #ifdef USE_NEON
                ret.vec = vmax_s32(v0.vec,v1.vec);
            #else
                ret.x = max(v0.x,v1.x);
                ret.y = max(v0.y,v1.y);
            #endif
            return ret;
        }

        IVec2 clamp(const IVec2 &v,const IVec2 &v0,const IVec2 &v1) {
            IVec2 ret;
            #ifdef USE_NEON
                ret.vec = vmin_s32(vmax_s32(v.vec,v0.vec),v1.vec);
            #else
                ret.x = clamp(v.x,v0.x,v1.x);
                ret.y = clamp(v.y,v0.y,v1.y);
            #endif
            return ret;
        }

        IVec2 lerp(const IVec2 &v0,const IVec2 &v1,s32 k) {
            IVec2 ret;
            return lerp(ret,v0,v1,k);
        }

    }//!namesapce util
    
}//!namespace sv
