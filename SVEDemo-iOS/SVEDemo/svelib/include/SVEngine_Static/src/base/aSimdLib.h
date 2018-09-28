#ifndef __SIMD_LIB_H__
#define __SIMD_LIB_H__

#include "aMathLib.h"
#include "SVVec4.h"
#include "SVMat4.h"
#include "SVMat4d.h"

/******************************************************************************\
*
* SimdLib
*
\******************************************************************************/

namespace sv {
    
    namespace util {
        
        //
        class Simd {
            
            Simd();
            
        public:
            
            // shutdown
            static void shutdown();
            
            // codepath
            static void setGeneric();
            static void setSSE();
            static void setSSE2();
            static void setAltiVec();
            static void setNeon();
            
            // math
            static void dot(f32 &ret,const f32 *v0,const f32 *v1,s32 num);
            static void mul(f32 *ret,const f32 *v0,f32 v1,s32 num);
            static void mul(f32 *ret,const f32 *v0,const f32 *v1,s32 num);
            static void mad(f32 *ret,const f32 *v0,f32 v1,const f32 *v2,s32 num);
            static void add(f32 *ret,const f32 *v0,const f32 *v1,s32 num);
            static void sub(f32 *ret,const f32 *v0,const f32 *v1,s32 num);
            
            // bounds
            static void minMaxVec3(FVec3 &min,FVec3 &max,const void *src,s32 src_stride,s32 num);
            static void minMaxVec4(FVec4 &min,FVec4 &max,const void *src,s32 src_stride,s32 num);
            static void minMaxVec3(DVec3 &min,DVec3 &max,const void *src,s32 src_stride,s32 num);
            static void minMaxVec4(DVec4 &min,DVec4 &max,const void *src,s32 src_stride,s32 num);
            
            // transformed bounds
            static void minMaxMat4Vec3(FVec3 &min,FVec3 &max,const FMat4 &m,const void *src,s32 src_stride,s32 num);
            static void minMaxMat4Vec4(FVec4 &min,FVec4 &max,const FMat4 &m,const void *src,s32 src_stride,s32 num);
            
            // vector dot products
            static void dotVec3Vec3(void *ret,s32 ret_stride,const FVec3 &v,const void *src,s32 src_stride,s32 num);
            static void dotVec4Vec4(void *ret,s32 ret_stride,const FVec4 &v,const void *src,s32 src_stride,s32 num);
            
            // vector normalizations
            static void normalizeVec3(void *ret,s32 ret_stride,s32 num);
            static void normalizeVec4(void *ret,s32 ret_stride,s32 num);
            
            // vector multiplications
            static void mulMat3Vec3(void *ret,s32 ret_stride,const FMat4 &m,const void *src,s32 src_stride,s32 num);
            static void mulMat4Vec3(void *ret,s32 ret_stride,const FMat4 &m,const void *src,s32 src_stride,s32 num);
            static void mulMat4Vec4(void *ret,s32 ret_stride,const FMat4 &m,const void *src,s32 src_stride,s32 num);
            static void mulMat3Vec3(void *ret,s32 ret_stride,const DMat4 &m,const void *src,s32 src_stride,s32 num);
            static void mulMat4Vec3(void *ret,s32 ret_stride,const DMat4 &m,const void *src,s32 src_stride,s32 num);
            static void mulMat4Vec4(void *ret,s32 ret_stride,const DMat4 &m,const void *src,s32 src_stride,s32 num);
            
            // vector projections
            static void projMat4Vec3(void *ret,s32 ret_stride,const FMat4 &m,const void *src,s32 src_stride,s32 num);
            static void projMat4Vec4(void *ret,s32 ret_stride,const FMat4 &m,const void *src,s32 src_stride,s32 num);
            
            // matrix multiplications
            static void mulMat4Mat4(FVec4 *ret,const FMat4 &m,const FMat4 **matrices,s32 num);
            static void mulMat4Mat4(FVec4 *ret,const DMat4 &m,const DMat4 **matrices,s32 num);
            
            // matrix palette skinning
            static void skinningMat3(FVec3 &ret,const FMat4 **matrices,const f32 *weights,s32 num,const FVec3 &src);
            static void skinningMat4(FVec3 &ret,const FMat4 **matrices,const f32 *weights,s32 num,const FVec3 &src);
            
            // matrix decomposition
            static void eliminate(f32 *ret,const f32 *column,const f32 *factor,s32 rows,s32 num);
        };
        
    }//!namespace util
    
}//!namespace sv


#endif /* __SIMD_LIB_H__ */
