#ifndef __aBOUNDS_H__
#define __aBOUNDS_H__

#include "SVBase.h"
#include "SVGeometry.h"

//

namespace sv {
    
    namespace util {
        
        class SVBoundSphere;
        class SVBoundBox;
        class SVBoundFrustum;
        //***************************************SVBoundSphere***************************************
        //
        class SVBoundSphere {
        public:
            SVBoundSphere();
            //
            SVBoundSphere(const FVec3 &center,f32 radius);
            //
            SVBoundSphere(const FVec3 *points,s32 num_points,s32 optimal);
            //
            SVBoundSphere(const SVBoundSphere &bs);
            //
            SVBoundSphere(const SVBoundSphere &bs,const FMat4 &transform);
            //
            explicit SVBoundSphere(const SVBoundBox &bb);
            //
            ~SVBoundSphere();
            //
            void clear();
            //
            void set(const FVec3 &center,f32 radius);
            //
            void set(const FVec3 *points,s32 num_points,s32 optimal);
            //
            void set(const SVBoundSphere &bs);
            //
            void set(const SVBoundSphere &bs,const FMat4 &transform);
            //
            void set(const SVBoundBox &bb);
            // transformation
            void setTransform(const FMat4 &transform);
            //
            void setTransform(const DMat4 &transform);
            // compare
            s32 compare(const SVBoundSphere &bs) const;
            //
            sv_inline s32 operator==(const SVBoundSphere &bs) const {
                return compare(bs);
            }
            //
            sv_inline s32 operator!=(const SVBoundSphere &bs) const {
                return !compare(bs);
            }
            // expand
            void expand(const FVec3 &point);
            //
            void expand(const FVec3 *points,s32 num_points);
            //
            void expand(const SVBoundSphere &bs);
            //
            void expand(const SVBoundBox &bb);
            // radius expand
            void expandRadius(const FVec3 &point);
            //
            void expandRadius(const FVec3 *points,s32 num_points);
            //
            void expandRadius(const SVBoundSphere &bs);
            //
            void expandRadius(const SVBoundBox &bb);
            // inside points
            s32 inside(const FVec3 &point) const;
            //
            s32 inside(const FVec3 &point,f32 radius) const;
            //
            s32 inside(const FVec3 &min,const FVec3 &max) const;
            //
            s32 insideValid(const FVec3 &point) const;
            //
            s32 insideValid(const FVec3 &point,f32 radius) const;
            //
            s32 insideValid(const FVec3 &min,const FVec3 &max) const;
            // inside bounds
            s32 inside(const SVBoundSphere &bs) const;
            //
            s32 inside(const SVBoundBox &bb) const;
            //
            s32 insideValid(const SVBoundSphere &bs) const;
            //
            s32 insideValid(const SVBoundBox &bb) const;
            //
            s32 insideAll(const SVBoundSphere &bs) const;
            ////
            s32 insideAll(const SVBoundBox &bb) const;
            
            s32 insideAllValid(const SVBoundSphere &bs) const;
            //
            s32 insideAllValid(const SVBoundBox &bb) const;
            // intersections
            s32 rayIntersection(const FVec3 &point,const FVec3 &direction) const;
            //
            s32 getIntersection(const FVec3 &p0,const FVec3 &p1) const;
            //
            s32 rayIntersectionValid(const FVec3 &point,const FVec3 &direction) const;
            //
            s32 getIntersectionValid(const FVec3 &p0,const FVec3 &p1) const;
            // distance
            f32 distance() const;
            //
            f32 distance(const FVec3 &point) const;
            //
            f32 distanceValid() const;
            //
            f32 distanceValid(const FVec3 &point) const;
            // parameters
            sv_inline s32 isValid() const {
                return (center.w > 0.0f);
            }
            //
            sv_inline const FVec3 &getCenter() const {
                return center;
            }
            //
            sv_inline f32 getRadius() const {
                return center.w;
            }
            
        private:
            FVec3 center;    // bounding sphere center and radius
        };
        
        //
        SVBoundSphere operator*(const FMat4 &m,const SVBoundSphere &bs);
        
        SVBoundSphere operator*(const DMat4 &m,const SVBoundSphere &bs);
        
        //
        sv_inline s32 SVBoundSphere::insideValid(const FVec3 &point) const {
            return (length2(center - point) <= center.w * center.w);
        }
        
        sv_inline s32 SVBoundSphere::insideValid(const FVec3 &point,f32 radius) const {
            radius += center.w;
            return (length2(center - point) <= radius * radius);
        }
        
        sv_inline s32 SVBoundSphere::insideValid(const FVec3 &min,const FVec3 &max) const {
#ifdef USE_SSE
            __m128 radius = _MM_SWIZZLE(center.vec,W,W,W,W);
            __m128 res_0 = _mm_cmplt_ps(_mm_add_ps(center.vec,radius),min.vec);
            __m128 res_1 = _mm_cmpgt_ps(_mm_sub_ps(center.vec,radius),max.vec);
            return ((_mm_movemask_ps(_mm_or_ps(res_0,res_1)) & 0x07) == 0);
#elif USE_ALTIVEC
            vec_float4 radius = VEC_SWIZZLE(center.vec,W,W,W,W);
            vec_uint4 res_0 = (vec_uint4)vec_cmplt(vec_add(center.vec,radius),min.vec);
            vec_uint4 res_1 = (vec_uint4)vec_cmpgt(vec_sub(center.vec,radius),max.vec);
            return ((vec_perm(vec_or(res_0,res_1),res_1,VEC_PERM2(B0,B0,B0,B0))[0] & 0xffffff00) == 0);
#elif USE_NEON
            float32x4_t radius = vdupq_lane_f32(vget_high_f32(center.vec),1);
            uint32x4_t res_0 = vcltq_f32(vaddq_f32(center.vec,radius),min.vec);
            uint32x4_t res_1 = vcgtq_f32(vsubq_f32(center.vec,radius),max.vec);
            return ((vmaskq_u32(vorrq_u32(res_0,res_1)) & 0x07) == 0);
#else
            if(center.x + center.w < min.x || center.x - center.w > max.x) return 0;
            if(center.y + center.w < min.y || center.y - center.w > max.y) return 0;
            if(center.z + center.w < min.z || center.z - center.w > max.z) return 0;
            return 1;
#endif
        }
        //
        sv_inline s32 SVBoundSphere::insideValid(const SVBoundSphere &bs) const {
            return insideValid(bs.center,bs.center.w);
        }
        //
        sv_inline s32 SVBoundSphere::insideAllValid(const SVBoundSphere &bs) const {
            f32 radius = center.w - bs.center.w;
            if(radius > 0.0f) return (length2(center - bs.center) <= radius * radius);
            return 0;
        }
        //
        sv_inline s32 SVBoundSphere::rayIntersectionValid(const FVec3 &point,const FVec3 &direction) const {
            f32 k = saturate(dot(direction,center - point) / length2(direction));
            return (length2(center - point - direction * k) <= center.w * center.w);
        }
        //
        sv_inline s32 SVBoundSphere::getIntersectionValid(const FVec3 &p0,const FVec3 &p1) const {
            return rayIntersectionValid(p0,p1 - p0);
        }
        //
        sv_inline f32 SVBoundSphere::distanceValid() const {
#ifdef USE_SSE
            f32 ret;
            __m128 direction = _mm_rcp_ss(_mm_rsqrt_ss(_mm_dot33_ps(center.vec,center.vec)));
            direction = _mm_sub_ss(direction,_MM_SWIZZLE(center.vec,W,W,W,W));
            _mm_store_ss(&ret,direction);
            return ret;
#elif USE_ALTIVEC
            FVec4 ret;
            vec_float4 direction = vec_re(vec_rsqrte(vec_dot33(center.vec,center.vec)));
            direction = vec_sub(direction,VEC_SWIZZLE(center.vec,W,W,W,W));
            vec_ste(direction,0,&ret.x);
            return ret.x;
#elif USE_NEON
            float32x4_t direction = vrecpeq_f32(vrsqrteq_f32(vdot33q_f32(center.vec,center.vec)));
            direction = vsubq_f32(direction,center.vec);
            return vgetq_lane_f32(direction,3);
#else
            return Math::sqrtFast(center.length2()) - center.w;
#endif
        }
        //
        sv_inline f32 SVBoundSphere::distanceValid(const FVec3 &point) const {
#ifdef USE_SSE
            f32 ret;
            __m128 direction = _mm_sub_ps(center.vec,point.vec);
            direction = _mm_rcp_ss(_mm_rsqrt_ss(_mm_dot33_ps(direction,direction)));
            direction = _mm_sub_ss(direction,_MM_SWIZZLE(center.vec,W,W,W,W));
            _mm_store_ss(&ret,direction);
            return ret;
#elif USE_ALTIVEC
            FVec4 ret;
            vec_float4 direction = vec_sub(center.vec,point.vec);
            direction = vec_re(vec_rsqrte(vec_dot33(direction,direction)));
            direction = vec_sub(direction,VEC_SWIZZLE(center.vec,W,W,W,W));
            vec_ste(direction,0,&ret.x);
            return ret.x;
#elif USE_NEON
            float32x4_t direction = vsubq_f32(center.vec,point.vec);
            direction = vrecpeq_f32(vrsqrteq_f32(vdot33q_f32(direction,direction)));
            direction = vsubq_f32(direction,center.vec);
            return vgetq_lane_f32(direction,3);
#else
            FVec3 direction;
            sub(direction,center,point);
            return Math::sqrtFast(direction.length2()) - center.w;
#endif
        }
        
        //********************************** SVBoundBox ********************************************\
        //
        class SVBoundBox {
        public:
            SVBoundBox();
            SVBoundBox(const FVec3 &min,const FVec3 &max);
            SVBoundBox(const FVec3 *points,s32 num_points);
            SVBoundBox(const SVBoundBox &bb);
            SVBoundBox(const SVBoundBox &bb,const FMat4 &transform);
            explicit SVBoundBox(const SVBoundSphere &bs);
            ~SVBoundBox();
            //
            void clear();
            //
            void set(const FVec3 &min,const FVec3 &max);
            void set(const FVec3 *points,s32 num_points);
            void set(const SVBoundSphere &bs);
            void set(const SVBoundBox &bb);
            void set(const SVBoundBox &bb,const FMat4 &transform);
            // transformation
            void setTransform(const FMat4 &transform);
            void setTransform(const DMat4 &transform);
            void setTransform(const SVBoundSphere &bs,const FMat4 &transform);
            void setTransform(const SVBoundSphere &bs,const DMat4 &transform);
            // compare
            s32 compare(const SVBoundBox &bb) const;
            sv_inline s32 operator==(const SVBoundBox &bb) const { return compare(bb); }
            sv_inline s32 operator!=(const SVBoundBox &bb) const { return !compare(bb); }
            // expand
            void expand(const FVec3 &point);
            void expand(const FVec3 *points,s32 num_points);
            void expand(const SVBoundSphere &bs);
            void expand(const SVBoundBox &bb);
            // inside points
            s32 inside(const FVec3 &point) const;
            s32 inside(const FVec3 &point,f32 radius) const;
            s32 inside(const FVec3 &min,const FVec3 &max) const;
            s32 insideValid(const FVec3 &point) const;
            s32 insideValid(const FVec3 &point,f32 radius) const;
            s32 insideValid(const FVec3 &min,const FVec3 &max) const;
            // inside bounds
            s32 inside(const SVBoundSphere &bs) const;
            s32 inside(const SVBoundBox &bb) const;
            s32 insideValid(const SVBoundSphere &bs) const;
            s32 insideValid(const SVBoundBox &bb) const;
            s32 insideAll(const SVBoundSphere &bs) const;
            s32 insideAll(const SVBoundBox &bb) const;
            s32 insideAllValid(const SVBoundSphere &bs) const;
            s32 insideAllValid(const SVBoundBox &bb) const;
            // inside cube
            s32 insideCube(s32 face,const FVec3 &offset) const;
            // intersections
            s32 rayIntersection(const FVec3 &point,const FVec3 &direction) const;
            s32 irayIntersection(const FVec3 &point,const FVec3 &idirection) const;
            s32 getIntersection(const FVec3 &p0,const FVec3 &p1) const;
            s32 rayIntersectionValid(const FVec3 &point,const FVec3 &direction) const;
            s32 irayIntersectionValid(const FVec3 &point,const FVec3 &idirection) const;
            s32 getIntersectionValid(const FVec3 &p0,const FVec3 &p1) const;
            // distance
            f32 distance() const;
            f32 distance(const FVec3 &point) const;
            f32 distanceValid() const;
            f32 distanceValid(const FVec3 &point) const;
            // parameters
            sv_inline s32 isValid() const { return (min.x <= max.x); }
            sv_inline const FVec3 &getMin() const { return min; }
            sv_inline const FVec3 &getMax() const { return max; }
            void getPoints(FVec3 *points,s32 num_points) const;
            void getPlanes(FVec4 *planes,s32 num_planes) const;
        private:
            FVec3 min;        // bounding box minimum
            FVec3 max;        // bounding box maximum
        };
        
        //
        SVBoundBox operator*(const FMat4 &m,const SVBoundBox &bb);
        SVBoundBox operator*(const DMat4 &m,const SVBoundBox &bb);
        
        //
        sv_inline s32 SVBoundSphere::insideValid(const SVBoundBox &bb) const {
            return insideValid(bb.getMin(),bb.getMax());
        }
        
        //
        sv_inline s32 SVBoundSphere::insideAllValid(const SVBoundBox &bb) const {
            const FVec3 &min = bb.getMin();
            const FVec3 &max = bb.getMax();
            if(insideValid(FVec3(min.x,min.y,min.z)) == 0) return 0;
            if(insideValid(FVec3(max.x,min.y,min.z)) == 0) return 0;
            if(insideValid(FVec3(min.x,max.y,min.z)) == 0) return 0;
            if(insideValid(FVec3(max.x,max.y,min.z)) == 0) return 0;
            if(insideValid(FVec3(min.x,min.y,max.z)) == 0) return 0;
            if(insideValid(FVec3(max.x,min.y,max.z)) == 0) return 0;
            if(insideValid(FVec3(min.x,max.y,max.z)) == 0) return 0;
            if(insideValid(FVec3(max.x,max.y,max.z)) == 0) return 0;
            return 1;
        }
        
        //
        sv_inline s32 SVBoundBox::insideValid(const FVec3 &point) const {
#ifdef USE_SSE
            __m128 res_0 = _mm_cmpgt_ps(min.vec,point.vec);
            __m128 res_1 = _mm_cmplt_ps(max.vec,point.vec);
            return ((_mm_movemask_ps(_mm_or_ps(res_0,res_1)) & 0x07) == 0);
#elif USE_ALTIVEC
            vec_uint4 res_0 = (vec_uint4)vec_cmpgt(min.vec,point.vec);
            vec_uint4 res_1 = (vec_uint4)vec_cmplt(max.vec,point.vec);
            return ((vec_perm(vec_or(res_0,res_1),res_1,VEC_PERM2(B0,B0,B0,B0))[0] & 0xffffff00) == 0);
#elif USE_NEON
            uint32x4_t res_0 = vcgtq_f32(min.vec,point.vec);
            uint32x4_t res_1 = vcltq_f32(max.vec,point.vec);
            return ((vmaskq_u32(vorrq_u32(res_0,res_1)) & 0x07) == 0);
#else
            if(min.x > point.x || max.x < point.x) return 0;
            if(min.y > point.y || max.y < point.y) return 0;
            if(min.z > point.z || max.z < point.z) return 0;
            return 1;
#endif
        }
        //
        sv_inline s32 SVBoundBox::insideValid(const FVec3 &point,f32 radius) const {
#ifdef USE_SSE
            __m128 r = FVec3(radius).vec;
            __m128 res_0 = _mm_cmpgt_ps(min.vec,_mm_add_ps(point.vec,r));
            __m128 res_1 = _mm_cmplt_ps(max.vec,_mm_sub_ps(point.vec,r));
            return ((_mm_movemask_ps(_mm_or_ps(res_0,res_1)) & 0x07) == 0);
#elif USE_ALTIVEC
            vec_float4 r = vec_splats(radius);
            vec_uint4 res_0 = (vec_uint4)vec_cmpgt(min.vec,vec_add(point.vec,r));
            vec_uint4 res_1 = (vec_uint4)vec_cmplt(max.vec,vec_sub(point.vec,r));
            return ((vec_perm(vec_or(res_0,res_1),res_1,VEC_PERM2(B0,B0,B0,B0))[0] & 0xffffff00) == 0);
#elif USE_NEON
            float32x4_t r = vdupq_n_f32(radius);
            uint32x4_t res_0 = vcgtq_f32(min.vec,vaddq_f32(point.vec,r));
            uint32x4_t res_1 = vcltq_f32(max.vec,vsubq_f32(point.vec,r));
            return ((vmaskq_u32(vorrq_u32(res_0,res_1)) & 0x07) == 0);
#else
            if(min.x > point.x + radius || max.x < point.x - radius) return 0;
            if(min.y > point.y + radius || max.y < point.y - radius) return 0;
            if(min.z > point.z + radius || max.z < point.z - radius) return 0;
            return 1;
#endif
        }
        //
        sv_inline s32 SVBoundBox::insideValid(const FVec3 &min_,const FVec3 &max_) const {
#ifdef USE_SSE
            __m128 res_0 = _mm_cmpgt_ps(min.vec,max_.vec);
            __m128 res_1 = _mm_cmplt_ps(max.vec,min_.vec);
            return ((_mm_movemask_ps(_mm_or_ps(res_0,res_1)) & 0x07) == 0);
#elif USE_ALTIVEC
            vec_uint4 res_0 = (vec_uint4)vec_cmpgt(min.vec,max_.vec);
            vec_uint4 res_1 = (vec_uint4)vec_cmplt(max.vec,min_.vec);
            return ((vec_perm(vec_or(res_0,res_1),res_1,VEC_PERM2(B0,B0,B0,B0))[0] & 0xffffff00) == 0);
#elif USE_NEON
            uint32x4_t res_0 = vcgtq_f32(min.vec,max_.vec);
            uint32x4_t res_1 = vcltq_f32(max.vec,min_.vec);
            return ((vmaskq_u32(vorrq_u32(res_0,res_1)) & 0x07) == 0);
#else
            if(min.x > max_.x || max.x < min_.x) return 0;
            if(min.y > max_.y || max.y < min_.y) return 0;
            if(min.z > max_.z || max.z < min_.z) return 0;
            return 1;
#endif
        }
        //
        sv_inline s32 SVBoundBox::insideValid(const SVBoundSphere &bs) const {
            const FVec3 &center = bs.getCenter();
#ifdef USE_SSE
            __m128 radius = _MM_SWIZZLE(center.vec,W,W,W,W);
            __m128 res_0 = _mm_cmpgt_ps(min.vec,_mm_add_ps(center.vec,radius));
            __m128 res_1 = _mm_cmplt_ps(max.vec,_mm_sub_ps(center.vec,radius));
            return ((_mm_movemask_ps(_mm_or_ps(res_0,res_1)) & 0x07) == 0);
#elif USE_ALTIVEC
            vec_float4 radius = VEC_SWIZZLE(center.vec,W,W,W,W);
            vec_uint4 res_0 = (vec_uint4)vec_cmpgt(min.vec,vec_add(center.vec,radius));
            vec_uint4 res_1 = (vec_uint4)vec_cmplt(max.vec,vec_sub(center.vec,radius));
            return ((vec_perm(vec_or(res_0,res_1),res_1,VEC_PERM2(B0,B0,B0,B0))[0] & 0xffffff00) == 0);
#elif USE_NEON
            float32x4_t radius = vdupq_lane_f32(vget_high_f32(center.vec),1);
            uint32x4_t res_0 = vcgtq_f32(min.vec,vaddq_f32(center.vec,radius));
            uint32x4_t res_1 = vcltq_f32(max.vec,vsubq_f32(center.vec,radius));
            return ((vmaskq_u32(vorrq_u32(res_0,res_1)) & 0x07) == 0);
#else
            f32 radius = bs.getRadius();
            if(min.x > center.x + radius || max.x < center.x - radius) return 0;
            if(min.y > center.y + radius || max.y < center.y - radius) return 0;
            if(min.z > center.z + radius || max.z < center.z - radius) return 0;
            return 1;
#endif
        }
        //
        sv_inline s32 SVBoundBox::insideValid(const SVBoundBox &bb) const {
            return insideValid(bb.min,bb.max);
        }
        //
        sv_inline s32 SVBoundBox::insideAllValid(const SVBoundSphere &bs) const {
            const FVec3 &center = bs.getCenter();
#ifdef USE_SSE
            __m128 radius = _MM_SWIZZLE(center.vec,W,W,W,W);
            __m128 res_0 = _mm_cmpgt_ps(min.vec,_mm_sub_ps(center.vec,radius));
            __m128 res_1 = _mm_cmplt_ps(max.vec,_mm_add_ps(center.vec,radius));
            return ((_mm_movemask_ps(_mm_or_ps(res_0,res_1)) & 0x07) == 0);
#elif USE_ALTIVEC
            vec_float4 radius = VEC_SWIZZLE(center.vec,W,W,W,W);
            vec_uint4 res_0 = (vec_uint4)vec_cmpgt(min.vec,vec_sub(center.vec,radius));
            vec_uint4 res_1 = (vec_uint4)vec_cmplt(max.vec,vec_add(center.vec,radius));
            return ((vec_perm(vec_or(res_0,res_1),res_1,VEC_PERM2(B0,B0,B0,B0))[0] & 0xffffff00) == 0);
#elif USE_NEON
            float32x4_t radius = vdupq_lane_f32(vget_high_f32(center.vec),1);
            uint32x4_t res_0 = vcgtq_f32(min.vec,vsubq_f32(center.vec,radius));
            uint32x4_t res_1 = vcltq_f32(max.vec,vaddq_f32(center.vec,radius));
            return ((vmaskq_u32(vorrq_u32(res_0,res_1)) & 0x07) == 0);
#else
            f32 radius = bs.getRadius();
            if(min.x > center.x - radius || max.x < center.x + radius) return 0;
            if(min.y > center.y - radius || max.y < center.y + radius) return 0;
            if(min.z > center.z - radius || max.z < center.z + radius) return 0;
            return 1;
#endif
            return 0;
        }
        //
        sv_inline s32 SVBoundBox::insideAllValid(const SVBoundBox &bb) const {
#ifdef USE_SSE
            __m128 res_0 = _mm_cmpgt_ps(min.vec,bb.min.vec);
            __m128 res_1 = _mm_cmplt_ps(max.vec,bb.max.vec);
            return ((_mm_movemask_ps(_mm_or_ps(res_0,res_1)) & 0x07) == 0);
#elif USE_ALTIVEC
            vec_uint4 res_0 = (vec_uint4)vec_cmpgt(min.vec,bb.min.vec);
            vec_uint4 res_1 = (vec_uint4)vec_cmplt(max.vec,bb.max.vec);
            return ((vec_perm(vec_or(res_0,res_1),res_1,VEC_PERM2(B0,B0,B0,B0))[0] & 0xffffff00) == 0);
#elif USE_NEON
            uint32x4_t res_0 = vcgtq_f32(min.vec,bb.min.vec);
            uint32x4_t res_1 = vcltq_f32(max.vec,bb.max.vec);
            return ((vmaskq_u32(vorrq_u32(res_0,res_1)) & 0x07) == 0);
#else
            if(min.x > bb.min.x || max.x < bb.max.x) return 0;
            if(min.y > bb.min.y || max.y < bb.max.y) return 0;
            if(min.z > bb.min.z || max.z < bb.max.z) return 0;
            return 1;
#endif
            return 0;
        }
        //
        sv_inline s32 SVBoundBox::rayIntersectionValid(const FVec3 &point,const FVec3 &direction) const {
            return rayBoundBoxIntersection(point,direction,min,max);
        }
        
        sv_inline s32 SVBoundBox::irayIntersectionValid(const FVec3 &point,const FVec3 &idirection) const {
            return irayBoundBoxIntersection(point,idirection,min,max);
        }
        
        sv_inline s32 SVBoundBox::getIntersectionValid(const FVec3 &p0,const FVec3 &p1) const {
            return rayBoundBoxIntersection(p0,p1 - p0,min,max);
        }
        //
        sv_inline f32 SVBoundBox::distanceValid() const {
#ifdef USE_SSE
            f32 ret;
            __m128 direction = _mm_min_ps(_mm_max_ps(FVec3_zero.vec,min.vec),max.vec);
            direction = _mm_rcp_ss(_mm_rsqrt_ss(_mm_dot33_ps(direction,direction)));
            _mm_store_ss(&ret,direction);
            return ret;
#elif USE_ALTIVEC
            FVec4 ret;
            vec_float4 direction = vec_min(vec_max(FVec3_zero.vec,min.vec),max.vec);
            direction = vec_re(vec_rsqrte(vec_dot33(direction,direction)));
            vec_ste(direction,0,&ret.x);
            return ret.x;
#elif USE_NEON
            float32x4_t direction = vminq_f32(vmaxq_f32(FVec3_zero.vec,min.vec),max.vec);
            direction = vrecpeq_f32(vrsqrteq_f32(vdot33q_f32(direction,direction)));
            return vgetq_lane_f32(direction,0);
#else
            FVec3 direction;
            if(min.x > 0.0f) {
                direction.x = min.x;
            } else if(max.x < 0.0f){
                direction.x = max.x;
            } else {
                direction.x = 0.0f;
            }
            
            if(min.y > 0.0f) {
                direction.y = min.y;
            } else if(max.y < 0.0f) {
                direction.y = max.y;
            } else {
                direction.y = 0.0f;
            }
            if(min.z > 0.0f) {
                direction.z = min.z;
            } else if(max.z < 0.0f) {
                direction.z = max.z;
            } else {
                direction.z = 0.0f;
            }
            return Math::sqrtFast(direction.length2());
#endif
        }
        
        sv_inline f32 SVBoundBox::distanceValid(const FVec3 &point) const {
#ifdef USE_SSE
            f32 ret;
            __m128 direction = _mm_sub_ps(_mm_min_ps(_mm_max_ps(point.vec,min.vec),max.vec),point.vec);
            direction = _mm_rcp_ss(_mm_rsqrt_ss(_mm_dot33_ps(direction,direction)));
            _mm_store_ss(&ret,direction);
            return ret;
#elif USE_ALTIVEC
            FVec4 ret;
            vec_float4 direction = vec_sub(vec_min(vec_max(point.vec,min.vec),max.vec),point.vec);
            direction = vec_re(vec_rsqrte(vec_dot33(direction,direction)));
            vec_ste(direction,0,&ret.x);
            return ret.x;
#elif USE_NEON
            float32x4_t direction = vsubq_f32(vminq_f32(vmaxq_f32(point.vec,min.vec),max.vec),point.vec);
            direction = vrecpeq_f32(vrsqrteq_f32(vdot33q_f32(direction,direction)));
            return vgetq_lane_f32(direction,0);
#else
            FVec3 direction;
            if(min.x > point.x) direction.x = min.x - point.x;
            else if(max.x < point.x) direction.x = max.x - point.x;
            else direction.x = 0.0f;
            if(min.y > point.y) direction.y = min.y - point.y;
            else if(max.y < point.y) direction.y = max.y - point.y;
            else direction.y = 0.0f;
            if(min.z > point.z) direction.z = min.z - point.z;
            else if(max.z < point.z) direction.z = max.z - point.z;
            else direction.z = 0.0f;
            return Math::sqrtFast(direction.length2());
#endif
        }
        
        //********************************* SVBoundFrustum *********************************************
        //
        class SVBoundFrustum {
        public:
            SVBoundFrustum();
            SVBoundFrustum(const FMat4 &projection,const FMat4 &modelview);
            SVBoundFrustum(const SVBoundFrustum &bf);
            SVBoundFrustum(const SVBoundFrustum &bf,const FMat4 &itransform);
            ~SVBoundFrustum();
            
            void clear();
            
            void set(const FMat4 &projection,const FMat4 &modelview);
            void set(const SVBoundFrustum &bf);
            void set(const SVBoundFrustum &bf,const FMat4 &itransform);
            
            // transformation
            void setITransform(const FMat4 &itransform);
            void setITransform(const DMat4 &itransform);
            
            // compare
            s32 compare(const SVBoundFrustum &bf) const;
            sv_inline s32 operator==(const SVBoundFrustum &bf) const { return compare(bf); }
            sv_inline s32 operator!=(const SVBoundFrustum &bf) const { return !compare(bf); }
            
            // portals
            s32 addPortal(const FVec3 *points,s32 num_points,const FMat4 &transform);
            void removePortal();
            
            s32 getNumPortals() const;
            const FVec3 *getPortalPoints(s32 num) const;
            
            // expand
            void expand(f32 radius);
            
            // inside points
            s32 inside(const FVec3 &point) const;
            s32 inside(const FVec3 &point,f32 radius) const;
            s32 inside(const FVec3 &min,const FVec3 &max) const;
            s32 inside(const FVec3 *points,s32 num) const;
            
            s32 insideFast(const FVec3 &point) const;
            s32 insideFast(const FVec3 &point,f32 radius) const;
            s32 insideFast(const FVec3 &min,const FVec3 &max) const;
            s32 insideFast(const FVec3 *points,s32 num) const;
            
            // inside bounds
            s32 inside(const SVBoundSphere &bs) const;
            s32 inside(const SVBoundBox &bb) const;
            s32 inside(const SVBoundFrustum &bf) const;
            
            s32 insideValid(const SVBoundSphere &bs) const;
            s32 insideValid(const SVBoundBox &bb) const;
            s32 insideValid(const SVBoundFrustum &bf) const;
            
            s32 insideValidFast(const SVBoundSphere &bs) const;
            s32 insideValidFast(const SVBoundBox &bb) const;
            s32 insideValidFast(const SVBoundFrustum &bf) const;
            
            // inside all bounds
            s32 insideAll(const SVBoundSphere &bs) const;
            s32 insideAll(const SVBoundBox &bb) const;
            s32 insideAll(const SVBoundFrustum &bf) const;
            
            s32 insideAllValid(const SVBoundSphere &bs) const;
            s32 insideAllValid(const SVBoundBox &bb) const;
            s32 insideAllValid(const SVBoundFrustum &bf) const;
            
            s32 insideAllValidFast(const SVBoundSphere &bs) const;
            s32 insideAllValidFast(const SVBoundBox &bb) const;
            s32 insideAllValidFast(const SVBoundFrustum &bf) const;
            
            // inside planes bounds
            s32 insidePlanes(const SVBoundSphere &bs) const;
            s32 insidePlanes(const SVBoundBox &bb) const;
            s32 insidePlanes(const SVBoundFrustum &bf) const;
            
            s32 insidePlanesValid(const SVBoundSphere &bs) const;
            s32 insidePlanesValid(const SVBoundBox &bb) const;
            s32 insidePlanesValid(const SVBoundFrustum &bf) const;
            
            s32 insidePlanesValidFast(const SVBoundSphere &bs) const;
            s32 insidePlanesValidFast(const SVBoundBox &bb) const;
            s32 insidePlanesValidFast(const SVBoundFrustum &bf) const;
            
            // inside portals
            s32 insidePortals(const SVBoundSphere &bs) const;
            s32 insidePortals(const SVBoundBox &bb) const;
            s32 insidePortals(const SVBoundFrustum &bf) const;
            
            s32 insidePortalsValid(const SVBoundSphere &bs) const;
            s32 insidePortalsValid(const SVBoundBox &bb) const;
            s32 insidePortalsValid(const SVBoundFrustum &bf) const;
            
            // inside shadow
            s32 insideShadow(const FVec3 &direction,const SVBoundSphere &object) const;
            s32 insideShadow(const SVBoundSphere &light,const FVec3 &offset,const SVBoundSphere &object) const;
            
            // parameters
            sv_inline s32 isValid() const { return valid; }
            sv_inline const FVec3 &getCamera() const { return camera; }
            sv_inline const FVec4 *getPlanes() const { return planes; }
            sv_inline const FVec3 *getPoints() const { return points; }
            
        private:
            
            struct Portal;
            
            enum {
                PLANE_L = 0,
                PLANE_R,
                PLANE_B,
                PLANE_T,
                PLANE_N,
                PLANE_F,
            };
            
            s32 inside_planes(const FVec3 &point) const;
            s32 inside_planes(const FVec3 &point,f32 radius) const;
            s32 inside_planes(const FVec3 &min,const FVec3 &max) const;
            s32 inside_planes(const FVec3 *points,s32 num_points) const;
            
            s32 inside_planes_fast(const FVec3 &point) const;
            s32 inside_planes_fast(const FVec3 &point,f32 radius) const;
            s32 inside_planes_fast(const FVec3 &min,const FVec3 &max) const;
            s32 inside_planes_fast(const FVec3 *points,s32 num_points) const;
            
            s32 inside_portal(const Portal &portal,const FVec3 &point) const;
            s32 inside_portal(const Portal &portal,const FVec3 &point,f32 radius) const;
            s32 inside_portal(const Portal &portal,const FVec3 &min,const FVec3 &max) const;
            s32 inside_portal(const Portal &portal,const FVec3 *points,s32 num_points) const;
            
            s32 inside_portals(const FVec3 &point) const;
            s32 inside_portals(const FVec3 &point,f32 radius) const;
            s32 inside_portals(const FVec3 &min,const FVec3 &max) const;
            s32 inside_portals(const FVec3 *points,s32 num_points) const;
            
            struct Portal {
                FVec4 plane;                    // portal plane
                FVec4 planes[4];                // aos clipping planes
                FVec4 tplanes[4];                // soa clipping planes
                FVec3 points[4];                // bounding points
            };
            
            s32 valid;                          // valid flag
            FVec3 camera;                        // camera position
            FVec4 planes[6];                    // aos clipping planes
            FVec4 tplanes[8];                    // soa clipping planes
            FVec3 points[8];                    // bounding points
            
            FVec4 garbage[4];                    // memory layout
            
            //SVArrayStack<Portal,16> portals;    // portals
        };
        
        //
        sv_inline s32 SVBoundFrustum::inside(const FVec3 &point) const {
            if(inside_portals(point) == 0)
                return 0;
            return inside_planes(point);
        }
        
        sv_inline s32 SVBoundFrustum::inside(const FVec3 &point,f32 radius) const {
            if(inside_portals(point,radius) == 0)
                return 0;
            return inside_planes(point,radius);
        }
        
        sv_inline s32 SVBoundFrustum::inside(const FVec3 &min,const FVec3 &max) const {
            if(inside_portals(min,max) == 0)
                return 0;
            return inside_planes(min,max);
        }
        
        sv_inline s32 SVBoundFrustum::inside(const FVec3 *points,s32 num_points) const {
            if(inside_portals(points,num_points) == 0)
                return 0;
            return inside_planes(points,num_points);
        }
        
        //
        sv_inline s32 SVBoundFrustum::insideFast(const FVec3 &point) const {
            if(inside_portals(point) == 0)
                return 0;
            return inside_planes_fast(point);
        }
        
        sv_inline s32 SVBoundFrustum::insideFast(const FVec3 &point,f32 radius) const {
            if(inside_portals(point,radius) == 0)
                return 0;
            return inside_planes_fast(point,radius);
        }
        
        sv_inline s32 SVBoundFrustum::insideFast(const FVec3 &min,const FVec3 &max) const {
            if(inside_portals(min,max) == 0)
                return 0;
            return inside_planes_fast(min,max);
        }
        
        sv_inline s32 SVBoundFrustum::insideFast(const FVec3 *points,s32 num_points) const {
            if(inside_portals(points,num_points) == 0)
                return 0;
            return inside_planes_fast(points,num_points);
        }
        //
        sv_inline s32 SVBoundFrustum::insideValid(const SVBoundSphere &bs) const {
            return inside(bs.getCenter(),bs.getRadius());
        }
        
        sv_inline s32 SVBoundFrustum::insideValid(const SVBoundBox &bb) const {
            return inside(bb.getMin(),bb.getMax());
        }
        
        sv_inline s32 SVBoundFrustum::insideValid(const SVBoundFrustum &bf) const {
            return inside(bf.points,8);
        }
        //
        sv_inline s32 SVBoundFrustum::insideValidFast(const SVBoundSphere &bs) const {
            return insideFast(bs.getCenter(),bs.getRadius());
        }
        
        sv_inline s32 SVBoundFrustum::insideValidFast(const SVBoundBox &bb) const {
            return insideFast(bb.getMin(),bb.getMax());
        }
        
        sv_inline s32 SVBoundFrustum::insideValidFast(const SVBoundFrustum &bf) const {
            return insideFast(bf.points,8);
        }
        
        //
        sv_inline s32 SVBoundFrustum::insideAllValid(const SVBoundSphere &bs) const {
            return inside(bs.getCenter(),-bs.getRadius());
        }
        
        sv_inline s32 SVBoundFrustum::insideAllValid(const SVBoundBox &bb) const {
            const FVec3 &min = bb.getMin();
            const FVec3 &max = bb.getMax();
            if(inside(FVec3(min.x,min.y,min.z)) == 0) return 0;
            if(inside(FVec3(max.x,min.y,min.z)) == 0) return 0;
            if(inside(FVec3(min.x,max.y,min.z)) == 0) return 0;
            if(inside(FVec3(max.x,max.y,min.z)) == 0) return 0;
            if(inside(FVec3(min.x,min.y,max.z)) == 0) return 0;
            if(inside(FVec3(max.x,min.y,max.z)) == 0) return 0;
            if(inside(FVec3(min.x,max.y,max.z)) == 0) return 0;
            if(inside(FVec3(max.x,max.y,max.z)) == 0) return 0;
            return 1;
        }
        
        sv_inline s32 SVBoundFrustum::insideAllValid(const SVBoundFrustum &bf) const {
            for(s32 i = 0; i < 8; i++) {
                if(inside(bf.points[i]) == 0) return 0;
            }
            return 1;
        }
        
        //
        sv_inline s32 SVBoundFrustum::insideAllValidFast(const SVBoundSphere &bs) const {
            return insideFast(bs.getCenter(),-bs.getRadius());
        }
        
        sv_inline s32 SVBoundFrustum::insideAllValidFast(const SVBoundBox &bb) const {
            const FVec3 &min = bb.getMin();
            const FVec3 &max = bb.getMax();
            if(insideFast(FVec3(min.x,min.y,min.z)) == 0) return 0;
            if(insideFast(FVec3(max.x,min.y,min.z)) == 0) return 0;
            if(insideFast(FVec3(min.x,max.y,min.z)) == 0) return 0;
            if(insideFast(FVec3(max.x,max.y,min.z)) == 0) return 0;
            if(insideFast(FVec3(min.x,min.y,max.z)) == 0) return 0;
            if(insideFast(FVec3(max.x,min.y,max.z)) == 0) return 0;
            if(insideFast(FVec3(min.x,max.y,max.z)) == 0) return 0;
            if(insideFast(FVec3(max.x,max.y,max.z)) == 0) return 0;
            return 1;
        }
        
        sv_inline s32 SVBoundFrustum::insideAllValidFast(const SVBoundFrustum &bf) const {
            for(s32 i = 0; i < 8; i++) {
                if(insideFast(bf.points[i]) == 0) return 0;
            }
            return 1;
        }
        
        //
        sv_inline s32 SVBoundFrustum::insidePlanesValid(const SVBoundSphere &bs) const {
            return inside_planes(bs.getCenter(),bs.getRadius());
        }
        
        sv_inline s32 SVBoundFrustum::insidePlanesValid(const SVBoundBox &bb) const {
            return inside_planes(bb.getMin(),bb.getMax());
        }
        
        sv_inline s32 SVBoundFrustum::insidePlanesValid(const SVBoundFrustum &bf) const {
            return inside_planes(bf.points,8);
        }
        
        //
        sv_inline s32 SVBoundFrustum::insidePlanesValidFast(const SVBoundSphere &bs) const {
            return inside_planes_fast(bs.getCenter(),bs.getRadius());
        }
        
        sv_inline s32 SVBoundFrustum::insidePlanesValidFast(const SVBoundBox &bb) const {
            return inside_planes_fast(bb.getMin(),bb.getMax());
        }
        
        sv_inline s32 SVBoundFrustum::insidePlanesValidFast(const SVBoundFrustum &bf) const {
            return inside_planes_fast(bf.points,8);
        }
        
        //
        sv_inline s32 SVBoundFrustum::insidePortalsValid(const SVBoundSphere &bs) const {
            return inside_portals(bs.getCenter(),bs.getRadius());
        }
        
        sv_inline s32 SVBoundFrustum::insidePortalsValid(const SVBoundBox &bb) const {
            return inside_portals(bb.getMin(),bb.getMax());
        }
        
        sv_inline s32 SVBoundFrustum::insidePortalsValid(const SVBoundFrustum &bf) const {
            return inside_portals(bf.points,8);
        }

        
    } //!namespace util
    
}//!namespace sv


#endif /* __BOUNDS_H__ */
