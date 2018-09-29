#ifndef __aVEC4_H__
#define __aVEC4_H__

#include "SVVec3.h"

namespace sv {
    
    namespace util {

        //****************************** FVec4 ********************************************
        ATTRIBUTE_ALIGNED16(struct) FVec4 {
            sv_inline FVec4() {
            }
            
            sv_inline FVec4(const FVec3 &v,f32 w)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(w) {
            }
            
            sv_inline FVec4(const FVec2 &v,f32 z,f32 w)
            : x(v.x)
            , y(v.y)
            , z(z)
            , w(w) {
            }
            
            sv_inline FVec4(f32 x,f32 y,f32 z,f32 w)
            : x(x)
            , y(y)
            , z(z)
            , w(w) {
            }
            
            explicit sv_inline FVec4(f32 v)
            : x(v)
            , y(v)
            , z(v)
            , w(v) {
            }
            
            explicit sv_inline FVec4(const FVec2 &v)
            : x(v.x)
            , y(v.y)
            , z(0.0f)
            , w(1.0f) {
            }
            
            explicit sv_inline FVec4(const FVec3 &v)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(1.0f) {
            }
            
            explicit sv_inline FVec4(const f32 *v)
            : x(v[0])
            , y(v[1])
            , z(v[2])
            , w(v[3]) {
            }
            
            explicit sv_inline FVec4(const DVec4 &v);
            
            explicit sv_inline FVec4(const HVec4 &v);
            
            explicit sv_inline FVec4(const IVec4 &v);
            
            explicit sv_inline FVec4(const BVec4 &v);
            
#ifdef USE_SSE
            sv_inline FVec4(const FVec4 &v)
            : vec(v.vec) {
            }
            
            explicit sv_inline FVec4(__m128 v)
            : vec(v) {
                
            }
#elif USE_ALTIVEC
            sv_inline FVec4(const FVec4 &v)
            : vec(v.vec) {
            }
            
            explicit sv_inline FVec4(vec_float4 v)
            : vec(v) {
            }
#elif USE_NEON
            sv_inline FVec4(const FVec4 &v)
            : vec(v.vec) {
            }
            
            explicit sv_inline FVec4(float32x4_t v)
            : vec(v) {
            }
#else
            sv_inline FVec4(const FVec4 &v)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(v.w) {
            }
#endif
            
            sv_inline FVec4 &operator=(const FVec4 &v) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                vec = v.vec;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
                return *this;
            }
            
            sv_inline FVec4 operator-() const {
                return FVec4(-x,-y,-z,-w);
            }
            
            sv_inline FVec4 &operator*=(f32 v) {
                x *= v; y *= v; z *= v; w *= v;
                return *this;
            }
            
            sv_inline FVec4 &operator*=(const FVec4 &v) {
                x *= v.x; y *= v.y; z *= v.z; w *= v.w;
                return *this;
            }
            
            sv_inline FVec4 &operator/=(f32 v) {
                f32 iv = Math::rcp(v);
                x *= iv; y *= iv; z *= iv; w *= iv;
                return *this;
            }
            
            sv_inline FVec4 &operator/=(const FVec4 &v) {
                x /= v.x; y /= v.y; z /= v.z; w /= v.w;
                return *this;
            }
            
            sv_inline FVec4 &operator+=(const FVec4 &v) {
                x += v.x; y += v.y; z += v.z; w += v.w;
                return *this;
            }
            
            sv_inline FVec4 &operator-=(const FVec4 &v) {
                x -= v.x; y -= v.y; z -= v.z; w -= v.w;
                return *this;
            }
            
            sv_inline operator f32*() {
                return v;
            }
            
            sv_inline operator const f32*() const {
                return v;
            }
            
            sv_inline operator void*() {
                return v;
            }
            
            sv_inline operator const void*() const {
                return v;
            }
            
            sv_inline f32 &operator[](s32 i) {
                assert((u32)i < 4 && "FVec4::operator[](): bad index");
                return v[i];
            }
            
            sv_inline f32 operator[](s32 i) const {
                assert((u32)i < 4 && "FVec4::operator[](): bad index");
                return v[i];
            }
            
            sv_inline void set(f32 v) {
                x = v; y = v; z = v; w = v;
            }
            
            sv_inline void set(f32 x_,f32 y_,f32 z_,f32 w_) {
                x = x_; y = y_; z = z_; w = w_;
            }
            
            sv_inline void set(const FVec2 &v,f32 z_ = 0.0f,f32 w_ = 1.0f) {
                x = v.x; y = v.y; z = z_; w = w_;
            }
            
            sv_inline void set(const FVec3 &v,f32 w_ = 1.0f) {
                x = v.x; y = v.y; z = v.z; w = w_;
            }
            
            sv_inline void set(const FVec4 &v) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                vec = v.vec;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
            }
            
            sv_inline void set(const f32 *v) {
                x = v[0]; y = v[1]; z = v[2]; w = v[3];
            }
            
            sv_inline void get(f32 *v) const {
                v[0] = x; v[1] = y; v[2] = z; v[3] = w;
            }
            
            sv_inline f32 *get() { return v; }
            
            sv_inline const f32 *get() const { return v; }
            
            sv_inline f32 length2() const {
                return x * x + y * y + z * z + w * w;
            }
            
            sv_inline f32 length() const {
                return Math::sqrt(x * x + y * y + z * z + w * w);
            }
            
            sv_inline FVec4 &normalize() {
                f32 ilength = Math::rsqrt(x * x + y * y + z * z + w * w);
                x *= ilength;
                y *= ilength;
                z *= ilength;
                w *= ilength;
                return *this;
            }
            
            sv_inline FVec4 &normalizeFast() {
                f32 ilength = Math::rsqrtFast(x * x + y * y + z * z + w * w);
                x *= ilength;
                y *= ilength;
                z *= ilength;
                w *= ilength;
                return *this;
            }
            
            sv_inline FVec4 &normalize3() {
                f32 ilength = Math::rsqrt(x * x + y * y + z * z);
                x *= ilength;
                y *= ilength;
                z *= ilength;
                return *this;
            }
            
            sv_inline FVec4 &normalizeFast3() {
                f32 ilength = Math::rsqrtFast(x * x + y * y + z * z);
                x *= ilength;
                y *= ilength;
                z *= ilength;
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
        extern const FVec4 FVec4_zero;
        extern const FVec4 FVec4_one;
        extern const FVec4 FVec4_epsilon;
        extern const FVec4 FVec4_infinity;
        //
        sv_inline FVec2::FVec2(const FVec4 &v)
        : x(v.x)
        , y(v.y) {
        }
        //
        sv_inline FVec3::FVec3(const FVec4 &v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(0.0f) {
        }
        //
        sv_inline s32 operator==(const FVec4 &v0,const FVec4 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z) && compare(v0.w,v1.w));
        }
        
        sv_inline s32 operator!=(const FVec4 &v0,const FVec4 &v1) {
            return !(compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z) && compare(v0.w,v1.w));
        }
        
        sv_inline FVec4 operator*(const FVec4 &v0,f32 v1) {
            return FVec4(v0.x * v1,v0.y * v1,v0.z * v1,v0.w * v1);
        }
        
        sv_inline FVec4 operator*(const FVec4 &v0,const FVec4 &v1) {
            return FVec4(v0.x * v1.x,v0.y * v1.y,v0.z * v1.z,v0.w * v1.w);
        }
        
        sv_inline FVec4 operator/(const FVec4 &v0,f32 v1) {
            f32 iv1 = Math::rcp(v1);
            return FVec4(v0.x * iv1,v0.y * iv1,v0.z * iv1,v0.w * iv1);
        }
        
        sv_inline FVec4 operator/(const FVec4 &v0,const FVec4 &v1) {
            return FVec4(v0.x / v1.x,v0.y / v1.y,v0.z / v1.z,v0.w / v1.w);
        }
        
        sv_inline FVec4 operator+(const FVec4 &v0,const FVec4 &v1) {
            return FVec4(v0.x + v1.x,v0.y + v1.y,v0.z + v1.z,v0.w + v1.w);
        }
        
        sv_inline FVec4 operator-(const FVec4 &v0,const FVec4 &v1) {
            return FVec4(v0.x - v1.x,v0.y - v1.y,v0.z - v1.z,v0.w - v1.w);
        }
        
        //
        sv_inline s32 compare(const FVec4 &v0,const FVec4 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z) && compare(v0.w,v1.w));
        }
        
        sv_inline s32 compare(const FVec4 &v0,const FVec4 &v1,f32 epsilon) {
            return (compare(v0.x,v1.x,epsilon) && compare(v0.y,v1.y,epsilon) && compare(v0.z,v1.z,epsilon) && compare(v0.w,v1.w,epsilon));
        }
        
        sv_inline f32 dot(const FVec3 &v0,const FVec4 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v1.w;
        }
        
        sv_inline f32 dot(const FVec4 &v0,const FVec3 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w;
        }
        
        sv_inline f32 dot(const FVec4 &v0,const FVec4 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
        }
        
        sv_inline f32 dot3(const FVec3 &v0,const FVec4 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }
        
        sv_inline f32 dot3(const FVec4 &v0,const FVec3 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }
        
        sv_inline f32 dot3(const FVec4 &v0,const FVec4 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }
        
        sv_inline FVec4 &mul(FVec4 &ret,const FVec4 &v0,f32 v1) {
            ret.x = v0.x * v1;
            ret.y = v0.y * v1;
            ret.z = v0.z * v1;
            ret.w = v0.w * v1;
            return ret;
        }
        
        sv_inline FVec4 &mul(FVec4 &ret,const FVec4 &v0,const FVec4 &v1) {
            ret.x = v0.x * v1.x;
            ret.y = v0.y * v1.y;
            ret.z = v0.z * v1.z;
            ret.w = v0.w * v1.w;
            return ret;
        }
        
        sv_inline FVec4 &mad(FVec4 &ret,const FVec4 &v0,f32 v1,const FVec4 &v2) {
            ret.x = v0.x * v1 + v2.x;
            ret.y = v0.y * v1 + v2.y;
            ret.z = v0.z * v1 + v2.z;
            ret.w = v0.w * v1 + v2.w;
            return ret;
        }
        
        sv_inline FVec4 &mad(FVec4 &ret,const FVec4 &v0,const FVec4 &v1,const FVec4 &v2) {
            ret.x = v0.x * v1.x + v2.x;
            ret.y = v0.y * v1.y + v2.y;
            ret.z = v0.z * v1.z + v2.z;
            ret.w = v0.w * v1.w + v2.w;
            return ret;
        }
        
        sv_inline FVec4 &add(FVec4 &ret,const FVec4 &v0,const FVec4 &v1) {
            ret.x = v0.x + v1.x;
            ret.y = v0.y + v1.y;
            ret.z = v0.z + v1.z;
            ret.w = v0.w + v1.w;
            return ret;
        }
        
        sv_inline FVec4 &sub(FVec4 &ret,const FVec4 &v0,const FVec4 &v1) {
            ret.x = v0.x - v1.x;
            ret.y = v0.y - v1.y;
            ret.z = v0.z - v1.z;
            ret.w = v0.w - v1.w;
            return ret;
        }
        
        sv_inline FVec4 &lerp(FVec4 &ret,const FVec4 &v0,const FVec4 &v1,f32 k) {
            ret.x = lerp(v0.x,v1.x,k);
            ret.y = lerp(v0.y,v1.y,k);
            ret.z = lerp(v0.z,v1.z,k);
            ret.w = lerp(v0.w,v1.w,k);
            return ret;
        }
        
        sv_inline FVec4 &cross(FVec4 &ret,const FVec3 &v0,const FVec3 &v1) {
            ret.x = v0.y * v1.z - v0.z * v1.y;
            ret.y = v0.z * v1.x - v0.x * v1.z;
            ret.z = v0.x * v1.y - v0.y * v1.x;
            return ret;
        }
        
        //
        sv_inline f32 length(const FVec4 &v) {
            return v.length();
        }
        
        sv_inline f32 length2(const FVec4 &v) {
            return v.length2();
        }
        
        sv_inline FVec4 normalize(const FVec4 &v) {
            FVec4 ret = v;
            return ret.normalize();
        }
        
        //
        FVec4 min(const FVec4 &v0,const FVec4 &v1);
        
        FVec4 max(const FVec4 &v0,const FVec4 &v1);
        
        FVec4 clamp(const FVec4 &v,const FVec4 &v0,const FVec4 &v1);
        
        FVec4 saturate(const FVec4 &v);
        
        FVec4 lerp(const FVec4 &v0,const FVec4 &v1,f32 k);
        
        //****************************** DVec4 ********************************************
        //
        ATTRIBUTE_ALIGNED16(struct) DVec4 {
            
            sv_inline DVec4() {
            }
            
            sv_inline DVec4(const DVec4 &v)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(v.w) {
            }
            
            sv_inline DVec4(const DVec3 &v,f64 w)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(w) {
            }
            
            sv_inline DVec4(const DVec2 &v,f64 z,f64 w)
            : x(v.x)
            , y(v.y)
            , z(z)
            , w(w) {
            }
            
            sv_inline DVec4(f64 x,f64 y,f64 z,f64 w)
            : x(x)
            , y(y)
            , z(z)
            , w(w) {
            }
            
            explicit sv_inline DVec4(f64 v)
            : x(v)
            , y(v)
            , z(v)
            , w(v) {
            }
            
            explicit sv_inline DVec4(const DVec2 &v)
            : x(v.x)
            , y(v.y)
            , z(0.0)
            , w(1.0) {
            }
            
            explicit sv_inline DVec4(const DVec3 &v)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(1.0) {
            }
            
            explicit sv_inline DVec4(const f64 *v)
            : x(v[0])
            , y(v[1])
            , z(v[2])
            , w(v[3]) {
            }
            
            explicit sv_inline DVec4(const FVec4 &v)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(v.w) {
            }
            
            explicit sv_inline DVec4(const HVec4 &v);
            explicit sv_inline DVec4(const IVec4 &v);
            explicit sv_inline DVec4(const BVec4 &v);
            
            sv_inline DVec4 &operator=(const DVec4 &v) {
#ifdef USE_SSE2
                vec0 = v.vec0; vec1 = v.vec1;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
                return *this;
            }
            
            sv_inline DVec4 operator-() const {
                return DVec4(-x,-y,-z,-w);
            }
            
            sv_inline DVec4 &operator*=(f64 v) {
                x *= v; y *= v; z *= v; w *= v;
                return *this;
            }
            
            sv_inline DVec4 &operator*=(const DVec4 &v) {
                x *= v.x; y *= v.y; z *= v.z; w *= v.w;
                return *this;
            }
            
            sv_inline DVec4 &operator/=(f64 v) {
                f64 iv = Math::rcp(v);
                x *= iv; y *= iv; z *= iv; w *= iv;
                return *this;
            }
            
            sv_inline DVec4 &operator/=(const DVec4 &v) {
                x /= v.x; y /= v.y; z /= v.z; w /= v.w;
                return *this;
            }
            
            sv_inline DVec4 &operator+=(const DVec4 &v) {
                x += v.x; y += v.y; z += v.z; w += v.w;
                return *this;
            }
            
            sv_inline DVec4 &operator-=(const DVec4 &v) {
                x -= v.x; y -= v.y; z -= v.z; w -= v.w;
                return *this;
            }
            
            sv_inline operator f64*() { return v; }
            
            sv_inline operator const f64*() const { return v; }
            
            sv_inline operator void*() { return v; }
            
            sv_inline operator const void*() const { return v; }
            
            sv_inline f64 &operator[](s32 i) {
                assert((u32)i < 4 && "DVec4::operator[](): bad index");
                return v[i];
            }
            
            sv_inline f64 operator[](s32 i) const {
                assert((u32)i < 4 && "DVec4::operator[](): bad index");
                return v[i];
            }
            
            sv_inline void set(f64 v) {
                x = v; y = v; z = v; w = v;
            }
            
            sv_inline void set(f64 x_,f64 y_,f64 z_,f64 w_) {
                x = x_; y = y_; z = z_; w = w_;
            }
            
            sv_inline void set(const DVec2 &v,f64 z_ = 0.0,f64 w_ = 1.0) {
                x = v.x; y = v.y; z = z_; w = w_;
            }
            
            sv_inline void set(const DVec3 &v,f64 w_ = 1.0) {
                x = v.x; y = v.y; z = v.z; w = w_;
            }
            
            sv_inline void set(const DVec4 &v) {
#ifdef USE_SSE2
                vec0 = v.vec0; vec1 = v.vec1;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
            }
            
            sv_inline void set(const f64 *v) {
                x = v[0]; y = v[1]; z = v[2]; w = v[3];
            }
            
            sv_inline void get(f64 *v) const {
                v[0] = x; v[1] = y; v[2] = z; v[3] = w;
            }
            
            sv_inline f64 *get() { return v; }
            
            sv_inline const f64 *get() const { return v; }
            
            sv_inline f64 length() const {
                return Math::sqrt(x * x + y * y + z * z + w * w);
            }
            
            sv_inline f64 length2() const {
                return x * x + y * y + z * z + w * w;
            }
            
            sv_inline DVec4 &normalize() {
                f64 ilength = Math::rsqrt(x * x + y * y + z * z + w * w);
                x *= ilength; y *= ilength; z *= ilength; w *= ilength;
                return *this;
            }
            
            sv_inline DVec4 &normalize3() {
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
        extern const DVec4 DVec4_zero;
        extern const DVec4 DVec4_one;
        extern const DVec4 DVec4_epsilon;
        extern const DVec4 DVec4_infinity;
        
        //
        sv_inline DVec2::DVec2(const DVec4 &v)
        : x(v.x)
        , y(v.y) {
        }
        
        sv_inline DVec3::DVec3(const DVec4 &v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(0.0) {
        }
        
        sv_inline FVec4::FVec4(const DVec4 &v)
        : x((f32)v.x)
        , y((f32)v.y)
        , z((f32)v.z)
        , w((f32)v.w) {
        }
        
        //
        sv_inline s32 operator==(const DVec4 &v0,const DVec4 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z) && compare(v0.w,v1.w));
        }
        
        sv_inline s32 operator!=(const DVec4 &v0,const DVec4 &v1) {
            return !(compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z) && compare(v0.w,v1.w));
        }
        
        sv_inline DVec4 operator*(const DVec4 &v0,f64 v1) {
            return DVec4(v0.x * v1,v0.y * v1,v0.z * v1,v0.w * v1);
        }
        
        sv_inline DVec4 operator*(const DVec4 &v0,const DVec4 &v1) {
            return DVec4(v0.x * v1.x,v0.y * v1.y,v0.z * v1.z,v0.w * v1.w);
        }
        
        sv_inline DVec4 operator/(const DVec4 &v0,f64 v1) {
            f64 iv1 = Math::rcp(v1);
            return DVec4(v0.x * iv1,v0.y * iv1,v0.z * iv1,v0.w * iv1);
        }
        
        sv_inline DVec4 operator/(const DVec4 &v0,const DVec4 &v1) {
            return DVec4(v0.x / v1.x,v0.y / v1.y,v0.z / v1.z,v0.w / v1.w);
        }
        
        sv_inline DVec4 operator+(const DVec4 &v0,const DVec4 &v1) {
            return DVec4(v0.x + v1.x,v0.y + v1.y,v0.z + v1.z,v0.w + v1.w);
        }
        
        sv_inline DVec4 operator-(const DVec4 &v0,const DVec4 &v1) {
            return DVec4(v0.x - v1.x,v0.y - v1.y,v0.z - v1.z,v0.w - v1.w);
        }
        
        //
        sv_inline s32 compare(const DVec4 &v0,const DVec4 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y) && compare(v0.z,v1.z) && compare(v0.w,v1.w));
        }
        
        sv_inline s32 compare(const DVec4 &v0,const DVec4 &v1,f64 epsilon) {
            return (compare(v0.x,v1.x,epsilon) && compare(v0.y,v1.y,epsilon) && compare(v0.z,v1.z,epsilon) && compare(v0.w,v1.w,epsilon));
        }
        
        sv_inline f64 dot(const DVec3 &v0,const DVec4 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v1.w;
        }
        
        sv_inline f64 dot(const DVec4 &v0,const DVec3 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w;
        }
        
        sv_inline f64 dot(const DVec4 &v0,const DVec4 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
        }
        
        sv_inline f64 dot3(const DVec3 &v0,const DVec4 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }
        
        sv_inline f64 dot3(const DVec4 &v0,const DVec3 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }
        
        sv_inline f64 dot3(const DVec4 &v0,const DVec4 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
        }
        
        sv_inline DVec4 &mul(DVec4 &ret,const DVec4 &v0,f64 v1) {
            ret.x = v0.x * v1;
            ret.y = v0.y * v1;
            ret.z = v0.z * v1;
            ret.w = v0.w * v1;
            return ret;
        }
        
        sv_inline DVec4 &mul(DVec4 &ret,const DVec4 &v0,const DVec4 &v1) {
            ret.x = v0.x * v1.x;
            ret.y = v0.y * v1.y;
            ret.z = v0.z * v1.z;
            ret.w = v0.w * v1.w;
            return ret;
        }
        
        sv_inline DVec4 &mad(DVec4 &ret,const DVec4 &v0,f64 v1,const DVec4 &v2) {
            ret.x = v0.x * v1 + v2.x;
            ret.y = v0.y * v1 + v2.y;
            ret.z = v0.z * v1 + v2.z;
            ret.w = v0.w * v1 + v2.w;
            return ret;
        }
        
        sv_inline DVec4 &mad(DVec4 &ret,const DVec4 &v0,const DVec4 &v1,const DVec4 &v2) {
            ret.x = v0.x * v1.x + v2.x;
            ret.y = v0.y * v1.y + v2.y;
            ret.z = v0.z * v1.z + v2.z;
            ret.w = v0.w * v1.w + v2.w;
            return ret;
        }
        
        sv_inline DVec4 &add(DVec4 &ret,const DVec4 &v0,const DVec4 &v1) {
            ret.x = v0.x + v1.x;
            ret.y = v0.y + v1.y;
            ret.z = v0.z + v1.z;
            ret.w = v0.w + v1.w;
            return ret;
        }
        
        sv_inline DVec4 &sub(DVec4 &ret,const DVec4 &v0,const DVec4 &v1) {
            ret.x = v0.x - v1.x;
            ret.y = v0.y - v1.y;
            ret.z = v0.z - v1.z;
            ret.w = v0.w - v1.w;
            return ret;
        }
        
        sv_inline DVec4 &lerp(DVec4 &ret,const DVec4 &v0,const DVec4 &v1,f64 k) {
            ret.x = lerp(v0.x,v1.x,k);
            ret.y = lerp(v0.y,v1.y,k);
            ret.z = lerp(v0.z,v1.z,k);
            ret.w = lerp(v0.w,v1.w,k);
            return ret;
        }
        
        sv_inline DVec4 &cross(DVec4 &ret,const DVec3 &v0,const DVec3 &v1) {
            ret.x = v0.y * v1.z - v0.z * v1.y;
            ret.y = v0.z * v1.x - v0.x * v1.z;
            ret.z = v0.x * v1.y - v0.y * v1.x;
            return ret;
        }

        sv_inline f64 length(const DVec4 &v) {
            return v.length();
        }
        
        sv_inline f64 length2(const DVec4 &v) {
            return v.length2();
        }
        
        sv_inline DVec4 normalize(const DVec4 &v) {
            DVec4 ret = v;
            return ret.normalize();
        }
        
        DVec4 min(const DVec4 &v0,const DVec4 &v1);
        
        DVec4 max(const DVec4 &v0,const DVec4 &v1);
        
        DVec4 clamp(const DVec4 &v,const DVec4 &v0,const DVec4 &v1);
        
        DVec4 saturate(const DVec4 &v);
        
        DVec4 lerp(const DVec4 &v0,const DVec4 &v1,f64 k);
        
        //****************************** HVec4 ********************************************
        ATTRIBUTE_ALIGNED8(struct) HVec4 {
            
            sv_inline HVec4() { }
            
            sv_inline HVec4(const HVec4 &v)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(v.w) {
            }
            
            //
            sv_inline HVec4(SVHalf x,SVHalf y,SVHalf z,SVHalf w)
            : x(x)
            , y(y)
            , z(z)
            , w(w) {
            }
            
            //
            explicit sv_inline HVec4(SVHalf v)
            : x(v)
            , y(v)
            , z(v)
            , w(v) {
            }
            
            //
            explicit sv_inline HVec4(f32 v)
            : x(v)
            , y(v)
            , z(v)
            , w(v) {
            }
            
            //
            explicit sv_inline HVec4(const FVec4 &v)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(v.w) {
            }
            
            //
            explicit sv_inline HVec4(const DVec4 &v)
            : x((f32)v.x)
            , y((f32)v.y)
            , z((f32)v.z)
            , w((f32)v.w) {
            }
            
            //
            sv_inline HVec4 &operator=(const HVec4 &v) {
                x = v.x;
                y = v.y;
                z = v.z;
                w = v.w;
                return *this;
            }
            
            sv_inline operator SVHalf*() { return &x; }
            
            sv_inline operator const SVHalf*() const { return &x; }
            
            sv_inline operator void*() { return &x; }
            
            sv_inline operator const void*() const { return &x; }
            
            sv_inline SVHalf &operator[](s32 i) {
                assert((u32)i < 4 && "HVec4::operator[](): bad index");
                return (&x)[i];
            }
            
            sv_inline SVHalf operator[](s32 i) const {
                assert((u32)i < 4 && "HVec4::operator[](): bad index");
                return (&x)[i];
            }
            
            SVHalf x,y,z,w;
        };
        //
        extern const HVec4 HVec4_zero;
        extern const HVec4 HVec4_one;
        //
        sv_inline FVec4::FVec4(const HVec4 &v)
        : x(v.x.getFloat())
        , y(v.y.getFloat())
        , z(v.z.getFloat())
        , w(v.w.getFloat()) {
        }
        
        //
        sv_inline DVec4::DVec4(const HVec4 &v)
        : x(v.x.getFloat())
        , y(v.y.getFloat())
        , z(v.z.getFloat())
        , w(v.w.getFloat()) {
        }
        
        //**********************************IVec4(int)*****************************************
        //
        ATTRIBUTE_ALIGNED16(struct) IVec4 {
            
            sv_inline IVec4() {
            }
            
            sv_inline IVec4(s32 x,s32 y,s32 z,s32 w)
            : x(x)
            , y(y)
            , z(z)
            , w(w) {
            }
            
            explicit sv_inline IVec4(s32 v)
            : x(v)
            , y(v)
            , z(v)
            , w(v) {
            }
            
            explicit sv_inline IVec4(const s32 *v)
            : x(v[0])
            , y(v[1])
            , z(v[2])
            , w(v[3]) {
            }
            
            explicit sv_inline IVec4(const FVec4 &v)
            : x(Math::ftoi(v.x))
            , y(Math::ftoi(v.y))
            , z(Math::ftoi(v.z))
            , w(Math::ftoi(v.w)) {
            }
            
            explicit sv_inline IVec4(const DVec4 &v)
            : x(Math::dtoi(v.x))
            , y(Math::dtoi(v.y))
            , z(Math::dtoi(v.z))
            , w(Math::dtoi(v.w)) {
            }
            
            explicit sv_inline IVec4(const BVec4 &v);
#ifdef USE_SSE2
            sv_inline IVec4(const IVec4 &v)
            : vec(v.vec) {
            }
            
            explicit sv_inline IVec4(__m128i v)
            : vec(v) {
            }
#elif USE_SSE
            sv_inline IVec4(const IVec4 &v)
            : vec(v.vec) {
            }
            
            explicit sv_inline IVec4(__m128 v)
            : vec(v) {
            }
#elif USE_ALTIVEC
            sv_inline IVec4(const IVec4 &v)
            : vec(v.vec) {
            }
            
            explicit sv_inline IVec4(vec_int4 v)
            : vec(v) {
            }
#elif USE_NEON
            sv_inline IVec4(const IVec4 &v)
            : vec(v.vec) {
            }
            
            explicit sv_inline IVec4(int32x4_t v)
            : vec(v) {
            }
#else
            sv_inline IVec4(const IVec4 &v)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(v.w) {
            }
#endif
            
            sv_inline IVec4 &operator=(const IVec4 &v) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                vec = v.vec;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
                return *this;
            }
            
            sv_inline IVec4 operator-() const {
                return IVec4(-x,-y,-z,-w);
            }
            
            sv_inline IVec4 &operator*=(s32 v) {
                x *= v; y *= v; z *= v; w *= v;
                return *this;
            }
            
            sv_inline IVec4 &operator*=(const IVec4 &v) {
                x *= v.x; y *= v.y; z *= v.z; w *= v.w;
                return *this;
            }
            
            sv_inline IVec4 &operator/=(s32 v) {
                x /= v; y /= v; z /= v; w /= v;
                return *this;
            }
            
            sv_inline IVec4 &operator/=(const IVec4 &v) {
                x /= v.x; y /= v.y; z /= v.z; w /= v.w;
                return *this;
            }
            
            sv_inline IVec4 &operator+=(const IVec4 &v) {
                x += v.x; y += v.y; z += v.z; w += v.w;
                return *this;
            }
            
            sv_inline IVec4 &operator-=(const IVec4 &v) {
                x -= v.x; y -= v.y; z -= v.z; w -= v.w;
                return *this;
            }
            
            sv_inline IVec4 &operator<<=(s32 v) {
                x <<= v; y <<= v; z <<= v; w <<= v;
                return *this;
            }
            
            sv_inline IVec4 &operator>>=(s32 v) {
                x >>= v; y >>= v; z >>= v; w >>= v;
                return *this;
            }
            
            sv_inline operator s32*() { return v; }
            
            sv_inline operator const s32*() const { return v; }
            
            sv_inline operator void*() { return v; }
            
            sv_inline operator const void*() const { return v; }
            
            sv_inline s32 &operator[](s32 i) {
                assert((u32)i < 4 && "IVec4::operator[](): bad index");
                return v[i];
            }
            
            sv_inline s32 operator[](s32 i) const {
                assert((u32)i < 4 && "IVec4::operator[](): bad index");
                return v[i];
            }
            
            sv_inline void set(s32 v) {
                x = v; y = v; z = v; w = v;
            }
            
            sv_inline void set(s32 x_,s32 y_,s32 z_,s32 w_) {
                x = x_; y = y_; z = z_; w = w_;
            }
            
            sv_inline void set(const IVec4 &v) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                vec = v.vec;
#else
                x = v.x; y = v.y; z = v.z; w = v.w;
#endif
            }
            
            sv_inline void set(const s32 *v) {
                x = v[0]; y = v[1]; z = v[2]; w = v[3];
            }
            
            sv_inline void get(s32 *v) const {
                v[0] = x; v[1] = y; v[2] = z; v[3] = w;
            }
            
            sv_inline s32 *get() { return v; }
            
            sv_inline const s32 *get() const { return v; }
            
            sv_inline s32 length2() const {
                return x * x + y * y + z * z + w * w;
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
        extern const IVec4 IVec4_zero;
        extern const IVec4 IVec4_one;
        
        //
        sv_inline FVec4::FVec4(const IVec4 &v)
        : x(Math::itof(v.x))
        , y(Math::itof(v.y))
        , z(Math::itof(v.z))
        , w(Math::itof(v.w)) {
        }
        
        sv_inline DVec4::DVec4(const IVec4 &v)
        : x(Math::itod(v.x))
        , y(Math::itod(v.y))
        , z(Math::itod(v.z))
        , w(Math::itod(v.w)) {
        }
        
        //
        sv_inline s32 operator==(const IVec4 &v0,const IVec4 &v1) {
            return (v0.x == v1.x && v0.y == v1.y && v0.z == v1.z && v0.w == v1.w);
        }
        
        sv_inline s32 operator!=(const IVec4 &v0,const IVec4 &v1) {
            return (v0.x != v1.x || v0.y != v1.y || v0.z != v1.z || v0.w != v1.w);
        }
        
        sv_inline IVec4 operator*(const IVec4 &v0,s32 v1) {
            return IVec4(v0.x * v1,v0.y * v1,v0.z * v1,v0.w * v1);
        }
        
        sv_inline IVec4 operator*(const IVec4 &v0,const IVec4 &v1) {
            return IVec4(v0.x * v1.x,v0.y * v1.y,v0.z * v1.z,v0.w * v1.w);
        }
        
        sv_inline IVec4 operator/(const IVec4 &v0,s32 v1) {
            return IVec4(v0.x / v1,v0.y / v1,v0.z / v1,v0.w / v1);
        }
        
        sv_inline IVec4 operator/(const IVec4 &v0,const IVec4 &v1) {
            return IVec4(v0.x / v1.x,v0.y / v1.y,v0.z / v1.z,v0.w / v1.w);
        }
        
        sv_inline IVec4 operator+(const IVec4 &v0,const IVec4 &v1) {
            return IVec4(v0.x + v1.x,v0.y + v1.y,v0.z + v1.z,v0.w + v1.w);
        }
        
        sv_inline IVec4 operator-(const IVec4 &v0,const IVec4 &v1) {
            return IVec4(v0.x - v1.x,v0.y - v1.y,v0.z - v1.z,v0.w - v1.w);
        }
        
        sv_inline IVec4 operator<<(const IVec4 &v0,s32 v1) {
            return IVec4(v0.x << v1,v0.y << v1,v0.z << v1,v0.w << v1);
        }
        
        sv_inline IVec4 operator>>(const IVec4 &v0,s32 v1) {
            return IVec4(v0.x >> v1,v0.y >> v1,v0.z >> v1,v0.w >> v1);
        }
        
        //
        sv_inline s32 dot(const IVec4 &v0,const IVec4 &v1) {
            return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
        }
        
        sv_inline IVec4 &lerp(IVec4 &ret,const IVec4 &v0,const IVec4 &v1,s32 k) {
            ret.x = lerp(v0.x,v1.x,k);
            ret.y = lerp(v0.y,v1.y,k);
            ret.z = lerp(v0.z,v1.z,k);
            ret.w = lerp(v0.w,v1.w,k);
            return ret;
        }
        
        //
        sv_inline s32 length2(const IVec4 &v) {
            return v.length2();
        }
        
        //
        IVec4 min(const IVec4 &v0,const IVec4 &v1);
        
        IVec4 max(const IVec4 &v0,const IVec4 &v1);
        
        IVec4 clamp(const IVec4 &v,const IVec4 &v0,const IVec4 &v1);
        
        IVec4 lerp(const IVec4 &v0,const IVec4 &v1,s32 k);
        
        //*************************** BVec4(byte) ****************************
        //
        struct BVec4 {
            sv_inline BVec4() {
            }
            
            //
            sv_inline BVec4(u8 x,u8 y,u8 z,u8 w)
            : x(x)
            , y(y)
            , z(z)
            , w(w) {
            }
            
            //
            explicit sv_inline BVec4(u8 v)
            : x(v)
            , y(v)
            , z(v)
            , w(v) {
            }
            
            //
            explicit sv_inline BVec4(const u8 *v)
            : x(v[0])
            , y(v[1])
            , z(v[2])
            , w(v[3]) {
            }
            
            //
            explicit sv_inline BVec4(const FVec4 &v)
            : x((u8)Math::ftoi(v.x))
            , y((u8)Math::ftoi(v.y))
            , z((u8)Math::ftoi(v.z))
            , w((u8)Math::ftoi(v.w)) {
            }
            
            //
            explicit sv_inline BVec4(const DVec4 &v)
            : x((u8)Math::dtoi(v.x))
            , y((u8)Math::dtoi(v.y))
            , z((u8)Math::dtoi(v.z))
            , w((u8)Math::dtoi(v.w)){
            }
            
            //
            explicit sv_inline BVec4(const IVec4 &v)
            : x((u8)v.x)
            , y((u8)v.y)
            , z((u8)v.z)
            , w((u8)v.w) {
            }
            
            //
            sv_inline BVec4(const BVec4 &v)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(v.w) {
            }
            
            //
            sv_inline BVec4 &operator=(const BVec4 &v) {
                vec = v.vec;
                return *this;
            }
            
            sv_inline operator u8*() { return v; }
            
            sv_inline operator const u8*() const { return v; }
            
            sv_inline operator void*() { return v; }
            
            sv_inline operator const void*() const { return v; }
            
            sv_inline u8 &operator[](s32 i) {
                assert((u32)i < 4 && "BVec4::operator[](): bad index");
                return v[i];
            }
            
            sv_inline u8 operator[](s32 i) const {
                assert((u32)i < 4 && "BVec4::operator[](): bad index");
                return v[i];
            }
            
            sv_inline void set(u8 v) {
                x = v; y = v; z = v; w = v;
            }
            
            sv_inline void set(u8 x_,u8 y_,u8 z_,u8 w_) {
                x = x_; y = y_; z = z_; w = w_;
            }
            
            sv_inline void set(const BVec4 &v) {
                x = v.x; y = v.y; z = v.z; w = v.w;
            }
            
            sv_inline void set(const u8 *v) {
                x = v[0]; y = v[1]; z = v[2]; w = v[3];
            }
            
            sv_inline void get(u8 *v) const {
                v[0] = x; v[1] = y; v[2] = z; v[3] = w;
            }
            
            sv_inline u8 *get() { return v; }
            
            sv_inline const u8 *get() const { return v; }
            
            union {
                struct {
                    u8 x,y,z,w;
                };
                u8 v[4];
                u32 vec;
            };
        };
        
        //
        extern const BVec4 BVec4_zero;
        extern const BVec4 BVec4_one;
        
        //
        sv_inline FVec4::FVec4(const BVec4 &v)
        : x(Math::itof(v.x))
        , y(Math::itof(v.y))
        , z(Math::itof(v.z))
        , w(Math::itof(v.w)) {
        }
        
        sv_inline DVec4::DVec4(const BVec4 &v)
        : x(Math::itod(v.x))
        , y(Math::itod(v.y))
        , z(Math::itod(v.z))
        , w(Math::itod(v.w)) {
        }
        
        sv_inline IVec4::IVec4(const BVec4 &v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(v.w) {
        }
        
        //
        sv_inline s32 operator==(const BVec4 &v0,const BVec4 &v1) {
            return (v0.vec == v1.vec);
        }
        
        sv_inline s32 operator!=(const BVec4 &v0,const BVec4 &v1) {
            return (v0.vec != v1.vec);
        }
        
        //
        BVec4 min(const BVec4 &v0,const BVec4 &v1);
        
        BVec4 max(const BVec4 &v0,const BVec4 &v1);
        
        BVec4 clamp(const BVec4 &v,const BVec4 &v0,const BVec4 &v1);

    }//!namespace util
    
}//!namespace sv


#endif /* __aVEC4_H__ */
