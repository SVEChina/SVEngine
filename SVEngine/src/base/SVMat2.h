#ifndef __aMAT2_H__
#define __aMAT2_H__

#include "SVVec2.h"

namespace sv {
    
    namespace util {

        struct FMat2;
        struct FMat3;
        struct FMat4;
        struct DMat4;
        struct SVQuat;
        
        /******************************************************************************\
         *
         * FMat2
         *
        \******************************************************************************/
        
        //
        SVE_ALIGNED16(struct) FMat2 {
            
            sv_inline FMat2() { }
            FMat2(const FMat2 &m);
            explicit FMat2(f32 v);
            explicit FMat2(const FMat3 &m);
            explicit FMat2(const FMat4 &m);
            explicit FMat2(const DMat4 &m);
            explicit FMat2(const f32 *m);
            
            sv_inline FMat2 &operator=(const FMat2 &m) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                col = m.col;
#else
                m00 = m.m00; m10 = m.m10;
                m01 = m.m01; m11 = m.m11;
#endif
                return *this;
            }
            
            FMat2 operator-() const;
            FMat2 &operator*=(f32 v);
            FMat2 &operator*=(const FMat2 &m);
            FMat2 &operator+=(const FMat2 &m);
            FMat2 &operator-=(const FMat2 &m);
            
            sv_inline operator f32*() { return mat; }
            sv_inline operator const f32*() const { return mat; }
            sv_inline operator void*() { return mat; }
            sv_inline operator const void*() const { return mat; }
            
            sv_inline f32 &operator[](s32 i) {
                assert((u32)i < 4 && "FMat2::operator[](): bad index");
                return mat[i];
            }
            sv_inline f32 operator[](s32 i) const {
                assert((u32)i < 4 && "FMat2::operator[](): bad index");
                return mat[i];
            }
            
            sv_inline void set(s32 row,s32 column,f32 v) {
                assert((u32)row < 2 && "FMat2::set(): bad row");
                assert((u32)column < 2 && "FMat2::set(): bad column");
                mat[column * 2 + row] = v;
            }
            sv_inline f32 &get(s32 row,s32 column) {
                assert((u32)row < 2 && "FMat2::get(): bad row");
                assert((u32)column < 2 && "FMat2::get(): bad column");
                return mat[column * 2 + row];
            }
            sv_inline f32 get(s32 row,s32 column) const {
                assert((u32)row < 2 && "FMat2::get(): bad row");
                assert((u32)column < 2 && "FMat2::get(): bad column");
                return mat[column * 2 + row];
            }
            
            void set(const FMat2 &m);
            void set(const FMat3 &m);
            void set(const FMat4 &m);
            void set(const DMat4 &m);
            void set(const f32 *m);
            void get(f32 *m) const;
            sv_inline f32 *get() { return mat; }
            sv_inline const f32 *get() const { return mat; }
            
            void setRow(s32 row,const FVec2 &v);
            FVec2 getRow(s32 row) const;
            
            void setColumn(s32 column,const FVec2 &v);
            FVec2 getColumn(s32 column) const;
            
            void setZero();
            void setIdentity();
            void setRotate(f32 angle);
            void setScale(const FVec2 &v);
            
            union {
                struct {
                    f32 m00,m10;
                    f32 m01,m11;
                };
                f32 mat[4];
#ifdef USE_SSE
                __m128 col;
#elif USE_ALTIVEC
                vec_float4 col;
#elif USE_NEON
                float32x4_t col;
#endif
            };
        };
        
        //
        extern const FMat2 FMat2_zero;
        extern const FMat2 FMat2_one;
        extern const FMat2 FMat2_identity;
        
        //
        s32 operator==(const FMat2 &m0,const FMat2 &m1);
        s32 operator!=(const FMat2 &m0,const FMat2 &m1);
        FMat2 operator*(const FMat2 &m,f32 v);
        FVec2 operator*(const FMat2 &m,const FVec2 &v);
        FVec2 operator*(const FVec2 &v,const FMat2 &m);
        DVec2 operator*(const FMat2 &m,const DVec2 &v);
        DVec2 operator*(const DVec2 &v,const FMat2 &m);
        FMat2 operator*(const FMat2 &m0,const FMat2 &m1);
        FMat2 operator+(const FMat2 &m0,const FMat2 &m1);
        FMat2 operator-(const FMat2 &m0,const FMat2 &m1);
        
        //
        s32 compare(const FMat2 &m0,const FMat2 &m1);
        s32 compare(const FMat2 &m0,const FMat2 &m1,f32 epsilon);
        f32 trace(const FMat2 &m);
        f32 determinant(const FMat2 &m);
        FMat2 &mul(FMat2 &ret,const FMat2 &m,f32 v);
        FVec2 &mul(FVec2 &ret,const FMat2 &m,const FVec2 &v);
        FVec2 &mul(FVec2 &ret,const FVec2 &v,const FMat2 &m);
        DVec2 &mul(DVec2 &ret,const FMat2 &m,const DVec2 &v);
        DVec2 &mul(DVec2 &ret,const DVec2 &v,const FMat2 &m);
        FMat2 &mul(FMat2 &ret,const FMat2 &m0,const FMat2 &m1);
        FMat2 &add(FMat2 &ret,const FMat2 &m0,const FMat2 &m1);
        FMat2 &sub(FMat2 &ret,const FMat2 &m0,const FMat2 &m1);
        FMat2 &transpose(FMat2 &ret,const FMat2 &m);
        FMat2 &inverse(FMat2 &ret,const FMat2 &m);
        FMat2 &inverse(FMat2 &ret,const FMat2 &m,f32 det);
        
        //
        FMat2 transpose(const FMat2 &m);
        FMat2 inverse(const FMat2 &m);
        FMat2 inverse(const FMat2 &m,f32 det);
        
    }//!namespace util
    
}//!namespace sv


#endif /* __aMAT2_H__ */
