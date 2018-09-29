#include "SVMat4.h"
#include "SVMat4d.h"
#include "SVMat2.h"
#include "SVMat3.h"
#include "SVQuat.h"

namespace sv {
    
    namespace util {

        /******************************************************************************\
        *
        * FMat4
        *
        \******************************************************************************/

        //
        static const f32 FMat4_identity_values[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };

        //
        const FMat4 FMat4_zero(0.0f);
        const FMat4 FMat4_one(1.0f);
        const FMat4 FMat4_identity(FMat4_identity_values);

        //
        FMat4::FMat4(f32 v) {
            m00 = v; m01 = v; m02 = v; m03 = v;
            m10 = v; m11 = v; m12 = v; m13 = v;
            m20 = v; m21 = v; m22 = v; m23 = v;
            m30 = v; m31 = v; m32 = v; m33 = v;
        }

        FMat4::FMat4(const FMat2 &m) {
            m00 = m.m00; m01 = m.m01; m02 = 0.0f; m03 = 0.0f;
            m10 = m.m10; m11 = m.m11; m12 = 0.0f; m13 = 0.0f;
            m20 = 0.0f;  m21 = 0.0f;  m22 = 1.0f; m23 = 0.0f;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f; m33 = 1.0f;
        }

        FMat4::FMat4(const FMat3 &m) {
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = 0.0f;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = 0.0f;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = 0.0f;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;  m33 = 1.0f;
        }

        FMat4::FMat4(const FMat4 &m) {
            #if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                col0 = m.col0;
                col1 = m.col1;
                col2 = m.col2;
                col3 = m.col3;
            #else
                m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = m.m03;
                m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = m.m13;
                m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = m.m23;
                m30 = m.m30; m31 = m.m31; m32 = m.m32; m33 = m.m33;
            #endif
        }

        FMat4::FMat4(const DMat4 &m) {
            m00 = (f32)m.m00; m01 = (f32)m.m01; m02 = (f32)m.m02; m03 = (f32)m.m03;
            m10 = (f32)m.m10; m11 = (f32)m.m11; m12 = (f32)m.m12; m13 = (f32)m.m13;
            m20 = (f32)m.m20; m21 = (f32)m.m21; m22 = (f32)m.m22; m23 = (f32)m.m23;
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
        }

        FMat4::FMat4(const SVQuat &q) {
            FMat3 m = q.getMat3();
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = 0.0f;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = 0.0f;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = 0.0f;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;  m33 = 1.0f;
        }

        FMat4::FMat4(const f32 *m) {
            m00 = m[0]; m01 = m[4]; m02 = m[8];  m03 = m[12];
            m10 = m[1]; m11 = m[5]; m12 = m[9];  m13 = m[13];
            m20 = m[2]; m21 = m[6]; m22 = m[10]; m23 = m[14];
            m30 = m[3]; m31 = m[7]; m32 = m[11]; m33 = m[15];
        }

        FMat4::FMat4(const FMat3 &m,const FVec3 &v) {
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = v.x;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = v.y;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = v.z;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;  m33 = 1.0f;
        }

        FMat4::FMat4(const SVQuat &q,const FVec3 &v) {
            FMat3 m = q.getMat3();
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = v.x;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = v.y;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = v.z;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;  m33 = 1.0f;
        }

        //
        FMat4 FMat4::operator-() const {
            FMat4 ret;
            ret.m00 = -m00; ret.m01 = -m01; ret.m02 = -m02; ret.m03 = -m03;
            ret.m10 = -m10; ret.m11 = -m11; ret.m12 = -m12; ret.m13 = -m13;
            ret.m20 = -m20; ret.m21 = -m21; ret.m22 = -m22; ret.m23 = -m23;
            ret.m30 = -m30; ret.m31 = -m31; ret.m32 = -m32; ret.m33 = -m33;
            return ret;
        }

        FMat4 &FMat4::operator*=(f32 v) {
            return mul(*this,*this,v);
        }

        FMat4 &FMat4::operator*=(const FMat4 &m) {
            return mul(*this,FMat4(*this),m);
        }

        FMat4 &FMat4::operator+=(const FMat4 &m) {
            return add(*this,*this,m);
        }

        FMat4 &FMat4::operator-=(const FMat4 &m) {
            return sub(*this,*this,m);
        }

        //
        void FMat4::set(const FMat2 &m) {
            m00 = m.m00; m01 = m.m01; m02 = 0.0f; m03 = 0.0f;
            m10 = m.m10; m11 = m.m11; m12 = 0.0f; m13 = 0.0f;
            m20 = 0.0f;  m21 = 0.0f;  m22 = 1.0f; m23 = 0.0f;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f; m33 = 1.0f;
        }

        void FMat4::set(const FMat3 &m) {
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = 0.0f;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = 0.0f;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = 0.0f;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;  m33 = 1.0f;
        }

        void FMat4::set(const FMat4 &m) {
            #if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                col0 = m.col0;
                col1 = m.col1;
                col2 = m.col2;
                col3 = m.col3;
            #else
                m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = m.m03;
                m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = m.m13;
                m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = m.m23;
                m30 = m.m30; m31 = m.m31; m32 = m.m32; m33 = m.m33;
            #endif
        }

        void FMat4::set(const DMat4 &m) {
            m00 = (f32)m.m00; m01 = (f32)m.m01; m02 = (f32)m.m02; m03 = (f32)m.m03;
            m10 = (f32)m.m10; m11 = (f32)m.m11; m12 = (f32)m.m12; m13 = (f32)m.m13;
            m20 = (f32)m.m20; m21 = (f32)m.m21; m22 = (f32)m.m22; m23 = (f32)m.m23;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;  m33 = 1.0f;
        }

        void FMat4::set(const SVQuat &q) {
            FMat3 m = q.getMat3();
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = 0.0f;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = 0.0f;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = 0.0f;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;  m33 = 1.0f;
        }

        void FMat4::set(const f32 *m) {
            m00 = m[0]; m01 = m[4]; m02 = m[8];  m03 = m[12];
            m10 = m[1]; m11 = m[5]; m12 = m[9];  m13 = m[13];
            m20 = m[2]; m21 = m[6]; m22 = m[10]; m23 = m[14];
            m30 = m[3]; m31 = m[7]; m32 = m[11]; m33 = m[15];
        }

        void FMat4::set(const FMat3 &m,const FVec3 &v) {
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = v.x;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = v.y;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = v.z;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;  m33 = 1.0f;
        }

        void FMat4::set(const SVQuat &q,const FVec3 &v) {
            FMat3 m = q.getMat3();
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = v.x;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = v.y;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = v.z;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;  m33 = 1.0f;
        }

        void FMat4::get(f32 *m) const {
            m[0] = m00; m[4] = m01; m[8] = m02;  m[12] = m03;
            m[1] = m10; m[5] = m11; m[9] = m12;  m[13] = m13;
            m[2] = m20; m[6] = m21; m[10] = m22; m[14] = m23;
            m[3] = m30; m[7] = m31; m[11] = m32; m[15] = m33;
        }

        //
        void FMat4::setRow(s32 row,const FVec4 &v) {
            assert((u32)row < 4 && "FMat4::setRow(): bad row");
            mat[row + 0] = v.x;
            mat[row + 4] = v.y;
            mat[row + 8] = v.z;
            mat[row + 12] = v.w;
        }

        void FMat4::setRow3(s32 row,const FVec3 &v) {
            assert((u32)row < 4 && "FMat4::setRow3(): bad row");
            mat[row + 0] = v.x;
            mat[row + 4] = v.y;
            mat[row + 8] = v.z;
        }

        FVec4 FMat4::getRow(s32 row) const {
            assert((u32)row < 4 && "FMat4::getRow(): bad row");
            return FVec4(mat[row + 0],mat[row + 4],mat[row + 8],mat[row + 12]);
        }

        FVec3 FMat4::getRow3(s32 row) const {
            assert((u32)row < 4 && "FMat4::getRow3(): bad row");
            return FVec3(mat[row + 0],mat[row + 4],mat[row + 8]);
        }

        //
        void FMat4::setColumn(s32 column,const FVec4 &v) {
            assert((u32)column < 4 && "FMat4::setColumn(): bad column");
            column *= 4;
            mat[column + 0] = v.x;
            mat[column + 1] = v.y;
            mat[column + 2] = v.z;
            mat[column + 3] = v.w;
        }

        void FMat4::setColumn3(s32 column,const FVec3 &v) {
            assert((u32)column < 4 && "FMat4::setColumn3(): bad column");
            column *= 4;
            mat[column + 0] = v.x;
            mat[column + 1] = v.y;
            mat[column + 2] = v.z;
        }

        FVec4 FMat4::getColumn(s32 column) const {
            assert((u32)column < 4 && "FMat4::getColumn(): bad column");
            return FVec4(mat + column * 4);
        }

        FVec3 FMat4::getColumn3(s32 column) const {
            assert((u32)column < 4 && "FMat4::getColumn3(): bad column");
            return FVec3(mat + column * 4);
        }

        //
        void FMat4::setDiagonal(const FVec4 &v) {
            m00 = v.x;  m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
            m10 = 0.0f; m11 = v.y;  m12 = 0.0f; m13 = 0.0f;
            m20 = 0.0f; m21 = 0.0f; m22 = v.z;  m23 = 0.0f;
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = v.w;
        }

        FVec4 FMat4::getDiagonal() const {
            return FVec4(m00,m11,m22,m33);
        }

        //
        void FMat4::setZero() {
            m00 = 0.0f; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
            m10 = 0.0f; m11 = 0.0f; m12 = 0.0f; m13 = 0.0f;
            m20 = 0.0f; m21 = 0.0f; m22 = 0.0f; m23 = 0.0f;
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 0.0f;
        }

        void FMat4::setIdentity() {
            m00 = 1.0f; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
            m10 = 0.0f; m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
            m20 = 0.0f; m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
        }

        void FMat4::setTranslate(const FVec3 &v) {
            m00 = 1.0f; m01 = 0.0f; m02 = 0.0f; m03 = v.x;
            m10 = 0.0f; m11 = 1.0f; m12 = 0.0f; m13 = v.y;
            m20 = 0.0f; m21 = 0.0f; m22 = 1.0f; m23 = v.z;
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
        }

        void FMat4::setRotate(const FVec3 &axis,f32 angle) {
            f32 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            FVec3 v = normalize(axis);
            f32 xx = v.x * v.x;
            f32 yy = v.y * v.y;
            f32 zz = v.z * v.z;
            f32 xy = v.x * v.y;
            f32 yz = v.y * v.z;
            f32 zx = v.z * v.x;
            f32 xs = v.x * s;
            f32 ys = v.y * s;
            f32 zs = v.z * s;
            m00 = (1.0f - c) * xx + c;  m01 = (1.0f - c) * xy - zs; m02 = (1.0f - c) * zx + ys; m03 = 0.0f;
            m10 = (1.0f - c) * xy + zs; m11 = (1.0f - c) * yy + c;  m12 = (1.0f - c) * yz - xs; m13 = 0.0f;
            m20 = (1.0f - c) * zx - ys; m21 = (1.0f - c) * yz + xs; m22 = (1.0f - c) * zz + c;  m23 = 0.0f;
            m30 = 0.0f;                 m31 = 0.0f;                 m32 = 0.0f;                 m33 = 1.0f;
        }

        void FMat4::setRotateX(f32 angle) {
            f32 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            m00 = 1.0f; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
            m10 = 0.0f; m11 = c;    m12 = -s;   m13 = 0.0f;
            m20 = 0.0f; m21 = s;    m22 = c;    m23 = 0.0f;
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
        }

        void FMat4::setRotateY(f32 angle) {
            f32 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            m00 = c;    m01 = 0.0f; m02 = s;    m03 = 0.0f;
            m10 = 0.0f; m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
            m20 = -s;   m21 = 0.0f; m22 = c;    m23 = 0.0f;
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
        }

        void FMat4::setRotateZ(f32 angle) {
            f32 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            m00 = c;    m01 = -s;   m02 = 0.0f; m03 = 0.0f;
            m10 = s;    m11 = c;    m12 = 0.0f; m13 = 0.0f;
            m20 = 0.0f; m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
        }

        void FMat4::setScale(const FVec3 &v) {
            m00 = v.x;  m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
            m10 = 0.0f; m11 = v.y;  m12 = 0.0f; m13 = 0.0f;
            m20 = 0.0f; m21 = 0.0f; m22 = v.z;  m23 = 0.0f;
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
        }

        //
        s32 operator==(const FMat4 &m0,const FMat4 &m1) {
            return compare(m0,m1);
        }

        s32 operator!=(const FMat4 &m0,const FMat4 &m1) {
            return !compare(m0,m1);
        }

        FMat4 operator*(const FMat4 &m,f32 v) {
            FMat4 ret;
            return mul(ret,m,v);
        }

        FVec2 operator*(const FMat4 &m,const FVec2 &v) {
            FVec2 ret;
            return mul(ret,m,v);
        }

        FVec2 operator*(const FVec2 &v,const FMat4 &m) {
            FVec2 ret;
            return mul(ret,v,m);
        }

        FVec3 operator*(const FMat4 &m,const FVec3 &v) {
            FVec3 ret;
            return mul(ret,m,v);
        }

        FVec3 operator*(const FVec3 &v,const FMat4 &m) {
            FVec3 ret;
            return mul(ret,v,m);
        }

        FVec4 operator*(const FMat4 &m,const FVec4 &v) {
            FVec4 ret;
            return mul(ret,m,v);
        }

        FVec4 operator*(const FVec4 &v,const FMat4 &m) {
            FVec4 ret;
            return mul(ret,v,m);
        }

        DVec2 operator*(const FMat4 &m,const DVec2 &v) {
            DVec2 ret;
            return mul(ret,m,v);
        }

        DVec2 operator*(const DVec2 &v,const FMat4 &m) {
            DVec2 ret;
            return mul(ret,v,m);
        }

        DVec3 operator*(const FMat4 &m,const DVec3 &v) {
            DVec3 ret;
            return mul(ret,m,v);
        }

        DVec3 operator*(const DVec3 &v,const FMat4 &m) {
            DVec3 ret;
            return mul(ret,v,m);
        }

        DVec4 operator*(const FMat4 &m,const DVec4 &v) {
            DVec4 ret;
            return mul(ret,m,v);
        }

        DVec4 operator*(const DVec4 &v,const FMat4 &m) {
            DVec4 ret;
            return mul(ret,v,m);
        }

        FMat4 operator*(const FMat4 &m0,const FMat4 &m1) {
            FMat4 ret;
            return mul(ret,m0,m1);
        }

        FMat4 operator+(const FMat4 &m0,const FMat4 &m1) {
            FMat4 ret;
            return add(ret,m0,m1);
        }

        FMat4 operator-(const FMat4 &m0,const FMat4 &m1) {
            FMat4 ret;
            return sub(ret,m0,m1);
        }

        //
        s32 compare(const FMat4 &m0,const FMat4 &m1) {
            return (compare(m0.m00,m1.m00) && compare(m0.m10,m1.m10) && compare(m0.m20,m1.m20) && compare(m0.m30,m1.m30) &&
                compare(m0.m01,m1.m01) && compare(m0.m11,m1.m11) && compare(m0.m21,m1.m21) && compare(m0.m31,m1.m31) &&
                compare(m0.m02,m1.m02) && compare(m0.m12,m1.m12) && compare(m0.m22,m1.m22) && compare(m0.m32,m1.m32) &&
                compare(m0.m03,m1.m03) && compare(m0.m13,m1.m13) && compare(m0.m23,m1.m23) && compare(m0.m33,m1.m33));
        }

        s32 compare(const FMat4 &m0,const FMat4 &m1,f32 epsilon) {
            return (compare(m0.m00,m1.m00,epsilon) && compare(m0.m10,m1.m10,epsilon) && compare(m0.m20,m1.m20,epsilon) && compare(m0.m30,m1.m30,epsilon) &&
                compare(m0.m01,m1.m01,epsilon) && compare(m0.m11,m1.m11,epsilon) && compare(m0.m21,m1.m21,epsilon) && compare(m0.m31,m1.m31,epsilon) &&
                compare(m0.m02,m1.m02,epsilon) && compare(m0.m12,m1.m12,epsilon) && compare(m0.m22,m1.m22,epsilon) && compare(m0.m32,m1.m32,epsilon) &&
                compare(m0.m03,m1.m03,epsilon) && compare(m0.m13,m1.m13,epsilon) && compare(m0.m23,m1.m23,epsilon) && compare(m0.m33,m1.m33,epsilon));
        }

        f32 trace(const FMat4 &m) {
            return m.m00 + m.m11 + m.m22 + m.m33;
        }

        f32 determinant(const FMat4 &m) {
            f32 det = 0.0f;
            det =  m.m00 * (m.m11 * (m.m22 * m.m33 - m.m23 * m.m32) - m.m12 * (m.m21 * m.m33 - m.m23 * m.m31) + m.m13 * (m.m21 * m.m32 - m.m22 * m.m31));
            det -= m.m01 * (m.m10 * (m.m22 * m.m33 - m.m23 * m.m32) - m.m12 * (m.m20 * m.m33 - m.m23 * m.m30) + m.m13 * (m.m20 * m.m32 - m.m22 * m.m30));
            det += m.m02 * (m.m10 * (m.m21 * m.m33 - m.m23 * m.m31) - m.m11 * (m.m20 * m.m33 - m.m23 * m.m30) + m.m13 * (m.m20 * m.m31 - m.m21 * m.m30));
            det -= m.m03 * (m.m10 * (m.m21 * m.m32 - m.m22 * m.m31) - m.m11 * (m.m20 * m.m32 - m.m22 * m.m30) + m.m12 * (m.m20 * m.m31 - m.m21 * m.m30));
            return det;
        }

        f32 determinant3(const FMat4 &m) {
            f32 det = 0.0f;
            det =  m.m00 * (m.m11 * m.m22 - m.m12 * m.m21);
            det -= m.m01 * (m.m10 * m.m22 - m.m12 * m.m20);
            det += m.m02 * (m.m10 * m.m21 - m.m11 * m.m20);
            return det;
        }

        FMat4 &mul(FMat4 &ret,const FMat4 &m,f32 v) {
            #ifdef USE_SSE
                __m128 temp = _mm_set1_ps(v);
                ret.col0 = _mm_mul_ps(m.col0,temp);
                ret.col1 = _mm_mul_ps(m.col1,temp);
                ret.col2 = _mm_mul_ps(m.col2,temp);
                ret.col3 = _mm_mul_ps(m.col3,temp);
            #elif USE_ALTIVEC
                vec_float4 temp = vec_splats(v);
                vec_float4 zero = vec_splats(0.0f);
                ret.col0 = vec_madd(m.col0,temp,zero);
                ret.col1 = vec_madd(m.col1,temp,zero);
                ret.col2 = vec_madd(m.col2,temp,zero);
                ret.col3 = vec_madd(m.col3,temp,zero);
            #elif USE_NEON
                ret.col0 = vmulq_n_f32(m.col0,v);
                ret.col1 = vmulq_n_f32(m.col1,v);
                ret.col2 = vmulq_n_f32(m.col2,v);
                ret.col3 = vmulq_n_f32(m.col3,v);
            #else
                ret.m00 = m.m00 * v; ret.m01 = m.m01 * v; ret.m02 = m.m02 * v; ret.m03 = m.m03 * v;
                ret.m10 = m.m10 * v; ret.m11 = m.m11 * v; ret.m12 = m.m12 * v; ret.m13 = m.m13 * v;
                ret.m20 = m.m20 * v; ret.m21 = m.m21 * v; ret.m22 = m.m22 * v; ret.m23 = m.m23 * v;
                ret.m30 = m.m30 * v; ret.m31 = m.m31 * v; ret.m32 = m.m32 * v; ret.m33 = m.m33 * v;
            #endif
            return ret;
        }

        FVec2 &mul(FVec2 &ret,const FMat4 &m,const FVec2 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m03;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m13;
            return ret;
        }

        FVec2 &mul(FVec2 &ret,const FVec2 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m30;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m31;
            return ret;
        }

        FVec3 &mul(FVec3 &ret,const FMat4 &m,const FVec3 &v) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(m.col0,_MM_SWIZZLE(v.vec,X,X,X,W));
                __m128 res_1 = _mm_mul_ps(m.col1,_MM_SWIZZLE(v.vec,Y,Y,Y,W));
                __m128 res_2 = _mm_mul_ps(m.col2,_MM_SWIZZLE(v.vec,Z,Z,Z,W));
                ret.vec = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,m.col3));
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_madd(m.col0,VEC_SWIZZLE(v.vec,X,X,X,W),m.col3);
                vec_float4 res_1 = vec_madd(m.col1,VEC_SWIZZLE(v.vec,Y,Y,Y,W),res_0);
                ret.vec = vec_madd(m.col2,VEC_SWIZZLE(v.vec,Z,Z,Z,W),res_1);
            #elif USE_NEON
                float32x2_t low = vget_low_f32(v.vec);
                float32x2_t high = vget_high_f32(v.vec);
                float32x4_t res_0 = vmlaq_lane_f32(m.col3,m.col0,low,0);
                float32x4_t res_1 = vmlaq_lane_f32(res_0,m.col1,low,1);
                ret.vec = vmlaq_lane_f32(res_1,m.col2,high,0);
            #else
                ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03;
                ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13;
                ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23;
            #endif
            return ret;
        }

        FVec3 &mul(FVec3 &ret,const FVec3 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32;
            return ret;
        }

        FVec4 &mul(FVec4 &ret,const FMat4 &m,const FVec4 &v) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(m.col0,_MM_SWIZZLE(v.vec,X,X,X,X));
                __m128 res_1 = _mm_mul_ps(m.col1,_MM_SWIZZLE(v.vec,Y,Y,Y,Y));
                __m128 res_2 = _mm_mul_ps(m.col2,_MM_SWIZZLE(v.vec,Z,Z,Z,Z));
                __m128 res_3 = _mm_mul_ps(m.col3,_MM_SWIZZLE(v.vec,W,W,W,W));
                ret.vec = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,res_3));
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_madd(m.col0,VEC_SWIZZLE(v.vec,X,X,X,X),vec_splats(0.0f));
                vec_float4 res_1 = vec_madd(m.col1,VEC_SWIZZLE(v.vec,Y,Y,Y,Y),res_0);
                vec_float4 res_2 = vec_madd(m.col2,VEC_SWIZZLE(v.vec,Z,Z,Z,Z),res_1);
                ret.vec = vec_madd(m.col3,VEC_SWIZZLE(v.vec,W,W,W,W),res_2);
            #elif USE_NEON
                float32x2_t low = vget_low_f32(v.vec);
                float32x2_t high = vget_high_f32(v.vec);
                float32x4_t res_0 = vmulq_lane_f32(m.col0,low,0);
                float32x4_t res_1 = vmlaq_lane_f32(res_0,m.col1,low,1);
                float32x4_t res_2 = vmlaq_lane_f32(res_1,m.col2,high,0);
                ret.vec = vmlaq_lane_f32(res_2,m.col3,high,1);
            #else
                ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w;
                ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w;
                ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w;
                ret.w = m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w;
            #endif
            return ret;
        }

        FVec4 &mul(FVec4 &ret,const FVec4 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w;
            ret.w = m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w;
            return ret;
        }

        DVec2 &mul(DVec2 &ret,const FMat4 &m,const DVec2 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m03;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m13;
            return ret;
        }

        DVec2 &mul(DVec2 &ret,const DVec2 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m30;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m31;
            return ret;
        }

        DVec3 &mul(DVec3 &ret,const FMat4 &m,const DVec3 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13;
            ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23;
            return ret;
        }

        DVec3 &mul(DVec3 &ret,const DVec3 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32;
            return ret;
        }

        DVec4 &mul(DVec4 &ret,const FMat4 &m,const DVec4 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w;
            ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w;
            ret.w = m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w;
            return ret;
        }

        DVec4 &mul(DVec4 &ret,const DVec4 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w;
            ret.w = m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w;
            return ret;
        }

        FVec2 &mul3(FVec2 &ret,const FMat4 &m,const FVec2 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y;
            ret.y = m.m10 * v.x + m.m11 * v.y;
            return ret;
        }

        FVec2 &mul3(FVec2 &ret,const FVec2 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y;
            ret.y = m.m01 * v.x + m.m11 * v.y;
            return ret;
        }

        FVec3 &mul3(FVec3 &ret,const FMat4 &m,const FVec3 &v) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(m.col0,_MM_SWIZZLE(v.vec,X,X,X,W));
                __m128 res_1 = _mm_mul_ps(m.col1,_MM_SWIZZLE(v.vec,Y,Y,Y,W));
                __m128 res_2 = _mm_mul_ps(m.col2,_MM_SWIZZLE(v.vec,Z,Z,Z,W));
                ret.vec = _mm_add_ps(_mm_add_ps(res_0,res_1),res_2);
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_madd(m.col0,VEC_SWIZZLE(v.vec,X,X,X,W),vec_splats(0.0f));
                vec_float4 res_1 = vec_madd(m.col1,VEC_SWIZZLE(v.vec,Y,Y,Y,W),res_0);
                ret.vec = vec_madd(m.col2,VEC_SWIZZLE(v.vec,Z,Z,Z,W),res_1);
            #elif USE_NEON
                float32x2_t low = vget_low_f32(v.vec);
                float32x2_t high = vget_high_f32(v.vec);
                float32x4_t res_0 = vmulq_lane_f32(m.col0,low,0);
                float32x4_t res_1 = vmlaq_lane_f32(res_0,m.col1,low,1);
                ret.vec = vmlaq_lane_f32(res_1,m.col2,high,0);
            #else
                ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z;
                ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
                ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;
            #endif
            return ret;
        }

        FVec3 &mul3(FVec3 &ret,const FVec3 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
            return ret;
        }

        FVec4 &mul3(FVec4 &ret,const FMat4 &m,const FVec4 &v) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(m.col0,_MM_SWIZZLE(v.vec,X,X,X,W));
                __m128 res_1 = _mm_mul_ps(m.col1,_MM_SWIZZLE(v.vec,Y,Y,Y,W));
                __m128 res_2 = _mm_mul_ps(m.col2,_MM_SWIZZLE(v.vec,Z,Z,Z,W));
                ret.vec = _mm_add_ps(_mm_add_ps(res_0,res_1),res_2);
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_madd(m.col0,VEC_SWIZZLE(v.vec,X,X,X,W),vec_splats(0.0f));
                vec_float4 res_1 = vec_madd(m.col1,VEC_SWIZZLE(v.vec,Y,Y,Y,W),res_0);
                ret.vec = vec_madd(m.col2,VEC_SWIZZLE(v.vec,Z,Z,Z,W),res_1);
            #elif USE_NEON
                float32x2_t low = vget_low_f32(v.vec);
                float32x2_t high = vget_high_f32(v.vec);
                float32x4_t res_0 = vmulq_lane_f32(m.col0,low,0);
                float32x4_t res_1 = vmlaq_lane_f32(res_0,m.col1,low,1);
                ret.vec = vmlaq_lane_f32(res_1,m.col2,high,0);
            #else
                ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z;
                ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
                ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;
            #endif
            return ret;
        }

        FVec4 &mul3(FVec4 &ret,const FVec4 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
            return ret;
        }

        DVec2 &mul3(DVec2 &ret,const FMat4 &m,const DVec2 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y;
            ret.y = m.m10 * v.x + m.m11 * v.y;
            return ret;
        }

        DVec2 &mul3(DVec2 &ret,const DVec2 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y;
            ret.y = m.m01 * v.x + m.m11 * v.y;
            return ret;
        }

        DVec3 &mul3(DVec3 &ret,const FMat4 &m,const DVec3 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
            ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;
            return ret;
        }

        DVec3 &mul3(DVec3 &ret,const DVec3 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
            return ret;
        }

        DVec4 &mul3(DVec4 &ret,const FMat4 &m,const DVec4 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
            ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;
            return ret;
        }

        DVec4 &mul3(DVec4 &ret,const DVec4 &v,const FMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
            return ret;
        }

        FMat4 &mul(FMat4 &ret,const FMat4 &m0,const FMat4 &m1) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(m0.col0,_MM_SWIZZLE(m1.col0,X,X,X,X));
                __m128 res_1 = _mm_mul_ps(m0.col1,_MM_SWIZZLE(m1.col0,Y,Y,Y,Y));
                __m128 res_2 = _mm_mul_ps(m0.col2,_MM_SWIZZLE(m1.col0,Z,Z,Z,Z));
                __m128 res_3 = _mm_mul_ps(m0.col3,_MM_SWIZZLE(m1.col0,W,W,W,W));
                ret.col0 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,res_3));
                res_0 = _mm_mul_ps(m0.col0,_MM_SWIZZLE(m1.col1,X,X,X,X));
                res_1 = _mm_mul_ps(m0.col1,_MM_SWIZZLE(m1.col1,Y,Y,Y,Y));
                res_2 = _mm_mul_ps(m0.col2,_MM_SWIZZLE(m1.col1,Z,Z,Z,Z));
                res_3 = _mm_mul_ps(m0.col3,_MM_SWIZZLE(m1.col1,W,W,W,W));
                ret.col1 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,res_3));
                res_0 = _mm_mul_ps(m0.col0,_MM_SWIZZLE(m1.col2,X,X,X,X));
                res_1 = _mm_mul_ps(m0.col1,_MM_SWIZZLE(m1.col2,Y,Y,Y,Y));
                res_2 = _mm_mul_ps(m0.col2,_MM_SWIZZLE(m1.col2,Z,Z,Z,Z));
                res_3 = _mm_mul_ps(m0.col3,_MM_SWIZZLE(m1.col2,W,W,W,W));
                ret.col2 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,res_3));
                res_0 = _mm_mul_ps(m0.col0,_MM_SWIZZLE(m1.col3,X,X,X,X));
                res_1 = _mm_mul_ps(m0.col1,_MM_SWIZZLE(m1.col3,Y,Y,Y,Y));
                res_2 = _mm_mul_ps(m0.col2,_MM_SWIZZLE(m1.col3,Z,Z,Z,Z));
                res_3 = _mm_mul_ps(m0.col3,_MM_SWIZZLE(m1.col3,W,W,W,W));
                ret.col3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,res_3));
            #elif USE_ALTIVEC
                vec_float4 zero = vec_splats(0.0f);
                vec_uchar16 xxxx = VEC_PERM2(X,X,X,X);
                vec_uchar16 yyyy = VEC_PERM2(Y,Y,Y,Y);
                vec_uchar16 zzzz = VEC_PERM2(Z,Z,Z,Z);
                vec_uchar16 wwww = VEC_PERM2(W,W,W,W);
                ret.col0 = vec_madd(m0.col0,vec_perm(m1.col0,m1.col0,xxxx),zero);
                ret.col1 = vec_madd(m0.col0,vec_perm(m1.col1,m1.col1,xxxx),zero);
                ret.col2 = vec_madd(m0.col0,vec_perm(m1.col2,m1.col2,xxxx),zero);
                ret.col3 = vec_madd(m0.col0,vec_perm(m1.col3,m1.col3,xxxx),zero);
                ret.col0 = vec_madd(m0.col1,vec_perm(m1.col0,m1.col0,yyyy),ret.col0);
                ret.col1 = vec_madd(m0.col1,vec_perm(m1.col1,m1.col1,yyyy),ret.col1);
                ret.col2 = vec_madd(m0.col1,vec_perm(m1.col2,m1.col2,yyyy),ret.col2);
                ret.col3 = vec_madd(m0.col1,vec_perm(m1.col3,m1.col3,yyyy),ret.col3);
                ret.col0 = vec_madd(m0.col2,vec_perm(m1.col0,m1.col0,zzzz),ret.col0);
                ret.col1 = vec_madd(m0.col2,vec_perm(m1.col1,m1.col1,zzzz),ret.col1);
                ret.col2 = vec_madd(m0.col2,vec_perm(m1.col2,m1.col2,zzzz),ret.col2);
                ret.col3 = vec_madd(m0.col2,vec_perm(m1.col3,m1.col3,zzzz),ret.col3);
                ret.col0 = vec_madd(m0.col3,vec_perm(m1.col0,m1.col0,wwww),ret.col0);
                ret.col1 = vec_madd(m0.col3,vec_perm(m1.col1,m1.col1,wwww),ret.col1);
                ret.col2 = vec_madd(m0.col3,vec_perm(m1.col2,m1.col2,wwww),ret.col2);
                ret.col3 = vec_madd(m0.col3,vec_perm(m1.col3,m1.col3,wwww),ret.col3);
            #elif USE_NEON
                float32x2_t low_0 = vget_low_f32(m1.col0);
                float32x2_t low_1 = vget_low_f32(m1.col1);
                float32x2_t low_2 = vget_low_f32(m1.col2);
                float32x2_t low_3 = vget_low_f32(m1.col3);
                ret.col0 = vmulq_lane_f32(m0.col0,low_0,0);
                ret.col1 = vmulq_lane_f32(m0.col0,low_1,0);
                ret.col2 = vmulq_lane_f32(m0.col0,low_2,0);
                ret.col3 = vmulq_lane_f32(m0.col0,low_3,0);
                ret.col0 = vmlaq_lane_f32(ret.col0,m0.col1,low_0,1);
                ret.col1 = vmlaq_lane_f32(ret.col1,m0.col1,low_1,1);
                ret.col2 = vmlaq_lane_f32(ret.col2,m0.col1,low_2,1);
                ret.col3 = vmlaq_lane_f32(ret.col3,m0.col1,low_3,1);
                float32x2_t high_0 = vget_high_f32(m1.col0);
                float32x2_t high_1 = vget_high_f32(m1.col1);
                float32x2_t high_2 = vget_high_f32(m1.col2);
                float32x2_t high_3 = vget_high_f32(m1.col3);
                ret.col0 = vmlaq_lane_f32(ret.col0,m0.col2,high_0,0);
                ret.col1 = vmlaq_lane_f32(ret.col1,m0.col2,high_1,0);
                ret.col2 = vmlaq_lane_f32(ret.col2,m0.col2,high_2,0);
                ret.col3 = vmlaq_lane_f32(ret.col3,m0.col2,high_3,0);
                ret.col0 = vmlaq_lane_f32(ret.col0,m0.col3,high_0,1);
                ret.col1 = vmlaq_lane_f32(ret.col1,m0.col3,high_1,1);
                ret.col2 = vmlaq_lane_f32(ret.col2,m0.col3,high_2,1);
                ret.col3 = vmlaq_lane_f32(ret.col3,m0.col3,high_3,1);
            #else
                ret.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10 + m0.m02 * m1.m20 + m0.m03 * m1.m30;
                ret.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10 + m0.m12 * m1.m20 + m0.m13 * m1.m30;
                ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20 + m0.m23 * m1.m30;
                ret.m30 = m0.m30 * m1.m00 + m0.m31 * m1.m10 + m0.m32 * m1.m20 + m0.m33 * m1.m30;
                ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21 + m0.m03 * m1.m31;
                ret.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11 + m0.m12 * m1.m21 + m0.m13 * m1.m31;
                ret.m21 = m0.m20 * m1.m01 + m0.m21 * m1.m11 + m0.m22 * m1.m21 + m0.m23 * m1.m31;
                ret.m31 = m0.m30 * m1.m01 + m0.m31 * m1.m11 + m0.m32 * m1.m21 + m0.m33 * m1.m31;
                ret.m02 = m0.m00 * m1.m02 + m0.m01 * m1.m12 + m0.m02 * m1.m22 + m0.m03 * m1.m32;
                ret.m12 = m0.m10 * m1.m02 + m0.m11 * m1.m12 + m0.m12 * m1.m22 + m0.m13 * m1.m32;
                ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22 + m0.m23 * m1.m32;
                ret.m32 = m0.m30 * m1.m02 + m0.m31 * m1.m12 + m0.m32 * m1.m22 + m0.m33 * m1.m32;
                ret.m03 = m0.m00 * m1.m03 + m0.m01 * m1.m13 + m0.m02 * m1.m23 + m0.m03 * m1.m33;
                ret.m13 = m0.m10 * m1.m03 + m0.m11 * m1.m13 + m0.m12 * m1.m23 + m0.m13 * m1.m33;
                ret.m23 = m0.m20 * m1.m03 + m0.m21 * m1.m13 + m0.m22 * m1.m23 + m0.m23 * m1.m33;
                ret.m33 = m0.m30 * m1.m03 + m0.m31 * m1.m13 + m0.m32 * m1.m23 + m0.m33 * m1.m33;
            #endif
            return ret;
        }

        FMat4 &mul4(FMat4 &ret,const FMat4 &m0,const FMat4 &m1) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(m0.col0,_MM_SWIZZLE(m1.col0,X,X,X,X));
                __m128 res_1 = _mm_mul_ps(m0.col1,_MM_SWIZZLE(m1.col0,Y,Y,Y,Y));
                __m128 res_2 = _mm_mul_ps(m0.col2,_MM_SWIZZLE(m1.col0,Z,Z,Z,Z));
                ret.col0 = _mm_add_ps(_mm_add_ps(res_0,res_1),res_2);
                res_0 = _mm_mul_ps(m0.col0,_MM_SWIZZLE(m1.col1,X,X,X,X));
                res_1 = _mm_mul_ps(m0.col1,_MM_SWIZZLE(m1.col1,Y,Y,Y,Y));
                res_2 = _mm_mul_ps(m0.col2,_MM_SWIZZLE(m1.col1,Z,Z,Z,Z));
                ret.col1 = _mm_add_ps(_mm_add_ps(res_0,res_1),res_2);
                res_0 = _mm_mul_ps(m0.col0,_MM_SWIZZLE(m1.col2,X,X,X,X));
                res_1 = _mm_mul_ps(m0.col1,_MM_SWIZZLE(m1.col2,Y,Y,Y,Y));
                res_2 = _mm_mul_ps(m0.col2,_MM_SWIZZLE(m1.col2,Z,Z,Z,Z));
                ret.col2 = _mm_add_ps(_mm_add_ps(res_0,res_1),res_2);
                res_0 = _mm_mul_ps(m0.col0,_MM_SWIZZLE(m1.col3,X,X,X,X));
                res_1 = _mm_mul_ps(m0.col1,_MM_SWIZZLE(m1.col3,Y,Y,Y,Y));
                res_2 = _mm_mul_ps(m0.col2,_MM_SWIZZLE(m1.col3,Z,Z,Z,Z));
                ret.col3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,m0.col3));
            #elif USE_ALTIVEC
                vec_float4 zero = vec_splats(0.0f);
                vec_uchar16 xxxx = VEC_PERM2(X,X,X,X);
                vec_uchar16 yyyy = VEC_PERM2(Y,Y,Y,Y);
                vec_uchar16 zzzz = VEC_PERM2(Z,Z,Z,Z);
                ret.col0 = vec_madd(m0.col0,vec_perm(m1.col0,m1.col0,xxxx),zero);
                ret.col1 = vec_madd(m0.col0,vec_perm(m1.col1,m1.col1,xxxx),zero);
                ret.col2 = vec_madd(m0.col0,vec_perm(m1.col2,m1.col2,xxxx),zero);
                ret.col3 = vec_madd(m0.col0,vec_perm(m1.col3,m1.col3,xxxx),m0.col3);
                ret.col0 = vec_madd(m0.col1,vec_perm(m1.col0,m1.col0,yyyy),ret.col0);
                ret.col1 = vec_madd(m0.col1,vec_perm(m1.col1,m1.col1,yyyy),ret.col1);
                ret.col2 = vec_madd(m0.col1,vec_perm(m1.col2,m1.col2,yyyy),ret.col2);
                ret.col3 = vec_madd(m0.col1,vec_perm(m1.col3,m1.col3,yyyy),ret.col3);
                ret.col0 = vec_madd(m0.col2,vec_perm(m1.col0,m1.col0,zzzz),ret.col0);
                ret.col1 = vec_madd(m0.col2,vec_perm(m1.col1,m1.col1,zzzz),ret.col1);
                ret.col2 = vec_madd(m0.col2,vec_perm(m1.col2,m1.col2,zzzz),ret.col2);
                ret.col3 = vec_madd(m0.col2,vec_perm(m1.col3,m1.col3,zzzz),ret.col3);
            #elif USE_NEON
                float32x2_t low_0 = vget_low_f32(m1.col0);
                float32x2_t low_1 = vget_low_f32(m1.col1);
                float32x2_t low_2 = vget_low_f32(m1.col2);
                float32x2_t low_3 = vget_low_f32(m1.col3);
                ret.col0 = vmulq_lane_f32(m0.col0,low_0,0);
                ret.col1 = vmulq_lane_f32(m0.col0,low_1,0);
                ret.col2 = vmulq_lane_f32(m0.col0,low_2,0);
                ret.col3 = vmlaq_lane_f32(m0.col3,m0.col0,low_3,0);
                ret.col0 = vmlaq_lane_f32(ret.col0,m0.col1,low_0,1);
                ret.col1 = vmlaq_lane_f32(ret.col1,m0.col1,low_1,1);
                ret.col2 = vmlaq_lane_f32(ret.col2,m0.col1,low_2,1);
                ret.col3 = vmlaq_lane_f32(ret.col3,m0.col1,low_3,1);
                float32x2_t high_0 = vget_high_f32(m1.col0);
                float32x2_t high_1 = vget_high_f32(m1.col1);
                float32x2_t high_2 = vget_high_f32(m1.col2);
                float32x2_t high_3 = vget_high_f32(m1.col3);
                ret.col0 = vmlaq_lane_f32(ret.col0,m0.col2,high_0,0);
                ret.col1 = vmlaq_lane_f32(ret.col1,m0.col2,high_1,0);
                ret.col2 = vmlaq_lane_f32(ret.col2,m0.col2,high_2,0);
                ret.col3 = vmlaq_lane_f32(ret.col3,m0.col2,high_3,0);
            #else
                ret.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10 + m0.m02 * m1.m20;
                ret.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10 + m0.m12 * m1.m20;
                ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
                ret.m30 = 0.0f;
                ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
                ret.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11 + m0.m12 * m1.m21;
                ret.m21 = m0.m20 * m1.m01 + m0.m21 * m1.m11 + m0.m22 * m1.m21;
                ret.m31 = 0.0f;
                ret.m02 = m0.m00 * m1.m02 + m0.m01 * m1.m12 + m0.m02 * m1.m22;
                ret.m12 = m0.m10 * m1.m02 + m0.m11 * m1.m12 + m0.m12 * m1.m22;
                ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
                ret.m32 = 0.0f;
                ret.m03 = m0.m00 * m1.m03 + m0.m01 * m1.m13 + m0.m02 * m1.m23 + m0.m03;
                ret.m13 = m0.m10 * m1.m03 + m0.m11 * m1.m13 + m0.m12 * m1.m23 + m0.m13;
                ret.m23 = m0.m20 * m1.m03 + m0.m21 * m1.m13 + m0.m22 * m1.m23 + m0.m23;
                ret.m33 = 1.0f;
            #endif
            return ret;
        }

        FMat4 &mul3(FMat4 &ret,const FMat4 &m0,const FMat4 &m1) {
            ret.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10 + m0.m02 * m1.m20;
            ret.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10 + m0.m12 * m1.m20;
            ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
            ret.m30 = 0.0f;
            ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
            ret.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11 + m0.m12 * m1.m21;
            ret.m21 = m0.m20 * m1.m01 + m0.m21 * m1.m11 + m0.m22 * m1.m21;
            ret.m31 = 0.0f;
            ret.m02 = m0.m00 * m1.m02 + m0.m01 * m1.m12 + m0.m02 * m1.m22;
            ret.m12 = m0.m10 * m1.m02 + m0.m11 * m1.m12 + m0.m12 * m1.m22;
            ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
            ret.m32 = 0.0f;
            ret.m03 = 0.0f;
            ret.m13 = 0.0f;
            ret.m23 = 0.0f;
            ret.m33 = 1.0f;
            return ret;
        }

        FMat4 &mult(FMat4 &ret,const FMat4 &m,const FVec3 &v) {
            #ifdef USE_SSE
                ret.col0 = m.col0;
                ret.col1 = m.col1;
                ret.col2 = m.col2;
                __m128 res_0 = _mm_mul_ps(m.col0,_MM_SWIZZLE(v.vec,X,X,X,X));
                __m128 res_1 = _mm_mul_ps(m.col1,_MM_SWIZZLE(v.vec,Y,Y,Y,Y));
                __m128 res_2 = _mm_mul_ps(m.col2,_MM_SWIZZLE(v.vec,Z,Z,Z,Z));
                ret.col3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,m.col3));
            #elif USE_ALTIVEC
                ret.col0 = m.col0;
                ret.col1 = m.col1;
                ret.col2 = m.col2;
                ret.col3 = vec_madd(m.col0,vec_perm(v.vec,v.vec,VEC_PERM2(X,X,X,X)),m.col3);
                ret.col3 = vec_madd(m.col1,vec_perm(v.vec,v.vec,VEC_PERM2(Y,Y,Y,Y)),ret.col3);
                ret.col3 = vec_madd(m.col2,vec_perm(v.vec,v.vec,VEC_PERM2(Z,Z,Z,Z)),ret.col3);
            #elif USE_NEON
                ret.col0 = m.col0;
                ret.col1 = m.col1;
                ret.col2 = m.col2;
                float32x2_t low = vget_low_f32(v.vec);
                float32x2_t high = vget_high_f32(v.vec);
                ret.col3 = vmlaq_lane_f32(m.col3,m.col0,low,0);
                ret.col3 = vmlaq_lane_f32(ret.col3,m.col1,low,1);
                ret.col3 = vmlaq_lane_f32(ret.col3,m.col2,high,0);
            #else
                ret.m00 = m.m00; ret.m01 = m.m01; ret.m02 = m.m02;
                ret.m10 = m.m10; ret.m11 = m.m11; ret.m12 = m.m12;
                ret.m20 = m.m20; ret.m21 = m.m21; ret.m22 = m.m22;
                ret.m30 = m.m30; ret.m31 = m.m31; ret.m32 = m.m32;
                ret.m03 = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03;
                ret.m13 = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13;
                ret.m23 = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23;
                ret.m33 = m.m33;
            #endif
            return ret;
        }

        FVec3 &proj(FVec3 &ret,const FMat4 &m,const FVec3 &v) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(m.col0,_MM_SWIZZLE(v.vec,X,X,X,X));
                __m128 res_1 = _mm_mul_ps(m.col1,_MM_SWIZZLE(v.vec,Y,Y,Y,Y));
                __m128 res_2 = _mm_mul_ps(m.col2,_MM_SWIZZLE(v.vec,Z,Z,Z,Z));
                __m128 res_3 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,m.col3));
                ret.vec = _mm_div_ps(res_3,_MM_SWIZZLE(res_3,W,W,W,W));
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_madd(m.col0,VEC_SWIZZLE(v.vec,X,X,X,X),m.col3);
                vec_float4 res_1 = vec_madd(m.col1,VEC_SWIZZLE(v.vec,Y,Y,Y,Y),res_0);
                vec_float4 res_2 = vec_madd(m.col2,VEC_SWIZZLE(v.vec,Z,Z,Z,Z),res_1);
                ret.vec = vec_madd(res_2,vec_rcp_nr(VEC_SWIZZLE(res_2,W,W,W,W)),vec_splats(0.0f));
            #elif USE_NEON
                float32x2_t low = vget_low_f32(v.vec);
                float32x2_t high = vget_high_f32(v.vec);
                float32x4_t res_0 = vmlaq_lane_f32(m.col3,m.col0,low,0);
                float32x4_t res_1 = vmlaq_lane_f32(res_0,m.col1,low,1);
                float32x4_t res_2 = vmlaq_lane_f32(res_1,m.col2,high,0);
                ret.vec = vmulq_lane_f32(res_2,vrcp_nr_f32(vget_high_f32(res_2)),1);
            #else
                f32 iw = Math::rcp(m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33);
                ret.x = (m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03) * iw;
                ret.y = (m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13) * iw;
                ret.z = (m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23) * iw;
            #endif
            return ret;
        }

        FVec4 &proj(FVec4 &ret,const FMat4 &m,const FVec4 &v) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(m.col0,_MM_SWIZZLE(v.vec,X,X,X,X));
                __m128 res_1 = _mm_mul_ps(m.col1,_MM_SWIZZLE(v.vec,Y,Y,Y,Y));
                __m128 res_2 = _mm_mul_ps(m.col2,_MM_SWIZZLE(v.vec,Z,Z,Z,Z));
                __m128 res_3 = _mm_mul_ps(m.col3,_MM_SWIZZLE(v.vec,W,W,W,W));
                __m128 res_4 = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,res_3));
                ret.vec = _mm_div_ps(res_4,_MM_SWIZZLE(res_4,W,W,W,W));
            #elif USE_ALTIVEC
                vec_float4 zero = vec_splats(0.0f);
                vec_float4 res_0 = vec_madd(m.col0,VEC_SWIZZLE(v.vec,X,X,X,X),zero);
                vec_float4 res_1 = vec_madd(m.col1,VEC_SWIZZLE(v.vec,Y,Y,Y,Y),res_0);
                vec_float4 res_2 = vec_madd(m.col2,VEC_SWIZZLE(v.vec,Z,Z,Z,Z),res_1);
                vec_float4 res_3 = vec_madd(m.col3,VEC_SWIZZLE(v.vec,W,W,W,W),res_2);
                ret.vec = vec_madd(res_3,vec_rcp_nr(VEC_SWIZZLE(res_3,W,W,W,W)),zero);
            #elif USE_NEON
                float32x2_t low = vget_low_f32(v.vec);
                float32x2_t high = vget_high_f32(v.vec);
                float32x4_t res_0 = vmulq_lane_f32(m.col0,low,0);
                float32x4_t res_1 = vmlaq_lane_f32(res_0,m.col1,low,1);
                float32x4_t res_2 = vmlaq_lane_f32(res_1,m.col2,high,0);
                float32x4_t res_3 = vmlaq_lane_f32(res_2,m.col3,high,1);
                ret.vec = vmulq_lane_f32(res_3,vrcp_nr_f32(vget_high_f32(res_3)),1);
            #else
                f32 iw = Math::rcp(m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w);
                ret.x = (m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w) * iw;
                ret.y = (m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w) * iw;
                ret.z = (m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w) * iw;
                ret.w = 1.0f;
            #endif
            return ret;
        }

        DVec3 &proj(DVec3 &ret,const FMat4 &m,const DVec3 &v) {
            f64 iw = Math::rcp(m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33);
            ret.x = (m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03) * iw;
            ret.y = (m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13) * iw;
            ret.z = (m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23) * iw;
            return ret;
        }

        DVec4 &proj(DVec4 &ret,const FMat4 &m,const DVec4 &v) {
            f64 iw = Math::rcp(m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w);
            ret.x = (m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w) * iw;
            ret.y = (m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w) * iw;
            ret.z = (m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w) * iw;
            ret.w = 1.0f;
            return ret;
        }

        FMat4 &add(FMat4 &ret,const FMat4 &m0,const FMat4 &m1) {
            #ifdef USE_SSE
                ret.col0 = _mm_add_ps(m0.col0,m1.col0);
                ret.col1 = _mm_add_ps(m0.col1,m1.col1);
                ret.col2 = _mm_add_ps(m0.col2,m1.col2);
                ret.col3 = _mm_add_ps(m0.col3,m1.col3);
            #elif USE_ALTIVEC
                ret.col0 = vec_add(m0.col0,m1.col0);
                ret.col1 = vec_add(m0.col1,m1.col1);
                ret.col2 = vec_add(m0.col2,m1.col2);
                ret.col3 = vec_add(m0.col3,m1.col3);
            #elif USE_NEON
                ret.col0 = vaddq_f32(m0.col0,m1.col0);
                ret.col1 = vaddq_f32(m0.col1,m1.col1);
                ret.col2 = vaddq_f32(m0.col2,m1.col2);
                ret.col3 = vaddq_f32(m0.col3,m1.col3);
            #else
                ret.m00 = m0.m00 + m1.m00; ret.m01 = m0.m01 + m1.m01; ret.m02 = m0.m02 + m1.m02; ret.m03 = m0.m03 + m1.m03;
                ret.m10 = m0.m10 + m1.m10; ret.m11 = m0.m11 + m1.m11; ret.m12 = m0.m12 + m1.m12; ret.m13 = m0.m13 + m1.m13;
                ret.m20 = m0.m20 + m1.m20; ret.m21 = m0.m21 + m1.m21; ret.m22 = m0.m22 + m1.m22; ret.m23 = m0.m23 + m1.m23;
                ret.m30 = m0.m30 + m1.m30; ret.m31 = m0.m31 + m1.m31; ret.m32 = m0.m32 + m1.m32; ret.m33 = m0.m33 + m1.m33;
            #endif
            return ret;
        }

        FMat4 &sub(FMat4 &ret,const FMat4 &m0,const FMat4 &m1) {
            #ifdef USE_SSE
                ret.col0 = _mm_sub_ps(m0.col0,m1.col0);
                ret.col1 = _mm_sub_ps(m0.col1,m1.col1);
                ret.col2 = _mm_sub_ps(m0.col2,m1.col2);
                ret.col3 = _mm_sub_ps(m0.col3,m1.col3);
            #elif USE_ALTIVEC
                ret.col0 = vec_sub(m0.col0,m1.col0);
                ret.col1 = vec_sub(m0.col1,m1.col1);
                ret.col2 = vec_sub(m0.col2,m1.col2);
                ret.col3 = vec_sub(m0.col3,m1.col3);
            #elif USE_NEON
                ret.col0 = vsubq_f32(m0.col0,m1.col0);
                ret.col1 = vsubq_f32(m0.col1,m1.col1);
                ret.col2 = vsubq_f32(m0.col2,m1.col2);
                ret.col3 = vsubq_f32(m0.col3,m1.col3);
            #else
                ret.m00 = m0.m00 - m1.m00; ret.m01 = m0.m01 - m1.m01; ret.m02 = m0.m02 - m1.m02; ret.m03 = m0.m03 - m1.m03;
                ret.m10 = m0.m10 - m1.m10; ret.m11 = m0.m11 - m1.m11; ret.m12 = m0.m12 - m1.m12; ret.m13 = m0.m13 - m1.m13;
                ret.m20 = m0.m20 - m1.m20; ret.m21 = m0.m21 - m1.m21; ret.m22 = m0.m22 - m1.m22; ret.m23 = m0.m23 - m1.m23;
                ret.m30 = m0.m30 - m1.m30; ret.m31 = m0.m31 - m1.m31; ret.m32 = m0.m32 - m1.m32; ret.m33 = m0.m33 - m1.m33;
            #endif
            return ret;
        }

        FMat4 &orthonormalize(FMat4 &ret,const FMat4 &m) {
            #ifdef USE_SSE
                __m128 x_yzx = _MM_SWIZZLE(m.col0,Y,Z,X,W);
                __m128 x_zxy = _MM_SWIZZLE(m.col0,Z,X,Y,W);
                __m128 y_yzx = _MM_SWIZZLE(m.col1,Y,Z,X,W);
                __m128 y_zxy = _MM_SWIZZLE(m.col1,Z,X,Y,W);
                __m128 z = _mm_sub_ps(_mm_mul_ps(x_yzx,y_zxy),_mm_mul_ps(x_zxy,y_yzx));
                __m128 z_yzx = _MM_SWIZZLE(z,Y,Z,X,W);
                __m128 z_zxy = _MM_SWIZZLE(z,Z,X,Y,W);
                __m128 y = _mm_sub_ps(_mm_mul_ps(z_yzx,x_zxy),_mm_mul_ps(z_zxy,x_yzx));
                __m128 col_0 = _mm_mul_ps(m.col0,m.col0);
                __m128 col_1 = _mm_mul_ps(y,y);
                __m128 col_2 = _mm_mul_ps(z,z);
                __m128 res_0 = _mm_shuffle_ps(col_0,col_1,_MM_PERM2(X,Y,X,Y));
                __m128 res_1 = _mm_shuffle_ps(col_0,col_1,_MM_PERM2(Z,W,Z,W));
                __m128 res_2 = _mm_shuffle_ps(col_2,col_2,_MM_PERM2(X,Y,X,Y));
                __m128 res_3 = _mm_shuffle_ps(col_2,col_2,_MM_PERM2(Z,W,Z,W));
                __m128 row_0 = _mm_shuffle_ps(res_0,res_2,_MM_PERM2(X,Z,X,Z));
                __m128 row_1 = _mm_shuffle_ps(res_0,res_2,_MM_PERM2(Y,W,Y,W));
                __m128 row_2 = _mm_shuffle_ps(res_1,res_3,_MM_PERM2(X,Z,X,Z));
                __m128 ilength = _mm_rsqrt_ps_nr(_mm_add_ps(_mm_add_ps(row_0,row_1),row_2));
                ret.col0 = _mm_mul_ps(m.col0,_MM_SWIZZLE(ilength,X,X,X,X));
                ret.col1 = _mm_mul_ps(y,_MM_SWIZZLE(ilength,Y,Y,Y,Y));
                ret.col2 = _mm_mul_ps(z,_MM_SWIZZLE(ilength,Z,Z,Z,Z));
                ret.col3 = m.col3;
            #elif USE_ALTIVEC
                vec_float4 zero = vec_splats(0.0f);
                vec_uchar16 yzxw = VEC_PERM2(Y,Z,X,W);
                vec_uchar16 zxyw = VEC_PERM2(Z,X,Y,W);
                vec_float4 x_yzx = vec_perm(m.col0,m.col0,yzxw);
                vec_float4 x_zxy = vec_perm(m.col0,m.col0,zxyw);
                vec_float4 y_yzx = vec_perm(m.col1,m.col1,yzxw);
                vec_float4 y_zxy = vec_perm(m.col1,m.col1,zxyw);
                vec_float4 z = vec_sub(vec_madd(x_yzx,y_zxy,zero),vec_madd(x_zxy,y_yzx,zero));
                vec_float4 z_yzx = vec_perm(z,z,yzxw);
                vec_float4 z_zxy = vec_perm(z,z,zxyw);
                vec_float4 y = vec_sub(vec_madd(z_yzx,x_zxy,zero),vec_madd(z_zxy,x_yzx,zero));
                vec_float4 col_0 = vec_madd(m.col0,m.col0,zero);
                vec_float4 col_1 = vec_madd(y,y,zero);
                vec_float4 col_2 = vec_madd(z,z,zero);
                vec_float4 res_0 = vec_perm(col_0,col_1,VEC_PERM2(X,Y,X,Y));
                vec_float4 res_1 = vec_perm(col_0,col_1,VEC_PERM2(Z,W,Z,W));
                vec_float4 res_2 = vec_perm(col_2,col_2,VEC_PERM2(X,Y,X,Y));
                vec_float4 res_3 = vec_perm(col_2,col_2,VEC_PERM2(Z,W,Z,W));
                vec_float4 row_0 = vec_perm(res_0,res_2,VEC_PERM2(X,Z,X,Z));
                vec_float4 row_1 = vec_perm(res_0,res_2,VEC_PERM2(Y,W,Y,W));
                vec_float4 row_2 = vec_perm(res_1,res_3,VEC_PERM2(X,Z,X,Z));
                vec_float4 ilength = vec_rsqrt_nr(vec_add(vec_add(row_0,row_1),row_2));
                ret.col0 = vec_madd(m.col0,VEC_SWIZZLE(ilength,X,X,X,X),zero);
                ret.col1 = vec_madd(y,VEC_SWIZZLE(ilength,Y,Y,Y,Y),zero);
                ret.col2 = vec_madd(z,VEC_SWIZZLE(ilength,Z,Z,Z,Z),zero);
                ret.col3 = m.col3;
            #elif USE_NEON
                float32x4_t z = vcrossq_f32(m.col0,m.col1);
                float32x4_t y = vcrossq_f32(z,m.col0);
                ret.col0 = vnormalize3q_f32(m.col0);
                ret.col1 = vnormalize3q_f32(y);
                ret.col2 = vnormalize3q_f32(z);
                ret.col3 = m.col3;
            #else
                FVec3 x = FVec3(m.m00,m.m10,m.m20);
                FVec3 y = FVec3(m.m01,m.m11,m.m21);
                FVec3 z = cross(x,y);
                cross(y,z,x);
                x.normalize();
                y.normalize();
                z.normalize();
                ret.m00 = x.x;   ret.m01 = y.x;   ret.m02 = z.x;   ret.m03 = m.m03;
                ret.m10 = x.y;   ret.m11 = y.y;   ret.m12 = z.y;   ret.m13 = m.m13;
                ret.m20 = x.z;   ret.m21 = y.z;   ret.m22 = z.z;   ret.m23 = m.m23;
                ret.m30 = m.m30; ret.m31 = m.m31; ret.m32 = m.m32; ret.m33 = m.m33;
            #endif
            return ret;
        }

        FMat4 &rotation(FMat4 &ret,const FMat4 &m) {
            ret.m00 = m.m00; ret.m01 = m.m01; ret.m02 = m.m02; ret.m03 = 0.0f;
            ret.m10 = m.m10; ret.m11 = m.m11; ret.m12 = m.m12; ret.m13 = 0.0f;
            ret.m20 = m.m20; ret.m21 = m.m21; ret.m22 = m.m22; ret.m23 = 0.0f;
            ret.m30 = 0.0f;  ret.m31 = 0.0f;  ret.m32 = 0.0f;  ret.m33 = 1.0f;
            return ret;
        }

        FMat4 &transpose(FMat4 &ret,const FMat4 &m) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_shuffle_ps(m.col0,m.col1,_MM_PERM2(X,Y,X,Y));
                __m128 res_1 = _mm_shuffle_ps(m.col0,m.col1,_MM_PERM2(Z,W,Z,W));
                __m128 res_2 = _mm_shuffle_ps(m.col2,m.col3,_MM_PERM2(X,Y,X,Y));
                __m128 res_3 = _mm_shuffle_ps(m.col2,m.col3,_MM_PERM2(Z,W,Z,W));
                ret.col0 = _mm_shuffle_ps(res_0,res_2,_MM_PERM2(X,Z,X,Z));
                ret.col1 = _mm_shuffle_ps(res_0,res_2,_MM_PERM2(Y,W,Y,W));
                ret.col2 = _mm_shuffle_ps(res_1,res_3,_MM_PERM2(X,Z,X,Z));
                ret.col3 = _mm_shuffle_ps(res_1,res_3,_MM_PERM2(Y,W,Y,W));
            #elif USE_ALTIVEC
                vec_float4 res_0 = vec_perm(m.col0,m.col1,VEC_PERM2(X,Y,X,Y));
                vec_float4 res_1 = vec_perm(m.col0,m.col1,VEC_PERM2(Z,W,Z,W));
                vec_float4 res_2 = vec_perm(m.col2,m.col3,VEC_PERM2(X,Y,X,Y));
                vec_float4 res_3 = vec_perm(m.col2,m.col3,VEC_PERM2(Z,W,Z,W));
                ret.col0 = vec_perm(res_0,res_2,VEC_PERM2(X,Z,X,Z));
                ret.col1 = vec_perm(res_0,res_2,VEC_PERM2(Y,W,Y,W));
                ret.col2 = vec_perm(res_1,res_3,VEC_PERM2(X,Z,X,Z));
                ret.col3 = vec_perm(res_1,res_3,VEC_PERM2(Y,W,Y,W));
            #else
                ret.m00 = m.m00; ret.m01 = m.m10; ret.m02 = m.m20; ret.m03 = m.m30;
                ret.m10 = m.m01; ret.m11 = m.m11; ret.m12 = m.m21; ret.m13 = m.m31;
                ret.m20 = m.m02; ret.m21 = m.m12; ret.m22 = m.m22; ret.m23 = m.m32;
                ret.m30 = m.m03; ret.m31 = m.m13; ret.m32 = m.m23; ret.m33 = m.m33;
            #endif
            return ret;
        }

        FMat4 &transpose3(FMat4 &ret,const FMat4 &m) {
            ret.m00 = m.m00; ret.m01 = m.m10; ret.m02 = m.m20; ret.m03 = m.m03;
            ret.m10 = m.m01; ret.m11 = m.m11; ret.m12 = m.m21; ret.m13 = m.m13;
            ret.m20 = m.m02; ret.m21 = m.m12; ret.m22 = m.m22; ret.m23 = m.m23;
            ret.m30 = m.m30; ret.m31 = m.m31; ret.m32 = m.m32; ret.m33 = m.m33;
            return ret;
        }

        FMat4 &inverse(FMat4 &ret,const FMat4 &m) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_shuffle_ps(m.col0,m.col1,_MM_PERM2(X,Y,X,Y));
                __m128 res_1 = _mm_shuffle_ps(m.col0,m.col1,_MM_PERM2(Z,W,Z,W));
                __m128 res_2 = _mm_shuffle_ps(m.col2,m.col3,_MM_PERM2(X,Y,X,Y));
                __m128 res_3 = _mm_shuffle_ps(m.col2,m.col3,_MM_PERM2(Z,W,Z,W));
                __m128 row_0 = _mm_shuffle_ps(res_0,res_2,_MM_PERM2(X,Z,X,Z));
                __m128 row_1 = _mm_shuffle_ps(res_2,res_0,_MM_PERM2(Y,W,Y,W));
                __m128 row_2 = _mm_shuffle_ps(res_1,res_3,_MM_PERM2(X,Z,X,Z));
                __m128 row_3 = _mm_shuffle_ps(res_3,res_1,_MM_PERM2(Y,W,Y,W));
                __m128 temp = _mm_mul_ps(row_2,row_3);
                temp = _MM_SWIZZLE(temp,Y,X,W,Z);
                res_0 = _mm_mul_ps(row_1,temp);
                res_1 = _mm_mul_ps(row_0,temp);
                temp = _MM_SWIZZLE(temp,Z,W,X,Y);
                res_0 = _mm_sub_ps(_mm_mul_ps(row_1,temp),res_0);
                res_1 = _mm_sub_ps(_mm_mul_ps(row_0,temp),res_1);
                res_1 = _MM_SWIZZLE(res_1,Z,W,X,Y);
                temp = _mm_mul_ps(row_1,row_2);
                temp = _MM_SWIZZLE(temp,Y,X,W,Z);
                res_0 = _mm_add_ps(_mm_mul_ps(row_3,temp),res_0);
                res_3 = _mm_mul_ps(row_0,temp);
                temp = _MM_SWIZZLE(temp,Z,W,X,Y);
                res_0 = _mm_sub_ps(res_0,_mm_mul_ps(row_3,temp));
                res_3 = _mm_sub_ps(_mm_mul_ps(row_0,temp),res_3);
                res_3 = _MM_SWIZZLE(res_3,Z,W,X,Y);
                temp = _mm_mul_ps(row_3,_MM_SWIZZLE(row_1,Z,W,X,Y));
                temp = _MM_SWIZZLE(temp,Y,X,W,Z);
                row_2 = _MM_SWIZZLE(row_2,Z,W,X,Y);
                res_0 = _mm_add_ps(_mm_mul_ps(row_2,temp),res_0);
                res_2 = _mm_mul_ps(row_0,temp);
                temp = _MM_SWIZZLE(temp,Z,W,X,Y);
                res_0 = _mm_sub_ps(res_0,_mm_mul_ps(row_2,temp));
                res_2 = _mm_sub_ps(_mm_mul_ps(row_0,temp),res_2);
                res_2 = _MM_SWIZZLE(res_2,Z,W,X,Y);
                temp = _mm_mul_ps(row_0,row_1);
                temp = _MM_SWIZZLE(temp,Y,X,W,Z);
                res_2 = _mm_add_ps(_mm_mul_ps(row_3,temp),res_2);
                res_3 = _mm_sub_ps(_mm_mul_ps(row_2,temp),res_3);
                temp = _MM_SWIZZLE(temp,Z,W,X,Y);
                res_2 = _mm_sub_ps(_mm_mul_ps(row_3,temp),res_2);
                res_3 = _mm_sub_ps(res_3,_mm_mul_ps(row_2,temp));
                temp = _mm_mul_ps(row_0,row_3);
                temp = _MM_SWIZZLE(temp,Y,X,W,Z);
                res_1 = _mm_sub_ps(res_1,_mm_mul_ps(row_2,temp));
                res_2 = _mm_add_ps(_mm_mul_ps(row_1,temp),res_2);
                temp = _MM_SWIZZLE(temp,Z,W,X,Y);
                res_1 = _mm_add_ps(_mm_mul_ps(row_2,temp),res_1);
                res_2 = _mm_sub_ps(res_2,_mm_mul_ps(row_1,temp));
                temp = _mm_mul_ps(row_0,row_2);
                temp = _MM_SWIZZLE(temp,Y,X,W,Z);
                res_1 = _mm_add_ps(_mm_mul_ps(row_3,temp),res_1);
                res_3 = _mm_sub_ps(res_3,_mm_mul_ps(row_1,temp));
                temp = _MM_SWIZZLE(temp,Z,W,X,Y);
                res_1 = _mm_sub_ps(res_1,_mm_mul_ps(row_3,temp));
                res_3 = _mm_add_ps(_mm_mul_ps(row_1,temp),res_3);
                __m128 det = _mm_mul_ps(row_0,res_0);
                det = _mm_add_ps(det,_MM_SWIZZLE(det,Y,X,W,Z));
                det = _mm_add_ss(det,_MM_SWIZZLE(det,Z,W,X,Y));
                temp = _MM_SWIZZLE(_mm_rcp_ss_nr(det),X,X,X,X);
                ret.col0 = _mm_mul_ps(res_0,temp);
                ret.col1 = _mm_mul_ps(res_1,temp);
                ret.col2 = _mm_mul_ps(res_2,temp);
                ret.col3 = _mm_mul_ps(res_3,temp);
            #elif USE_ALTIVEC
                vec_float4 zero = vec_splats(0.0f);
                vec_uchar16 yxwz = VEC_PERM2(Y,X,W,Z);
                vec_uchar16 zwxy = VEC_PERM2(Z,W,X,Y);
                vec_float4 res_0 = vec_perm(m.col0,m.col1,VEC_PERM2(X,Y,X,Y));
                vec_float4 res_1 = vec_perm(m.col0,m.col1,VEC_PERM2(Z,W,Z,W));
                vec_float4 res_2 = vec_perm(m.col2,m.col3,VEC_PERM2(X,Y,X,Y));
                vec_float4 res_3 = vec_perm(m.col2,m.col3,VEC_PERM2(Z,W,Z,W));
                vec_float4 row_0 = vec_perm(res_0,res_2,VEC_PERM2(X,Z,X,Z));
                vec_float4 row_1 = vec_perm(res_2,res_0,VEC_PERM2(Y,W,Y,W));
                vec_float4 row_2 = vec_perm(res_1,res_3,VEC_PERM2(X,Z,X,Z));
                vec_float4 row_3 = vec_perm(res_3,res_1,VEC_PERM2(Y,W,Y,W));
                vec_float4 temp = vec_madd(row_2,row_3,zero);
                temp = vec_perm(temp,temp,yxwz);
                res_0 = vec_madd(row_1,temp,zero);
                res_1 = vec_madd(row_0,temp,zero);
                temp = vec_perm(temp,temp,zwxy);
                res_0 = vec_sub(vec_madd(row_1,temp,zero),res_0);
                res_1 = vec_sub(vec_madd(row_0,temp,zero),res_1);
                res_1 = vec_perm(res_1,res_1,zwxy);
                temp = vec_madd(row_1,row_2,zero);
                temp = vec_perm(temp,temp,yxwz);
                res_0 = vec_madd(row_3,temp,res_0);
                res_3 = vec_madd(row_0,temp,zero);
                temp = vec_perm(temp,temp,zwxy);
                res_0 = vec_nmsub(row_3,temp,res_0);
                res_3 = vec_sub(vec_madd(row_0,temp,zero),res_3);
                res_3 = vec_perm(res_3,res_3,zwxy);
                temp = vec_madd(vec_perm(row_1,row_1,zwxy),row_3,zero);
                temp = vec_perm(temp,temp,yxwz);
                row_2 = vec_perm(row_2,row_2,zwxy);
                res_0 = vec_madd(row_2,temp,res_0);
                res_2 = vec_madd(row_0,temp,zero);
                temp = vec_perm(temp,temp,zwxy);
                res_0 = vec_nmsub(row_2,temp,res_0);
                res_2 = vec_sub(vec_madd(row_0,temp,zero),res_2);
                res_2 = vec_perm(res_2,res_2,zwxy);
                temp = vec_madd(row_0,row_1,zero);
                temp = vec_perm(temp,temp,yxwz);
                res_2 = vec_madd(row_3,temp,res_2);
                res_3 = vec_sub(vec_madd(row_2,temp,zero),res_3);
                temp = vec_perm(temp,temp,zwxy);
                res_2 = vec_sub(vec_madd(row_3,temp,zero),res_2);
                res_3 = vec_nmsub(row_2,temp,res_3);
                temp = vec_madd(row_0,row_3,zero);
                temp = vec_perm(temp,temp,yxwz);
                res_1 = vec_nmsub(row_2,temp,res_1);
                res_2 = vec_madd(row_1,temp,res_2);
                temp = vec_perm(temp,temp,zwxy);
                res_1 = vec_madd(row_2,temp,res_1);
                res_2 = vec_nmsub(row_1,temp,res_2);
                temp = vec_madd(row_0,row_2,zero);
                temp = vec_perm(temp,temp,yxwz);
                res_1 = vec_madd(row_3,temp,res_1);
                res_3 = vec_nmsub(row_1,temp,res_3);
                temp = vec_perm(temp,temp,zwxy);
                res_1 = vec_nmsub(row_3,temp,res_1);
                res_3 = vec_madd(row_1,temp,res_3);
                vec_float4 det = vec_madd(row_0,res_0,zero);
                det = vec_add(det,vec_sld(det,det,8));
                det = vec_add(det,vec_sld(det,det,4));
                temp = vec_rcp_nr(det);
                ret.col0 = vec_madd(res_0,temp,zero);
                ret.col1 = vec_madd(res_1,temp,zero);
                ret.col2 = vec_madd(res_2,temp,zero);
                ret.col3 = vec_madd(res_3,temp,zero);
            #elif defined(USE_NEON) && (defined(_WINRT) || defined(_IOS))
                float32x4x4_t transpose = vld4q_f32((const float32_t*)m.mat);
                float32x4_t row_0 = transpose.val[0];
                float32x4_t row_1 = vextq_f32(transpose.val[1],transpose.val[1],2);
                float32x4_t row_2 = transpose.val[2];
                float32x4_t row_3 = vextq_f32(transpose.val[3],transpose.val[3],2);
                float32x4_t temp = vmulq_f32(row_2,row_3);
                temp = vrev64q_f32(temp);
                float32x4_t res_0 = vmulq_f32(row_1,temp);
                float32x4_t res_1 = vmulq_f32(row_0,temp);
                temp = vextq_f32(temp,temp,2);
                res_0 = vnegq_f32(vmlsq_f32(res_0,row_1,temp));
                res_1 = vnegq_f32(vmlsq_f32(res_1,row_0,temp));
                res_1 = vextq_f32(res_1,res_1,2);
                temp = vmulq_f32(row_1,row_2);
                temp = vrev64q_f32(temp);
                res_0 = vmlaq_f32(res_0,row_3,temp);
                float32x4_t res_3 = vmulq_f32(row_0,temp);
                temp = vextq_f32(temp,temp,2);
                res_0 = vmlsq_f32(res_0,row_3,temp);
                res_3 = vnegq_f32(vmlsq_f32(res_3,row_0,temp));
                res_3 = vextq_f32(res_3,res_3,2);
                temp = vmulq_f32(row_3,vextq_f32(row_1,row_1,2));
                temp = vrev64q_f32(temp);
                row_2 = vextq_f32(row_2,row_2,2);
                res_0 = vmlaq_f32(res_0,row_2,temp);
                float32x4_t res_2 = vmulq_f32(row_0,temp);
                temp = vextq_f32(temp,temp,2);
                res_0 = vmlsq_f32(res_0,row_2,temp);
                res_2 = vnegq_f32(vmlsq_f32(res_2,row_0,temp));
                res_2 = vextq_f32(res_2,res_2,2);
                temp = vmulq_f32(row_0,row_1);
                temp = vrev64q_f32(temp);
                res_2 = vmlaq_f32(res_2,row_3,temp);
                res_3 = vnegq_f32(vmlsq_f32(res_3,row_2,temp));
                temp = vextq_f32(temp,temp,2);
                res_2 = vnegq_f32(vmlsq_f32(res_2,row_3,temp));
                res_3 = vmlsq_f32(res_3,row_2,temp);
                temp = vmulq_f32(row_0,row_3);
                temp = vrev64q_f32(temp);
                res_1 = vmlsq_f32(res_1,row_2,temp);
                res_2 = vmlaq_f32(res_2,row_1,temp);
                temp = vextq_f32(temp,temp,2);
                res_1 = vmlaq_f32(res_1,row_2,temp);
                res_2 = vmlsq_f32(res_2,row_1,temp);
                temp = vmulq_f32(row_0,row_2);
                temp = vrev64q_f32(temp);
                res_1 = vmlaq_f32(res_1,row_3,temp);
                res_3 = vmlsq_f32(res_3,row_1,temp);
                temp = vextq_f32(temp,temp,2);
                res_1 = vmlsq_f32(res_1,row_3,temp);
                res_3 = vmlaq_f32(res_3,row_1,temp);
                float32x4_t det = vmulq_f32(row_0,res_0);
                det = vaddq_f32(det,vextq_f32(det,det,2));
                det = vaddq_f32(det,vextq_f32(det,det,1));
                float32x2_t idet = vrcp_nr_f32(vget_low_f32(det));
                ret.col0 = vmulq_lane_f32(res_0,idet,0);
                ret.col1 = vmulq_lane_f32(res_1,idet,0);
                ret.col2 = vmulq_lane_f32(res_2,idet,0);
                ret.col3 = vmulq_lane_f32(res_3,idet,0);
            #elif USE_NEON
                asm volatile(
                    "vld4.32  { d0, d2, d4, d6 }, [%r1]!	\n"
                    "vld4.32  { d1, d3, d5, d7 }, [%r1]		\n"
                    "vext.32    q1, q1, q1, #2				\n"
                    "vext.32    q3, q3, q3, #2				\n"
                    "vmul.f32   q8, q2, q3					\n"
                    "vrev64.32  q8, q8						\n"
                    "vmul.f32   q4, q1, q8					\n"
                    "vmul.f32   q5, q0, q8					\n"
                    "vext.32    q8, q8, q8, #2				\n"
                    "vmls.f32   q4, q1, q8					\n"
                    "vneg.f32   q4, q4						\n"
                    "vmls.f32   q5, q0, q8					\n"
                    "vneg.f32   q5, q5						\n"
                    "vext.32    q5, q5, q5, #2				\n"
                    "vmul.f32   q8, q1, q2					\n"
                    "vrev64.32  q8, q8						\n"
                    "vmla.f32   q4, q3, q8					\n"
                    "vmul.f32   q7, q0, q8					\n"
                    "vext.f32   q8, q8, q8, #2				\n"
                    "vmls.f32   q4, q3, q8					\n"
                    "vmls.f32   q7, q0, q8					\n"
                    "vneg.f32   q7, q7						\n"
                    "vext.32    q7, q7, q7, #2				\n"
                    "vext.32    q8, q1, q1, #2				\n"
                    "vmul.f32   q8, q3, q8					\n"
                    "vrev64.32  q8, q8						\n"
                    "vext.32    q2, q2, q2, #2				\n"
                    "vmla.f32   q4, q2, q8					\n"
                    "vmul.f32   q6, q0, q8					\n"
                    "vext.f32   q8, q8, q8, #2				\n"
                    "vmls.f32   q4, q2, q8					\n"
                    "vmls.f32   q6, q0, q8					\n"
                    "vneg.f32   q6, q6						\n"
                    "vext.32    q6, q6, q6, #2				\n"
                    "vmul.f32   q8, q0, q1					\n"
                    "vrev64.32  q8, q8						\n"
                    "vmla.f32   q6, q3, q8					\n"
                    "vmls.f32   q7, q2, q8					\n"
                    "vneg.f32   q7, q7						\n"
                    "vext.f32   q8, q8, q8, #2				\n"
                    "vmls.f32   q6, q3, q8					\n"
                    "vneg.f32   q6, q6						\n"
                    "vmls.f32   q7, q2, q8					\n"
                    "vmul.f32   q8, q0, q3					\n"
                    "vrev64.32  q8, q8						\n"
                    "vmls.f32   q5, q2, q8					\n"
                    "vmla.f32   q6, q1, q8					\n"
                    "vext.f32   q8, q8, q8, #2				\n"
                    "vmla.f32   q5, q2, q8					\n"
                    "vmls.f32   q6, q1, q8					\n"
                    "vmul.f32   q8, q0, q2					\n"
                    "vrev64.32  q8, q8						\n"
                    "vmla.f32   q5, q3, q8					\n"
                    "vmls.f32   q7, q1, q8					\n"
                    "vext.f32   q8, q8, q8, #2				\n"
                    "vmls.f32   q5, q3, q8					\n"
                    "vmla.f32   q7, q1, q8					\n"
                    "vmul.f32   q0, q0, q4					\n"
                    "vext.32    q1, q0, q0, #2				\n"
                    "vadd.f32   q0, q0, q1					\n"
                    "vext.32    q1, q0, q0, #1				\n"
                    "vadd.f32   q0, q0, q1					\n"
                    "vrecpe.f32 q1, q0						\n"
                    "vrecps.f32 q2, q1, q0					\n"
                    "vmul.f32   q1, q1, q2					\n"
                    "vrecps.f32 q2, q1, q0					\n"
                    "vmul.f32   q8, q1, q2					\n"
                    "vmul.f32   q0, q4, q8					\n"
                    "vmul.f32   q1, q5, q8					\n"
                    "vmul.f32   q2, q6, q8					\n"
                    "vmul.f32   q3, q7, q8					\n"
                    "vstm      %r0, { q0 - q3 }				\n"
                    : : "r"(ret.mat), "r"(m.mat)
                    : "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8"
                );
            #else
                f32 temp[12];
                temp[0]  = m.m22 * m.m33;
                temp[1]  = m.m23 * m.m32;
                temp[2]  = m.m21 * m.m33;
                temp[3]  = m.m23 * m.m31;
                temp[4]  = m.m21 * m.m32;
                temp[5]  = m.m22 * m.m31;
                temp[6]  = m.m20 * m.m33;
                temp[7]  = m.m23 * m.m30;
                temp[8]  = m.m20 * m.m32;
                temp[9]  = m.m22 * m.m30;
                temp[10] = m.m20 * m.m31;
                temp[11] = m.m21 * m.m30;
                ret.m00  = temp[0] * m.m11 + temp[3] * m.m12 + temp[4]  * m.m13;
                ret.m00 -= temp[1] * m.m11 + temp[2] * m.m12 + temp[5]  * m.m13;
                ret.m10  = temp[1] * m.m10 + temp[6] * m.m12 + temp[9]  * m.m13;
                ret.m10 -= temp[0] * m.m10 + temp[7] * m.m12 + temp[8]  * m.m13;
                ret.m20  = temp[2] * m.m10 + temp[7] * m.m11 + temp[10] * m.m13;
                ret.m20 -= temp[3] * m.m10 + temp[6] * m.m11 + temp[11] * m.m13;
                ret.m30  = temp[5] * m.m10 + temp[8] * m.m11 + temp[11] * m.m12;
                ret.m30 -= temp[4] * m.m10 + temp[9] * m.m11 + temp[10] * m.m12;
                ret.m01  = temp[1] * m.m01 + temp[2] * m.m02 + temp[5]  * m.m03;
                ret.m01 -= temp[0] * m.m01 + temp[3] * m.m02 + temp[4]  * m.m03;
                ret.m11  = temp[0] * m.m00 + temp[7] * m.m02 + temp[8]  * m.m03;
                ret.m11 -= temp[1] * m.m00 + temp[6] * m.m02 + temp[9]  * m.m03;
                ret.m21  = temp[3] * m.m00 + temp[6] * m.m01 + temp[11] * m.m03;
                ret.m21 -= temp[2] * m.m00 + temp[7] * m.m01 + temp[10] * m.m03;
                ret.m31  = temp[4] * m.m00 + temp[9] * m.m01 + temp[10] * m.m02;
                ret.m31 -= temp[5] * m.m00 + temp[8] * m.m01 + temp[11] * m.m02;
                temp[0]  = m.m02 * m.m13;
                temp[1]  = m.m03 * m.m12;
                temp[2]  = m.m01 * m.m13;
                temp[3]  = m.m03 * m.m11;
                temp[4]  = m.m01 * m.m12;
                temp[5]  = m.m02 * m.m11;
                temp[6]  = m.m00 * m.m13;
                temp[7]  = m.m03 * m.m10;
                temp[8]  = m.m00 * m.m12;
                temp[9]  = m.m02 * m.m10;
                temp[10] = m.m00 * m.m11;
                temp[11] = m.m01 * m.m10;
                ret.m02  = temp[0]  * m.m31 + temp[3]  * m.m32 + temp[4]  * m.m33;
                ret.m02 -= temp[1]  * m.m31 + temp[2]  * m.m32 + temp[5]  * m.m33;
                ret.m12  = temp[1]  * m.m30 + temp[6]  * m.m32 + temp[9]  * m.m33;
                ret.m12 -= temp[0]  * m.m30 + temp[7]  * m.m32 + temp[8]  * m.m33;
                ret.m22  = temp[2]  * m.m30 + temp[7]  * m.m31 + temp[10] * m.m33;
                ret.m22 -= temp[3]  * m.m30 + temp[6]  * m.m31 + temp[11] * m.m33;
                ret.m32  = temp[5]  * m.m30 + temp[8]  * m.m31 + temp[11] * m.m32;
                ret.m32 -= temp[4]  * m.m30 + temp[9]  * m.m31 + temp[10] * m.m32;
                ret.m03  = temp[2]  * m.m22 + temp[5]  * m.m23 + temp[1]  * m.m21;
                ret.m03 -= temp[4]  * m.m23 + temp[0]  * m.m21 + temp[3]  * m.m22;
                ret.m13  = temp[8]  * m.m23 + temp[0]  * m.m20 + temp[7]  * m.m22;
                ret.m13 -= temp[6]  * m.m22 + temp[9]  * m.m23 + temp[1]  * m.m20;
                ret.m23  = temp[6]  * m.m21 + temp[11] * m.m23 + temp[3]  * m.m20;
                ret.m23 -= temp[10] * m.m23 + temp[2]  * m.m20 + temp[7]  * m.m21;
                ret.m33  = temp[10] * m.m22 + temp[4]  * m.m20 + temp[9]  * m.m21;
                ret.m33 -= temp[8]  * m.m21 + temp[11] * m.m22 + temp[5]  * m.m20;
                f32 idet = Math::rcp(m.m00 * ret.m00 + m.m01 * ret.m10 + m.m02 * ret.m20 + m.m03 * ret.m30);
                ret.m00 *= idet;
                ret.m10 *= idet;
                ret.m20 *= idet;
                ret.m30 *= idet;
                ret.m01 *= idet;
                ret.m11 *= idet;
                ret.m21 *= idet;
                ret.m31 *= idet;
                ret.m02 *= idet;
                ret.m12 *= idet;
                ret.m22 *= idet;
                ret.m32 *= idet;
                ret.m03 *= idet;
                ret.m13 *= idet;
                ret.m23 *= idet;
                ret.m33 *= idet;
            #endif
            return ret;
        }

        FMat4 &inverse4(FMat4 &ret,const FMat4 &m) {
            f32 idet = Math::rcp(determinant3(m));
            ret.m00 =  (m.m11 * m.m22 - m.m12 * m.m21) * idet;
            ret.m10 = -(m.m10 * m.m22 - m.m12 * m.m20) * idet;
            ret.m20 =  (m.m10 * m.m21 - m.m11 * m.m20) * idet;
            ret.m30 = 0.0f;
            ret.m01 = -(m.m01 * m.m22 - m.m02 * m.m21) * idet;
            ret.m11 =  (m.m00 * m.m22 - m.m02 * m.m20) * idet;
            ret.m21 = -(m.m00 * m.m21 - m.m01 * m.m20) * idet;
            ret.m31 = 0.0f;
            ret.m02 =  (m.m01 * m.m12 - m.m02 * m.m11) * idet;
            ret.m12 = -(m.m00 * m.m12 - m.m02 * m.m10) * idet;
            ret.m22 =  (m.m00 * m.m11 - m.m01 * m.m10) * idet;
            ret.m32 = 0.0f;
            ret.m03 = -(ret.m00 * m.m03 + ret.m01 * m.m13 + ret.m02 * m.m23);
            ret.m13 = -(ret.m10 * m.m03 + ret.m11 * m.m13 + ret.m12 * m.m23);
            ret.m23 = -(ret.m20 * m.m03 + ret.m21 * m.m13 + ret.m22 * m.m23);
            ret.m33 = 1.0f;
            return ret;
        }

        FMat4 &lerp(FMat4 &ret,const FMat4 &m0,const FMat4 &m1,f32 k) {
            FVec3 positions[3];
            SVQuat rotations[3];
            FVec3 scales[3];
            decomposeTransform(m0,positions[0],rotations[0],scales[0]);
            decomposeTransform(m1,positions[1],rotations[1],scales[1]);
            lerp(positions[2],positions[0],positions[1],k);
            slerp(rotations[2],rotations[0],rotations[1],k);
            lerp(scales[2],scales[0],scales[1],k);
            return composeTransform(ret,positions[2],rotations[2],scales[2]);
        }

        //
        FMat4 orthonormalize(const FMat4 &m) {
            FMat4 ret;
            return orthonormalize(ret,m);
        }

        FMat4 rotation(const FMat4 &m) {
            FMat4 ret;
            return rotation(ret,m);
        }

        FMat4 transpose(const FMat4 &m) {
            FMat4 ret;
            return transpose(ret,m);
        }

        FMat4 transpose3(const FMat4 &m) {
            FMat4 ret;
            return transpose3(ret,m);
        }

        FMat4 inverse(const FMat4 &m) {
            FMat4 ret;
            return inverse(ret,m);
        }

        FMat4 inverse4(const FMat4 &m) {
            FMat4 ret;
            return inverse4(ret,m);
        }

        FMat4 lerp(const FMat4 &m0,const FMat4 &m1,f32 k) {
            FMat4 ret;
            return lerp(ret,m0,m1,k);
        }

        //
        FMat4 translate(const FVec3 &v) {
            FMat4 ret;
            ret.setTranslate(v);
            return ret;
        }

        FMat4 translate(f32 x,f32 y,f32 z) {
            FMat4 ret;
            ret.setTranslate(FVec3(x,y,z));
            return ret;
        }

        FMat4 rotate(const FVec3 &axis,f32 angle) {
            FMat4 ret;
            ret.setRotate(axis,angle);
            return ret;
        }

        FMat4 rotate(f32 x,f32 y,f32 z,f32 angle) {
            return rotate(FVec3(x,y,z),angle);
        }

        FMat4 rotate(const SVQuat &q) {
            return FMat4(q.getMat3());
        }

        FMat4 rotateX(f32 angle) {
            FMat4 ret;
            ret.setRotateX(angle);
            return ret;
        }

        FMat4 rotateY(f32 angle) {
            FMat4 ret;
            ret.setRotateY(angle);
            return ret;
        }

        FMat4 rotateZ(f32 angle) {
            FMat4 ret;
            ret.setRotateZ(angle);
            return ret;
        }

        FMat4 scale(const FVec3 &v) {
            FMat4 ret;
            ret.setScale(v);
            return ret;
        }

        FMat4 scale(f32 x,f32 y,f32 z) {
            return scale(FVec3(x,y,z));
        }

        //
        FMat4 reflect(const FVec4 &plane) {
            FMat4 ret;
            f32 x = plane.x;
            f32 y = plane.y;
            f32 z = plane.z;
            f32 x2 = x * 2.0f;
            f32 y2 = y * 2.0f;
            f32 z2 = z * 2.0f;
            ret.m00 = 1.0f - x * x2; ret.m01 = -y * x2;       ret.m02 = -z * x2;       ret.m03 = -plane.w * x2;
            ret.m10 = -x * y2;       ret.m11 = 1.0f - y * y2; ret.m12 = -z * y2;       ret.m13 = -plane.w * y2;
            ret.m20 = -x * z2;       ret.m21 = -y * z2;       ret.m22 = 1.0f - z * z2; ret.m23 = -plane.w * z2;
            ret.m30 = 0.0f;          ret.m31 = 0.0f;          ret.m32 = 0.0f;          ret.m33 = 1.0f;
            return ret;
        }

        FMat4 ortho(f32 l,f32 r,f32 b,f32 t,f32 n,f32 f) {
            FMat4 ret;
            f32 rl = r - l;
            f32 tb = t - b;
            f32 fn = f - n;
            ret.m00 = 2.0f / rl; ret.m01 = 0.0f;      ret.m02 = 0.0f;       ret.m03 = -(r + l) / rl;
            ret.m10 = 0.0f;      ret.m11 = 2.0f / tb; ret.m12 = 0.0f;       ret.m13 = -(t + b) / tb;
            ret.m20 = 0.0f;      ret.m21 = 0.0f;      ret.m22 = -2.0f / fn; ret.m23 = -(f + n) / fn;
            ret.m30 = 0.0f;      ret.m31 = 0.0f;      ret.m32 = 0.0f;       ret.m33 = 1.0f;
            return ret;
        }

        FMat4 frustum(f32 l,f32 r,f32 b,f32 t,f32 n,f32 f) {
            FMat4 ret;
            f32 rl = r - l;
            f32 tb = t - b;
            f32 fn = f - n;
            ret.m00 = 2.0f * n / rl; ret.m01 = 0.0f;          ret.m02 = (r + l) / rl;  ret.m03 = 0.0f;
            ret.m10 = 0.0f;          ret.m11 = 2.0f * n / tb; ret.m12 = (t + b) / tb;  ret.m13 = 0.0f;
            ret.m20 = 0.0f;          ret.m21 = 0.0f;          ret.m22 = -(f + n) / fn; ret.m23 = -2.0f * f * n / fn;
            ret.m30 = 0.0f;          ret.m31 = 0.0f;          ret.m32 = -1.0f;         ret.m33 = 0.0f;
            return ret;
        }

        FMat4 perspective(f32 fov,f32 aspect,f32 n,f32 f) {
            FMat4 ret;
            f32 h = 1.0f;
            f32 w = aspect;
            if(!compare(fov,90.0f)) {
                h = Math::tan(fov * DEG2RAD * 0.5f);
                w = h * aspect;
            }
            f32 fn = f - n;
            ret.m00 = 1.0f / w; ret.m01 = 0.0f;     ret.m02 = 0.0f;          ret.m03 = 0.0f;
            ret.m10 = 0.0f;     ret.m11 = 1.0f / h; ret.m12 = 0.0f;          ret.m13 = 0.0f;
            ret.m20 = 0.0f;     ret.m21 = 0.0f;     ret.m22 = -(f + n) / fn; ret.m23 = -2.0f * f * n / fn;
            ret.m30 = 0.0f;     ret.m31 = 0.0f;     ret.m32 = -1.0f;         ret.m33 = 0.0f;
            return ret;
        }

        FMat4 setTo(const FVec3 &position,const FVec3 &direction,const FVec3 &up) {
            FMat4 ret;
            FVec3 z = normalize(position - direction);
            FVec3 x = normalize(cross(up,z));
            FVec3 y = normalize(cross(z,x));
            ret.m00 = x.x;  ret.m01 = y.x;  ret.m02 = z.x;  ret.m03 = position.x;
            ret.m10 = x.y;  ret.m11 = y.y;  ret.m12 = z.y;  ret.m13 = position.y;
            ret.m20 = x.z;  ret.m21 = y.z;  ret.m22 = z.z;  ret.m23 = position.z;
            ret.m30 = 0.0f; ret.m31 = 0.0f; ret.m32 = 0.0f; ret.m33 = 1.0f;
            return ret;
        }

        FMat4 lookAt(const FVec3 &position,const FVec3 &direction,const FVec3 &up) {
            FMat4 ret,m0,m1;
            FVec3 z = normalize(position - direction);
            FVec3 x = normalize(cross(up,z));
            FVec3 y = normalize(cross(z,x));
            m0.m00 = x.x;  m0.m01 = x.y;  m0.m02 = x.z;  m0.m03 = 0.0f;
            m0.m10 = y.x;  m0.m11 = y.y;  m0.m12 = y.z;  m0.m13 = 0.0f;
            m0.m20 = z.x;  m0.m21 = z.y;  m0.m22 = z.z;  m0.m23 = 0.0f;
            m0.m30 = 0.0f; m0.m31 = 0.0f; m0.m32 = 0.0f; m0.m33 = 1.0f;
            m1.setTranslate(-position);
            return mul(ret,m0,m1);
        }

        FMat4 obliqueProjection(const FMat4 &projection,const FVec4 &plane) {
            FMat4 ret,transform;
            FVec4 v = plane * inverse(projection);
            s32 z = Math::abs(Math::ftoi(v.z));
            if(z) v /= Math::itof(z);
            v.w -= 1.0f;
            if(v.z < 0.0f) v = -v;
            transform.setIdentity();
            transform.setRow(2,v);
            return mul(ret,transform,projection);
        }

        FMat4 symmetryProjection(const FMat4 &projection) {
            FMat4 ret;
            f32 r = (projection.m02 + 1.0f) / projection.m00;
            f32 t = (projection.m12 + 1.0f) / projection.m11;
            f32 l = r - 2.0f / projection.m00;
            f32 b = t - 2.0f / projection.m11;
            f32 w = max(Math::abs(r),Math::abs(l));
            f32 h = max(Math::abs(t),Math::abs(b));
            ret.m00 = 1.0f / w; ret.m01 = 0.0f;     ret.m02 = 0.0f;           ret.m03 = 0.0f;
            ret.m10 = 0.0f;     ret.m11 = 1.0f / h; ret.m12 = 0.0f;           ret.m13 = 0.0f;
            ret.m20 = 0.0f;     ret.m21 = 0.0f;     ret.m22 = projection.m22; ret.m23 = projection.m23;
            ret.m30 = 0.0f;     ret.m31 = 0.0f;     ret.m32 = -1.0f;          ret.m33 = 0.0f;
            return ret;
        }

        FMat4 cubeTransform(s32 face) {
            static const FMat4 transforms[6] = {
                rotateY(90.0f) * rotateX(180.0f),
                rotateY(-90.0f) * rotateX(180.0f),
                rotateX(-90.0f),
                rotateX(90.0f),
                rotateX(180.0f),
                rotateZ(180.0f),
            };
            assert((u32)face <= 5 && "cubeTransform(): bad face number");
            return transforms[face];
        }

        //
        void decomposeTransform(const FMat4 &m,FVec4 &position,SVQuat &rot) {
            FMat3 rotate,scale;
            FMat3 rotation = FMat3(m);
            orthonormalize(rotate,rotation);
            mul(scale,transpose(rotate),rotation);
            position.x = m.m03;
            position.y = m.m13;
            position.z = m.m23;
            position.w = (scale.m00 + scale.m11 + scale.m22) * (1.0f / 3.0f);
            rot = rotate.getQuat();
        }

        FMat4 &composeTransform(FMat4 &ret,const FVec4 &position,const SVQuat &rot) {
            f32 x2 = (rot.x + rot.x) * position.w;
            f32 y2 = (rot.y + rot.y) * position.w;
            f32 z2 = (rot.z + rot.z) * position.w;
            f32 xx2 = rot.x * x2;
            f32 yy2 = rot.y * y2;
            f32 zz2 = rot.z * z2;
            f32 zx2 = rot.z * x2;
            f32 xy2 = rot.x * y2;
            f32 yz2 = rot.y * z2;
            f32 wx2 = rot.w * x2;
            f32 wy2 = rot.w * y2;
            f32 wz2 = rot.w * z2;
            ret.m00 = position.w - yy2 - zz2;
            ret.m10 = xy2 + wz2;
            ret.m20 = zx2 - wy2;
            ret.m30 = 0.0f;
            ret.m01 = xy2 - wz2;
            ret.m11 = position.w - xx2 - zz2;
            ret.m21 = yz2 + wx2;
            ret.m31 = 0.0f;
            ret.m02 = zx2 + wy2;
            ret.m12 = yz2 - wx2;
            ret.m22 = position.w - xx2 - yy2;
            ret.m32 = 0.0f;
            ret.m03 = position.x;
            ret.m13 = position.y;
            ret.m23 = position.z;
            ret.m33 = 1.0f;
            return ret;
        }

        void decomposeTransform(const FMat4 &m,FVec3 &position,SVQuat &rot,FVec3 &s) {
            FMat3 rotate,scale;
            FMat3 rotation = FMat3(m);
            orthonormalize(rotate,rotation);
            mul(scale,transpose(rotate),rotation);
            position.x = m.m03;
            position.y = m.m13;
            position.z = m.m23;
            rot = rotate.getQuat();
            s.x = scale.m00;
            s.y = scale.m11;
            s.z = scale.m22;
        }

        FMat4 &composeTransform(FMat4 &ret,const FVec3 &position,const SVQuat &rot,const FVec3 &s) {
            FMat3 rotation,scale;
            scale.setDiagonal(s);
            ret.set(mul(rotation,rot.getMat3(),scale),position);
            return ret;
        }

        void decomposeProjection(const FMat4 &projection,f32 &znear,f32 &zfar) {
            if(compare(projection.m32,-1.0f)) {
                znear = projection.m23 / (projection.m22 - 1.0f);
                zfar = projection.m23 / (projection.m22 + 1.0f);
            } else {
                znear = (projection.m23 + 1.0f) / projection.m22;
                zfar = (projection.m23 - 1.0f) / projection.m22;
            }
        }

        //
        const FMat4 &hardwareProjectionGL(const FMat4 &projection,s32 width,s32 height) {
            return projection;
        }

        const FMat4 &hardwareProjectionD3D9(const FMat4 &projection,s32 width,s32 height) {
            static FMat4 ret,temp;
            static const FMat4 offset = translate(0.0f,0.0f,0.5f) * scale(1.0f,1.0f,0.5f);
            return mul(ret,translate(-1.0f / width,1.0f / height,0.0f),mul(temp,offset,projection));
        }

        const FMat4 &hardwareProjectionD3D10(const FMat4 &projection,s32 width,s32 height) {
            static FMat4 ret;
            static const FMat4 offset = translate(0.0f,0.0f,0.5f) * scale(1.0f,1.0f,0.5f);
            return mul(ret,offset,projection);
        }

        const FMat4 &(*hardwareProjection)(const FMat4 &projection,s32 width,s32 height) = hardwareProjectionGL;


    }//!namesapce util
    
}//!namespace sv
