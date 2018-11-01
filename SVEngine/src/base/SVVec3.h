#ifndef __aVEC3_H__
#define __aVEC3_H__

#include "SVVec2.h"

namespace sv {
    
    namespace util {
        
        /******************************************************************************\
         *
         * FVec3
         *
         \******************************************************************************/
        
        //
        ATTRIBUTE_ALIGNED16(struct) FVec3 {
            
            sv_inline FVec3() {
            }
            
            sv_inline FVec3(const FVec2 &v,f32 z)
            : x(v.x)
            , y(v.y)
            , z(z)
            , w(0.0f) {
                
            }
            
            sv_inline FVec3(f32 x,f32 y,f32 z)
            : x(x)
            , y(y)
            , z(z)
            , w(0.0f) {
                
            }
            
            explicit sv_inline FVec3(f32 v)
            : x(v)
            , y(v)
            , z(v)
            , w(0.0f) {
                
            }
            
            explicit sv_inline FVec3(const FVec2 &v)
            : x(v.x)
            , y(v.y)
            , z(0.0f)
            , w(0.0f) {
                
            }
            
            explicit sv_inline FVec3(const FVec4 &v);
            
            explicit sv_inline FVec3(const f32 *v)
            : x(v[0])
            , y(v[1])
            , z(v[2])
            , w(0.0f) {
                
            }
            
            explicit sv_inline FVec3(const DVec3 &v);
            
            explicit sv_inline FVec3(const HVec3 &v);
            
            explicit sv_inline FVec3(const IVec3 &v);
#ifdef USE_SSE
            sv_inline FVec3(const FVec3 &v)
            : vec(v.vec) {
                
            }
            explicit sv_inline FVec3(__m128 v)
            : vec(v) {
                
            }
#elif USE_ALTIVEC
            sv_inline FVec3(const FVec3 &v)
            : vec(v.vec) {
                
            }
            
            explicit sv_inline FVec3(vec_float4 v)
            : vec(v) {
                
            }
            
#elif USE_NEON
            sv_inline FVec3(const FVec3 &v)
            :vec(v.vec) {
                
            }
            explicit sv_inline FVec3(float32x4_t v)
            : vec(v) {
                
            }
#else
            sv_inline FVec3(const FVec3 &v)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(0.0f) {
                
            }
#endif
            
            sv_inline FVec3 &operator=(const FVec3 &v) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                vec = v.vec;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
                return *this;
            }
            
            sv_inline FVec3 operator-() const {
                return FVec3(-x,-y,-z);
            }
            
            sv_inline FVec3 &operator*=(f32 v) {
                x *= v; y *= v; z *= v;
                return *this;
            }
            
            sv_inline FVec3 &operator*=(const FVec3 &v) {
                x *= v.x; y *= v.y; z *= v.z;
                return *this;
            }
            
            sv_inline FVec3 &operator/=(f32 v) {
                f32 iv = Math::rcp(v);
                x *= iv; y *= iv; z *= iv;
                return *this;
            }
            
            sv_inline FVec3 &operator/=(const FVec3 &v) {
                x /= v.x; y /= v.y; z /= v.z;
                return *this;
            }
            
            sv_inline FVec3 &operator+=(const FVec3 &v) {
                x += v.x; y += v.y; z += v.z;
                return *this;
            }
            sv_inline FVec3 &operator-=(const FVec3 &v) {
                x -= v.x; y -= v.y; z -= v.z;
                return *this;
            }
            
            sv_inline operator f32*() { return v; }
            
            sv_inline operator const f32*() const { return v; }
            
            sv_inline operator void*() { return v; }
            
            sv_inline operator const void*() const { return v; }
            
            sv_inline f32 &operator[](s32 i) {
                assert((u32)i < 3 && "FVec3::operator[](): bad index");
                return v[i];
            }
            sv_inline f32 operator[](s32 i) const {
                assert((u32)i < 3 && "FVec3::operator[](): bad index");
                return v[i];
            }
            
            sv_inline void set(f32 v) {
                x = v; y = v; z = v;
            }
            
            sv_inline void set(f32 x_,f32 y_,f32 z_) {
                x = x_; y = y_; z = z_;
            }
            
            sv_inline void set(const FVec2 &v,f32 z_ = 0.0f) {
                x = v.x; y = v.y; z = z_;
            }
            
            sv_inline void set(const FVec3 &v) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                vec = v.vec;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
            }
            
            sv_inline void set(const f32 *v) {
                x = v[0]; y = v[1]; z = v[2];
            }
            
            sv_inline void get(f32 *v) const {
                v[0] = x; v[1] = y; v[2] = z;
            }
            
            sv_inline f32 *get() { return v; }
            
            sv_inline const f32 *get() const { return v; }
            
            sv_inline f32 length2() const {
                return x * x + y * y + z * z;
            }
            
            sv_inline f32 length() const {
                return Math::sqrt(x * x + y * y + z * z);
            }
            
            sv_inline FVec3 &normalize() {
                f32 ilength = Math::rsqrt(x * x + y * y + z * z);
                x *= ilength; y *= ilength; z *= ilength;
                return *this;
            }
            
            sv_inline FVec3 &normalizeFast() {
                f32 ilength = Math::rsqrtFast(x * x + y * y + z * z);
                x *= ilength; y *= ilength; z *= ilength;
                return *this;
            }
            
            union {
                struct {
                    f32 x,y,z,w;
                };
                f32 v[4];
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
        extern const FVec3 FVec3_zero;
        extern const FVec3 FVec3_one;
        extern const FVec3 FVec3_epsilon;
        extern const FVec3 FVec3_infinity;
        
        //
        sv_inline FVec2::FVec2(const FVec3 &v) : x(v.x), y(v.y) { }
        
        //
        sv_inline s32 operator==(const FVec3 &v0,const FVec3 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z));
        }
        
        sv_inline s32 operator!=(const FVec3 &v0,const FVec3 &v1) {
            return !(compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z));
        }
        
        sv_inline FVec3 operator*(const FVec3 &v0,f32 v1) {
            return FVec3(v0.x * v1,v0.y * v1,v0.z * v1);
        }
        
        sv_inline FVec3 operator*(const FVec3 &v0,const FVec3 &v1) {
            return FVec3(v0.x * v1.x,v0.y * v1.y,v0.z * v1.z);
        }
        
        sv_inline FVec3 operator/(const FVec3 &v0,f32 v1) {
            f32 iv1 = Math::rcp(v1);
            return FVec3(v0.x * iv1,v0.y * iv1,v0.z * iv1);
        }
        
        sv_inline FVec3 operator/(const FVec3 &v0,const FVec3 &v1) {
            return FVec3(v0.x / v1.x,v0.y / v1.y,v0.z / v1.z);
        }
        
        sv_inline FVec3 operator+(const FVec3 &v0,const FVec3 &v1) {
            return FVec3(v0.x + v1.x,v0.y + v1.y,v0.z + v1.z);
        }
        
        sv_inline FVec3 operator-(const FVec3 &v0,const FVec3 &v1) {
            return FVec3(v0.x - v1.x,v0.y - v1.y,v0.z - v1.z);
        }
        
        //
        sv_inline s32 compare(const FVec3 &v0,const FVec3 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z));
        }
        
        sv_inline s32 compare(const FVec3 &v0,const FVec3 &v1,f32 epsilon) {
            return (compare(v0.x,v1.x,epsilon) && compare(v0.y,v1.y,epsilon) && compare(v0.z,v1.z,epsilon));
        }
        
        sv_inline f32 dot(const FVec2 &v0,const FVec3 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v1.z;
        }
        
        sv_inline f32 dot(const FVec3 &v0,const FVec2 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z;
        }
        
        sv_inline f32 dot(const FVec3 &v0,const FVec3 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }
        
        sv_inline FVec3 &mul(FVec3 &ret,const FVec3 &v0,f32 v1) {
            ret.x = v0.x * v1;
            ret.y = v0.y * v1;
            ret.z = v0.z * v1;
            return ret;
        }
        
        sv_inline FVec3 &mul(FVec3 &ret,const FVec3 &v0,const FVec3 &v1) {
            ret.x = v0.x * v1.x;
            ret.y = v0.y * v1.y;
            ret.z = v0.z * v1.z;
            return ret;
        }
        
        sv_inline FVec3 &mad(FVec3 &ret,const FVec3 &v0,f32 v1,const FVec3 &v2) {
            ret.x = v0.x * v1 + v2.x;
            ret.y = v0.y * v1 + v2.y;
            ret.z = v0.z * v1 + v2.z;
            return ret;
        }
        
        sv_inline FVec3 &mad(FVec3 &ret,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2) {
            ret.x = v0.x * v1.x + v2.x;
            ret.y = v0.y * v1.y + v2.y;
            ret.z = v0.z * v1.z + v2.z;
            return ret;
        }
        
        sv_inline FVec3 &add(FVec3 &ret,const FVec3 &v0,const FVec3 &v1) {
            ret.x = v0.x + v1.x;
            ret.y = v0.y + v1.y;
            ret.z = v0.z + v1.z;
            return ret;
        }
        
        sv_inline FVec3 &sub(FVec3 &ret,const FVec3 &v0,const FVec3 &v1) {
            ret.x = v0.x - v1.x;
            ret.y = v0.y - v1.y;
            ret.z = v0.z - v1.z;
            return ret;
        }
        
        sv_inline FVec3 &lerp(FVec3 &ret,const FVec3 &v0,const FVec3 &v1,f32 k) {
            ret.x = lerp(v0.x,v1.x,k);
            ret.y = lerp(v0.y,v1.y,k);
            ret.z = lerp(v0.z,v1.z,k);
            return ret;
        }
        
        sv_inline FVec3 &cross(FVec3 &ret,const FVec3 &v0,const FVec3 &v1) {
            ret.x = v0.y * v1.z - v0.z * v1.y;
            ret.y = v0.z * v1.x - v0.x * v1.z;
            ret.z = v0.x * v1.y - v0.y * v1.x;
            return ret;
        }
        
        sv_inline FVec3 &reflect(FVec3 &ret,const FVec3 &v0,const FVec3 &v1) {
            f32 k = dot(v0,v1) * 2.0f;
            ret.x = v0.x - v1.x * k;
            ret.y = v0.y - v1.y * k;
            ret.z = v0.z - v1.z * k;
            return ret;
        }
        
        //
        sv_inline f32 length(const FVec3 &v) {
            return v.length();
        }
        
        sv_inline f32 length2(const FVec3 &v) {
            return v.length2();
        }
        
        sv_inline FVec3 normalize(const FVec3 &v) {
            FVec3 ret = v;
            return ret.normalize();
        }
        
        sv_inline FVec3 cross(const FVec3 &v0,const FVec3 &v1) {
            FVec3 ret;
            return cross(ret,v0,v1);
        }
        
        sv_inline FVec3 reflect(const FVec3 &v0,const FVec3 &v1) {
            FVec3 ret;
            return reflect(ret,v0,v1);
        }
        // Orthogonal vectors.
        sv_inline FVec3 orthogonal(FVec3 &ret, const FVec3& v) {
            if (fabs(v.x) > fabs(v.z)) {
                ret.x = -v.y;
                ret.y = -v.x;
                ret.z = 0.0f;
            }else{
                ret.x = 0.0f;
                ret.y = -v.z;
                ret.z = v.y;
            }
            return ret;
        }
        
        sv_inline FVec3 orthonormalize(const FVec3& _a, const FVec3& _b) {
            return normalize(_a - _b * dot(_a, _b));
        }
        
        // Vector angles and slerps.
        template <typename T>
        sv_inline T angle(const FVec3& a, const FVec3& b) {
            return acos(clamp(dot(normalize(a), normalize(b)), -1.0f, 1.0f));
        }
        
        //
        FVec3 min(const FVec3 &v0,const FVec3 &v1);
        FVec3 max(const FVec3 &v0,const FVec3 &v1);
        FVec3 clamp(const FVec3 &v,const FVec3 &v0,const FVec3 &v1);
        FVec3 saturate(const FVec3 &v);
        FVec3 lerp(const FVec3 &v0,const FVec3 &v1,f32 k);
        

        /******************************************************************************\
         *
         * DVec3
         *
         \******************************************************************************/
        
        //
        ATTRIBUTE_ALIGNED16(struct) DVec3 {
            
            sv_inline DVec3() { }
            sv_inline DVec3(const DVec3 &v) : x(v.x), y(v.y), z(v.z), w(0.0) { }
            sv_inline DVec3(const DVec2 &v,f64 z) : x(v.x), y(v.y), z(z), w(0.0) { }
            sv_inline DVec3(f64 x,f64 y,f64 z) : x(x), y(y), z(z), w(0.0) { }
            explicit sv_inline DVec3(f64 v) : x(v), y(v), z(v), w(0.0) { }
            explicit sv_inline DVec3(const DVec2 &v) : x(v.x), y(v.y), z(0.0), w(0.0) { }
            explicit sv_inline DVec3(const DVec4 &v);
            explicit sv_inline DVec3(const f64 *v) : x(v[0]), y(v[1]), z(v[2]), w(0.0) { }
            explicit sv_inline DVec3(const FVec3 &v) : x(v.x), y(v.y), z(v.z), w(0.0) { }
            explicit sv_inline DVec3(const HVec3 &v);
            explicit sv_inline DVec3(const IVec3 &v);
            
            sv_inline DVec3 &operator=(const DVec3 &v) {
#ifdef USE_SSE2
                vec0 = v.vec0; vec1 = v.vec1;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
                return *this;
            }
            sv_inline DVec3 operator-() const {
                return DVec3(-x,-y,-z);
            }
            sv_inline DVec3 &operator*=(f64 v) {
                x *= v; y *= v; z *= v;
                return *this;
            }
            sv_inline DVec3 &operator*=(const DVec3 &v) {
                x *= v.x; y *= v.y; z *= v.z;
                return *this;
            }
            sv_inline DVec3 &operator/=(f64 v) {
                f64 iv = Math::rcp(v);
                x *= iv; y *= iv; z *= iv;
                return *this;
            }
            sv_inline DVec3 &operator/=(const DVec3 &v) {
                x /= v.x; y /= v.y; z /= v.z;
                return *this;
            }
            sv_inline DVec3 &operator+=(const DVec3 &v) {
                x += v.x; y += v.y; z += v.z;
                return *this;
            }
            sv_inline DVec3 &operator-=(const DVec3 &v) {
                x -= v.x; y -= v.y; z -= v.z;
                return *this;
            }
            
            sv_inline operator f64*() { return v; }
            sv_inline operator const f64*() const { return v; }
            sv_inline operator void*() { return v; }
            sv_inline operator const void*() const { return v; }
            
            sv_inline f64 &operator[](s32 i) {
                assert((u32)i < 3 && "DVec3::operator[](): bad index");
                return v[i];
            }
            sv_inline f64 operator[](s32 i) const {
                assert((u32)i < 3 && "DVec3::operator[](): bad index");
                return v[i];
            }
            
            sv_inline void set(f64 v) {
                x = v; y = v; z = v;
            }
            sv_inline void set(f64 x_,f64 y_,f64 z_) {
                x = x_; y = y_; z = z_;
            }
            sv_inline void set(const DVec2 &v,f64 z_ = 0.0) {
                x = v.x; y = v.y; z = z_;
            }
            sv_inline void set(const DVec3 &v) {
#ifdef USE_SSE2
                vec0 = v.vec0; vec1 = v.vec1;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
            }
            sv_inline void set(const f64 *v) {
                x = v[0]; y = v[1]; z = v[2];
            }
            sv_inline void get(f64 *v) const {
                v[0] = x; v[1] = y; v[2] = z;
            }
            sv_inline f64 *get() { return v; }
            sv_inline const f64 *get() const { return v; }
            
            sv_inline f64 length() const {
                return Math::sqrt(x * x + y * y + z * z);
            }
            sv_inline f64 length2() const {
                return x * x + y * y + z * z;
            }
            sv_inline DVec3 &normalize() {
                f64 ilength = Math::rsqrt(x * x + y * y + z * z);
                x *= ilength; y *= ilength; z *= ilength;
                return *this;
            }
            
            union {
                struct {
                    f64 x,y,z,w;
                };
                f64 v[4];
#ifdef USE_SSE2
                struct {
                    __m128d vec0;
                    __m128d vec1;
                };
#endif
            };
        };
        
        //
        extern const DVec3 DVec3_zero;
        extern const DVec3 DVec3_one;
        extern const DVec3 DVec3_epsilon;
        extern const DVec3 DVec3_infinity;
        
        //
        sv_inline DVec2::DVec2(const DVec3 &v) : x(v.x), y(v.y) { }
        sv_inline FVec3::FVec3(const DVec3 &v) : x((f32)v.x), y((f32)v.y), z((f32)v.z), w(0.0f) { }
        
        //
        sv_inline s32 operator==(const DVec3 &v0,const DVec3 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z));
        }
        
        sv_inline s32 operator!=(const DVec3 &v0,const DVec3 &v1) {
            return !(compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z));
        }
        
        sv_inline DVec3 operator*(const DVec3 &v0,f64 v1) {
            return DVec3(v0.x * v1,v0.y * v1,v0.z * v1);
        }
        
        sv_inline DVec3 operator*(const DVec3 &v0,const DVec3 &v1) {
            return DVec3(v0.x * v1.x,v0.y * v1.y,v0.z * v1.z);
        }
        
        sv_inline DVec3 operator/(const DVec3 &v0,f64 v1) {
            f64 iv1 = Math::rcp(v1);
            return DVec3(v0.x * iv1,v0.y * iv1,v0.z * iv1);
        }
        
        sv_inline DVec3 operator/(const DVec3 &v0,const DVec3 &v1) {
            return DVec3(v0.x / v1.x,v0.y / v1.y,v0.z / v1.z);
        }
        
        sv_inline DVec3 operator+(const DVec3 &v0,const DVec3 &v1) {
            return DVec3(v0.x + v1.x,v0.y + v1.y,v0.z + v1.z);
        }
        
        sv_inline DVec3 operator-(const DVec3 &v0,const DVec3 &v1) {
            return DVec3(v0.x - v1.x,v0.y - v1.y,v0.z - v1.z);
        }
        
        //
        sv_inline s32 compare(const DVec3 &v0,const DVec3 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z));
        }
        
        sv_inline s32 compare(const DVec3 &v0,const DVec3 &v1,f64 epsilon) {
            return (compare(v0.x,v1.x,epsilon) && compare(v0.y,v1.y,epsilon) && compare(v0.z,v1.z,epsilon));
        }
        
        sv_inline f64 dot(const DVec2 &v0,const DVec3 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v1.z;
        }
        
        sv_inline f64 dot(const DVec3 &v0,const DVec2 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z;
        }
        
        sv_inline f64 dot(const DVec3 &v0,const DVec3 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }
        
        sv_inline DVec3 &mul(DVec3 &ret,const DVec3 &v0,f64 v1) {
            ret.x = v0.x * v1;
            ret.y = v0.y * v1;
            ret.z = v0.z * v1;
            return ret;
        }
        
        sv_inline DVec3 &mul(DVec3 &ret,const DVec3 &v0,const DVec3 &v1) {
            ret.x = v0.x * v1.x;
            ret.y = v0.y * v1.y;
            ret.z = v0.z * v1.z;
            return ret;
        }
        
        sv_inline DVec3 &mad(DVec3 &ret,const DVec3 &v0,f64 v1,const DVec3 &v2) {
            ret.x = v0.x * v1 + v2.x;
            ret.y = v0.y * v1 + v2.y;
            ret.z = v0.z * v1 + v2.z;
            return ret;
        }
        
        sv_inline DVec3 &mad(DVec3 &ret,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2) {
            ret.x = v0.x * v1.x + v2.x;
            ret.y = v0.y * v1.y + v2.y;
            ret.z = v0.z * v1.z + v2.z;
            return ret;
        }
        
        sv_inline FVec3 &add(FVec3 &ret,const DVec3 &v0,const DVec3 &v1) {
            ret.x = (f32)(v0.x + v1.x);
            ret.y = (f32)(v0.y + v1.y);
            ret.z = (f32)(v0.z + v1.z);
            return ret;
        }
        
        sv_inline DVec3 &add(DVec3 &ret,const DVec3 &v0,const DVec3 &v1) {
            ret.x = v0.x + v1.x;
            ret.y = v0.y + v1.y;
            ret.z = v0.z + v1.z;
            return ret;
        }
        
        sv_inline FVec3 &sub(FVec3 &ret,const DVec3 &v0,const DVec3 &v1) {
            ret.x = (f32)(v0.x - v1.x);
            ret.y = (f32)(v0.y - v1.y);
            ret.z = (f32)(v0.z - v1.z);
            return ret;
        }
        
        sv_inline DVec3 &sub(DVec3 &ret,const DVec3 &v0,const DVec3 &v1) {
            ret.x = v0.x - v1.x;
            ret.y = v0.y - v1.y;
            ret.z = v0.z - v1.z;
            return ret;
        }
        
        sv_inline DVec3 &lerp(DVec3 &ret,const DVec3 &v0,const DVec3 &v1,f64 k) {
            ret.x = lerp(v0.x,v1.x,k);
            ret.y = lerp(v0.y,v1.y,k);
            ret.z = lerp(v0.z,v1.z,k);
            return ret;
        }
        
        sv_inline DVec3 &cross(DVec3 &ret,const DVec3 &v0,const DVec3 &v1) {
            ret.x = v0.y * v1.z - v0.z * v1.y;
            ret.y = v0.z * v1.x - v0.x * v1.z;
            ret.z = v0.x * v1.y - v0.y * v1.x;
            return ret;
        }
        
        sv_inline DVec3 &reflect(DVec3 &ret,const DVec3 &v0,const DVec3 &v1) {
            f64 k = 2.0 * (v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
            ret.x = v0.x - v1.x * k;
            ret.y = v0.y - v1.y * k;
            ret.z = v0.z - v1.z * k;
            return ret;
        }
        
        //
        sv_inline f64 length(const DVec3 &v) {
            return v.length();
        }
        
        sv_inline f64 length2(const DVec3 &v) {
            return v.length2();
        }
        
        sv_inline DVec3 normalize(const DVec3 &v) {
            DVec3 ret = v;
            return ret.normalize();
        }
        
        sv_inline DVec3 cross(const DVec3 &v0,const DVec3 &v1) {
            DVec3 ret;
            return cross(ret,v0,v1);
        }
        
        sv_inline DVec3 reflect(const DVec3 &v0,const DVec3 &v1) {
            DVec3 ret;
            return reflect(ret,v0,v1);
        }
        
        //
        DVec3 min(const DVec3 &v0,const DVec3 &v1);
        DVec3 max(const DVec3 &v0,const DVec3 &v1);
        DVec3 clamp(const DVec3 &v,const DVec3 &v0,const DVec3 &v1);
        DVec3 saturate(const DVec3 &v);
        DVec3 lerp(const DVec3 &v0,const DVec3 &v1,f64 k);
        
        /******************************************************************************\
         *
         * HVec3
         *
         \******************************************************************************/
        
        //
        ATTRIBUTE_ALIGNED8(struct) HVec3 {
            
            sv_inline HVec3() { }
            sv_inline HVec3(const HVec3 &v) : x(v.x), y(v.y), z(v.z) { }
            sv_inline HVec3(SVHalf x,SVHalf y,SVHalf z) : x(x), y(y), z(z) { }
            explicit sv_inline HVec3(SVHalf v) : x(v), y(v), z(v) { }
            explicit sv_inline HVec3(f32 v) : x(v), y(v), z(v) { }
            explicit sv_inline HVec3(const FVec3 &v) : x(v.x), y(v.y), z(v.z) { }
            explicit sv_inline HVec3(const DVec3 &v) : x((f32)v.x), y((f32)v.y), z((f32)v.z) { }
            
            sv_inline HVec3 &operator=(const HVec3 &v) {
                x = v.x; y = v.y; z = v.z; w = v.w;
                return *this;
            }
            
            sv_inline operator SVHalf*() { return &x; }
            sv_inline operator const SVHalf*() const { return &x; }
            sv_inline operator void*() { return &x; }
            sv_inline operator const void*() const { return &x; }
            
            sv_inline SVHalf &operator[](s32 i) {
                assert((u32)i < 3 && "HVec3::operator[](): bad index");
                return (&x)[i];
            }
            sv_inline SVHalf operator[](s32 i) const {
                assert((u32)i < 3 && "HVec3::operator[](): bad index");
                return (&x)[i];
            }
            
            SVHalf x,y,z,w;
        };
        
        //
        extern const HVec3 HVec3_zero;
        extern const HVec3 HVec3_one;
        
        //
        sv_inline FVec3::FVec3(const HVec3 &v) : x(v.x.getFloat()), y(v.y.getFloat()), z(v.z.getFloat()), w(0.0f) { }
        sv_inline DVec3::DVec3(const HVec3 &v) : x(v.x.getFloat()), y(v.y.getFloat()), z(v.z.getFloat()), w(0.0) { }
        

        /******************************************************************************\
         *
         * IVec3
         *
         \******************************************************************************/
        
        //
        ATTRIBUTE_ALIGNED16(struct) IVec3 {
            
            sv_inline IVec3() { }
            sv_inline IVec3(s32 x,s32 y,s32 z) : x(x), y(y), z(z), w(0) { }
            explicit sv_inline IVec3(s32 v) : x(v), y(v), z(v), w(0) { }
            explicit sv_inline IVec3(const s32 *v) : x(v[0]), y(v[1]), z(v[2]) { }
            explicit sv_inline IVec3(const FVec3 &v) : x(Math::ftoi(v.x)), y(Math::ftoi(v.y)), z(Math::ftoi(v.z)) { }
            explicit sv_inline IVec3(const DVec3 &v) : x(Math::dtoi(v.x)), y(Math::dtoi(v.y)), z(Math::dtoi(v.z)) { }
#ifdef USE_SSE2
            sv_inline IVec3(const IVec3 &v) : vec(v.vec) { }
            explicit sv_inline IVec3(__m128i v) : vec(v) { }
#elif USE_SSE
            sv_inline IVec3(const IVec3 &v) : vec(v.vec) { }
            explicit sv_inline IVec3(__m128 v) : vec(v) { }
#elif USE_ALTIVEC
            sv_inline IVec3(const IVec3 &v) : vec(v.vec) { }
            explicit sv_inline IVec3(vec_int4 v) : vec(v) { }
#elif USE_NEON
            sv_inline IVec3(const IVec3 &v) : vec(v.vec) { }
            explicit sv_inline IVec3(int32x4_t v) : vec(v) { }
#else
            sv_inline IVec3(const IVec3 &v) : x(v.x), y(v.y), z(v.z), w(0) { }
#endif
            
            sv_inline IVec3 &operator=(const IVec3 &v) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                vec = v.vec;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
                return *this;
            }
            sv_inline IVec3 operator-() const {
                return IVec3(-x,-y,-z);
            }
            sv_inline IVec3 &operator*=(s32 v) {
                x *= v; y *= v; z *= v;
                return *this;
            }
            sv_inline IVec3 &operator*=(const IVec3 &v) {
                x *= v.x; y *= v.y; z *= v.z;
                return *this;
            }
            sv_inline IVec3 &operator/=(s32 v) {
                x /= v; y /= v; z /= v;
                return *this;
            }
            sv_inline IVec3 &operator/=(const IVec3 &v) {
                x /= v.x; y /= v.y; z /= v.z;
                return *this;
            }
            sv_inline IVec3 &operator+=(const IVec3 &v) {
                x += v.x; y += v.y; z += v.z;
                return *this;
            }
            sv_inline IVec3 &operator-=(const IVec3 &v) {
                x -= v.x; y -= v.y; z -= v.z;
                return *this;
            }
            sv_inline IVec3 &operator<<=(s32 v) {
                x <<= v; y <<= v; z <<= v;
                return *this;
            }
            sv_inline IVec3 &operator>>=(s32 v) {
                x >>= v; y >>= v; z >>= v;
                return *this;
            }
            
            sv_inline operator s32*() { return v; }
            sv_inline operator const s32*() const { return v; }
            sv_inline operator void*() { return v; }
            sv_inline operator const void*() const { return v; }
            
            sv_inline s32 &operator[](s32 i) {
                assert((u32)i < 3 && "IVec3::operator[](): bad index");
                return v[i];
            }
            sv_inline s32 operator[](s32 i) const {
                assert((u32)i < 3 && "IVec3::operator[](): bad index");
                return v[i];
            }
            
            sv_inline void set(s32 v) {
                x = v; y = v; z = v;
            }
            sv_inline void set(s32 x_,s32 y_,s32 z_) {
                x = x_; y = y_; z = z_;
            }
            sv_inline void set(const IVec3 &v) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                vec = v.vec;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
            }
            sv_inline void set(const s32 *v) {
                x = v[0]; y = v[1]; z = v[2];
            }
            sv_inline void get(s32 *v) const {
                v[0] = x; v[1] = y; v[2] = z;
            }
            sv_inline s32 *get() { return v; }
            sv_inline const s32 *get() const { return v; }
            
            sv_inline s32 length2() const {
                return x * x + y * y + z * z;
            }
            
            union {
                struct {
                    s32 x,y,z,w;
                };
                s32 v[4];
#ifdef USE_SSE2
                __m128i vec;
#elif USE_SSE
                __m128 vec;
#elif USE_ALTIVEC
                vec_int4 vec;
#elif USE_NEON
                int32x4_t vec;
#endif
            };
        };
        
        //
        extern const IVec3 IVec3_zero;
        extern const IVec3 IVec3_one;
        
        //
        sv_inline FVec3::FVec3(const IVec3 &v) : x(Math::itof(v.x)), y(Math::itof(v.y)), z(Math::itof(v.z)), w(0.0f) { }
        sv_inline DVec3::DVec3(const IVec3 &v) : x(Math::itod(v.x)), y(Math::itod(v.y)), z(Math::itod(v.z)), w(0.0) { }
        
        //
        sv_inline s32 operator==(const IVec3 &v0,const IVec3 &v1) {
            return (v0.x == v1.x && v0.y == v1.y && v0.z == v1.z);
        }
        
        sv_inline s32 operator!=(const IVec3 &v0,const IVec3 &v1) {
            return (v0.x != v1.x || v0.y != v1.y || v0.z != v1.z);
        }
        
        sv_inline IVec3 operator*(const IVec3 &v0,s32 v1) {
            return IVec3(v0.x * v1,v0.y * v1,v0.z * v1);
        }
        
        sv_inline IVec3 operator*(const IVec3 &v0,const IVec3 &v1) {
            return IVec3(v0.x * v1.x,v0.y * v1.y,v0.z * v1.z);
        }
        
        sv_inline IVec3 operator/(const IVec3 &v0,s32 v1) {
            return IVec3(v0.x / v1,v0.y / v1,v0.z / v1);
        }
        
        sv_inline IVec3 operator/(const IVec3 &v0,const IVec3 &v1) {
            return IVec3(v0.x / v1.x,v0.y / v1.y,v0.z / v1.z);
        }
        
        sv_inline IVec3 operator+(const IVec3 &v0,const IVec3 &v1) {
            return IVec3(v0.x + v1.x,v0.y + v1.y,v0.z + v1.z);
        }
        
        sv_inline IVec3 operator-(const IVec3 &v0,const IVec3 &v1) {
            return IVec3(v0.x - v1.x,v0.y - v1.y,v0.z - v1.z);
        }
        
        sv_inline IVec3 operator<<(const IVec3 &v0,s32 v1) {
            return IVec3(v0.x << v1,v0.y << v1,v0.z << v1);
        }
        
        sv_inline IVec3 operator>>(const IVec3 &v0,s32 v1) {
            return IVec3(v0.x >> v1,v0.y >> v1,v0.z >> v1);
        }
        
        //
        sv_inline s32 dot(const IVec3 &v0,const IVec3 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }
        
        sv_inline IVec3 &lerp(IVec3 &ret,const IVec3 &v0,const IVec3 &v1,s32 k) {
            ret.x = lerp(v0.x,v1.x,k);
            ret.y = lerp(v0.y,v1.y,k);
            ret.z = lerp(v0.z,v1.z,k);
            return ret;
        }
        
        sv_inline IVec3 &cross(IVec3 &ret,const IVec3 &v0,const IVec3 &v1) {
            ret.x = v0.y * v1.z - v0.z * v1.y;
            ret.y = v0.z * v1.x - v0.x * v1.z;
            ret.z = v0.x * v1.y - v0.y * v1.x;
            return ret;
        }
        
        //
        sv_inline s32 length2(const IVec3 &v) {
            return v.length2();
        }
        
        sv_inline IVec3 cross(const IVec3 &v0,const IVec3 &v1) {
            IVec3 ret;
            return cross(ret,v0,v1);
        }
        
        //
        IVec3 min(const IVec3 &v0,const IVec3 &v1);
        IVec3 max(const IVec3 &v0,const IVec3 &v1);
        IVec3 clamp(const IVec3 &v,const IVec3 &v0,const IVec3 &v1);
        IVec3 lerp(const IVec3 &v0,const IVec3 &v1,s32 k);

    }//!namespace util
    
}//!namespace sv


#endif /* __aVEC3_H__ */
