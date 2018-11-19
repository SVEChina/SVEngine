#ifndef __aMAT3_H__
#define __aMAT3_H__

#include "SVMat2.h"

namespace sv {
    
    namespace util {
        
        struct FMat2;
        struct FMat3;
        struct FMat4;
        struct DMat4;
        struct SVQuat;
        
        /******************************************************************************\
         *
         * FMat3
         *
         \******************************************************************************/
        
        //
        SVE_ALIGNED16(struct) FMat3 {
            sv_inline FMat3() { }
            FMat3(const FMat3 &m);
            explicit FMat3(f32 v);
            explicit FMat3(const FMat2 &m);
            explicit FMat3(const FMat4 &m);
            explicit FMat3(const DMat4 &m);
            explicit FMat3(const SVQuat &q);
            explicit FMat3(const f32 *m);
            
            sv_inline FMat3 &operator=(const FMat3 &m) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                col0 = m.col0;
                col1 = m.col1;
                col2 = m.col2;
#else
                m00 = m.m00; m10 = m.m10; m20 = m.m20; m30 = m.m30;
                m01 = m.m01; m11 = m.m11; m21 = m.m21; m31 = m.m31;
                m02 = m.m02; m12 = m.m12; m22 = m.m22; m32 = m.m32;
#endif
                return *this;
            }
            
            FMat3 operator-() const;
            FMat3 &operator*=(f32 v);
            FMat3 &operator*=(const FMat3 &m);
            FMat3 &operator+=(const FMat3 &m);
            FMat3 &operator-=(const FMat3 &m);
            
            sv_inline operator f32*() { return mat; }
            sv_inline operator const f32*() const { return mat; }
            sv_inline operator void*() { return mat; }
            sv_inline operator const void*() const { return mat; }
            
            sv_inline f32 &operator[](s32 i) {
                assert((u32)i < 12 && "FMat3::operator[](): bad index");
                return mat[i];
            }
            
            sv_inline f32 operator[](s32 i) const {
                assert((u32)i < 12 && "FMat3::operator[](): bad index");
                return mat[i];
            }
            
            sv_inline void set(s32 row,s32 column,f32 v) {
                assert((u32)row < 3 && "FMat3::set(): bad row");
                assert((u32)column < 3 && "FMat3::set(): bad column");
                mat[column * 4 + row] = v;
            }
            
            sv_inline f32 &get(s32 row,s32 column) {
                assert((u32)row < 3 && "FMat3::get(): bad row");
                assert((u32)column < 3 && "FMat3::get(): bad column");
                return mat[column * 4 + row];
            }
            
            sv_inline f32 get(s32 row,s32 column) const {
                assert((u32)row < 3 && "FMat3::get(): bad row");
                assert((u32)column < 3 && "FMat3::get(): bad column");
                return mat[column * 4 + row];
            }
            
            void set(const FMat2 &m);
            void set(const FMat3 &m);
            void set(const FMat4 &m);
            void set(const DMat4 &m);
            void set(const SVQuat &q);
            void set(const f32 *m);
            void get(f32 *m) const;
            sv_inline f32 *get() { return mat; }
            sv_inline const f32 *get() const { return mat; }
            
            void setRow(s32 row,const FVec3 &v);
            FVec3 getRow(s32 row) const;
            
            void setColumn(s32 column,const FVec3 &v);
            FVec3 getColumn(s32 column) const;
            
            void setDiagonal(const FVec3 &v);
            FVec3 getDiagonal() const;
            
            void setZero();
            void setIdentity();
            void setSkewSymmetric(const FVec3 &v);
            void setRotate(const FVec3 &axis,f32 angle);
            void setRotateX(f32 angle);
            void setRotateY(f32 angle);
            void setRotateZ(f32 angle);
            void setScale(const FVec3 &v);
            
            SVQuat getQuat() const;
            
            union {
                struct {
                    f32 m00,m10,m20,m30;
                    f32 m01,m11,m21,m31;
                    f32 m02,m12,m22,m32;
                };
                f32 mat[12];
#ifdef USE_SSE
                struct {
                    __m128 col0;
                    __m128 col1;
                    __m128 col2;
                };
#elif USE_ALTIVEC
                struct {
                    vec_float4 col0;
                    vec_float4 col1;
                    vec_float4 col2;
                };
#elif USE_NEON
                struct {
                    float32x4_t col0;
                    float32x4_t col1;
                    float32x4_t col2;
                };
#endif
            };
        };
        //
        extern const FMat3 FMat3_zero;
        extern const FMat3 FMat3_one;
        extern const FMat3 FMat3_identity;
        //
        s32 operator==(const FMat3 &m0,const FMat3 &m1);
        s32 operator!=(const FMat3 &m0,const FMat3 &m1);
        FMat3 operator*(const FMat3 &m,f32 v);
        FVec2 operator*(const FMat3 &m,const FVec2 &v);
        FVec2 operator*(const FVec2 &v,const FMat3 &m);
        FVec3 operator*(const FMat3 &m,const FVec3 &v);
        FVec3 operator*(const FVec3 &v,const FMat3 &m);
        DVec2 operator*(const FMat3 &m,const DVec2 &v);
        DVec2 operator*(const DVec2 &v,const FMat3 &m);
        DVec3 operator*(const FMat3 &m,const DVec3 &v);
        DVec3 operator*(const DVec3 &v,const FMat3 &m);
        FMat3 operator*(const FMat3 &m0,const FMat3 &m1);
        FMat3 operator+(const FMat3 &m0,const FMat3 &m1);
        FMat3 operator-(const FMat3 &m0,const FMat3 &m1);
        //
        s32 compare(const FMat3 &m0,const FMat3 &m1);
        s32 compare(const FMat3 &m0,const FMat3 &m1,f32 epsilon);
        f32 trace(const FMat3 &m);
        f32 determinant(const FMat3 &m);
        FMat3 &mul(FMat3 &ret,const FMat3 &m,f32 v);
        FVec2 &mul(FVec2 &ret,const FMat3 &m,const FVec2 &v);
        FVec2 &mul(FVec2 &ret,const FVec2 &v,const FMat3 &m);
        FVec3 &mul(FVec3 &ret,const FMat3 &m,const FVec3 &v);
        FVec3 &mul(FVec3 &ret,const FVec3 &v,const FMat3 &m);
        DVec2 &mul(DVec2 &ret,const FMat3 &m,const DVec2 &v);
        DVec2 &mul(DVec2 &ret,const DVec2 &v,const FMat3 &m);
        DVec3 &mul(DVec3 &ret,const FMat3 &m,const DVec3 &v);
        DVec3 &mul(DVec3 &ret,const DVec3 &v,const FMat3 &m);
        FMat3 &mul(FMat3 &ret,const FMat3 &m,const FVec3 &v);
        FMat3 &mul(FMat3 &ret,const FVec3 &v,const FMat3 &m);
        FMat3 &mul(FMat3 &ret,const FMat3 &m0,const FMat3 &m1);
        FMat3 &add(FMat3 &ret,const FMat3 &m0,const FMat3 &m1);
        FMat3 &sub(FMat3 &ret,const FMat3 &m0,const FMat3 &m1);
        FMat3 &orthonormalize(FMat3 &ret,const FMat3 &m);
        FMat3 &transpose(FMat3 &ret,const FMat3 &m);
        FMat3 &inverse(FMat3 &ret,const FMat3 &m);
        FMat3 &inverse(FMat3 &ret,const FMat3 &m,f32 det);
        //
        FMat3 orthonormalize(const FMat3 &m);
        FMat3 transpose(const FMat3 &m);
        FMat3 inverse(const FMat3 &m);
        FMat3 inverse(const FMat3 &m,f32 det);
        //
        FMat3 rotate3(const FVec3 &axis,f32 angle);
        FMat3 rotate3(f32 x,f32 y,f32 z,f32 angle);
        FMat3 rotate3(const SVQuat &q);
        FMat3 rotateX3(f32 angle);
        FMat3 rotateY3(f32 angle);
        FMat3 rotateZ3(f32 angle);
        FMat3 scale3(const FVec3 &v);
        FMat3 scale3(f32 x,f32 y,f32 z);
        //
        FMat3 jacobi(const FMat3 &m,FMat3 &v);
        
    }//!namespace util
    
}//!namespace sv


#endif /* __aMAT3_H__ */
