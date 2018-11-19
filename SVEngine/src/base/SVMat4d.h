#ifndef __aMAT4d_H__
#define __aMAT4d_H__

#include "SVMat4.h"

namespace sv {
    
    namespace util {
        
        /******************************************************************************\
         *
         * DMat4
         *
         \******************************************************************************/
        
        //
        SVE_ALIGNED16(struct) DMat4 {
            
            sv_inline DMat4() { }
            DMat4(const DMat4 &m);
            explicit DMat4(f64 v);
            explicit DMat4(const FMat2 &m);
            explicit DMat4(const FMat3 &m);
            explicit DMat4(const FMat4 &m);
            explicit DMat4(const SVQuat &q);
            DMat4(const f64 *m);
            DMat4(const FMat3 &m,const DVec3 &v);
            DMat4(const SVQuat &q,const DVec3 &v);
            
            sv_inline DMat4 &operator=(const DMat4 &m) {
#ifdef USE_SSE2
                col0 = m.col0; col1 = m.col1;
                col2 = m.col2; col3 = m.col3;
                col4 = m.col4; col5 = m.col5;
#else
                m00 = m.m00; m10 = m.m10; m20 = m.m20;
                m01 = m.m01; m11 = m.m11; m21 = m.m21;
                m02 = m.m02; m12 = m.m12; m22 = m.m22;
                m03 = m.m03; m13 = m.m13; m23 = m.m23;
#endif
                return *this;
            }
            
            DMat4 operator-() const;
            DMat4 &operator*=(f64 v);
            DMat4 &operator*=(const DMat4 &m);
            DMat4 &operator+=(const DMat4 &m);
            DMat4 &operator-=(const DMat4 &m);
            
            sv_inline operator f64*() { return mat; }
            sv_inline operator const f64*() const { return mat; }
            sv_inline operator void*() { return mat; }
            sv_inline operator const void*() const { return mat; }
            
            sv_inline f64 &operator[](s32 i) {
                assert((u32)i < 12 && "DMat4::operator[](): bad index");
                return mat[i];
            }
            sv_inline f64 operator[](s32 i) const {
                assert((u32)i < 12 && "DMat4::operator[](): bad index");
                return mat[i];
            }
            
            sv_inline void set(s32 row,s32 column,f64 v) {
                assert((u32)row < 3 && "DMat4::set(): bad row");
                assert((u32)column < 4 && "DMat4::set(): bad column");
                mat[column * 3 + row] = v;
            }
            sv_inline f64 &get(s32 row,s32 column) {
                assert((u32)row < 3 && "DMat4::get(): bad row");
                assert((u32)column < 4 && "DMat4::get(): bad column");
                return mat[column * 3 + row];
            }
            sv_inline f64 get(s32 row,s32 column) const {
                assert((u32)row < 3 && "DMat4::get(): bad row");
                assert((u32)column < 4 && "DMat4::get(): bad column");
                return mat[column * 3 + row];
            }
            
            void set(const FMat2 &m);
            void set(const FMat3 &m);
            void set(const FMat4 &m);
            void set(const DMat4 &m);
            void set(const SVQuat &q);
            void set(const f64 *v);
            void set(const FMat3 &m,const DVec3 &v);
            void set(const SVQuat &q,const DVec3 &v);
            void get(f64 *m) const;
            sv_inline f64 *get() { return mat; }
            sv_inline const f64 *get() const { return mat; }
            
            void setRow(s32 row,const DVec4 &v);
            void setRow3(s32 row,const DVec3 &v);
            DVec4 getRow(s32 row) const;
            DVec3 getRow3(s32 row) const;
            
            void setColumn(s32 column,const DVec4 &v);
            void setColumn3(s32 column,const DVec3 &v);
            DVec4 getColumn(s32 column) const;
            DVec3 getColumn3(s32 column) const;
            
            void setZero();
            void setIdentity();
            void setTranslate(const DVec3 &v);
            void setRotate(const DVec3 &axis,f64 angle);
            void setRotateX(f64 angle);
            void setRotateY(f64 angle);
            void setRotateZ(f64 angle);
            void setScale(const DVec3 &v);
            
            union {
                struct {
                    f64 m00,m10,m20;
                    f64 m01,m11,m21;
                    f64 m02,m12,m22;
                    f64 m03,m13,m23;
                };
                f64 mat[12];
#ifdef USE_SSE2
                struct {
                    __m128d col0;
                    __m128d col1;
                    __m128d col2;
                    __m128d col3;
                    __m128d col4;
                    __m128d col5;
                };
#endif
            };
        };
        
        //
        extern const DMat4 DMat4_zero;
        extern const DMat4 DMat4_one;
        extern const DMat4 DMat4_identity;
        
        //
        s32 operator==(const DMat4 &m0,const DMat4 &m1);
        s32 operator!=(const DMat4 &m0,const DMat4 &m1);
        DMat4 operator*(const DMat4 &m,f64 v);
        FVec2 operator*(const DMat4 &m,const FVec2 &v);
        FVec2 operator*(const FVec2 &v,const DMat4 &m);
        FVec3 operator*(const DMat4 &m,const FVec3 &v);
        FVec3 operator*(const FVec3 &v,const DMat4 &m);
        FVec4 operator*(const DMat4 &m,const FVec4 &v);
        FVec4 operator*(const FVec4 &v,const DMat4 &m);
        DVec2 operator*(const DMat4 &m,const DVec2 &v);
        DVec2 operator*(const DVec2 &v,const DMat4 &m);
        DVec3 operator*(const DMat4 &m,const DVec3 &v);
        DVec3 operator*(const DVec3 &v,const DMat4 &m);
        DVec4 operator*(const DMat4 &m,const DVec4 &v);
        DVec4 operator*(const DVec4 &v,const DMat4 &m);
        DMat4 operator*(const DMat4 &m0,const DMat4 &m1);
        DMat4 operator+(const DMat4 &m0,const DMat4 &m1);
        DMat4 operator-(const DMat4 &m0,const DMat4 &m1);
        
        //
        s32 compare(const DMat4 &m0,const DMat4 &m1);
        s32 compare(const DMat4 &m0,const DMat4 &m1,f64 epsilon);
        f64 determinant(const DMat4 &m);
        DMat4 &mul(DMat4 &ret,const DMat4 &m,f64 v);
        FVec2 &mul(FVec2 &ret,const DMat4 &m,const FVec2 &v);
        FVec2 &mul(FVec2 &ret,const FVec2 &v,const DMat4 &m);
        FVec3 &mul(FVec3 &ret,const DMat4 &m,const FVec3 &v);
        FVec3 &mul(FVec3 &ret,const FVec3 &v,const DMat4 &m);
        FVec4 &mul(FVec4 &ret,const DMat4 &m,const FVec4 &v);
        FVec4 &mul(FVec4 &ret,const FVec4 &v,const DMat4 &m);
        FVec2 &mul(FVec2 &ret,const DMat4 &m,const DVec2 &v);
        FVec2 &mul(FVec2 &ret,const DVec2 &v,const DMat4 &m);
        FVec3 &mul(FVec3 &ret,const DMat4 &m,const DVec3 &v);
        FVec3 &mul(FVec3 &ret,const DVec3 &v,const DMat4 &m);
        FVec4 &mul(FVec4 &ret,const DMat4 &m,const DVec4 &v);
        FVec4 &mul(FVec4 &ret,const DVec4 &v,const DMat4 &m);
        DVec2 &mul(DVec2 &ret,const DMat4 &m,const DVec2 &v);
        DVec2 &mul(DVec2 &ret,const DVec2 &v,const DMat4 &m);
        DVec3 &mul(DVec3 &ret,const DMat4 &m,const DVec3 &v);
        DVec3 &mul(DVec3 &ret,const DVec3 &v,const DMat4 &m);
        DVec4 &mul(DVec4 &ret,const DMat4 &m,const DVec4 &v);
        DVec4 &mul(DVec4 &ret,const DVec4 &v,const DMat4 &m);
        FVec2 &mul3(FVec2 &ret,const DMat4 &m,const FVec2 &v);
        FVec2 &mul3(FVec2 &ret,const FVec2 &v,const DMat4 &m);
        FVec3 &mul3(FVec3 &ret,const DMat4 &m,const FVec3 &v);
        FVec3 &mul3(FVec3 &ret,const FVec3 &v,const DMat4 &m);
        FVec4 &mul3(FVec4 &ret,const DMat4 &m,const FVec4 &v);
        FVec4 &mul3(FVec4 &ret,const FVec4 &v,const DMat4 &m);
        FVec2 &mul3(FVec2 &ret,const DMat4 &m,const DVec2 &v);
        FVec2 &mul3(FVec2 &ret,const DVec2 &v,const DMat4 &m);
        FVec3 &mul3(FVec3 &ret,const DMat4 &m,const DVec3 &v);
        FVec3 &mul3(FVec3 &ret,const DVec3 &v,const DMat4 &m);
        FVec4 &mul3(FVec4 &ret,const DMat4 &m,const DVec4 &v);
        FVec4 &mul3(FVec4 &ret,const DVec4 &v,const DMat4 &m);
        DVec2 &mul3(DVec2 &ret,const DMat4 &m,const DVec2 &v);
        DVec2 &mul3(DVec2 &ret,const DVec2 &v,const DMat4 &m);
        DVec3 &mul3(DVec3 &ret,const DMat4 &m,const DVec3 &v);
        DVec3 &mul3(DVec3 &ret,const DVec3 &v,const DMat4 &m);
        DVec4 &mul3(DVec4 &ret,const DMat4 &m,const DVec4 &v);
        DVec4 &mul3(DVec4 &ret,const DVec4 &v,const DMat4 &m);
        DMat4 &mul(DMat4 &ret,const DMat4 &m0,const DMat4 &m1);
        DMat4 &mul4(DMat4 &ret,const DMat4 &m0,const DMat4 &m1);
        DMat4 &mul3(DMat4 &ret,const DMat4 &m0,const DMat4 &m1);
        DMat4 &mult(DMat4 &ret,const DMat4 &m,const DVec3 &v);
        DMat4 &add(DMat4 &ret,const DMat4 &m0,const DMat4 &m1);
        DMat4 &sub(DMat4 &ret,const DMat4 &m0,const DMat4 &m1);
        DMat4 &orthonormalize(DMat4 &ret,const DMat4 &m);
        DMat4 &rotation(DMat4 &ret,const DMat4 &m);
        DMat4 &inverse(DMat4 &ret,const DMat4 &m);
        DMat4 &lerp(DMat4 &ret,const DMat4 &m0,const DMat4 &q1,f64 k);
        
        //
        DMat4 orthonormalize(const DMat4 &m);
        DMat4 rotation(const DMat4 &m);
        DMat4 inverse(const DMat4 &m);
        DMat4 lerp(const DMat4 &m0,const DMat4 &m1,f64 k);
        
        //
        DMat4 translate(const DVec3 &v);
        DMat4 translate(f64 x,f64 y,f64 z);
        
        //
        DMat4 reflect(const DVec4 &plane);
        DMat4 setTo(const DVec3 &position,const DVec3 &direction,const FVec3 &up);
        DMat4 lookAt(const DVec3 &position,const DVec3 &direction,const FVec3 &up);
        
        //
        void decomposeTransform(const DMat4 &m,DVec3 &position,SVQuat &rot,FVec3 &scale);
        DMat4 &composeTransform(DMat4 &ret,const DVec3 &position,const SVQuat &rot,const FVec3 &scale);
        
    }//!namespace util
    
}//!namespace sv


#endif /* __aMAT4d_H__ */
