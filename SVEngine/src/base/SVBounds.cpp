#include "SVSimdLib.h"
#include "SVBounds.h"
#include "SVVec3.h"
#include "SVMat4.h"

//
#define BOUNDS_EPSILON 1e-4f

namespace sv {
    
    namespace util {
        
        SVBoundSphere::SVBoundSphere() {
            clear();
        }

        SVBoundSphere::SVBoundSphere(const FVec3 &center,f32 radius) {
            set(center,radius);
        }

        SVBoundSphere::SVBoundSphere(const FVec3 *points,s32 num_points,s32 optimal) {
            set(points,num_points,optimal);
        }

        SVBoundSphere::SVBoundSphere(const SVBoundSphere &bs) {
            set(bs);
        }

        SVBoundSphere::SVBoundSphere(const SVBoundSphere &bs,const FMat4 &transform) {
            set(bs,transform);
        }

        SVBoundSphere::SVBoundSphere(const SVBoundBox &bb) {
            set(bb);
        }

        SVBoundSphere::~SVBoundSphere() {
            
        }

        //
        void SVBoundSphere::clear() {
            center = FVec3_zero;
            center.w = -1.0f;
        }

        //
        void SVBoundSphere::set(const FVec3 &c,f32 r) {
            center = c;
            center.w = r;
        }

        void SVBoundSphere::set(const FVec3 *points,s32 num_points,s32 optimal) {
            clear();
            if(num_points > 2 && optimal) {
                center.w = INFINITY;
                for(s32 i = 0; i < num_points; i++) {
                    for(s32 j = i + 1; j < num_points; j++) {
                        f32 radius2 = -INFINITY;
                        FVec3 point = (points[i] + points[j]) * 0.5f;
                        for(s32 k = 0; k < num_points; k++) {
                            f32 length2 = (points[k] - point).length2();
                            if(radius2 < length2)
                                radius2 = length2;
                        }
                        if(center.w > radius2) {
                            center = point;
                            center.w = radius2;
                        }
                    }
                }
                center.w = (center.w > 0.0f) ? Math::sqrt(center.w) : -1.0f;
            } else {
                expand(points,num_points);
            }
        }
        //
        void SVBoundSphere::set(const SVBoundSphere &bs) {
            center = bs.center;
            center.w = bs.center.w;
        }
        //
        void SVBoundSphere::set(const SVBoundSphere &bs,const FMat4 &transform) {
            center = bs.center;
            center.w = bs.center.w;
            setTransform(transform);
        }
        //
        void SVBoundSphere::set(const SVBoundBox &bb) {
            clear();
            expand(bb);
        }
        //
        void SVBoundSphere::setTransform(const FMat4 &transform) {
            f32 radius = center.w;
            #ifdef USE_SSE
                __m128 col_0 = transform.col0;
                __m128 col_1 = transform.col1;
                __m128 col_2 = transform.col2;
                __m128 res_0 = _mm_mul_ps(col_0,_MM_SWIZZLE(center.vec,X,X,X,W));
                __m128 res_1 = _mm_mul_ps(col_1,_MM_SWIZZLE(center.vec,Y,Y,Y,W));
                __m128 res_2 = _mm_mul_ps(col_2,_MM_SWIZZLE(center.vec,Z,Z,Z,W));
                center.vec = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,transform.col3));
                col_0 = _mm_mul_ps(col_0,col_0);
                col_1 = _mm_mul_ps(col_1,col_1);
                col_2 = _mm_mul_ps(col_2,col_2);
                col_0 = _mm_add_ps(col_0,_MM_SWIZZLE(col_0,Y,X,W,Z));
                col_1 = _mm_add_ps(col_1,_MM_SWIZZLE(col_1,Y,X,W,Z));
                col_2 = _mm_add_ps(col_2,_MM_SWIZZLE(col_2,Y,X,W,Z));
                col_0 = _mm_add_ss(col_0,_MM_SWIZZLE(col_0,Z,W,X,Y));
                col_1 = _mm_add_ss(col_1,_MM_SWIZZLE(col_1,Z,W,X,Y));
                col_2 = _mm_add_ss(col_2,_MM_SWIZZLE(col_2,Z,W,X,Y));
                col_0 = _mm_max_ss(_mm_max_ss(col_0,col_1),col_2);
                col_0 = _mm_mul_ss(col_0,_mm_rsqrt_ss(col_0));
                center.w = radius * _mm_cvtss_f32(col_0);
            #elif USE_ALTIVEC
                vec_float4 zero = vec_splats(0.0f);
                vec_float4 col_0 = transform.col0;
                vec_float4 col_1 = transform.col1;
                vec_float4 col_2 = transform.col2;
                vec_float4 res_0 = vec_madd(col_0,VEC_SWIZZLE(center.vec,X,X,X,W),transform.col3);
                vec_float4 res_1 = vec_madd(col_1,VEC_SWIZZLE(center.vec,Y,Y,Y,W),res_0);
                center.vec = vec_madd(col_2,VEC_SWIZZLE(center.vec,Z,Z,Z,W),res_1);
                col_0 = vec_madd(col_0,col_0,zero);
                col_1 = vec_madd(col_1,col_1,zero);
                col_2 = vec_madd(col_2,col_2,zero);
                col_0 = vec_add(col_0,vec_sld(col_0,col_0,8));
                col_1 = vec_add(col_1,vec_sld(col_1,col_1,8));
                col_2 = vec_add(col_2,vec_sld(col_2,col_2,8));
                col_0 = vec_add(col_0,vec_sld(col_0,col_0,4));
                col_1 = vec_add(col_1,vec_sld(col_1,col_1,4));
                col_2 = vec_add(col_2,vec_sld(col_2,col_2,4));
                col_0 = vec_max(vec_max(col_0,col_1),col_2);
                col_0 = vec_madd(col_0,vec_rsqrte(col_0),zero);
                center.w = radius * vec_extract(col_0,0);
            #elif USE_NEON
                float32x4_t col_0 = transform.col0;
                float32x4_t col_1 = transform.col1;
                float32x4_t col_2 = transform.col2;
                float32x2_t low = vget_low_f32(center.vec);
                float32x2_t high = vget_high_f32(center.vec);
                float32x4_t res_0 = vmlaq_lane_f32(transform.col3,col_0,low,0);
                float32x4_t res_1 = vmlaq_lane_f32(res_0,col_1,low,1);
                center.vec = vmlaq_lane_f32(res_1,col_2,high,0);
                col_0 = vdot33q_f32(col_0,col_0);
                col_1 = vdot33q_f32(col_1,col_1);
                col_2 = vdot33q_f32(col_2,col_2);
                col_0 = vmaxq_f32(col_0,vmaxq_f32(col_1,col_2));
                col_0 = vmulq_f32(col_0,vrsqrteq_f32(col_0));
                center.w = radius * vgetq_lane_f32(col_0,0);
            #else
                center = transform * center;
                f32 x = transform.m00 * transform.m00 + transform.m10 * transform.m10 + transform.m20 * transform.m20;
                f32 y = transform.m01 * transform.m01 + transform.m11 * transform.m11 + transform.m21 * transform.m21;
                f32 z = transform.m02 * transform.m02 + transform.m12 * transform.m12 + transform.m22 * transform.m22;
                f32 scale = Math::sqrtFast(::max(::max(x,y),z));
                center.w = radius * scale;
            #endif
        }
        //
        void SVBoundSphere::setTransform(const DMat4 &transform) {
            setTransform(FMat4(transform));
        }
        //
        s32 SVBoundSphere::compare(const SVBoundSphere &bs) const {
            return (center == bs.center && ::compare(center.w,bs.center.w));
        }
        //
        void SVBoundSphere::expand(const FVec3 &point) {
            if(isValid()) {
                FVec3 direction = point - center;
                f32 length = direction.length();
                if(length > center.w) {
                    f32 delta = (length - center.w) * 0.5f;
                    center += direction * (delta / length);
                    center.w += delta;
                }
            } else {
                center = point;
                center.w = BOUNDS_EPSILON;
            }
        }
        //
        void SVBoundSphere::expand(const FVec3 *points,s32 num_points) {
            if(isValid()) {
                for(s32 i = 0; i < num_points; i++) {
                    FVec3 direction = points[i] - center;
                    f32 length = direction.length();
                    if(length > center.w) {
                        f32 delta = (length - center.w) * 0.5f;
                        center += direction * (delta / length);
                        center.w += delta;
                    }
                }
            } else {
                FVec3 min,max;
                Simd::minMaxVec3(min,max,points,sizeof(FVec3),num_points);
                center = (min + max) * 0.5f;
                f32 radius2 = -INFINITY;
                for(s32 i = 0; i < num_points; i++) {
                    f32 length2 = (points[i] - center).length2();
                    if(radius2 < length2)
                        radius2 = length2;
                }
                center.w = (radius2 > 0.0f) ? Math::sqrt(radius2) : -1.0f;
            }
        }

        void SVBoundSphere::expand(const SVBoundSphere &bs) {
            if(bs.isValid()) {
                if(isValid()) {
                    FVec3 direction = bs.center - center;
                    f32 length = direction.length();
                    if(length > EPSILON) {
                        if(length + center.w < bs.center.w) {
                            center = bs.center;
                            center.w = bs.center.w;
                        } else if(length + bs.center.w > center.w) {
                            FVec3 p0 = center - direction * (center.w / length);
                            FVec3 p1 = bs.center + direction * (bs.center.w / length);
                            center = (p0 + p1) * 0.5f;
                            center.w = (p1 - center).length();
                        }
                    } else {
                        if(center.w < bs.center.w)
                            center.w = bs.center.w;
                    }
                } else {
                    center = bs.center;
                    center.w = bs.center.w;
                }
            }
        }

        void SVBoundSphere::expand(const SVBoundBox &bb) {
            if(bb.isValid()) {
                const FVec3 &min = bb.getMin();
                const FVec3 &max = bb.getMax();
                if(isValid()) {
                    expand(FVec3(min.x,min.y,min.z));
                    expand(FVec3(max.x,min.y,min.z));
                    expand(FVec3(min.x,max.y,min.z));
                    expand(FVec3(max.x,max.y,min.z));
                    expand(FVec3(min.x,min.y,max.z));
                    expand(FVec3(max.x,min.y,max.z));
                    expand(FVec3(min.x,max.y,max.z));
                    expand(FVec3(max.x,max.y,max.z));
                } else {
                    center = (min + max) * 0.5f;
                    center.w = length(max - center);
                }
            }
        }

        //
        void SVBoundSphere::expandRadius(const FVec3 &point) {
            if(isValid()) {
                f32 radius = length(center - point);
                if(center.w < radius)
                    center.w = radius;
            } else {
                center = point;
                center.w = BOUNDS_EPSILON;
            }
        }

        void SVBoundSphere::expandRadius(const FVec3 *points,s32 num_points) {
            if(isValid()) {
                for(s32 i = 0; i < num_points; i++) {
                    f32 radius = length(center - points[i]);
                    if(center.w < radius)
                        center.w = radius;
                }
            } else {
                FVec3 min,max;
                Simd::minMaxVec3(min,max,points,sizeof(FVec3),num_points);
                center = (min + max) * 0.5f;
                f32 radius2 = -INFINITY;
                for(s32 i = 0; i < num_points; i++) {
                    f32 length2 = (points[i] - center).length2();
                    if(radius2 < length2)
                        radius2 = length2;
                }
                center.w = (radius2 > 0.0f) ? Math::sqrt(radius2) : -1.0f;
            }
        }

        void SVBoundSphere::expandRadius(const SVBoundSphere &bs) {
            if(bs.isValid()) {
                if(isValid()) {
                    f32 radius = length(bs.center - center) + bs.center.w;
                    if(center.w < radius)
                        center.w = radius;
                } else {
                    center = bs.center;
                    center.w = bs.center.w;
                }
            }
        }

        void SVBoundSphere::expandRadius(const SVBoundBox &bb) {
            if(bb.isValid()) {
                const FVec3 &min = bb.getMin();
                const FVec3 &max = bb.getMax();
                if(isValid()) {
                    expandRadius(FVec3(min.x,min.y,min.z));
                    expandRadius(FVec3(max.x,min.y,min.z));
                    expandRadius(FVec3(min.x,max.y,min.z));
                    expandRadius(FVec3(max.x,max.y,min.z));
                    expandRadius(FVec3(min.x,min.y,max.z));
                    expandRadius(FVec3(max.x,min.y,max.z));
                    expandRadius(FVec3(min.x,max.y,max.z));
                    expandRadius(FVec3(max.x,max.y,max.z));
                } else {
                    center = (min + max) * 0.5f;
                    center.w = length(max - center);
                }
            }
        }

        //
        s32 SVBoundSphere::inside(const FVec3 &point) const {
            if(isValid())
                return insideValid(point);
            return 0;
        }

        s32 SVBoundSphere::inside(const FVec3 &point,f32 radius) const {
            if(isValid())
                return insideValid(point,radius);
            return 0;
        }

        s32 SVBoundSphere::inside(const FVec3 &min,const FVec3 &max) const {
            if(isValid())
                return insideValid(min,max);
            return 0;
        }

        //
        s32 SVBoundSphere::inside(const SVBoundSphere &bs) const {
            if(isValid() && bs.isValid())
                return insideValid(bs.center,bs.center.w);
            return 0;
        }

        s32 SVBoundSphere::inside(const SVBoundBox &bb) const {
            if(isValid() && bb.isValid())
                return insideValid(bb.getMin(),bb.getMax());
            return 0;
        }

        //
        s32 SVBoundSphere::insideAll(const SVBoundSphere &bs) const {
            if(isValid() && bs.isValid())
                return insideAllValid(bs);
            return 0;
        }

        s32 SVBoundSphere::insideAll(const SVBoundBox &bb) const {
            if(isValid() && bb.isValid())
                return insideAllValid(bb);
            return 0;
        }

        //
        s32 SVBoundSphere::rayIntersection(const FVec3 &p,const FVec3 &direction) const {
            if(isValid())
                return rayIntersectionValid(p,direction);
            return 0;
        }

        s32 SVBoundSphere::getIntersection(const FVec3 &p0,const FVec3 &p1) const {
            if(isValid())
                return getIntersectionValid(p0,p1);
            return 0;
        }

        //
        f32 SVBoundSphere::distance() const {
            if(isValid())
                return distanceValid();
            return INFINITY;
        }

        f32 SVBoundSphere::distance(const FVec3 &p) const {
            if(isValid())
                return distanceValid(p);
            return INFINITY;
        }

        //
        SVBoundSphere operator*(const FMat4 &m,const SVBoundSphere &bs) {
            SVBoundSphere ret = bs;
            ret.setTransform(m);
            return ret;
        }

        SVBoundSphere operator*(const DMat4 &m,const SVBoundSphere &bs) {
            SVBoundSphere ret = bs;
            ret.setTransform(m);
            return ret;
        }


        //*********************************** SVBoundBox ********************************
   
        SVBoundBox::SVBoundBox() {
            clear();
        }

        SVBoundBox::SVBoundBox(const FVec3 &min,const FVec3 &max) {
            set(min,max);
        }

        SVBoundBox::SVBoundBox(const FVec3 *points,s32 num_points) {
            set(points,num_points);
        }

        SVBoundBox::SVBoundBox(const SVBoundBox &bb) {
            set(bb);
        }

        SVBoundBox::SVBoundBox(const SVBoundBox &bb,const FMat4 &transform) {
            set(bb,transform);
        }

        SVBoundBox::SVBoundBox(const SVBoundSphere &bs) {
            set(bs);
        }

        SVBoundBox::~SVBoundBox() {
            
        }

        //
        void SVBoundBox::clear() {
            min = FVec3_infinity;
            max = -FVec3_infinity;
        }

        //
        void SVBoundBox::set(const FVec3 &min_,const FVec3 &max_) {
            min = min_;
            max = max_;
        }

        void SVBoundBox::set(const FVec3 *points,s32 num_points) {
            clear();
            expand(points,num_points);
        }

        void SVBoundBox::set(const SVBoundSphere &bs) {
            clear();
            expand(bs);
        }

        void SVBoundBox::set(const SVBoundBox &bb) {
            min = bb.min;
            max = bb.max;
        }

        void SVBoundBox::set(const SVBoundBox &bb,const FMat4 &transform) {
            min = bb.min;
            max = bb.max;
            setTransform(transform);
        }

        //
        void SVBoundBox::setTransform(const FMat4 &transform) {
            #ifdef USE_SSE
                __m128 sign = _mm_set1_ps(IntFloat(0x7fffffff).f);
                __m128 center = _mm_mul_ps(_mm_add_ps(min.vec,max.vec),_mm_set1_ps(0.5f));
                __m128 axis = _mm_sub_ps(max.vec,center);
                __m128 col_0 = transform.col0;
                __m128 col_1 = transform.col1;
                __m128 col_2 = transform.col2;
                __m128 res_0 = _mm_mul_ps(col_0,_MM_SWIZZLE(center,X,X,X,W));
                __m128 res_1 = _mm_mul_ps(col_1,_MM_SWIZZLE(center,Y,Y,Y,W));
                __m128 res_2 = _mm_mul_ps(col_2,_MM_SWIZZLE(center,Z,Z,Z,W));
                center = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,transform.col3));
                col_0 = _mm_mul_ps(_mm_and_ps(col_0,sign),_MM_SWIZZLE(axis,X,X,X,W));
                col_1 = _mm_mul_ps(_mm_and_ps(col_1,sign),_MM_SWIZZLE(axis,Y,Y,Y,W));
                col_2 = _mm_mul_ps(_mm_and_ps(col_2,sign),_MM_SWIZZLE(axis,Z,Z,Z,W));
                axis = _mm_add_ps(_mm_add_ps(col_0,col_1),col_2);
                min.vec = _mm_sub_ps(center,axis);
                max.vec = _mm_add_ps(center,axis);
            #elif USE_ALTIVEC
                vec_float4 zero = vec_splats(0.0f);
                vec_float4 center = vec_madd(vec_add(min.vec,max.vec),vec_splats(0.5f),zero);
                vec_float4 axis = vec_sub(max.vec,center);
                vec_float4 col_0 = transform.col0;
                vec_float4 col_1 = transform.col1;
                vec_float4 col_2 = transform.col2;
                vec_float4 res_0 = vec_madd(col_0,VEC_SWIZZLE(center,X,X,X,W),transform.col3);
                vec_float4 res_1 = vec_madd(col_1,VEC_SWIZZLE(center,Y,Y,Y,W),res_0);
                center = vec_madd(col_2,VEC_SWIZZLE(center,Z,Z,Z,W),res_1);
                col_0 = vec_madd(vec_abs(col_0),VEC_SWIZZLE(axis,X,X,X,W),zero);
                col_1 = vec_madd(vec_abs(col_1),VEC_SWIZZLE(axis,Y,Y,Y,W),col_0);
                axis = vec_madd(vec_abs(col_2),VEC_SWIZZLE(axis,Z,Z,Z,W),col_1);
                min.vec = vec_sub(center,axis);
                max.vec = vec_add(center,axis);
            #elif USE_NEON
                float32x4_t center = vmulq_n_f32(vaddq_f32(min.vec,max.vec),0.5f);
                float32x4_t axis = vsubq_f32(max.vec,center);
                float32x4_t col_0 = transform.col0;
                float32x4_t col_1 = transform.col1;
                float32x4_t col_2 = transform.col2;
                float32x2_t low = vget_low_f32(center);
                float32x2_t high = vget_high_f32(center);
                float32x4_t res_0 = vmlaq_lane_f32(transform.col3,col_0,low,0);
                float32x4_t res_1 = vmlaq_lane_f32(res_0,col_1,low,1);
                center = vmlaq_lane_f32(res_1,col_2,high,0);
                low = vget_low_f32(axis);
                high = vget_high_f32(axis);
                col_0 = vmulq_lane_f32(vabsq_f32(col_0),low,0);
                col_1 = vmlaq_lane_f32(col_0,vabsq_f32(col_1),low,1);
                axis = vmlaq_lane_f32(col_1,vabsq_f32(col_2),high,0);
                min.vec = vsubq_f32(center,axis);
                max.vec = vaddq_f32(center,axis);
            #else
                FVec3 center = (min + max) * 0.5f;
                FVec3 axis = max - center;
                f32 x = Math::abs(transform.m00) * axis.x + Math::abs(transform.m01) * axis.y + Math::abs(transform.m02) * axis.z;
                f32 y = Math::abs(transform.m10) * axis.x + Math::abs(transform.m11) * axis.y + Math::abs(transform.m12) * axis.z;
                f32 z = Math::abs(transform.m20) * axis.x + Math::abs(transform.m21) * axis.y + Math::abs(transform.m22) * axis.z;
                center = transform * center;
                min = center - FVec3(x,y,z);
                max = center + FVec3(x,y,z);
            #endif
        }

        void SVBoundBox::setTransform(const DMat4 &transform) {
            setTransform(FMat4(transform));
        }

        void SVBoundBox::setTransform(const SVBoundSphere &bs,const FMat4 &transform) {
            setTransform(transform);
            #ifdef USE_SSE
                __m128 col_0 = transform.col0;
                __m128 col_1 = transform.col1;
                __m128 col_2 = transform.col2;
                __m128 res_0 = _mm_mul_ps(col_0,_MM_SWIZZLE(bs.getCenter().vec,X,X,X,W));
                __m128 res_1 = _mm_mul_ps(col_1,_MM_SWIZZLE(bs.getCenter().vec,Y,Y,Y,W));
                __m128 res_2 = _mm_mul_ps(col_2,_MM_SWIZZLE(bs.getCenter().vec,Z,Z,Z,W));
                __m128 center = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,transform.col3));
                col_0 = _mm_mul_ps(col_0,col_0);
                col_1 = _mm_mul_ps(col_1,col_1);
                col_2 = _mm_mul_ps(col_2,col_2);
                col_0 = _mm_add_ps(col_0,_MM_SWIZZLE(col_0,Y,X,W,Z));
                col_1 = _mm_add_ps(col_1,_MM_SWIZZLE(col_1,Y,X,W,Z));
                col_2 = _mm_add_ps(col_2,_MM_SWIZZLE(col_2,Y,X,W,Z));
                col_0 = _mm_add_ps(col_0,_MM_SWIZZLE(col_0,Z,W,X,Y));
                col_1 = _mm_add_ps(col_1,_MM_SWIZZLE(col_1,Z,W,X,Y));
                col_2 = _mm_add_ps(col_2,_MM_SWIZZLE(col_2,Z,W,X,Y));
                col_0 = _mm_max_ps(_mm_max_ps(col_0,col_1),col_2);
                col_0 = _mm_mul_ps(col_0,_mm_rsqrt_ps(col_0));
                __m128 radius = _mm_mul_ps(col_0,FVec3(bs.getRadius()).vec);
                min.vec = _mm_max_ps(min.vec,_mm_sub_ps(center,radius));
                max.vec = _mm_min_ps(max.vec,_mm_add_ps(center,radius));
            #elif USE_ALTIVEC
                vec_float4 zero = vec_splats(0.0f);
                vec_float4 col_0 = transform.col0;
                vec_float4 col_1 = transform.col1;
                vec_float4 col_2 = transform.col2;
                vec_float4 res_0 = vec_madd(col_0,VEC_SWIZZLE(bs.getCenter().vec,X,X,X,W),transform.col3);
                vec_float4 res_1 = vec_madd(col_1,VEC_SWIZZLE(bs.getCenter().vec,Y,Y,Y,W),res_0);
                vec_float4 center = vec_madd(col_2,VEC_SWIZZLE(bs.getCenter().vec,Z,Z,Z,W),res_1);
                col_0 = vec_madd(col_0,col_0,zero);
                col_1 = vec_madd(col_1,col_1,zero);
                col_2 = vec_madd(col_2,col_2,zero);
                col_0 = vec_add(col_0,vec_sld(col_0,col_0,8));
                col_1 = vec_add(col_1,vec_sld(col_1,col_1,8));
                col_2 = vec_add(col_2,vec_sld(col_2,col_2,8));
                col_0 = vec_add(col_0,vec_sld(col_0,col_0,4));
                col_1 = vec_add(col_1,vec_sld(col_1,col_1,4));
                col_2 = vec_add(col_2,vec_sld(col_2,col_2,4));
                col_0 = vec_max(vec_max(col_0,col_1),col_2);
                col_0 = vec_madd(col_0,vec_rsqrte(col_0),zero);
                vec_float4 radius = vec_madd(col_0,vec_splats(bs.getRadius()),zero);
                min.vec = vec_max(min.vec,vec_sub(center,radius));
                max.vec = vec_min(max.vec,vec_add(center,radius));
            #elif USE_NEON
                float32x4_t col_0 = transform.col0;
                float32x4_t col_1 = transform.col1;
                float32x4_t col_2 = transform.col2;
                float32x2_t low = vget_low_f32(bs.getCenter().vec);
                float32x2_t high = vget_high_f32(bs.getCenter().vec);
                float32x4_t res_0 = vmlaq_lane_f32(transform.col3,col_0,low,0);
                float32x4_t res_1 = vmlaq_lane_f32(res_0,col_1,low,1);
                float32x4_t center = vmlaq_lane_f32(res_1,col_2,high,0);
                col_0 = vdot33q_f32(col_0,col_0);
                col_1 = vdot33q_f32(col_1,col_1);
                col_2 = vdot33q_f32(col_2,col_2);
                col_0 = vmaxq_f32(col_0,vmaxq_f32(col_1,col_2));
                col_0 = vmulq_f32(col_0,vrsqrteq_f32(col_0));
                float32x4_t radius = vmulq_n_f32(col_0,bs.getRadius());
                min.vec = vmaxq_f32(min.vec,vsubq_f32(center,radius));
                max.vec = vminq_f32(max.vec,vaddq_f32(center,radius));
            #else
                FVec3 center = transform * bs.getCenter();
                f32 x = transform.m00 * transform.m00 + transform.m10 * transform.m10 + transform.m20 * transform.m20;
                f32 y = transform.m01 * transform.m01 + transform.m11 * transform.m11 + transform.m21 * transform.m21;
                f32 z = transform.m02 * transform.m02 + transform.m12 * transform.m12 + transform.m22 * transform.m22;
                f32 radius = Math::sqrtFast(::max(::max(x,y),z)) * bs.getRadius();
                min = ::max(min,center - FVec3(radius));
                max = ::min(max,center + FVec3(radius));
            #endif
        }

        void SVBoundBox::setTransform(const SVBoundSphere &bs,const DMat4 &transform) {
            setTransform(bs,FMat4(transform));
        }

        //
        s32 SVBoundBox::compare(const SVBoundBox &bb) const {
            return (min == bb.min && max == bb.max);
        }

        //
        void SVBoundBox::expand(const FVec3 &point) {
            if(isValid()) {
                #ifdef USE_SSE
                    min.vec = _mm_min_ps(min.vec,point.vec);
                    max.vec = _mm_max_ps(max.vec,point.vec);
                #elif USE_ALTIVEC
                    min.vec = vec_min(min.vec,point.vec);
                    max.vec = vec_max(max.vec,point.vec);
                #elif USE_NEON
                    min.vec = vminq_f32(min.vec,point.vec);
                    max.vec = vmaxq_f32(max.vec,point.vec);
                #else
                    min = ::min(min,point);
                    max = ::max(max,point);
                #endif
            } else {
                min = point - FVec3(BOUNDS_EPSILON);
                max = point + FVec3(BOUNDS_EPSILON);
            }
        }

        void SVBoundBox::expand(const FVec3 *points,s32 num_points) {
            if(isValid()) {
                FVec3 min_,max_;
                Simd::minMaxVec3(min_,max_,points,sizeof(FVec3),num_points);
                #ifdef USE_SSE
                    min.vec = _mm_min_ps(min.vec,min_.vec);
                    max.vec = _mm_max_ps(max.vec,max_.vec);
                #elif USE_ALTIVEC
                    min.vec = vec_min(min.vec,min_.vec);
                    max.vec = vec_max(max.vec,max_.vec);
                #elif USE_NEON
                    min.vec = vminq_f32(min.vec,min_.vec);
                    max.vec = vmaxq_f32(max.vec,max_.vec);
                #else
                    min = ::min(min,min_);
                    max = ::max(max,max_);
                #endif
            } else {
                Simd::minMaxVec3(min,max,points,sizeof(FVec3),num_points);
            }
        }

        void SVBoundBox::expand(const SVBoundSphere &bs) {
            if(bs.isValid()) {
                const FVec3 &center = bs.getCenter();
                f32 radius = bs.getRadius();
                if(isValid()) {
                    #ifdef USE_SSE
                        __m128 r = FVec3(radius).vec;
                        min.vec = _mm_min_ps(min.vec,_mm_sub_ps(center.vec,r));
                        max.vec = _mm_max_ps(max.vec,_mm_add_ps(center.vec,r));
                    #elif USE_ALTIVEC
                        vec_float4 r = vec_splats(radius);
                        min.vec = vec_min(min.vec,vec_sub(center.vec,r));
                        max.vec = vec_max(max.vec,vec_add(center.vec,r));
                    #elif USE_NEON
                        float32x4_t r = vdupq_n_f32(radius);
                        min.vec = vminq_f32(min.vec,vsubq_f32(center.vec,r));
                        max.vec = vmaxq_f32(max.vec,vaddq_f32(center.vec,r));
                    #else
                        min = ::min(min,center - FVec3(radius));
                        max = ::max(max,center + FVec3(radius));
                    #endif
                } else {
                    min = center - FVec3(radius);
                    max = center + FVec3(radius);
                }
            }
        }

        void SVBoundBox::expand(const SVBoundBox &bb) {
            if(bb.isValid()) {
                if(isValid()) {
                    #ifdef USE_SSE
                        min.vec = _mm_min_ps(min.vec,bb.min.vec);
                        max.vec = _mm_max_ps(max.vec,bb.max.vec);
                    #elif USE_ALTIVEC
                        min.vec = vec_min(min.vec,bb.min.vec);
                        max.vec = vec_max(max.vec,bb.max.vec);
                    #elif USE_NEON
                        min.vec = vminq_f32(min.vec,bb.min.vec);
                        max.vec = vmaxq_f32(max.vec,bb.max.vec);
                    #else
                        min = ::min(min,bb.min);
                        max = ::max(max,bb.max);
                    #endif
                } else {
                    min = bb.min;
                    max = bb.max;
                }
            }
        }

        //
        s32 SVBoundBox::inside(const FVec3 &point) const {
            if(isValid())
                return insideValid(point);
            return 0;
        }

        s32 SVBoundBox::inside(const FVec3 &point,f32 radius) const {
            if(isValid())
                return insideValid(point,radius);
            return 0;
        }

        s32 SVBoundBox::inside(const FVec3 &min,const FVec3 &max) const {
            if(isValid())
                return insideValid(min,max);
            return 0;
        }

        //
        s32 SVBoundBox::inside(const SVBoundSphere &bs) const {
            if(isValid() && bs.isValid())
                return insideValid(bs.getCenter(),bs.getRadius());
            return 0;
        }

        s32 SVBoundBox::inside(const SVBoundBox &bb) const {
            if(isValid() && bb.isValid())
                return insideValid(bb.min,bb.max);
            return 0;
        }

        //
        s32 SVBoundBox::insideAll(const SVBoundSphere &bs) const {
            if(isValid() && bs.isValid()) return insideAllValid(bs);
            return 0;
        }

        s32 SVBoundBox::insideAll(const SVBoundBox &bb) const {
            if(isValid() && bb.isValid()) return insideAllValid(bb);
            return 0;
        }

        //
        s32 SVBoundBox::insideCube(s32 face,const FVec3 &offset) const {
            if(isValid()) {
                FVec3 min = getMin() - offset;
                FVec3 max = getMax() - offset;
                switch(face) {
                    case 0: return (max.x >= 0.0f && min.y <=  max.x && max.y >= -max.x && min.z <=  max.x && max.z >= -max.x);
                    case 1: return (min.x <= 0.0f && min.y <= -min.x && max.y >=  min.x && min.z <= -min.x && max.z >=  min.x);
                    case 2: return (max.y >= 0.0f && min.x <=  max.y && max.x >= -max.y && min.z <=  max.y && max.z >= -max.y);
                    case 3: return (min.y <= 0.0f && min.x <= -min.y && max.x >=  min.y && min.z <= -min.y && max.z >=  min.y);
                    case 4: return (max.z >= 0.0f && min.x <=  max.z && max.x >= -max.z && min.y <=  max.z && max.y >= -max.z);
                    case 5: return (min.z <= 0.0f && min.x <= -min.z && max.x >=  min.z && min.y <= -min.z && max.y >=  min.z);
                };
                assert(0 && "SVBoundBox::insideCube(): bad face number");
            }
            return 0;
        }

        //
        s32 SVBoundBox::rayIntersection(const FVec3 &p,const FVec3 &direction) const {
            if(isValid())
                return rayIntersectionValid(p,direction);
            return 0;
        }

        s32 SVBoundBox::irayIntersection(const FVec3 &p,const FVec3 &idirection) const {
            if(isValid())
                return irayIntersectionValid(p,idirection);
            return 0;
        }

        s32 SVBoundBox::getIntersection(const FVec3 &p0,const FVec3 &p1) const {
            if(isValid())
                return getIntersectionValid(p0,p1);
            return 0;
        }

        //
        f32 SVBoundBox::distance() const {
            if(isValid())
                return distanceValid();
            return INFINITY;
        }

        f32 SVBoundBox::distance(const FVec3 &p) const {
            if(isValid())
                return distanceValid(p);
            return INFINITY;
        }

        //
        void SVBoundBox::getPoints(FVec3 *points,s32 num_points) const {
            assert(num_points == 8 && "SVBoundBox::getPoints(): bad points number");
            points[0].set(min.x,min.y,min.z);
            points[1].set(max.x,min.y,min.z);
            points[2].set(min.x,max.y,min.z);
            points[3].set(max.x,max.y,min.z);
            points[4].set(min.x,min.y,max.z);
            points[5].set(max.x,min.y,max.z);
            points[6].set(min.x,max.y,max.z);
            points[7].set(max.x,max.y,max.z);
        }

        void SVBoundBox::getPlanes(FVec4 *planes,s32 num_planes) const {
            assert(num_planes == 6 && "SVBoundBox::getPlanes(): bad planes number");
            planes[0].set( 1.0f, 0.0f, 0.0f,-max.x);
            planes[1].set(-1.0f, 0.0f, 0.0f, min.x);
            planes[2].set( 0.0f, 1.0f, 0.0f,-max.y);
            planes[3].set( 0.0f,-1.0f, 0.0f, min.y);
            planes[4].set( 0.0f, 0.0f, 1.0f,-max.z);
            planes[5].set( 0.0f, 0.0f,-1.0f, min.z);
        }

        //
        SVBoundBox operator*(const FMat4 &m,const SVBoundBox &bb) {
            SVBoundBox ret = bb;
            ret.setTransform(m);
            return ret;
        }

        SVBoundBox operator*(const DMat4 &m,const SVBoundBox &bb) {
            SVBoundBox ret = bb;
            ret.setTransform(m);
            return ret;
        }

        //******************************** SVBoundFrustum **********************************************

        SVBoundFrustum::SVBoundFrustum()
        : valid(0) {
        }

        SVBoundFrustum::SVBoundFrustum(const FMat4 &projection,const FMat4 &modelview) {
            set(projection,modelview);
        }

        SVBoundFrustum::SVBoundFrustum(const SVBoundFrustum &bf) {
            set(bf);
        }

        SVBoundFrustum::SVBoundFrustum(const SVBoundFrustum &bf,const FMat4 &itransform) {
            set(bf,itransform);
        }

        SVBoundFrustum::~SVBoundFrustum() {
        }

        //
        void SVBoundFrustum::clear() {
            valid = 0;
            // camera
            camera = FVec3_zero;
            // clipping planes
            for(s32 i = 0; i < 6; i++) {
                planes[i] = FVec4_zero;
            }
            // clipping planes and points
            for(s32 i = 0; i < 8; i++) {
                tplanes[i] = FVec4_zero;
                points[i] = FVec3_zero;
            }
//fyz
//            // portals
//            portals.clear();
        }

        //
        void SVBoundFrustum::set(const FMat4 &projection,const FMat4 &modelview) {
            valid = 1;
            // camera
            camera = inverse(modelview).getColumn3(3);
            // modelview projection matrix
            FMat4 mvp = projection * modelview;
            // points
            points[0].set(-1.0f,-1.0f,-1.0f);
            points[1].set( 1.0f,-1.0f,-1.0f);
            points[2].set(-1.0f, 1.0f,-1.0f);
            points[3].set( 1.0f, 1.0f,-1.0f);
            points[4].set(-1.0f,-1.0f, 1.0f);
            points[5].set( 1.0f,-1.0f, 1.0f);
            points[6].set(-1.0f, 1.0f, 1.0f);
            points[7].set( 1.0f, 1.0f, 1.0f);
            Simd::projMat4Vec3(points,sizeof(FVec3),inverse(mvp),points,sizeof(FVec3),8);
            // clipping planes
            planes[0].set(mvp.m30 + mvp.m00,mvp.m31 + mvp.m01,mvp.m32 + mvp.m02,mvp.m33 + mvp.m03);
            planes[1].set(mvp.m30 - mvp.m00,mvp.m31 - mvp.m01,mvp.m32 - mvp.m02,mvp.m33 - mvp.m03);
            planes[2].set(mvp.m30 + mvp.m10,mvp.m31 + mvp.m11,mvp.m32 + mvp.m12,mvp.m33 + mvp.m13);
            planes[3].set(mvp.m30 - mvp.m10,mvp.m31 - mvp.m11,mvp.m32 - mvp.m12,mvp.m33 - mvp.m13);
            planes[4].set(mvp.m30 + mvp.m20,mvp.m31 + mvp.m21,mvp.m32 + mvp.m22,mvp.m33 + mvp.m23);
            planes[5].set(mvp.m30 - mvp.m20,mvp.m31 - mvp.m21,mvp.m32 - mvp.m22,mvp.m33 - mvp.m23);
            for(s32 i = 0; i < 6; i++) {
                planes[i] /= length(FVec3(planes[i]));
            }
            for(s32 i = 0; i < 4; i++) {
                tplanes[0][i] = planes[i].x;
                tplanes[1][i] = planes[i].y;
                tplanes[2][i] = planes[i].z;
                tplanes[3][i] = planes[i].w;
            }
            for(s32 i = 0, j = 4; i < 2; i++, j++) {
                tplanes[4][i] = planes[j].x;
                tplanes[5][i] = planes[j].y;
                tplanes[6][i] = planes[j].z;
                tplanes[7][i] = planes[j].w;
            }
//fyz
//            // portals
//            portals.clear();
        }

        void SVBoundFrustum::set(const SVBoundFrustum &bf) {
            valid = bf.valid;
            // camera
            camera = bf.camera;
            // clipping planes and points
            for(s32 i = 0; i < 6; i++) {
                planes[i] = bf.planes[i];
            }
            for(s32 i = 0; i < 8; i++) {
                tplanes[i] = bf.tplanes[i];
                points[i] = bf.points[i];
            }
//fyz
//            // portals
//            portals = bf.portals;
        }

        void SVBoundFrustum::set(const SVBoundFrustum &bf,const FMat4 &itransform) {
            set(bf);
            setITransform(itransform);
        }

        //
        void SVBoundFrustum::setITransform(const FMat4 &itransform) {
            FMat4 transform = inverse(itransform);
            // camera
            camera = transform * camera;
            // points
            Simd::mulMat4Vec3(points,sizeof(FVec3),transform,points,sizeof(FVec3),8);
            // clipping planes
            for(s32 i = 0; i < 6; i++) {
                planes[i] = planes[i] * itransform;
                planes[i] /= length(FVec3(planes[i]));
            }
            for(s32 i = 0; i < 4; i++) {
                tplanes[0][i] = planes[i].x;
                tplanes[1][i] = planes[i].y;
                tplanes[2][i] = planes[i].z;
                tplanes[3][i] = planes[i].w;
            }
            for(s32 i = 0, j = 4; i < 2; i++, j++) {
                tplanes[4][i] = planes[j].x;
                tplanes[5][i] = planes[j].y;
                tplanes[6][i] = planes[j].z;
                tplanes[7][i] = planes[j].w;
            }
//fyz
//            // portals
//            for(s32 i = 0; i < portals.size(); i++) {
//                Portal &portal = portals[i];
//                portal.plane = portal.plane * itransform;
//                portal.plane /= length(FVec3(portal.plane));
//                Simd::mulMat4Vec3(portal.points,sizeof(FVec3),transform,portal.points,sizeof(FVec3),4);
//                for(s32 j = 0; j < 4; j++) {
//                    portal.planes[j] = portal.planes[j] * itransform;
//                    portal.planes[j] /= length(FVec3(portal.planes[j]));
//                    portal.tplanes[0][j] = portal.planes[j].x;
//                    portal.tplanes[1][j] = portal.planes[j].y;
//                    portal.tplanes[2][j] = portal.planes[j].z;
//                    portal.tplanes[3][j] = portal.planes[j].w;
//                }
//            }
        }

        void SVBoundFrustum::setITransform(const DMat4 &itransform) {
            setITransform(FMat4(itransform));
        }

        //
        s32 SVBoundFrustum::compare(const SVBoundFrustum &bf) const {
            if(valid != bf.valid)
                return 0;
            if(camera != bf.camera)
                return 0;
            //fyz
            //if(portals.size() != bf.portals.size())
                //return 0;
            // clipping planes
            if(planes[0] != bf.planes[0]) return 0;
            if(planes[1] != bf.planes[1]) return 0;
            if(planes[2] != bf.planes[2]) return 0;
            if(planes[3] != bf.planes[3]) return 0;
            if(planes[4] != bf.planes[4]) return 0;
            if(planes[5] != bf.planes[5]) return 0;
//fyz
//            // portals
//            for(s32 i = 0; i < portals.size(); i++) {
//                if(portals[i].plane != bf.portals[i].plane) return 0;
//                if(portals[i].planes[0] != bf.portals[i].planes[0]) return 0;
//                if(portals[i].planes[1] != bf.portals[i].planes[1]) return 0;
//                if(portals[i].planes[2] != bf.portals[i].planes[2]) return 0;
//                if(portals[i].planes[3] != bf.portals[i].planes[3]) return 0;
//            }
            
            return 1;
        }

        //
        s32 SVBoundFrustum::addPortal(const FVec3 *points,s32 num_points,const FMat4 &transform) {
            assert(num_points == 4 && "SVBoundFrustum::addPortal(): bad points number");
            Portal portal;
            // portal points
            for(s32 i = 0; i < num_points; i++) {
                portal.points[i] = transform * points[i];
            }
            // check visibility
            if(inside(portal.points,num_points) == 0)
                return 0;
            // portal normal
            FVec3 normal = cross(portal.points[1] - portal.points[0],portal.points[2] - portal.points[0]);
            f32 length = normal.length();
            if(length < EPSILON)
                return 0;
            normal /= length;
            // portal plane
            f32 angle = dot(camera - portal.points[0],normal);
            if(angle > 0.0f)
                normal = -normal;
            portal.plane = FVec4(normal,-dot(normal,portal.points[0]));
            // clipping planes
            s32 j = (angle > 0.0f) ? num_points - 1 : 1;
            for(s32 i = 0; i < num_points; i++) {
                normal = normalize(cross(portal.points[i] - camera,portal.points[j] - camera));
                portal.planes[i] = FVec4(normal,-dot(normal,camera));
                if(++j == num_points)
                    j = 0;
            }
            // copy planes
            for(s32 i = 0; i < 4; i++) {
                portal.tplanes[0][i] = portal.planes[i].x;
                portal.tplanes[1][i] = portal.planes[i].y;
                portal.tplanes[2][i] = portal.planes[i].z;
                portal.tplanes[3][i] = portal.planes[i].w;
            }
//fyz
//            // add portal
//            portals.append(portal);
            return 1;
        }

        void SVBoundFrustum::removePortal() {
            //fyz
//            assert(portals.size() > 0 && "SVBoundFrustum::removePortal(): portals underflow");
//            portals.remove();
        }

        s32 SVBoundFrustum::getNumPortals() const {
//            return portals.size();
            return 0;
        }

        const FVec3 *SVBoundFrustum::getPortalPoints(s32 num) const {
//            assert(num >= 0 && num < portals.size() && "SVBoundFrustum::getPortalPoints(): bad portal number");
//            return portals[num].points;
            return nullptr;
        }

        //
        void SVBoundFrustum::expand(f32 radius) {
            
            // clipping planes
            for(s32 i = 0; i < 6; i++) {
                planes[i].w += radius;
            }
            for(s32 i = 0; i < 4; i++) {
                tplanes[0][i] = planes[i].x;
                tplanes[1][i] = planes[i].y;
                tplanes[2][i] = planes[i].z;
                tplanes[3][i] = planes[i].w;
            }
            for(s32 i = 0, j = 4; i < 2; i++, j++) {
                tplanes[4][i] = planes[j].x;
                tplanes[5][i] = planes[j].y;
                tplanes[6][i] = planes[j].z;
                tplanes[7][i] = planes[j].w;
            }
        }

        //
        static sv_inline s32 inside_plane(const FVec4 &plane,const FVec3 &min,const FVec3 &max) {
            #ifdef USE_SSE
                __m128 min_xyz = _mm_mul_ps(min.vec,plane.vec);
                __m128 max_xyz = _mm_mul_ps(max.vec,plane.vec);
                __m128 min_max_x = _mm_shuffle_ps(min_xyz,max_xyz,_MM_PERM2(X,X,X,X));
                __m128 min_max_y = _mm_shuffle_ps(min_xyz,max_xyz,_MM_PERM2(Y,Y,Y,Y));
                min_max_x = _MM_SWIZZLE(min_max_x,X,Z,X,Z);
                min_max_y = _mm_add_ps(min_max_y,_MM_SWIZZLE(plane.vec,W,W,W,W));
                __m128 min_max_xy = _mm_add_ps(min_max_x,min_max_y);
                __m128 res_0 = _mm_add_ps(min_max_xy,_MM_SWIZZLE(min_xyz,Z,Z,Z,Z));
                __m128 res_1 = _mm_add_ps(min_max_xy,_MM_SWIZZLE(max_xyz,Z,Z,Z,Z));
                if(_mm_movemask_ps(_mm_and_ps(res_0,res_1)) != 0x0f) return 1;
            #elif USE_ALTIVEC
                vec_float4 plane_w = vec_perm(vec_splats(0.0f),plane.vec,VEC_PERM4(LX,LX,RW,LX));
                vec_float4 min_xyz = vec_madd(min.vec,plane.vec,plane_w);
                vec_float4 max_xyz = vec_madd(max.vec,plane.vec,plane_w);
                vec_float4 min_max_x = vec_perm(min_xyz,max_xyz,VEC_PERM4(LX,RX,LX,RX));
                vec_float4 min_max_y = vec_perm(min_xyz,max_xyz,VEC_PERM4(LY,LY,RY,RY));
                vec_float4 min_max_xy = vec_add(min_max_x,min_max_y);
                vec_float4 res_0 = vec_add(min_max_xy,VEC_SWIZZLE(min_xyz,Z,Z,Z,Z));
                vec_float4 res_1 = vec_add(min_max_xy,VEC_SWIZZLE(max_xyz,Z,Z,Z,Z));
                vec_uint4 res_2 = (vec_uint4)vec_and(res_0,res_1);
                if((VEC_SWIZZLE(res_2,B0,B0,B0,B0)[0] & 0x80808080) != 0x80808080) return 1;
            #else
                f32 min_x = min.x * plane.x;
                f32 min_y = min.y * plane.y;
                f32 min_zw = min.z * plane.z + plane.w;
                f32 min_min_xy = min_x + min_y;
                if(min_min_xy + min_zw > 0.0f) return 1;
                f32 max_x = max.x * plane.x;
                f32 max_min_xy = max_x + min_y;
                if(max_min_xy + min_zw > 0.0f) return 1;
                f32 max_y = max.y * plane.y;
                f32 min_max_xy = min_x + max_y;
                if(min_max_xy + min_zw > 0.0f) return 1;
                f32 max_max_xy = max_x + max_y;
                if(max_max_xy + min_zw > 0.0f) return 1;
                f32 max_zw = max.z * plane.z + plane.w;
                if(min_min_xy + max_zw > 0.0f) return 1;
                if(max_min_xy + max_zw > 0.0f) return 1;
                if(min_max_xy + max_zw > 0.0f) return 1;
                if(max_max_xy + max_zw > 0.0f) return 1;
            #endif
            return 0;
        }

        static sv_inline s32 inside_plane(const FVec4 &plane,const FVec3 *points,s32 num_points) {
            for(s32 i = 0; i < num_points; i++) {
                if(dot(plane,points[i]) > 0.0f) return 1;
            }
            return 0;
        }

        //
        s32 SVBoundFrustum::inside_planes(const FVec3 &point) const {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(tplanes[0].vec,_mm_set1_ps(point.x));
                __m128 res_1 = _mm_mul_ps(tplanes[1].vec,_mm_set1_ps(point.y));
                __m128 res_2 = _mm_mul_ps(tplanes[2].vec,_mm_set1_ps(point.z));
                __m128 res_3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,tplanes[3].vec));
                if(_mm_movemask_ps(res_3) & 0x0f) return 0;
                res_0 = _mm_mul_ps(tplanes[4].vec,_mm_set1_ps(point.x));
                res_1 = _mm_mul_ps(tplanes[5].vec,_mm_set1_ps(point.y));
                res_2 = _mm_mul_ps(tplanes[6].vec,_mm_set1_ps(point.z));
                res_3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,tplanes[7].vec));
                if(_mm_movemask_ps(res_3) & 0x03) return 0;
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_madd(tplanes[0].vec,vec_splats(point.x),tplanes[3].vec);
                vec_float4 res_1 = vec_madd(tplanes[1].vec,vec_splats(point.y),res_0);
                vec_float4 res_2 = vec_madd(tplanes[2].vec,vec_splats(point.z),res_1);
                if(VEC_SWIZZLE((vec_uint4)res_2,B0,B0,B0,B0)[0] & 0x80808080) return 0;
                res_0 = vec_madd(tplanes[4].vec,vec_splats(point.x),tplanes[7].vec);
                res_1 = vec_madd(tplanes[5].vec,vec_splats(point.y),res_0);
                res_2 = vec_madd(tplanes[6].vec,vec_splats(point.z),res_1);
                if(VEC_SWIZZLE((vec_uint4)res_2,B0,B0,B0,B0)[0] & 0x80800000) return 0;
            #elif USE_NEON
                float32x4_t res_0 = vmlaq_n_f32(tplanes[3].vec,tplanes[0].vec,point.x);
                float32x4_t res_1 = vmlaq_n_f32(res_0,tplanes[1].vec,point.y);
                float32x4_t res_2 = vmlaq_n_f32(res_1,tplanes[2].vec,point.z);
                if(vmaskq_u32(vcltq_f32(res_2,vdupq_n_f32(0.0f)))) return 0;
                float32x2_t res_3 = vmla_n_f32(vget_low_f32(tplanes[7].vec),vget_low_f32(tplanes[4].vec),point.x);
                float32x2_t res_4 = vmla_n_f32(res_3,vget_low_f32(tplanes[5].vec),point.y);
                float32x2_t res_5 = vmla_n_f32(res_4,vget_low_f32(tplanes[6].vec),point.z);
                if(vmask_u32(vclt_f32(res_5,vdup_n_f32(0.0f)))) return 0;
            #else
                if(dot(planes[0],point) < 0.0f) return 0;
                if(dot(planes[1],point) < 0.0f) return 0;
                if(dot(planes[2],point) < 0.0f) return 0;
                if(dot(planes[3],point) < 0.0f) return 0;
                if(dot(planes[4],point) < 0.0f) return 0;
                if(dot(planes[5],point) < 0.0f) return 0;
            #endif
            return 1;
        }

        s32 SVBoundFrustum::inside_planes(const FVec3 &point,f32 radius) const {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(tplanes[0].vec,_mm_set1_ps(point.x));
                __m128 res_1 = _mm_mul_ps(tplanes[1].vec,_mm_set1_ps(point.y));
                __m128 res_2 = _mm_mul_ps(tplanes[2].vec,_mm_set1_ps(point.z));
                __m128 res_3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,tplanes[3].vec));
                if(_mm_movemask_ps(_mm_add_ps(res_3,_mm_set1_ps(radius))) & 0x0f)
                    return 0;
                res_0 = _mm_mul_ps(tplanes[4].vec,_mm_set1_ps(point.x));
                res_1 = _mm_mul_ps(tplanes[5].vec,_mm_set1_ps(point.y));
                res_2 = _mm_mul_ps(tplanes[6].vec,_mm_set1_ps(point.z));
                res_3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,tplanes[7].vec));
                if(_mm_movemask_ps(_mm_add_ps(res_3,_mm_set1_ps(radius))) & 0x03)
                    return 0;
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_madd(tplanes[0].vec,vec_splats(point.x),tplanes[3].vec);
                vec_float4 res_1 = vec_madd(tplanes[1].vec,vec_splats(point.y),res_0);
                vec_float4 res_2 = vec_madd(tplanes[2].vec,vec_splats(point.z),res_1);
                vec_uint4 res_3 = (vec_uint4)vec_add(res_2,vec_splats(radius));
                if(VEC_SWIZZLE(res_3,B0,B0,B0,B0)[0] & 0x80808080)
                    return 0;
                res_0 = vec_madd(tplanes[4].vec,vec_splats(point.x),tplanes[7].vec);
                res_1 = vec_madd(tplanes[5].vec,vec_splats(point.y),res_0);
                res_2 = vec_madd(tplanes[6].vec,vec_splats(point.z),res_1);
                res_3 = (vec_uint4)vec_add(res_2,vec_splats(radius));
                if(VEC_SWIZZLE(res_3,B0,B0,B0,B0)[0] & 0x80800000)
                    return 0;
            #elif USE_NEON
                float32x4_t res_0 = vmlaq_n_f32(tplanes[3].vec,tplanes[0].vec,point.x);
                float32x4_t res_1 = vmlaq_n_f32(res_0,tplanes[1].vec,point.y);
                float32x4_t res_2 = vmlaq_n_f32(res_1,tplanes[2].vec,point.z);
                if(vmaskq_u32(vcltq_f32(res_2,vdupq_n_f32(-radius))))
                    return 0;
                float32x2_t res_3 = vmla_n_f32(vget_low_f32(tplanes[7].vec),vget_low_f32(tplanes[4].vec),point.x);
                float32x2_t res_4 = vmla_n_f32(res_3,vget_low_f32(tplanes[5].vec),point.y);
                float32x2_t res_5 = vmla_n_f32(res_4,vget_low_f32(tplanes[6].vec),point.z);
                if(vmask_u32(vclt_f32(res_5,vdup_n_f32(-radius))))
                    return 0;
            #else
                if(dot(planes[0],point) < -radius) return 0;
                if(dot(planes[1],point) < -radius) return 0;
                if(dot(planes[2],point) < -radius) return 0;
                if(dot(planes[3],point) < -radius) return 0;
                if(dot(planes[4],point) < -radius) return 0;
                if(dot(planes[5],point) < -radius) return 0;
            #endif
            return 1;
        }

        s32 SVBoundFrustum::inside_planes(const FVec3 &min,const FVec3 &max) const {
            if(inside_plane(planes[0],min,max) == 0) return 0;
            if(inside_plane(planes[1],min,max) == 0) return 0;
            if(inside_plane(planes[2],min,max) == 0) return 0;
            if(inside_plane(planes[3],min,max) == 0) return 0;
            if(inside_plane(planes[4],min,max) == 0) return 0;
            if(inside_plane(planes[5],min,max) == 0) return 0;
            return 1;
        }

        s32 SVBoundFrustum::inside_planes(const FVec3 *points,s32 num_points) const {
            if(inside_plane(planes[0],points,num_points) == 0) return 0;
            if(inside_plane(planes[1],points,num_points) == 0) return 0;
            if(inside_plane(planes[2],points,num_points) == 0) return 0;
            if(inside_plane(planes[3],points,num_points) == 0) return 0;
            if(inside_plane(planes[4],points,num_points) == 0) return 0;
            if(inside_plane(planes[5],points,num_points) == 0) return 0;
            return 1;
        }

        //
        s32 SVBoundFrustum::inside_planes_fast(const FVec3 &point) const {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(tplanes[0].vec,_mm_set1_ps(point.x));
                __m128 res_1 = _mm_mul_ps(tplanes[1].vec,_mm_set1_ps(point.y));
                __m128 res_2 = _mm_mul_ps(tplanes[2].vec,_mm_set1_ps(point.z));
                __m128 res_3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,tplanes[3].vec));
                if(_mm_movemask_ps(res_3) & 0x0f)
                    return 0;
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_madd(tplanes[0].vec,vec_splats(point.x),tplanes[3].vec);
                vec_float4 res_1 = vec_madd(tplanes[1].vec,vec_splats(point.y),res_0);
                vec_float4 res_2 = vec_madd(tplanes[2].vec,vec_splats(point.z),res_1);
                if(VEC_SWIZZLE((vec_uint4)res_2,B0,B0,B0,B0)[0] & 0x80808080)
                    return 0;
            #elif USE_NEON
                float32x4_t res_0 = vmlaq_n_f32(tplanes[3].vec,tplanes[0].vec,point.x);
                float32x4_t res_1 = vmlaq_n_f32(res_0,tplanes[1].vec,point.y);
                float32x4_t res_2 = vmlaq_n_f32(res_1,tplanes[2].vec,point.z);
                if(vmaskq_u32(vcltq_f32(res_2,vdupq_n_f32(0.0f))))
                    return 0;
            #else
                if(dot(planes[0],point) < 0.0f) return 0;
                if(dot(planes[1],point) < 0.0f) return 0;
                if(dot(planes[2],point) < 0.0f) return 0;
                if(dot(planes[3],point) < 0.0f) return 0;
            #endif
            return 1;
        }

        s32 SVBoundFrustum::inside_planes_fast(const FVec3 &point,f32 radius) const {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(tplanes[0].vec,_mm_set1_ps(point.x));
                __m128 res_1 = _mm_mul_ps(tplanes[1].vec,_mm_set1_ps(point.y));
                __m128 res_2 = _mm_mul_ps(tplanes[2].vec,_mm_set1_ps(point.z));
                __m128 res_3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,tplanes[3].vec));
                if(_mm_movemask_ps(_mm_add_ps(res_3,_mm_set1_ps(radius))) & 0x0f)
                    return 0;
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_madd(tplanes[0].vec,vec_splats(point.x),tplanes[3].vec);
                vec_float4 res_1 = vec_madd(tplanes[1].vec,vec_splats(point.y),res_0);
                vec_float4 res_2 = vec_madd(tplanes[2].vec,vec_splats(point.z),res_1);
                vec_uint4 res_3 = (vec_uint4)vec_add(res_2,vec_splats(radius));
                if(VEC_SWIZZLE(res_3,B0,B0,B0,B0)[0] & 0x80808080)
                    return 0;
            #elif USE_NEON
                float32x4_t res_0 = vmlaq_n_f32(tplanes[3].vec,tplanes[0].vec,point.x);
                float32x4_t res_1 = vmlaq_n_f32(res_0,tplanes[1].vec,point.y);
                float32x4_t res_2 = vmlaq_n_f32(res_1,tplanes[2].vec,point.z);
                if(vmaskq_u32(vcltq_f32(res_2,vdupq_n_f32(-radius))))
                    return 0;
            #else
                if(dot(planes[0],point) < -radius) return 0;
                if(dot(planes[1],point) < -radius) return 0;
                if(dot(planes[2],point) < -radius) return 0;
                if(dot(planes[3],point) < -radius) return 0;
            #endif
            return 1;
        }

        s32 SVBoundFrustum::inside_planes_fast(const FVec3 &min,const FVec3 &max) const {
            if(inside_plane(planes[0],min,max) == 0) return 0;
            if(inside_plane(planes[1],min,max) == 0) return 0;
            if(inside_plane(planes[2],min,max) == 0) return 0;
            if(inside_plane(planes[3],min,max) == 0) return 0;
            return 1;
        }

        s32 SVBoundFrustum::inside_planes_fast(const FVec3 *points,s32 num_points) const {
            if(inside_plane(planes[0],points,num_points) == 0) return 0;
            if(inside_plane(planes[1],points,num_points) == 0) return 0;
            if(inside_plane(planes[2],points,num_points) == 0) return 0;
            if(inside_plane(planes[3],points,num_points) == 0) return 0;
            return 1;
        }

        //
        sv_inline s32 SVBoundFrustum::inside_portal(const Portal &portal,const FVec3 &point) const {
            if(dot(portal.plane,point) < 0.0f) return 0;
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(portal.tplanes[0].vec,_mm_set1_ps(point.x));
                __m128 res_1 = _mm_mul_ps(portal.tplanes[1].vec,_mm_set1_ps(point.y));
                __m128 res_2 = _mm_mul_ps(portal.tplanes[2].vec,_mm_set1_ps(point.z));
                __m128 res_3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,portal.tplanes[3].vec));
                if(_mm_movemask_ps(res_3))
                    return 0;
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_madd(portal.tplanes[0].vec,vec_splats(point.x),portal.tplanes[3].vec);
                vec_float4 res_1 = vec_madd(portal.tplanes[1].vec,vec_splats(point.y),res_0);
                vec_float4 res_2 = vec_madd(portal.tplanes[2].vec,vec_splats(point.z),res_1);
                if(VEC_SWIZZLE((vec_uint4)res_2,B0,B0,B0,B0)[0] & 0x80808080)
                    return 0;
            #elif USE_NEON
                float32x4_t res_0 = vmlaq_n_f32(portal.tplanes[3].vec,portal.tplanes[0].vec,point.x);
                float32x4_t res_1 = vmlaq_n_f32(res_0,portal.tplanes[1].vec,point.y);
                float32x4_t res_2 = vmlaq_n_f32(res_1,portal.tplanes[2].vec,point.z);
                if(vmaskq_u32(vcltq_f32(res_2,vdupq_n_f32(0.0f))))
                    return 0;
            #else
                if(dot(portal.planes[0],point) < 0.0f) return 0;
                if(dot(portal.planes[1],point) < 0.0f) return 0;
                if(dot(portal.planes[2],point) < 0.0f) return 0;
                if(dot(portal.planes[3],point) < 0.0f) return 0;
            #endif
            return 1;
        }

        sv_inline s32 SVBoundFrustum::inside_portal(const Portal &portal,const FVec3 &point,f32 radius) const {
            if(dot(portal.plane,point) < -radius)
                return 0;
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(portal.tplanes[0].vec,_mm_set1_ps(point.x));
                __m128 res_1 = _mm_mul_ps(portal.tplanes[1].vec,_mm_set1_ps(point.y));
                __m128 res_2 = _mm_mul_ps(portal.tplanes[2].vec,_mm_set1_ps(point.z));
                __m128 res_3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,portal.tplanes[3].vec));
                if(_mm_movemask_ps(_mm_add_ps(res_3,_mm_set1_ps(radius))))
                    return 0;
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_madd(portal.tplanes[0].vec,vec_splats(point.x),portal.tplanes[3].vec);
                vec_float4 res_1 = vec_madd(portal.tplanes[1].vec,vec_splats(point.y),res_0);
                vec_float4 res_2 = vec_madd(portal.tplanes[2].vec,vec_splats(point.z),res_1);
                vec_uint4 res_3 = (vec_uint4)vec_add(res_2,vec_splats(radius));
                if(VEC_SWIZZLE(res_3,B0,B0,B0,B0)[0] & 0x80808080)
                    return 0;
            #elif USE_NEON
                float32x4_t res_0 = vmlaq_n_f32(portal.tplanes[3].vec,portal.tplanes[0].vec,point.x);
                float32x4_t res_1 = vmlaq_n_f32(res_0,portal.tplanes[1].vec,point.y);
                float32x4_t res_2 = vmlaq_n_f32(res_1,portal.tplanes[2].vec,point.z);
                if(vmaskq_u32(vcltq_f32(res_2,vdupq_n_f32(-radius))))
                    return 0;
            #else
                if(dot(portal.planes[0],point) < -radius)
                    return 0;
                if(dot(portal.planes[1],point) < -radius)
                    return 0;
                if(dot(portal.planes[2],point) < -radius)
                    return 0;
                if(dot(portal.planes[3],point) < -radius)
                    return 0;
            #endif
            return 1;
        }

        sv_inline s32 SVBoundFrustum::inside_portal(const Portal &portal,const FVec3 &min,const FVec3 &max) const {
            if(inside_plane(portal.plane,min,max) == 0)
                return 0;
            if(inside_plane(portal.planes[0],min,max) == 0)
                return 0;
            if(inside_plane(portal.planes[1],min,max) == 0)
                return 0;
            if(inside_plane(portal.planes[2],min,max) == 0)
                return 0;
            if(inside_plane(portal.planes[3],min,max) == 0)
                return 0;
            return 1;
        }

        sv_inline s32 SVBoundFrustum::inside_portal(const Portal &portal,const FVec3 *points,s32 num_points) const {
            if(inside_plane(portal.plane,points,num_points) == 0)
                return 0;
            if(inside_plane(portal.planes[0],points,num_points) == 0)
                return 0;
            if(inside_plane(portal.planes[1],points,num_points) == 0)
                return 0;
            if(inside_plane(portal.planes[2],points,num_points) == 0)
                return 0;
            if(inside_plane(portal.planes[3],points,num_points) == 0)
                return 0;
            return 1;
        }

        //
        s32 SVBoundFrustum::inside_portals(const FVec3 &point) const {
//            for(s32 i = 0; i < portals.size(); i++) {
//                if(inside_portal(portals[i],point) == 0) return 0;
//            }
            return 1;
        }

        s32 SVBoundFrustum::inside_portals(const FVec3 &point,f32 radius) const {
//            for(s32 i = 0; i < portals.size(); i++) {
//                if(inside_portal(portals[i],point,radius) == 0) return 0;
//            }
            return 1;
        }

        s32 SVBoundFrustum::inside_portals(const FVec3 &min,const FVec3 &max) const {
//            for(s32 i = 0; i < portals.size(); i++) {
//                if(inside_portal(portals[i],min,max) == 0) return 0;
//            }
            return 1;
        }

        s32 SVBoundFrustum::inside_portals(const FVec3 *points,s32 num_points) const {
//            for(s32 i = 0; i < portals.size(); i++) {
//                if(inside_portal(portals[i],points,num_points) == 0) return 0;
//            }
            return 1;
        }

        //
        s32 SVBoundFrustum::inside(const SVBoundSphere &bs) const {
            if(bs.isValid())
                return insideValid(bs);
            return 0;
        }

        s32 SVBoundFrustum::inside(const SVBoundBox &bb) const {
            if(bb.isValid())
                return insideValid(bb);
            return 0;
        }

        s32 SVBoundFrustum::inside(const SVBoundFrustum &bf) const {
            if(bf.isValid())
                return insideValid(bf);
            return 0;
        }

        //
        s32 SVBoundFrustum::insideAll(const SVBoundSphere &bs) const {
            if(bs.isValid())
                return insideAllValid(bs);
            return 0;
        }

        s32 SVBoundFrustum::insideAll(const SVBoundBox &bb) const {
            if(bb.isValid())
                return insideAllValid(bb);
            return 0;
        }

        s32 SVBoundFrustum::insideAll(const SVBoundFrustum &bf) const {
            if(bf.isValid())
                return insideAllValid(bf);
            return 0;
        }

        //
        s32 SVBoundFrustum::insidePlanes(const SVBoundSphere &bs) const {
            if(bs.isValid())
                return insidePlanesValid(bs);
            return 0;
        }

        s32 SVBoundFrustum::insidePlanes(const SVBoundBox &bb) const {
            if(bb.isValid())
                return insidePlanesValid(bb);
            return 0;
        }

        s32 SVBoundFrustum::insidePlanes(const SVBoundFrustum &bf) const {
            if(bf.isValid())
                return insidePlanesValid(bf);
            return 0;
        }

        //
        s32 SVBoundFrustum::insidePortals(const SVBoundSphere &bs) const {
            if(bs.isValid())
                return insidePortalsValid(bs);
            return 0;
        }

        s32 SVBoundFrustum::insidePortals(const SVBoundBox &bb) const {
            if(bb.isValid())
                return insidePortalsValid(bb);
            return 0;
        }

        s32 SVBoundFrustum::insidePortals(const SVBoundFrustum &bf) const {
            if(bf.isValid())
                return insidePortalsValid(bf);
            return 0;
        }

        //
        s32 SVBoundFrustum::insideShadow(const FVec3 &direction,const SVBoundSphere &object) const {
            if(object.isValid()) {
                // object is inside the bound frustum
                if(inside_planes(object.getCenter(),object.getRadius()))
                    return 1;
                // shadow volume is inside the bound frustum
                for(s32 i = 0; i < 6; i++) {
                    f32 k = dot3(planes[i],direction);
                    if(Math::abs(k) < EPSILON)
                        continue;
                    k = -dot(planes[i],object.getCenter()) / k;
                    if(k > object.getRadius())
                        continue;
                    if(inside_planes(object.getCenter() + direction * k,object.getRadius()))
                        return 1;
                }
                return 0;
            }
            return 0;
        }

        s32 SVBoundFrustum::insideShadow(const SVBoundSphere &light,const FVec3 &offset,const SVBoundSphere &object) const {
            if(light.isValid() && object.isValid()) {
                // object is outside the light bounds
                if(light.inside(object) == 0)
                    return 0;
                // object is inside the bound frustum
                if(inside_planes(object.getCenter(),object.getRadius()))
                    return 1;
                // direction from light center to object center
                FVec3 direction = object.getCenter() - offset;
                f32 distance = length(direction);
                if(distance < object.getRadius() + EPSILON)
                    return 1;
                direction /= distance;
                // basis
                FVec3 x,y;
                orthoBasis(direction,x,y);
                // near points
                FVec3 x0 = x * object.getRadius();
                FVec3 y0 = y * object.getRadius();
                FVec3 z0 = offset + direction * (distance - object.getRadius());
                // far points
                f32 radius = light.getRadius() + dot(direction,light.getCenter() - offset);
                f32 k = object.getRadius() * radius / (distance - object.getRadius());
                FVec3 x1 = x * k;
                FVec3 y1 = y * k;
                FVec3 z1 = offset + direction * radius;
                // check visibility
                FVec3 points[8] = {
                    x0 + y0 + z0,x0 - y0 + z0,-x0 - y0 + z0,-x0 + y0 + z0,
                    x1 + y1 + z1,x1 - y1 + z1,-x1 - y1 + z1,-x1 + y1 + z1,
                };
                return inside_planes(points,8);
            }
            return 0;
        }

    } //!namespace util

}//!namespace sv
