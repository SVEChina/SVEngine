#ifndef __aVEC2_H__
#define __aVEC2_H__

#include "SVVecbase.h"
#include "SVCoreMath.h"

namespace sv {
    
    namespace util {
        
        struct FVec2;
        struct FVec3;
        struct FVec4;
        struct DVec2;
        struct DVec3;
        struct DVec4;
        struct HVec2;
        struct HVec3;
        struct HVec4;
        struct IVec2;
        struct IVec3;
        struct IVec4;
        struct BVec4;
        
        //******************************** FVec2***********************************
        //
        SVE_ALIGNED8(struct) FVec2 {
            
            sv_inline FVec2() { }
            sv_inline FVec2(const FVec2 &v) : x(v.x), y(v.y) { }
            sv_inline FVec2(f32 x,f32 y) : x(x), y(y) { }
            explicit sv_inline FVec2(f32 v) : x(v), y(v) { }
            explicit sv_inline FVec2(const FVec3 &v);
            explicit sv_inline FVec2(const FVec4 &v);
            explicit sv_inline FVec2(const f32 *v) : x(v[0]), y(v[1]) { }
            explicit sv_inline FVec2(const DVec2 &v);
            explicit sv_inline FVec2(const HVec2 &v);
            explicit sv_inline FVec2(const IVec2 &v);
#ifdef USE_SSE
            explicit sv_inline FVec2(__m64 v) : vec(v) { }
#elif USE_NEON
            explicit sv_inline FVec2(float32x2_t v) : vec(v) { }
#endif
            
            sv_inline FVec2 &operator=(const FVec2 &v) {
                x = v.x; y = v.y;
                return *this;
            }
            sv_inline FVec2 operator-() const {
                return FVec2(-x,-y);
            }
            sv_inline FVec2 &operator*=(f32 v) {
                x *= v; y *= v;
                return *this;
            }
            sv_inline FVec2 &operator*=(const FVec2 &v) {
                x *= v.x; y *= v.y;
                return *this;
            }
            sv_inline FVec2 &operator/=(f32 v) {
                f32 iv = Math::rcp(v);
                x *= iv; y *= iv;
                return *this;
            }
            sv_inline FVec2 &operator/=(const FVec2 &v) {
                x /= v.x; y /= v.y;
                return *this;
            }
            sv_inline FVec2 &operator+=(const FVec2 &v) {
                x += v.x; y += v.y;
                return *this;
            }
            sv_inline FVec2 &operator-=(const FVec2 &v) {
                x -= v.x; y -= v.y;
                return *this;
            }
            
            sv_inline operator f32*() { return v; }
            sv_inline operator const f32*() const { return v; }
            sv_inline operator void*() { return v; }
            sv_inline operator const void*() const { return v; }
            
            sv_inline f32 &operator[](s32 i) {
                assert((u32)i < 2 && "FVec2::operator[](): bad index");
                return v[i];
            }
            sv_inline f32 operator[](s32 i) const {
                assert((u32)i < 2 && "FVec2::operator[](): bad index");
                return v[i];
            }
            
            sv_inline void set(f32 v) {
                x = v; y = v;
            }
            sv_inline void set(f32 x_,f32 y_) {
                x = x_; y = y_;
            }
            sv_inline void set(const FVec2 &v) {
                x = v.x; y = v.y;
            }
            sv_inline void set(const f32 *v) {
                x = v[0]; y = v[1];
            }
            sv_inline void get(f32 *v) const {
                v[0] = x; v[1] = y;
            }
            sv_inline f32 *get() { return v; }
            sv_inline const f32 *get() const { return v; }
            
            sv_inline f32 length2() const {
                return x * x + y * y;
            }
            sv_inline f32 length() const {
                return Math::sqrt(x * x + y * y);
            }
            sv_inline FVec2 &normalize() {
                f32 ilength = Math::rsqrt(x * x + y * y);
                x *= ilength; y *= ilength;
                return *this;
            }
            sv_inline FVec2 &normalizeFast() {
                f32 ilength = Math::rsqrtFast(x * x + y * y);
                x *= ilength; y *= ilength;
                return *this;
            }
            
            sv_inline FVec2 getPerp(){
                return FVec2(-y, x);
            }
            
            union {
                struct {
                    f32 x,y;
                };
                f32 v[2];
#ifdef USE_SSE
                __m64 vec;
#elif USE_NEON
                float32x2_t vec;
#endif
            };
        };
        
        //
        extern const FVec2 FVec2_zero;
        extern const FVec2 FVec2_one;
        extern const FVec2 FVec2_epsilon;
        extern const FVec2 FVec2_infinity;
        
        //
        sv_inline s32 operator==(const FVec2 &v0,const FVec2 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y));
        }
        
        sv_inline s32 operator!=(const FVec2 &v0,const FVec2 &v1) {
            return !(compare(v0.x,v1.x) && compare(v0.y,v1.y));
        }
        
        sv_inline FVec2 operator*(const FVec2 &v0,f32 v1) {
            return FVec2(v0.x * v1,v0.y * v1);
        }
        
        sv_inline FVec2 operator*(const FVec2 &v0,const FVec2 &v1) {
            return FVec2(v0.x * v1.x,v0.y * v1.y);
        }
        
        sv_inline FVec2 operator/(const FVec2 &v0,f32 v1) {
            f32 iv1 = Math::rcp(v1);
            return FVec2(v0.x * iv1,v0.y * iv1);
        }
        
        sv_inline FVec2 operator/(const FVec2 &v0,const FVec2 &v1) {
            return FVec2(v0.x / v1.x,v0.y / v1.y);
        }
        
        sv_inline FVec2 operator+(const FVec2 &v0,const FVec2 &v1) {
            return FVec2(v0.x + v1.x,v0.y + v1.y);
        }
        
        sv_inline FVec2 operator-(const FVec2 &v0,const FVec2 &v1) {
            return FVec2(v0.x - v1.x,v0.y - v1.y);
        }
        
        //
        sv_inline s32 compare(const FVec2 &v0,const FVec2 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y));
        }
        
        sv_inline s32 compare(const FVec2 &v0,const FVec2 &v1,f32 epsilon) {
            return (compare(v0.x,v1.x,epsilon) && compare(v0.y,v1.y,epsilon));
        }
        
        sv_inline f32 dot(const FVec2 &v0,const FVec2 &v1) {
            return v0.x * v1.x + v0.y * v1.y;
        }
        
        sv_inline FVec2 &mul(FVec2 &ret,const FVec2 &v0,f32 v1) {
            ret.x = v0.x * v1;
            ret.y = v0.y * v1;
            return ret;
        }
        
        sv_inline FVec2 &mul(FVec2 &ret,const FVec2 &v0,const FVec2 &v1) {
            ret.x = v0.x * v1.x;
            ret.y = v0.y * v1.y;
            return ret;
        }
        
        sv_inline FVec2 &mad(FVec2 &ret,const FVec2 &v0,f32 v1,const FVec2 &v2) {
            ret.x = v0.x * v1 + v2.x;
            ret.y = v0.y * v1 + v2.y;
            return ret;
        }
        
        sv_inline FVec2 &mad(FVec2 &ret,const FVec2 &v0,const FVec2 &v1,const FVec2 &v2) {
            ret.x = v0.x * v1.x + v2.x;
            ret.y = v0.y * v1.y + v2.y;
            return ret;
        }
        
        sv_inline FVec2 &add(FVec2 &ret,const FVec2 &v0,const FVec2 &v1) {
            ret.x = v0.x + v1.x;
            ret.y = v0.y + v1.y;
            return ret;
        }
        
        sv_inline FVec2 &sub(FVec2 &ret,const FVec2 &v0,const FVec2 &v1) {
            ret.x = v0.x - v1.x;
            ret.y = v0.y - v1.y;
            return ret;
        }
        
        sv_inline FVec2 &lerp(FVec2 &ret,const FVec2 &v0,const FVec2 &v1,f32 k) {
            ret.x = lerp(v0.x,v1.x,k);
            ret.y = lerp(v0.y,v1.y,k);
            return ret;
        }
        
        //
        sv_inline f32 length(const FVec2 &v) {
            return v.length();
        }
        
        sv_inline f32 length2(const FVec2 &v) {
            return v.length2();
        }
        
        sv_inline FVec2 normalize(const FVec2 &v) {
            FVec2 ret = v;
            return ret.normalize();
        }
        
        sv_inline f32 cross(const FVec2 &v0,const FVec2 &v1) {
            return v0.x * v1.y - v0.y * v1.x;
        }
        
        sv_inline f32 getDistanceFrom(const FVec2 &v0 ,const FVec2 &v1){
            return FVec2(v0.x - v1.x, v0.y - v1.y).length();
        }
        
       sv_inline FVec2 &rotateBy(f64 degrees,  FVec2 &v0,  FVec2 &v1)  {
            degrees *=  DEGTORAD64;
            const f64 cs = cos(degrees);
            const f64 sn = sin(degrees);
            f32 t_x=v0.x;
            f32 t_y=v0.y;
            t_x -= v1.x;
            t_y -= v1.y;
           
            v0.x= (t_x * cs - t_y * sn);
            v0.y= (t_x * sn + t_y * cs);
            
            v0.x += v1.x;
            v0.y += v1.y;
            return v0;
        }
        
        //
        FVec2 min(const FVec2 &v0,const FVec2 &v1);
        FVec2 max(const FVec2 &v0,const FVec2 &v1);
        FVec2 clamp(const FVec2 &v,const FVec2 &v0,const FVec2 &v1);
        FVec2 saturate(const FVec2 &v);
        FVec2 lerp(const FVec2 &v0,const FVec2 &v1,f32 k);
        
        /******************************************************************************\
         *
         * DVec2
         *
         \******************************************************************************/
        
        //
        SVE_ALIGNED16(struct) DVec2 {
            
            sv_inline DVec2() { }
            sv_inline DVec2(f64 x,f64 y) : x(x), y(y) { }
            explicit sv_inline DVec2(f64 v) : x(v), y(v) { }
            explicit sv_inline DVec2(const DVec3 &v);
            explicit sv_inline DVec2(const DVec4 &v);
            explicit sv_inline DVec2(const f64 *v) : x(v[0]), y(v[1]) { }
            explicit sv_inline DVec2(const FVec2 &v) : x(v.x), y(v.y) { }
            explicit sv_inline DVec2(const HVec2 &v);
            explicit sv_inline DVec2(const IVec2 &v);
#ifdef USE_SSE2
            sv_inline DVec2(const DVec2 &v) : vec(v.vec) { }
            explicit sv_inline DVec2(__m128d v) : vec(v) { }
#else
            sv_inline DVec2(const DVec2 &v) : x(v.x), y(v.y) { }
#endif
            
            sv_inline DVec2 &operator=(const DVec2 &v) {
#ifdef USE_SSE2
                vec = v.vec;
#else
                x = v.x; y = v.y;
#endif
                return *this;
            }
            sv_inline DVec2 operator-() const {
                return DVec2(-x,-y);
            }
            sv_inline DVec2 &operator*=(f64 v) {
                x *= v; y *= v;
                return *this;
            }
            sv_inline DVec2 &operator*=(const DVec2 &v) {
                x *= v.x; y *= v.y;
                return *this;
            }
            sv_inline DVec2 &operator/=(f64 v) {
                f64 iv = Math::rcp(v);
                x *= iv; y *= iv;
                return *this;
            }
            sv_inline DVec2 &operator/=(const DVec2 &v) {
                x /= v.x; y /= v.y;
                return *this;
            }
            sv_inline DVec2 &operator+=(const DVec2 &v) {
                x += v.x; y += v.y;
                return *this;
            }
            sv_inline DVec2 &operator-=(const DVec2 &v) {
                x -= v.x; y -= v.y;
                return *this;
            }
            
            sv_inline operator f64*() { return v; }
            sv_inline operator const f64*() const { return v; }
            sv_inline operator void*() { return v; }
            sv_inline operator const void*() const { return v; }
            
            sv_inline f64 &operator[](s32 i) {
                assert((u32)i < 2 && "DVec2::operator[](): bad index");
                return v[i];
            }
            sv_inline f64 operator[](s32 i) const {
                assert((u32)i < 2 && "DVec2::operator[](): bad index");
                return v[i];
            }
            
            sv_inline void set(f64 v) {
                x = v; y = v;
            }
            sv_inline void set(f64 x_,f64 y_) {
                x = x_; y = y_;
            }
            sv_inline void set(const DVec2 &v) {
#ifdef USE_SSE2
                vec = v.vec;
#else
                x = v.x; y = v.y;
#endif
            }
            sv_inline void set(const f64 *v) {
                x = v[0]; y = v[1];
            }
            sv_inline void get(f64 *v) const {
                v[0] = x; v[1] = y;
            }
            sv_inline f64 *get() { return v; }
            sv_inline const f64 *get() const { return v; }
            
            sv_inline f64 length() const {
                return Math::sqrt(x * x + y * y);
            }
            sv_inline f64 length2() const {
                return x * x + y * y;
            }
            sv_inline DVec2 &normalize() {
                f64 ilength = Math::rsqrt(x * x + y * y);
                x *= ilength; y *= ilength;
                return *this;
            }
            
            union {
                struct {
                    f64 x,y;
                };
                f64 v[2];
#ifdef USE_SSE2
                __m128d vec;
#endif
            };
        };
        
        //
        extern const DVec2 DVec2_zero;
        extern const DVec2 DVec2_one;
        extern const DVec2 DVec2_epsilon;
        extern const DVec2 DVec2_infinity;
        
        //
        sv_inline FVec2::FVec2(const DVec2 &v) : x((f32)v.x), y((f32)v.y) { }
        
        //
        sv_inline s32 operator==(const DVec2 &v0,const DVec2 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y));
        }
        
        sv_inline s32 operator!=(const DVec2 &v0,const DVec2 &v1) {
            return !(compare(v0.x,v1.x) && compare(v0.y,v1.y));
        }
        
        sv_inline DVec2 operator*(const DVec2 &v0,f64 v1) {
            return DVec2(v0.x * v1,v0.y * v1);
        }
        
        sv_inline DVec2 operator*(const DVec2 &v0,const DVec2 &v1) {
            return DVec2(v0.x * v1.x,v0.y * v1.y);
        }
        
        sv_inline DVec2 operator/(const DVec2 &v0,f64 v1) {
            f64 iv1 = Math::rcp(v1);
            return DVec2(v0.x * iv1,v0.y * iv1);
        }
        
        sv_inline DVec2 operator/(const DVec2 &v0,const DVec2 &v1) {
            return DVec2(v0.x / v1.x,v0.y / v1.y);
        }
        
        sv_inline DVec2 operator+(const DVec2 &v0,const DVec2 &v1) {
            return DVec2(v0.x + v1.x,v0.y + v1.y);
        }
        
        sv_inline DVec2 operator-(const DVec2 &v0,const DVec2 &v1) {
            return DVec2(v0.x - v1.x,v0.y - v1.y);
        }
        
        //
        sv_inline s32 compare(const DVec2 &v0,const DVec2 &v1) {
            return (compare(v0.x,v1.x) && compare(v0.y,v1.y));
        }
        
        sv_inline s32 compare(const DVec2 &v0,const DVec2 &v1,f64 epsilon) {
            return (compare(v0.x,v1.x,epsilon) && compare(v0.y,v1.y,epsilon));
        }
        
        sv_inline f64 dot(const DVec2 &v0,const DVec2 &v1) {
            return v0.x * v1.x + v0.y * v1.y;
        }
        
        sv_inline DVec2 &mul(DVec2 &ret,const DVec2 &v0,f64 v1) {
            ret.x = v0.x * v1;
            ret.y = v0.y * v1;
            return ret;
        }
        
        sv_inline DVec2 &mul(DVec2 &ret,const DVec2 &v0,const DVec2 &v1) {
            ret.x = v0.x * v1.x;
            ret.y = v0.y * v1.y;
            return ret;
        }
        
        sv_inline DVec2 &mad(DVec2 &ret,const DVec2 &v0,f64 v1,const DVec2 &v2) {
            ret.x = v0.x * v1 + v2.x;
            ret.y = v0.y * v1 + v2.y;
            return ret;
        }
        
        sv_inline DVec2 &mad(DVec2 &ret,const DVec2 &v0,const DVec2 &v1,const DVec2 &v2) {
            ret.x = v0.x * v1.x + v2.x;
            ret.y = v0.y * v1.y + v2.y;
            return ret;
        }
        
        sv_inline DVec2 &add(DVec2 &ret,const DVec2 &v0,const DVec2 &v1) {
            ret.x = v0.x + v1.x;
            ret.y = v0.y + v1.y;
            return ret;
        }
        
        sv_inline DVec2 &sub(DVec2 &ret,const DVec2 &v0,const DVec2 &v1) {
            ret.x = v0.x - v1.x;
            ret.y = v0.y - v1.y;
            return ret;
        }
        
        sv_inline DVec2 &lerp(DVec2 &ret,const DVec2 &v0,const DVec2 &v1,f64 k) {
            ret.x = lerp(v0.x,v1.x,k);
            ret.y = lerp(v0.y,v1.y,k);
            return ret;
        }
        
        //
        sv_inline f64 length(const DVec2 &v) {
            return v.length();
        }
        
        sv_inline f64 length2(const DVec2 &v) {
            return v.length2();
        }
        
        sv_inline DVec2 normalize(const DVec2 &v) {
            DVec2 ret = v;
            return ret.normalize();
        }
        
        //
        DVec2 min(const DVec2 &v0,const DVec2 &v1);
        DVec2 max(const DVec2 &v0,const DVec2 &v1);
        DVec2 clamp(const DVec2 &v,const DVec2 &v0,const DVec2 &v1);
        DVec2 saturate(const DVec2 &v);
        DVec2 lerp(const DVec2 &v0,const DVec2 &v1,f64 k);
        
        /******************************************************************************\
         *
         * HVec2
         *
         \******************************************************************************/
        
        //
        SVE_ALIGNED4(struct) HVec2 {
            
            sv_inline HVec2() { }
            sv_inline HVec2(const HVec2 &v) : x(v.x), y(v.y) { }
            sv_inline HVec2(SVHalf x,SVHalf y) : x(x), y(y) { }
            explicit sv_inline HVec2(SVHalf v) : x(v), y(v) { }
            explicit sv_inline HVec2(f32 v) : x(v), y(v) { }
            explicit sv_inline HVec2(const FVec2 &v) : x(v.x), y(v.y) { }
            explicit sv_inline HVec2(const DVec2 &v) : x((f32)v.x), y((f32)v.y) { }
            
            sv_inline HVec2 &operator=(const HVec2 &v) {
                x = v.x; y = v.y;
                return *this;
            }
            
            sv_inline operator SVHalf*() { return &x; }
            sv_inline operator const SVHalf*() const { return &x; }
            sv_inline operator void*() { return &x; }
            sv_inline operator const void*() const { return &x; }
            
            sv_inline SVHalf &operator[](s32 i) {
                assert((u32)i < 2 && "HVec2::operator[](): bad index");
                return (&x)[i];
            }
            sv_inline SVHalf operator[](s32 i) const {
                assert((u32)i < 2 && "HVec2::operator[](): bad index");
                return (&x)[i];
            }
            
            SVHalf x,y;
        };
        
        //
        extern const HVec2 HVec2_zero;
        extern const HVec2 HVec2_one;
        
        //
        sv_inline FVec2::FVec2(const HVec2 &v) : x(v.x.getFloat()), y(v.y.getFloat()) { }
        sv_inline DVec2::DVec2(const HVec2 &v) : x(v.x.getFloat()), y(v.y.getFloat()) { }
        
        /******************************************************************************\
         *
         * IVec2
         *
         \******************************************************************************/
        
        //
        SVE_ALIGNED8(struct) IVec2 {
            
            sv_inline IVec2() { }
            sv_inline IVec2(const IVec2 &v) : x(v.x), y(v.y) { }
            sv_inline IVec2(s32 x,s32 y) : x(x), y(y) { }
            explicit sv_inline IVec2(s32 v) : x(v), y(v) { }
            explicit sv_inline IVec2(const s32 *v) : x(v[0]), y(v[1]) { }
            explicit sv_inline IVec2(const FVec2 &v) : x(Math::ftoi(v.x)), y(Math::ftoi(v.y)) { }
            explicit sv_inline IVec2(const DVec2 &v) : x(Math::dtoi(v.x)), y(Math::dtoi(v.y)) { }
#ifdef USE_SSE
            explicit sv_inline IVec2(__m64 v) : vec(v) { }
#elif USE_NEON
            explicit sv_inline IVec2(int32x2_t v) : vec(v) { }
#endif
            
            sv_inline IVec2 &operator=(const IVec2 &v) {
                x = v.x; y = v.y;
                return *this;
            }
            
            sv_inline IVec2 operator-() const {
                return IVec2(-x,-y);
            }
            sv_inline IVec2 &operator*=(s32 v) {
                x *= v; y *= v;
                return *this;
            }
            sv_inline IVec2 &operator*=(const IVec2 &v) {
                x *= v.x; y *= v.y;
                return *this;
            }
            sv_inline IVec2 &operator/=(s32 v) {
                x /= v; y /= v;
                return *this;
            }
            sv_inline IVec2 &operator/=(const IVec2 &v) {
                x /= v.x; y /= v.y;
                return *this;
            }
            sv_inline IVec2 &operator+=(const IVec2 &v) {
                x += v.x; y += v.y;
                return *this;
            }
            sv_inline IVec2 &operator-=(const IVec2 &v) {
                x -= v.x; y -= v.y;
                return *this;
            }
            sv_inline IVec2 &operator<<=(s32 v) {
                x <<= v; y <<= v;
                return *this;
            }
            sv_inline IVec2 &operator>>=(s32 v) {
                x >>= v; y >>= v;
                return *this;
            }
            
            sv_inline operator s32*() { return v; }
            sv_inline operator const s32*() const { return v; }
            sv_inline operator void*() { return v; }
            sv_inline operator const void*() const { return v; }
            
            sv_inline s32 &operator[](s32 i) {
                assert((u32)i < 2 && "IVec2::operator[](): bad index");
                return v[i];
            }
            sv_inline s32 operator[](s32 i) const {
                assert((u32)i < 2 && "IVec2::operator[](): bad index");
                return v[i];
            }
            
            sv_inline void set(s32 v) {
                x = v; y = v;
            }
            sv_inline void set(s32 x_,s32 y_) {
                x = x_; y = y_;
            }
            sv_inline void set(const IVec2 &v) {
                x = v.x; y = v.y;
            }
            sv_inline void set(const s32 *v) {
                x = v[0]; y = v[1];
            }
            sv_inline void get(s32 *v) const {
                v[0] = x; v[1] = y;
            }
            sv_inline s32 *get() { return v; }
            sv_inline const s32 *get() const { return v; }
            
            sv_inline s32 length2() const {
                return x * x + y * y;
            }
            
            union {
                struct {
                    s32 x,y;
                };
                s32 v[2];
#ifdef USE_SSE
                __m64 vec;
#elif USE_NEON
                int32x2_t vec;
#endif
            };
        };
        
        //
        extern const IVec2 IVec2_zero;
        extern const IVec2 IVec2_one;
        
        //
        sv_inline FVec2::FVec2(const IVec2 &v) : x(Math::itof(v.x)), y(Math::itof(v.y)) { }
        sv_inline DVec2::DVec2(const IVec2 &v) : x(Math::itod(v.x)), y(Math::itod(v.y)) { }
        
        //
        sv_inline s32 operator==(const IVec2 &v0,const IVec2 &v1) {
            return (v0.x == v1.x && v0.y == v1.y);
        }
        
        sv_inline s32 operator!=(const IVec2 &v0,const IVec2 &v1) {
            return (v0.x != v1.x || v0.y != v1.y);
        }
        
        sv_inline IVec2 operator*(const IVec2 &v0,s32 v1) {
            return IVec2(v0.x * v1,v0.y * v1);
        }
        
        sv_inline IVec2 operator*(const IVec2 &v0,const IVec2 &v1) {
            return IVec2(v0.x * v1.x,v0.y * v1.y);
        }
        
        sv_inline IVec2 operator/(const IVec2 &v0,s32 v1) {
            return IVec2(v0.x / v1,v0.y / v1);
        }
        
        sv_inline IVec2 operator/(const IVec2 &v0,const IVec2 &v1) {
            return IVec2(v0.x / v1.x,v0.y / v1.y);
        }
        
        sv_inline IVec2 operator+(const IVec2 &v0,const IVec2 &v1) {
            return IVec2(v0.x + v1.x,v0.y + v1.y);
        }
        
        sv_inline IVec2 operator-(const IVec2 &v0,const IVec2 &v1) {
            return IVec2(v0.x - v1.x,v0.y - v1.y);
        }
        
        sv_inline IVec2 operator<<(const IVec2 &v0,s32 v1) {
            return IVec2(v0.x << v1,v0.y << v1);
        }
        
        sv_inline IVec2 operator>>(const IVec2 &v0,s32 v1) {
            return IVec2(v0.x >> v1,v0.y >> v1);
        }
        
        //
        sv_inline s32 dot(const IVec2 &v0,const IVec2 &v1) {
            return v0.x * v1.x + v0.y * v1.y;
        }
        
        sv_inline IVec2 &lerp(IVec2 &ret,const IVec2 &v0,const IVec2 &v1,s32 k) {
            ret.x = lerp(v0.x,v1.x,k);
            ret.y = lerp(v0.y,v1.y,k);
            return ret;
        }
        
        //
        sv_inline s32 length2(const IVec2 &v) {
            return v.length2();
        }
        
        //
        IVec2 min(const IVec2 &v0,const IVec2 &v1);
        IVec2 max(const IVec2 &v0,const IVec2 &v1);
        IVec2 clamp(const IVec2 &v,const IVec2 &v0,const IVec2 &v1);
        IVec2 lerp(const IVec2 &v0,const IVec2 &v1,s32 k);
        
    }//!namespace util
    
}//!namespace sv


#endif /* __aVEC2_H__ */
