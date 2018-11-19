#ifndef __aMAT4_H__
#define __aMAT4_H__

#include "SVMat3.h"

namespace sv {
    
    namespace util {
        
        struct DMat4;
        struct SVQuat;
        
        //************** FMat4 ************************
        SVE_ALIGNED16(struct) FMat4 {
            sv_inline FMat4() {
            }
            
            FMat4(const FMat4 &m);
            explicit FMat4(f32 v);
            explicit FMat4(const FMat2 &m);
            explicit FMat4(const FMat3 &m);
            explicit FMat4(const DMat4 &m);
            explicit FMat4(const SVQuat &q);
            explicit FMat4(const f32 *m);
            FMat4(const FMat3 &m,const FVec3 &v);
            FMat4(const SVQuat &q,const FVec3 &v);
            
            sv_inline FMat4 &operator=(const FMat4 &m) {
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                col0 = m.col0;
                col1 = m.col1;
                col2 = m.col2;
                col3 = m.col3;
#else
                m00 = m.m00; m10 = m.m10; m20 = m.m20; m30 = m.m30;
                m01 = m.m01; m11 = m.m11; m21 = m.m21; m31 = m.m31;
                m02 = m.m02; m12 = m.m12; m22 = m.m22; m32 = m.m32;
                m03 = m.m03; m13 = m.m13; m23 = m.m23; m33 = m.m33;
#endif
                return *this;
            }
            //
            FMat4 operator-() const;
            FMat4 &operator*=(f32 v);
            FMat4 &operator*=(const FMat4 &m);
            FMat4 &operator+=(const FMat4 &m);
            FMat4 &operator-=(const FMat4 &m);
            //
            sv_inline operator f32*() { return mat; }
            sv_inline operator const f32*() const { return mat; }
            sv_inline operator void*() { return mat; }
            sv_inline operator const void*() const { return mat; }
            //
            sv_inline f32 &operator[](s32 i) {
                assert((u32)i < 16 && "FMat4::operator[](): bad index");
                return mat[i];
            }
            sv_inline f32 operator[](s32 i) const {
                assert((u32)i < 16 && "FMat4::operator[](): bad index");
                return mat[i];
            }
            
            sv_inline void set(s32 row,s32 column,f32 v) {
                assert((u32)row < 4 && "FMat4::set(): bad row");
                assert((u32)column < 4 && "FMat4::set(): bad column");
                mat[column * 4 + row] = v;
            }
            
            sv_inline f32 &get(s32 row,s32 column) {
                assert((u32)row < 4 && "FMat4::get(): bad row");
                assert((u32)column < 4 && "FMat4::get(): bad column");
                return mat[column * 4 + row];
            }
            
            sv_inline f32 get(s32 row,s32 column) const {
                assert((u32)row < 4 && "FMat4::get(): bad row");
                assert((u32)column < 4 && "FMat4::get(): bad column");
                return mat[column * 4 + row];
            }
            //
            void set(const FMat2 &m);
            void set(const FMat3 &m);
            void set(const FMat4 &m);
            void set(const DMat4 &m);
            void set(const SVQuat &q);
            void set(const f32 *m);
            void set(const FMat3 &m,const FVec3 &v);
            void set(const SVQuat &q,const FVec3 &v);
            void get(f32 *m) const;
            sv_inline f32 *get() { return mat; }
            sv_inline const f32 *get() const { return mat; }
            //
            void setRow(s32 row,const FVec4 &v);
            void setRow3(s32 row,const FVec3 &v);
            FVec4 getRow(s32 row) const;
            FVec3 getRow3(s32 row) const;
            //
            void setColumn(s32 column,const FVec4 &v);
            void setColumn3(s32 column,const FVec3 &v);
            FVec4 getColumn(s32 column) const;
            FVec3 getColumn3(s32 column) const;
            //
            void setDiagonal(const FVec4 &v);
            FVec4 getDiagonal() const;
            //
            void setZero();
            void setIdentity();
            void setTranslate(const FVec3 &v);
            void setRotate(const FVec3 &axis,f32 angle);
            void setRotateX(f32 angle);
            void setRotateY(f32 angle);
            void setRotateZ(f32 angle);
            void setScale(const FVec3 &v);
            
            union {
                struct {
                    f32 m00,m10,m20,m30;
                    f32 m01,m11,m21,m31;
                    f32 m02,m12,m22,m32;
                    f32 m03,m13,m23,m33;
                };
                f32 mat[16];
#ifdef USE_SSE
                struct {
                    __m128 col0;
                    __m128 col1;
                    __m128 col2;
                    __m128 col3;
                };
#elif USE_ALTIVEC
                struct {
                    vec_float4 col0;
                    vec_float4 col1;
                    vec_float4 col2;
                    vec_float4 col3;
                };
#elif USE_NEON
                struct {
                    float32x4_t col0;
                    float32x4_t col1;
                    float32x4_t col2;
                    float32x4_t col3;
                };
#endif
            };
        };
        //
        extern const FMat4 FMat4_zero;
        extern const FMat4 FMat4_one;
        extern const FMat4 FMat4_identity;
        //
        s32 operator==(const FMat4 &m0,const FMat4 &m1);
        s32 operator!=(const FMat4 &m0,const FMat4 &m1);
        FMat4 operator*(const FMat4 &m,f32 v);
        FVec2 operator*(const FMat4 &m,const FVec2 &v);
        FVec2 operator*(const FVec2 &v,const FMat4 &m);
        FVec3 operator*(const FMat4 &m,const FVec3 &v);
        FVec3 operator*(const FVec3 &v,const FMat4 &m);
        FVec4 operator*(const FMat4 &m,const FVec4 &v);
        FVec4 operator*(const FVec4 &v,const FMat4 &m);
        DVec2 operator*(const FMat4 &m,const DVec2 &v);
        DVec2 operator*(const DVec2 &v,const FMat4 &m);
        DVec3 operator*(const FMat4 &m,const DVec3 &v);
        DVec3 operator*(const DVec3 &v,const FMat4 &m);
        DVec4 operator*(const FMat4 &m,const DVec4 &v);
        DVec4 operator*(const DVec4 &v,const FMat4 &m);
        FMat4 operator*(const FMat4 &m0,const FMat4 &m1);
        FMat4 operator+(const FMat4 &m0,const FMat4 &m1);
        FMat4 operator-(const FMat4 &m0,const FMat4 &m1);
        //
        s32 compare(const FMat4 &m0,const FMat4 &m1);
        s32 compare(const FMat4 &m0,const FMat4 &m1,f32 epsilon);
        f32 trace(const FMat4 &m);
        f32 determinant(const FMat4 &m);
        f32 determinant3(const FMat4 &m);
        FMat4 &mul(FMat4 &ret,const FMat4 &m,f32 v);
        FVec2 &mul(FVec2 &ret,const FMat4 &m,const FVec2 &v);
        FVec2 &mul(FVec2 &ret,const FVec2 &v,const FMat4 &m);
        FVec3 &mul(FVec3 &ret,const FMat4 &m,const FVec3 &v);
        FVec3 &mul(FVec3 &ret,const FVec3 &v,const FMat4 &m);
        FVec4 &mul(FVec4 &ret,const FMat4 &m,const FVec4 &v);
        FVec4 &mul(FVec4 &ret,const FVec4 &v,const FMat4 &m);
        DVec2 &mul(DVec2 &ret,const FMat4 &m,const DVec2 &v);
        DVec2 &mul(DVec2 &ret,const DVec2 &v,const FMat4 &m);
        DVec3 &mul(DVec3 &ret,const FMat4 &m,const DVec3 &v);
        DVec3 &mul(DVec3 &ret,const DVec3 &v,const FMat4 &m);
        DVec4 &mul(DVec4 &ret,const FMat4 &m,const DVec4 &v);
        DVec4 &mul(DVec4 &ret,const DVec4 &v,const FMat4 &m);
        FVec2 &mul3(FVec2 &ret,const FMat4 &m,const FVec2 &v);
        FVec2 &mul3(FVec2 &ret,const FVec2 &v,const FMat4 &m);
        FVec3 &mul3(FVec3 &ret,const FMat4 &m,const FVec3 &v);
        FVec3 &mul3(FVec3 &ret,const FVec3 &v,const FMat4 &m);
        FVec4 &mul3(FVec4 &ret,const FMat4 &m,const FVec4 &v);
        FVec4 &mul3(FVec4 &ret,const FVec4 &v,const FMat4 &m);
        DVec2 &mul3(DVec2 &ret,const FMat4 &m,const DVec2 &v);
        DVec2 &mul3(DVec2 &ret,const DVec2 &v,const FMat4 &m);
        DVec3 &mul3(DVec3 &ret,const FMat4 &m,const DVec3 &v);
        DVec3 &mul3(DVec3 &ret,const DVec3 &v,const FMat4 &m);
        DVec4 &mul3(DVec4 &ret,const FMat4 &m,const DVec4 &v);
        DVec4 &mul3(DVec4 &ret,const DVec4 &v,const FMat4 &m);
        FMat4 &mul(FMat4 &ret,const FMat4 &m0,const FMat4 &m1);
        FMat4 &mul4(FMat4 &ret,const FMat4 &m0,const FMat4 &m1);
        FMat4 &mul3(FMat4 &ret,const FMat4 &m0,const FMat4 &m1);
        FMat4 &mult(FMat4 &ret,const FMat4 &m,const FVec3 &v);
        FVec3 &proj(FVec3 &ret,const FMat4 &m,const FVec3 &v);
        FVec4 &proj(FVec4 &ret,const FMat4 &m,const FVec4 &v);
        DVec3 &proj(DVec3 &ret,const FMat4 &m,const DVec3 &v);
        DVec4 &proj(DVec4 &ret,const FMat4 &m,const DVec4 &v);
        FMat4 &add(FMat4 &ret,const FMat4 &m0,const FMat4 &m1);
        FMat4 &sub(FMat4 &ret,const FMat4 &m0,const FMat4 &m1);
        FMat4 &orthonormalize(FMat4 &ret,const FMat4 &m);
        FMat4 &rotation(FMat4 &ret,const FMat4 &m);
        FMat4 &transpose(FMat4 &ret,const FMat4 &m);
        FMat4 &transpose3(FMat4 &ret,const FMat4 &m);
        FMat4 &inverse(FMat4 &ret,const FMat4 &m);
        FMat4 &inverse4(FMat4 &ret,const FMat4 &m);
        FMat4 &lerp(FMat4 &ret,const FMat4 &m0,const FMat4 &q1,f32 k);
        //
        FMat4 orthonormalize(const FMat4 &m);
        FMat4 rotation(const FMat4 &m);
        FMat4 transpose(const FMat4 &m);
        FMat4 transpose3(const FMat4 &m);
        FMat4 inverse(const FMat4 &m);
        FMat4 inverse4(const FMat4 &m);
        FMat4 lerp(const FMat4 &m0,const FMat4 &q1,f32 k);
        //
        FMat4 translate(const FVec3 &v);
        FMat4 translate(f32 x,f32 y,f32 z);
        FMat4 rotate(const FVec3 &axis,f32 angle);
        FMat4 rotate(f32 x,f32 y,f32 z,f32 angle);
        FMat4 rotate(const SVQuat &q);
        FMat4 rotateX(f32 angle);
        FMat4 rotateY(f32 angle);
        FMat4 rotateZ(f32 angle);
        FMat4 scale(const FVec3 &v);
        FMat4 scale(f32 x,f32 y,f32 z);
        //
        FMat4 reflect(const FVec4 &plane);
        FMat4 ortho(f32 left,f32 right,f32 bottom,f32 top,f32 znear,f32 zfar);
        FMat4 frustum(f32 left,f32 right,f32 bottom,f32 top,f32 znear,f32 zfar);
        FMat4 perspective(f32 fov,f32 aspect,f32 znear,f32 zfar);
        FMat4 setTo(const FVec3 &position,const FVec3 &direction,const FVec3 &up);
        FMat4 lookAt(const FVec3 &position,const FVec3 &direction,const FVec3 &up);
        FMat4 obliqueProjection(const FMat4 &projection,const FVec4 &plane);
        FMat4 symmetryProjection(const FMat4 &projection);
        FMat4 cubeTransform(s32 face);
        //
        void decomposeTransform(const FMat4 &m,FVec4 &position,SVQuat &rot);
        FMat4 &composeTransform(FMat4 &ret,const FVec4 &position,const SVQuat &rot);
        void decomposeTransform(const FMat4 &m,FVec3 &position,SVQuat &rot,FVec3 &scale);
        FMat4 &composeTransform(FMat4 &ret,const FVec3 &position,const SVQuat &rot,const FVec3 &scale);
        void decomposeProjection(const FMat4 &projection,f32 &znear,f32 &zfar);
        //
        const FMat4 &hardwareProjectionGL(const FMat4 &projection,s32 width,s32 height);
        const FMat4 &hardwareProjectionD3D9(const FMat4 &projection,s32 width,s32 height);
        const FMat4 &hardwareProjectionD3D10(const FMat4 &projection,s32 width,s32 height);
        extern const FMat4 &(*hardwareProjection)(const FMat4 &projection,s32 width,s32 height);
        
    }//!namespace util
    
}//!namespace sv


#endif /* __aMAT4_H__ */
