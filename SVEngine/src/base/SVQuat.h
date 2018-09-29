#ifndef __aQUAT_H__
#define __aQUAT_H__

#include "SVVec4.h"
#include "SVMat4.h"

namespace sv {
    
    namespace util {
        
        /******************************************************************************\
         *
         * SVQuat
         *
         \******************************************************************************/
        
        //
        ATTRIBUTE_ALIGNED16(struct) SVQuat {
            
            sv_inline SVQuat() { }
            SVQuat(const FVec3 &axis,f32 angle);
            SVQuat(f32 x,f32 y,f32 z,f32 angle);
            SVQuat(f32 angle_x,f32 angle_y,f32 angle_z);
            explicit sv_inline SVQuat(const f32 *q) : x(q[0]), y(q[1]), z(q[2]), w(q[3]) { }
            explicit sv_inline SVQuat(const FVec3 &v) : x(v.x), y(v.y), z(v.z), w(0.0f) { }
            explicit sv_inline SVQuat(const FVec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
            explicit SVQuat(const FMat3 &m);
            explicit SVQuat(const FMat4 &m);
            explicit SVQuat(const DMat4 &m);
#ifdef USE_SSE
            sv_inline SVQuat(const SVQuat &q) : vec(q.vec) { }
            explicit sv_inline SVQuat(__m128 vec) : vec(vec) { }
#elif USE_ALTIVEC
            sv_inline SVQuat(const SVQuat &q) : vec(q.vec) { }
            explicit sv_inline SVQuat(vec_float4 vec) : vec(vec) { }
#elif USE_NEON
            sv_inline SVQuat(const SVQuat &q) : vec(q.vec) { }
            explicit sv_inline SVQuat(float32x4_t vec) : vec(vec) { }
#else
            sv_inline SVQuat(const SVQuat &q) : x(q.x), y(q.y), z(q.z), w(q.w) { }
#endif
            
            sv_inline SVQuat &operator=(const SVQuat &q) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                vec = q.vec;
#else
                x = q.x; y = q.y; z = q.z; w = q.w;
#endif
                return *this;
            }
            
            SVQuat operator-() const;
            SVQuat &operator*=(f32 v);
            SVQuat &operator*=(const SVQuat &q);
            SVQuat &operator+=(const SVQuat &q);
            SVQuat &operator-=(const SVQuat &q);
            
            sv_inline operator f32*() { return q; }
            sv_inline operator const f32*() const { return q; }
            sv_inline operator void*() { return q; }
            sv_inline operator const void*() const { return q; }
            
            sv_inline f32 &operator[](s32 i) {
                assert((u32)i < 4 && "SVQuat::operator[](): bad index");
                return q[i];
            }
            sv_inline f32 operator[](s32 i) const {
                assert((u32)i < 4 && "SVQuat::operator[](): bad index");
                return q[i];
            }
            
            void set(const FVec3 &v);
            void set(const FMat3 &m);
            void set(const FMat4 &m);
            void set(const DMat4 &m);
            void set(const FVec3 &axis,f32 angle);
            void set(f32 x,f32 y,f32 z,f32 angle);
            void set(f32 angle_x,f32 angle_y,f32 angle_z);
            void get(FVec3 &axis,f32 &angle) const;
            
            sv_inline void set(const f32 *q) {
                x = q[0]; y = q[1]; z = q[2]; w = q[3];
            }
            sv_inline void get(f32 *q) const {
                q[0] = x; q[1] = y; q[2] = z; q[3] = w;
            }
            sv_inline f32 *get() { return q; }
            sv_inline const f32 *get() const { return q; }
            
            FMat3 getMat3() const;
            f32 getAngle(const FVec3 &axis) const;
            
            sv_inline SVQuat &normalize() {
                f32 ilength = Math::rsqrt(x * x + y * y + z * z + w * w);
                x *= ilength; y *= ilength; z *= ilength; w *= ilength;
                return *this;
            }
            sv_inline SVQuat &normalizeFast() {
                f32 ilength = Math::rsqrtFast(x * x + y * y + z * z + w * w);
                x *= ilength; y *= ilength; z *= ilength; w *= ilength;
                return *this;
            }
            
            union {
                struct {
                    f32 x,y,z,w;
                };
                f32 q[4];
#ifdef USE_SSE
                __m128 vec;
#elif USE_ALTIVEC
                vec_float4 vec;
#elif USE_NEON
                float32x4_t vec;
#endif
            };
        };
        
        //
        extern const SVQuat quat_identity;
        
        //
        s32 operator==(const SVQuat &q0,const SVQuat &q1);
        s32 operator!=(const SVQuat &q0,const SVQuat &q1);
        SVQuat operator*(const SVQuat &q,f32 v);
        FVec3 operator*(const SVQuat &q,const FVec3 &v);
        FVec3 operator*(const FVec3 &v,const SVQuat &q);
        DVec3 operator*(const SVQuat &q,const DVec3 &v);
        DVec3 operator*(const DVec3 &v,const SVQuat &q);
        SVQuat operator*(const SVQuat &q0,const SVQuat &q1);
        SVQuat operator+(const SVQuat &q0,const SVQuat &q1);
        SVQuat operator-(const SVQuat &q0,const SVQuat &q1);
        
        //
        sv_inline s32 compare(const SVQuat &q0,const SVQuat &q1) {
            return (compare(q0.x,q1.x) && compare(q0.y,q1.y) && compare(q0.z,q1.z) && compare(q0.w,q1.w));
        }
        
        sv_inline s32 compare(const SVQuat &q0,const SVQuat &q1,f32 epsilon) {
            return (compare(q0.x,q1.x,epsilon) && compare(q0.y,q1.y,epsilon) && compare(q0.z,q1.z,epsilon) && compare(q0.w,q1.w,epsilon));
        }
        
        sv_inline f32 dot(const SVQuat &q0,const SVQuat &q1) {
            return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
        }
        
        sv_inline SVQuat &mul(SVQuat &ret,const SVQuat &q,f32 v) {
            ret.x = q.x * v;
            ret.y = q.y * v;
            ret.z = q.z * v;
            ret.w = q.w * v;
            return ret;
        }
        
        sv_inline SVQuat &mad(SVQuat &ret,const SVQuat &q0,f32 v,const SVQuat &q1) {
            ret.x = q0.x * v + q1.x;
            ret.y = q0.y * v + q1.y;
            ret.z = q0.z * v + q1.z;
            ret.w = q0.w * v + q1.w;
            return ret;
        }
        
        sv_inline SVQuat &add(SVQuat &ret,const SVQuat &q0,const SVQuat &q1) {
            ret.x = q0.x + q1.x;
            ret.y = q0.y + q1.y;
            ret.z = q0.z + q1.z;
            ret.w = q0.w + q1.w;
            return ret;
        }
        
        sv_inline SVQuat &sub(SVQuat &ret,const SVQuat &q0,const SVQuat &q1) {
            ret.x = q0.x - q1.x;
            ret.y = q0.y - q1.y;
            ret.z = q0.z - q1.z;
            ret.w = q0.w - q1.w;
            return ret;
        }
        
        sv_inline SVQuat &inverse(SVQuat &ret,const SVQuat &q) {
            ret.x = -q.x;
            ret.y = -q.y;
            ret.z = -q.z;
            ret.w = q.w;
            return ret;
        }
        
        //
        FVec3 &mul(FVec3 &ret,const SVQuat &q,const FVec3 &v);
        FVec3 &mul(FVec3 &ret,const FVec3 &v,const SVQuat &q);
        DVec3 &mul(DVec3 &ret,const SVQuat &q,const DVec3 &v);
        DVec3 &mul(DVec3 &ret,const DVec3 &v,const SVQuat &q);
        SVQuat &mul(SVQuat &ret,const SVQuat &q0,const SVQuat &q1);
        SVQuat &slerp(SVQuat &ret,const SVQuat &q0,const SVQuat &q1,f32 k);
        
        //
        SVQuat normalize(const SVQuat &q);
        SVQuat inverse(const SVQuat &q);
        SVQuat slerp(const SVQuat &q0,const SVQuat &q1,f32 k);
        
        //
        void decomposeTransform(const FMat4 &m,SVQuat &q0,SVQuat &q1);
        FMat4 &composeTransform(FMat4 &ret,const SVQuat &q0,const SVQuat &q1);
        
    }//!namespace util
    
}//!namespace sv


#endif /* __aQUAT_H__ */
