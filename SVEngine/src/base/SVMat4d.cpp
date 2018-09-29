#include "SVMat4d.h"
#include "SVMat2.h"
#include "SVMat3.h"
#include "SVMat4.h"
#include "SVQuat.h"

namespace sv {
    
    namespace util {

        /******************************************************************************\
        *
        * DMat4
        *
        \******************************************************************************/

        //
        static const f64 DMat4_identity_values[12] = {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 0.0,
        };

        //
        const DMat4 DMat4_zero(0.0);
        const DMat4 DMat4_one(1.0);
        const DMat4 DMat4_identity(DMat4_identity_values);

        //
        DMat4::DMat4(f64 v) {
            m00 = v; m01 = v; m02 = v; m03 = v;
            m10 = v; m11 = v; m12 = v; m13 = v;
            m20 = v; m21 = v; m22 = v; m23 = v;
        }

        DMat4::DMat4(const FMat2 &m) {
            m00 = m.m00; m01 = m.m01; m02 = 0.0; m03 = 0.0;
            m10 = m.m10; m11 = m.m11; m12 = 0.0; m13 = 0.0;
            m20 = 0.0;   m21 = 0.0;   m22 = 1.0; m23 = 0.0;
        }

        DMat4::DMat4(const FMat3 &m) {
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = 0.0;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = 0.0;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = 0.0;
        }

        DMat4::DMat4(const FMat4 &m) {
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = m.m03;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = m.m13;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = m.m23;
        }

        DMat4::DMat4(const DMat4 &m) {
            #ifdef USE_SSE2
                col0 = m.col0; col1 = m.col1;
                col2 = m.col2; col3 = m.col3;
                col4 = m.col4; col5 = m.col5;
            #else
                m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = m.m03;
                m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = m.m13;
                m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = m.m23;
            #endif
        }

        DMat4::DMat4(const SVQuat &q) {
            FMat3 m = q.getMat3();
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = 0.0;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = 0.0;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = 0.0;
        }

        DMat4::DMat4(const f64 *m) {
            m00 = m[0]; m01 = m[3]; m02 = m[6]; m03 = m[9];
            m10 = m[1]; m11 = m[4]; m12 = m[7]; m13 = m[10];
            m20 = m[2]; m21 = m[5]; m22 = m[8]; m23 = m[11];
        }

        DMat4::DMat4(const FMat3 &m,const DVec3 &v) {
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = v.x;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = v.y;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = v.z;
        }

        DMat4::DMat4(const SVQuat &q,const DVec3 &v) {
            FMat3 m = q.getMat3();
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = v.x;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = v.y;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = v.z;
        }

        //
        DMat4 DMat4::operator-() const {
            DMat4 ret;
            ret.m00 = -m00; ret.m01 = -m01; ret.m02 = -m02; ret.m03 = -m03;
            ret.m10 = -m10; ret.m11 = -m11; ret.m12 = -m12; ret.m13 = -m13;
            ret.m20 = -m20; ret.m21 = -m21; ret.m22 = -m22; ret.m23 = -m23;
            return ret;
        }

        DMat4 &DMat4::operator*=(f64 v) {
            return mul(*this,*this,v);
        }

        DMat4 &DMat4::operator*=(const DMat4 &m) {
            return mul(*this,DMat4(*this),m);
        }

        DMat4 &DMat4::operator+=(const DMat4 &m) {
            return add(*this,*this,m);
        }

        DMat4 &DMat4::operator-=(const DMat4 &m) {
            return sub(*this,*this,m);
        }

        //
        void DMat4::set(const FMat2 &m) {
            m00 = m.m00; m01 = m.m01; m02 = 0.0; m03 = 0.0;
            m10 = m.m10; m11 = m.m11; m12 = 0.0; m13 = 0.0;
            m20 = 0.0;   m21 = 0.0;   m22 = 1.0; m23 = 0.0;
        }

        void DMat4::set(const FMat3 &m) {
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = 0.0;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = 0.0;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = 0.0;
        }

        void DMat4::set(const FMat4 &m) {
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = m.m03;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = m.m13;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = m.m23;
        }

        void DMat4::set(const DMat4 &m) {
            #ifdef USE_SSE2
                col0 = m.col0; col1 = m.col1;
                col2 = m.col2; col3 = m.col3;
                col4 = m.col4; col5 = m.col5;
            #else
                m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = m.m03;
                m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = m.m13;
                m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = m.m23;
            #endif
        }

        void DMat4::set(const SVQuat &q) {
            FMat3 m = q.getMat3();
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = 0.0;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = 0.0;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = 0.0;
        }

        void DMat4::set(const f64 *m) {
            m00 = m[0]; m01 = m[3]; m02 = m[6]; m03 = m[9];
            m10 = m[1]; m11 = m[4]; m12 = m[7]; m13 = m[10];
            m20 = m[2]; m21 = m[5]; m22 = m[8]; m23 = m[11];
        }

        void DMat4::set(const FMat3 &m,const DVec3 &v) {
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = v.x;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = v.y;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = v.z;
        }

        void DMat4::set(const SVQuat &q,const DVec3 &v) {
            FMat3 m = q.getMat3();
            m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = v.x;
            m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = v.y;
            m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = v.z;
        }

        void DMat4::get(f64 *m) const {
            m[0] = m00; m[3] = m01; m[6] = m02; m[9] = m03;
            m[1] = m10; m[4] = m11; m[7] = m12; m[10] = m13;
            m[2] = m20; m[5] = m21; m[8] = m22; m[11] = m23;
        }

        //
        void DMat4::setRow(s32 row,const DVec4 &v) {
            assert((u32)row < 4 && "DMat4::setRow(): bad row");
            switch(row) {
                case 0: m00 = v.x; m01 = v.y; m02 = v.z; m03 = v.w; break;
                case 1: m10 = v.x; m11 = v.y; m12 = v.z; m13 = v.w; break;
                case 2: m20 = v.x; m21 = v.y; m22 = v.z; m23 = v.w; break;
            }
        }

        void DMat4::setRow3(s32 row,const DVec3 &v) {
            assert((u32)row < 4 && "DMat4::setRow3(): bad row");
            switch(row) {
                case 0: m00 = v.x; m01 = v.y; m02 = v.z; break;
                case 1: m10 = v.x; m11 = v.y; m12 = v.z; break;
                case 2: m20 = v.x; m21 = v.y; m22 = v.z; break;
            }
        }

        DVec4 DMat4::getRow(s32 row) const {
            assert((u32)row < 4 && "DMat4::getRow(): bad row");
            switch(row) {
                case 0: return DVec4(m00,m01,m02,m03);
                case 1: return DVec4(m10,m11,m12,m13);
                case 2: return DVec4(m20,m21,m22,m23);
                case 3: return DVec4(0.0,0.0,0.0,1.0);
            }
            return DVec4_zero;
        }

        DVec3 DMat4::getRow3(s32 row) const {
            assert((u32)row < 4 && "DMat4::getRow3(): bad row");
            switch(row) {
                case 0: return DVec3(m00,m01,m02);
                case 1: return DVec3(m10,m11,m12);
                case 2: return DVec3(m20,m21,m22);
                case 3: return DVec3(0.0,0.0,0.0);
            }
            return DVec3_zero;
        }

        //
        void DMat4::setColumn(s32 column,const DVec4 &v) {
            assert((u32)column < 4 && "DMat4::setColumn(): bad column");
            switch(column) {
                case 0: m00 = v.x; m10 = v.y; m20 = v.z; break;
                case 1: m01 = v.x; m11 = v.y; m21 = v.z; break;
                case 2: m02 = v.x; m12 = v.y; m22 = v.z; break;
                case 3: m03 = v.x; m13 = v.y; m23 = v.z; break;
            }
        }

        void DMat4::setColumn3(s32 column,const DVec3 &v) {
            assert((u32)column < 4 && "DMat4::setColumn3(): bad column");
            switch(column) {
                case 0: m00 = v.x; m10 = v.y; m20 = v.z; break;
                case 1: m01 = v.x; m11 = v.y; m21 = v.z; break;
                case 2: m02 = v.x; m12 = v.y; m22 = v.z; break;
                case 3: m03 = v.x; m13 = v.y; m23 = v.z; break;
            }
        }

        DVec4 DMat4::getColumn(s32 column) const {
            assert((u32)column < 4 && "DMat4::getColumn(): bad column");
            switch(column) {
                case 0: return DVec4(m00,m10,m20,0.0);
                case 1: return DVec4(m01,m11,m21,0.0);
                case 2: return DVec4(m02,m12,m22,0.0);
                case 3: return DVec4(m03,m13,m23,1.0);
            }
            return DVec4_zero;
        }

        DVec3 DMat4::getColumn3(s32 column) const {
            assert((u32)column < 4 && "DMat4::getColumn3(): bad column");
            switch(column) {
                case 0: return DVec3(m00,m10,m20);
                case 1: return DVec3(m01,m11,m21);
                case 2: return DVec3(m02,m12,m22);
                case 3: return DVec3(m03,m13,m23);
            }
            return DVec3_zero;
        }

        //
        void DMat4::setZero() {
            m00 = 0.0; m01 = 0.0; m02 = 0.0; m03 = 0.0;
            m10 = 0.0; m11 = 0.0; m12 = 0.0; m13 = 0.0;
            m20 = 0.0; m21 = 0.0; m22 = 0.0; m23 = 0.0;
        }

        void DMat4::setIdentity() {
            m00 = 1.0; m01 = 0.0; m02 = 0.0; m03 = 0.0;
            m10 = 0.0; m11 = 1.0; m12 = 0.0; m13 = 0.0;
            m20 = 0.0; m21 = 0.0; m22 = 1.0; m23 = 0.0;
        }

        void DMat4::setTranslate(const DVec3 &v) {
            m00 = 1.0; m01 = 0.0; m02 = 0.0; m03 = v.x;
            m10 = 0.0; m11 = 1.0; m12 = 0.0; m13 = v.y;
            m20 = 0.0; m21 = 0.0; m22 = 1.0; m23 = v.z;
        }

        void DMat4::setRotate(const DVec3 &axis,f64 angle) {
            f64 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            DVec3 v = normalize(axis);
            f64 xx = v.x * v.x;
            f64 yy = v.y * v.y;
            f64 zz = v.z * v.z;
            f64 xy = v.x * v.y;
            f64 yz = v.y * v.z;
            f64 zx = v.z * v.x;
            f64 xs = v.x * s;
            f64 ys = v.y * s;
            f64 zs = v.z * s;
            m00 = (1.0 - c) * xx + c;  m01 = (1.0 - c) * xy - zs; m02 = (1.0 - c) * zx + ys; m03 = 0.0;
            m10 = (1.0 - c) * xy + zs; m11 = (1.0 - c) * yy + c;  m12 = (1.0 - c) * yz - xs; m13 = 0.0;
            m20 = (1.0 - c) * zx - ys; m21 = (1.0 - c) * yz + xs; m22 = (1.0 - c) * zz + c;  m23 = 0.0;
        }

        void DMat4::setRotateX(f64 angle) {
            f64 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            m00 = 1.0; m01 = 0.0; m02 = 0.0; m03 = 0.0;
            m10 = 0.0; m11 = c;   m12 = -s;  m13 = 0.0;
            m20 = 0.0; m21 = s;   m22 = c;   m23 = 0.0;
        }

        void DMat4::setRotateY(f64 angle) {
            f64 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            m00 = c;   m01 = 0.0; m02 = s;   m03 = 0.0;
            m10 = 0.0; m11 = 1.0; m12 = 0.0; m13 = 0.0;
            m20 = -s;  m21 = 0.0; m22 = c;   m23 = 0.0;
        }

        void DMat4::setRotateZ(f64 angle) {
            f64 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            m00 = c;   m01 = -s;  m02 = 0.0; m03 = 0.0;
            m10 = s;   m11 = c;   m12 = 0.0; m13 = 0.0;
            m20 = 0.0; m21 = 0.0; m22 = 1.0; m23 = 0.0;
        }

        void DMat4::setScale(const DVec3 &v) {
            m00 = v.x; m01 = 0.0; m02 = 0.0; m03 = 0.0;
            m10 = 0.0; m11 = v.y; m12 = 0.0; m13 = 0.0;
            m20 = 0.0; m21 = 0.0; m22 = v.z; m23 = 0.0;
        }

        //
        s32 operator==(const DMat4 &m0,const DMat4 &m1) {
            return compare(m0,m1);
        }

        s32 operator!=(const DMat4 &m0,const DMat4 &m1) {
            return !compare(m0,m1);
        }

        DMat4 operator*(const DMat4 &m,f64 v) {
            DMat4 ret;
            return mul(ret,m,v);
        }

        FVec2 operator*(const DMat4 &m,const FVec2 &v) {
            FVec2 ret;
            return mul(ret,m,v);
        }

        FVec2 operator*(const FVec2 &v,const DMat4 &m) {
            FVec2 ret;
            return mul(ret,v,m);
        }

        FVec3 operator*(const DMat4 &m,const FVec3 &v) {
            FVec3 ret;
            return mul(ret,m,v);
        }

        FVec3 operator*(const FVec3 &v,const DMat4 &m) {
            FVec3 ret;
            return mul(ret,v,m);
        }

        FVec4 operator*(const DMat4 &m,const FVec4 &v) {
            FVec4 ret;
            return mul(ret,m,v);
        }

        FVec4 operator*(const FVec4 &v,const DMat4 &m) {
            FVec4 ret;
            return mul(ret,v,m);
        }

        DVec2 operator*(const DMat4 &m,const DVec2 &v) {
            DVec2 ret;
            return mul(ret,m,v);
        }

        DVec2 operator*(const DVec2 &v,const DMat4 &m) {
            DVec2 ret;
            return mul(ret,v,m);
        }

        DVec3 operator*(const DMat4 &m,const DVec3 &v) {
            DVec3 ret;
            return mul(ret,m,v);
        }

        DVec3 operator*(const DVec3 &v,const DMat4 &m) {
            DVec3 ret;
            return mul(ret,v,m);
        }

        DVec4 operator*(const DMat4 &m,const DVec4 &v) {
            DVec4 ret;
            return mul(ret,m,v);
        }

        DVec4 operator*(const DVec4 &v,const DMat4 &m) {
            DVec4 ret;
            return mul(ret,v,m);
        }

        DMat4 operator*(const DMat4 &m0,const DMat4 &m1) {
            DMat4 ret;
            return mul(ret,m0,m1);
        }

        DMat4 operator+(const DMat4 &m0,const DMat4 &m1) {
            DMat4 ret;
            return add(ret,m0,m1);
        }

        DMat4 operator-(const DMat4 &m0,const DMat4 &m1) {
            DMat4 ret;
            return sub(ret,m0,m1);
        }

        //
        s32 compare(const DMat4 &m0,const DMat4 &m1) {
            return (compare(m0.m00,m1.m00) && compare(m0.m10,m1.m10) && compare(m0.m20,m1.m20) &&
                compare(m0.m01,m1.m01) && compare(m0.m11,m1.m11) && compare(m0.m21,m1.m21) &&
                compare(m0.m02,m1.m02) && compare(m0.m12,m1.m12) && compare(m0.m22,m1.m22) &&
                compare(m0.m03,m1.m03) && compare(m0.m13,m1.m13) && compare(m0.m23,m1.m23));
        }

        s32 compare(const DMat4 &m0,const DMat4 &m1,f64 epsilon) {
            return (compare(m0.m00,m1.m00,epsilon) && compare(m0.m10,m1.m10,epsilon) && compare(m0.m20,m1.m20,epsilon) &&
                compare(m0.m01,m1.m01,epsilon) && compare(m0.m11,m1.m11,epsilon) && compare(m0.m21,m1.m21,epsilon) &&
                compare(m0.m02,m1.m02,epsilon) && compare(m0.m12,m1.m12,epsilon) && compare(m0.m22,m1.m22,epsilon) &&
                compare(m0.m03,m1.m03,epsilon) && compare(m0.m13,m1.m13,epsilon) && compare(m0.m23,m1.m23,epsilon));
        }

        f64 determinant(const DMat4 &m) {
            f64 det = 0.0;
            det =  m.m00 * (m.m11 * m.m22 - m.m12 * m.m21);
            det -= m.m01 * (m.m10 * m.m22 - m.m12 * m.m20);
            det += m.m02 * (m.m10 * m.m21 - m.m11 * m.m20);
            return det;
        }

        DMat4 &mul(DMat4 &ret,const DMat4 &m,f64 v) {
            #ifdef USE_SSE2
                __m128d temp = _mm_set1_pd(v);
                ret.col0 = _mm_mul_pd(m.col0,temp); ret.col1 = _mm_mul_pd(m.col1,temp);
                ret.col2 = _mm_mul_pd(m.col2,temp); ret.col3 = _mm_mul_pd(m.col3,temp);
                ret.col4 = _mm_mul_pd(m.col4,temp); ret.col5 = _mm_mul_pd(m.col5,temp);
            #else
                ret.m00 = m.m00 * v; ret.m01 = m.m01 * v; ret.m02 = m.m02 * v; ret.m03 = m.m03 * v;
                ret.m10 = m.m10 * v; ret.m11 = m.m11 * v; ret.m12 = m.m12 * v; ret.m13 = m.m13 * v;
                ret.m20 = m.m20 * v; ret.m21 = m.m21 * v; ret.m22 = m.m22 * v; ret.m23 = m.m23 * v;
            #endif
            return ret;
        }

        FVec2 &mul(FVec2 &ret,const DMat4 &m,const FVec2 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y + m.m03);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y + m.m13);
            return ret;
        }

        FVec2 &mul(FVec2 &ret,const FVec2 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y);
            return ret;
        }

        FVec3 &mul(FVec3 &ret,const DMat4 &m,const FVec3 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13);
            ret.z = (f32)(m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23);
            return ret;
        }

        FVec3 &mul(FVec3 &ret,const FVec3 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y + m.m20 * v.z);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y + m.m21 * v.z);
            ret.z = (f32)(m.m02 * v.x + m.m12 * v.y + m.m22 * v.z);
            return ret;
        }

        FVec4 &mul(FVec4 &ret,const DMat4 &m,const FVec4 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w);
            ret.z = (f32)(m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w);
            ret.w = v.w;
            return ret;
        }

        FVec4 &mul(FVec4 &ret,const FVec4 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y + m.m20 * v.z);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y + m.m21 * v.z);
            ret.z = (f32)(m.m02 * v.x + m.m12 * v.y + m.m22 * v.z);
            ret.w = (f32)(m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + v.w);
            return ret;
        }

        FVec2 &mul(FVec2 &ret,const DMat4 &m,const DVec2 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y + m.m03);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y + m.m13);
            return ret;
        }

        FVec2 &mul(FVec2 &ret,const DVec2 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y);
            return ret;
        }

        FVec3 &mul(FVec3 &ret,const DMat4 &m,const DVec3 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13);
            ret.z = (f32)(m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23);
            return ret;
        }

        FVec3 &mul(FVec3 &ret,const DVec3 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y + m.m20 * v.z);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y + m.m21 * v.z);
            ret.z = (f32)(m.m02 * v.x + m.m12 * v.y + m.m22 * v.z);
            return ret;
        }

        FVec4 &mul(FVec4 &ret,const DMat4 &m,const DVec4 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w);
            ret.z = (f32)(m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w);
            ret.w = (f32)v.w;
            return ret;
        }

        FVec4 &mul(FVec4 &ret,const DVec4 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y + m.m20 * v.z);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y + m.m21 * v.z);
            ret.z = (f32)(m.m02 * v.x + m.m12 * v.y + m.m22 * v.z);
            ret.w = (f32)(m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + v.w);
            return ret;
        }

        DVec2 &mul(DVec2 &ret,const DMat4 &m,const DVec2 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m03;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m13;
            return ret;
        }

        DVec2 &mul(DVec2 &ret,const DVec2 &v,const DMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y;
            ret.y = m.m01 * v.x + m.m11 * v.y;
            return ret;
        }

        DVec3 &mul(DVec3 &ret,const DMat4 &m,const DVec3 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13;
            ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23;
            return ret;
        }

        DVec3 &mul(DVec3 &ret,const DVec3 &v,const DMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
            return ret;
        }

        DVec4 &mul(DVec4 &ret,const DMat4 &m,const DVec4 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w;
            ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w;
            ret.w = v.w;
            return ret;
        }

        DVec4 &mul(DVec4 &ret,const DVec4 &v,const DMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
            ret.w = m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + v.w;
            return ret;
        }

        FVec2 &mul3(FVec2 &ret,const DMat4 &m,const FVec2 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y);
            return ret;
        }

        FVec2 &mul3(FVec2 &ret,const FVec2 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y);
            return ret;
        }

        FVec3 &mul3(FVec3 &ret,const DMat4 &m,const FVec3 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y + m.m02 * v.z);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y + m.m12 * v.z);
            ret.z = (f32)(m.m20 * v.x + m.m21 * v.y + m.m22 * v.z);
            return ret;
        }

        FVec3 &mul3(FVec3 &ret,const FVec3 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y + m.m20 * v.z);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y + m.m21 * v.z);
            ret.z = (f32)(m.m02 * v.x + m.m12 * v.y + m.m22 * v.z);
            return ret;
        }

        FVec4 &mul3(FVec4 &ret,const DMat4 &m,const FVec4 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y + m.m02 * v.z);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y + m.m12 * v.z);
            ret.z = (f32)(m.m20 * v.x + m.m21 * v.y + m.m22 * v.z);
            return ret;
        }

        FVec4 &mul3(FVec4 &ret,const FVec4 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y + m.m20 * v.z);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y + m.m21 * v.z);
            ret.z = (f32)(m.m02 * v.x + m.m12 * v.y + m.m22 * v.z);
            return ret;
        }

        FVec2 &mul3(FVec2 &ret,const DMat4 &m,const DVec2 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y);
            return ret;
        }

        FVec2 &mul3(FVec2 &ret,const DVec2 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y);
            return ret;
        }

        FVec3 &mul3(FVec3 &ret,const DMat4 &m,const DVec3 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y + m.m02 * v.z);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y + m.m12 * v.z);
            ret.z = (f32)(m.m20 * v.x + m.m21 * v.y + m.m22 * v.z);
            return ret;
        }

        FVec3 &mul3(FVec3 &ret,const DVec3 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y + m.m20 * v.z);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y + m.m21 * v.z);
            ret.z = (f32)(m.m02 * v.x + m.m12 * v.y + m.m22 * v.z);
            return ret;
        }

        FVec4 &mul3(FVec4 &ret,const DMat4 &m,const DVec4 &v) {
            ret.x = (f32)(m.m00 * v.x + m.m01 * v.y + m.m02 * v.z);
            ret.y = (f32)(m.m10 * v.x + m.m11 * v.y + m.m12 * v.z);
            ret.z = (f32)(m.m20 * v.x + m.m21 * v.y + m.m22 * v.z);
            return ret;
        }

        FVec4 &mul3(FVec4 &ret,const DVec4 &v,const DMat4 &m) {
            ret.x = (f32)(m.m00 * v.x + m.m10 * v.y + m.m20 * v.z);
            ret.y = (f32)(m.m01 * v.x + m.m11 * v.y + m.m21 * v.z);
            ret.z = (f32)(m.m02 * v.x + m.m12 * v.y + m.m22 * v.z);
            return ret;
        }

        DVec2 &mul3(DVec2 &ret,const DMat4 &m,const DVec2 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y;
            ret.y = m.m10 * v.x + m.m11 * v.y;
            return ret;
        }

        DVec2 &mul3(DVec2 &ret,const DVec2 &v,const DMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y;
            ret.y = m.m01 * v.x + m.m11 * v.y;
            return ret;
        }

        DVec3 &mul3(DVec3 &ret,const DMat4 &m,const DVec3 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
            ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;
            return ret;
        }

        DVec3 &mul3(DVec3 &ret,const DVec3 &v,const DMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
            return ret;
        }

        DVec4 &mul3(DVec4 &ret,const DMat4 &m,const DVec4 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
            ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;
            return ret;
        }

        DVec4 &mul3(DVec4 &ret,const DVec4 &v,const DMat4 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
            return ret;
        }

        DMat4 &mul(DMat4 &ret,const DMat4 &m0,const DMat4 &m1) {
            #ifdef USE_SSE2
                __m128d col_01 = _mm_shuffle_pd(m0.col0,m0.col1,_MM_PERM22(Y,X));
                __m128d col_12 = _mm_shuffle_pd(m0.col1,m0.col2,_MM_PERM22(Y,X));
                __m128d col_34 = _mm_shuffle_pd(m0.col3,m0.col4,_MM_PERM22(Y,X));
                __m128d res_0 = _mm_mul_pd(m0.col0,_MM_SWIZZLE2(m1.col0,X,X));
                __m128d res_1 = _mm_mul_pd(col_12,_MM_SWIZZLE2(m1.col0,Y,Y));
                __m128d res_2 = _mm_mul_pd(m0.col3,_MM_SWIZZLE2(m1.col1,X,X));
                ret.col0 = _mm_add_pd(_mm_add_pd(res_0,res_1),res_2);
                res_0 = _mm_mul_pd(col_01,_MM_SWIZZLE2(m1.col1,Y,Y));
                res_1 = _mm_mul_pd(m0.col2,_MM_SWIZZLE2(m1.col2,X,X));
                res_2 = _mm_mul_pd(col_34,_MM_SWIZZLE2(m1.col2,Y,Y));
                ret.col2 = _mm_add_pd(_mm_add_pd(res_0,res_1),res_2);
                res_0 = _mm_mul_pd(m0.col0,_MM_SWIZZLE2(m1.col3,X,X));
                res_1 = _mm_mul_pd(col_12,_MM_SWIZZLE2(m1.col3,Y,Y));
                res_2 = _mm_mul_pd(m0.col3,_MM_SWIZZLE2(m1.col4,X,X));
                ret.col3 = _mm_add_pd(_mm_add_pd(res_0,res_1),res_2);
                res_0 = _mm_mul_pd(col_01,_MM_SWIZZLE2(m1.col4,Y,Y));
                res_1 = _mm_mul_pd(m0.col2,_MM_SWIZZLE2(m1.col5,X,X));
                res_2 = _mm_mul_pd(col_34,_MM_SWIZZLE2(m1.col5,Y,Y));
                ret.col5 = _mm_add_pd(_mm_add_pd(res_0,res_1),_mm_add_pd(res_2,m0.col5));
                ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
                ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
                ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
                ret.m03 = m0.m00 * m1.m03 + m0.m01 * m1.m13 + m0.m02 * m1.m23 + m0.m03;
            #else
                ret.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10 + m0.m02 * m1.m20;
                ret.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10 + m0.m12 * m1.m20;
                ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
                ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
                ret.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11 + m0.m12 * m1.m21;
                ret.m21 = m0.m20 * m1.m01 + m0.m21 * m1.m11 + m0.m22 * m1.m21;
                ret.m02 = m0.m00 * m1.m02 + m0.m01 * m1.m12 + m0.m02 * m1.m22;
                ret.m12 = m0.m10 * m1.m02 + m0.m11 * m1.m12 + m0.m12 * m1.m22;
                ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
                ret.m03 = m0.m00 * m1.m03 + m0.m01 * m1.m13 + m0.m02 * m1.m23 + m0.m03;
                ret.m13 = m0.m10 * m1.m03 + m0.m11 * m1.m13 + m0.m12 * m1.m23 + m0.m13;
                ret.m23 = m0.m20 * m1.m03 + m0.m21 * m1.m13 + m0.m22 * m1.m23 + m0.m23;
            #endif
            return ret;
        }

        DMat4 &mul4(DMat4 &ret,const DMat4 &m0,const DMat4 &m1) {
            return mul(ret,m0,m1);
        }

        DMat4 &mul3(DMat4 &ret,const DMat4 &m0,const DMat4 &m1) {
            #ifdef USE_SSE2
                __m128d col_01 = _mm_shuffle_pd(m0.col0,m0.col1,_MM_PERM22(Y,X));
                __m128d col_12 = _mm_shuffle_pd(m0.col1,m0.col2,_MM_PERM22(Y,X));
                __m128d col_34 = _mm_shuffle_pd(m0.col3,m0.col4,_MM_PERM22(Y,X));
                __m128d res_0 = _mm_mul_pd(m0.col0,_MM_SWIZZLE2(m1.col0,X,X));
                __m128d res_1 = _mm_mul_pd(col_12,_MM_SWIZZLE2(m1.col0,Y,Y));
                __m128d res_2 = _mm_mul_pd(m0.col3,_MM_SWIZZLE2(m1.col1,X,X));
                ret.col0 = _mm_add_pd(_mm_add_pd(res_0,res_1),res_2);
                res_0 = _mm_mul_pd(col_01,_MM_SWIZZLE2(m1.col1,Y,Y));
                res_1 = _mm_mul_pd(m0.col2,_MM_SWIZZLE2(m1.col2,X,X));
                res_2 = _mm_mul_pd(col_34,_MM_SWIZZLE2(m1.col2,Y,Y));
                ret.col2 = _mm_add_pd(_mm_add_pd(res_0,res_1),res_2);
                res_0 = _mm_mul_pd(m0.col0,_MM_SWIZZLE2(m1.col3,X,X));
                res_1 = _mm_mul_pd(col_12,_MM_SWIZZLE2(m1.col3,Y,Y));
                res_2 = _mm_mul_pd(m0.col3,_MM_SWIZZLE2(m1.col4,X,X));
                ret.col3 = _mm_add_pd(_mm_add_pd(res_0,res_1),res_2);
                ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
                ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
                ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
            #else
                ret.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10 + m0.m02 * m1.m20;
                ret.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10 + m0.m12 * m1.m20;
                ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
                ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
                ret.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11 + m0.m12 * m1.m21;
                ret.m21 = m0.m20 * m1.m01 + m0.m21 * m1.m11 + m0.m22 * m1.m21;
                ret.m02 = m0.m00 * m1.m02 + m0.m01 * m1.m12 + m0.m02 * m1.m22;
                ret.m12 = m0.m10 * m1.m02 + m0.m11 * m1.m12 + m0.m12 * m1.m22;
                ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
            #endif
            return ret;
        }

        DMat4 &mult(DMat4 &ret,const DMat4 &m,const DVec3 &v) {
            ret.m00 = m.m00; ret.m01 = m.m01; ret.m02 = m.m02;
            ret.m10 = m.m10; ret.m11 = m.m11; ret.m12 = m.m12;
            ret.m20 = m.m20; ret.m21 = m.m21; ret.m22 = m.m22;
            ret.m03 = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03;
            ret.m13 = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13;
            ret.m23 = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23;
            return ret;
        }

        DMat4 &add(DMat4 &ret,const DMat4 &m0,const DMat4 &m1) {
            #ifdef USE_SSE2
                ret.col0 = _mm_add_pd(m0.col0,m1.col0); ret.col1 = _mm_add_pd(m0.col1,m1.col1);
                ret.col2 = _mm_add_pd(m0.col2,m1.col2); ret.col3 = _mm_add_pd(m0.col3,m1.col3);
                ret.col4 = _mm_add_pd(m0.col4,m1.col4); ret.col5 = _mm_add_pd(m0.col5,m1.col5);
            #else
                ret.m00 = m0.m00 + m1.m00; ret.m01 = m0.m01 + m1.m01; ret.m02 = m0.m02 + m1.m02; ret.m03 = m0.m03 + m1.m03;
                ret.m10 = m0.m10 + m1.m10; ret.m11 = m0.m11 + m1.m11; ret.m12 = m0.m12 + m1.m12; ret.m13 = m0.m13 + m1.m13;
                ret.m20 = m0.m20 + m1.m20; ret.m21 = m0.m21 + m1.m21; ret.m22 = m0.m22 + m1.m22; ret.m23 = m0.m23 + m1.m23;
            #endif
            return ret;
        }

        DMat4 &sub(DMat4 &ret,const DMat4 &m0,const DMat4 &m1) {
            #ifdef USE_SSE2
                ret.col0 = _mm_sub_pd(m0.col0,m1.col0); ret.col1 = _mm_sub_pd(m0.col1,m1.col1);
                ret.col2 = _mm_sub_pd(m0.col2,m1.col2); ret.col3 = _mm_sub_pd(m0.col3,m1.col3);
                ret.col4 = _mm_sub_pd(m0.col4,m1.col4); ret.col5 = _mm_sub_pd(m0.col5,m1.col5);
            #else
                ret.m00 = m0.m00 - m1.m00; ret.m01 = m0.m01 - m1.m01; ret.m02 = m0.m02 - m1.m02; ret.m03 = m0.m03 - m1.m03;
                ret.m10 = m0.m10 - m1.m10; ret.m11 = m0.m11 - m1.m11; ret.m12 = m0.m12 - m1.m12; ret.m13 = m0.m13 - m1.m13;
                ret.m20 = m0.m20 - m1.m20; ret.m21 = m0.m21 - m1.m21; ret.m22 = m0.m22 - m1.m22; ret.m23 = m0.m23 - m1.m23;
            #endif
            return ret;
        }

        DMat4 &orthonormalize(DMat4 &ret,const DMat4 &m) {
            DVec3 x = DVec3(m.m00,m.m10,m.m20);
            DVec3 y = DVec3(m.m01,m.m11,m.m21);
            DVec3 z = cross(x,y);
            cross(y,z,x);
            x.normalize();
            y.normalize();
            z.normalize();
            ret.m00 = x.x; ret.m01 = y.x; ret.m02 = z.x; ret.m03 = m.m03;
            ret.m10 = x.y; ret.m11 = y.y; ret.m12 = z.y; ret.m13 = m.m13;
            ret.m20 = x.z; ret.m21 = y.z; ret.m22 = z.z; ret.m23 = m.m23;
            return ret;
        }

        DMat4 &rotation(DMat4 &ret,const DMat4 &m) {
            ret.m00 = m.m00; ret.m01 = m.m01; ret.m02 = m.m02; ret.m03 = 0.0;
            ret.m10 = m.m10; ret.m11 = m.m11; ret.m12 = m.m12; ret.m13 = 0.0;
            ret.m20 = m.m20; ret.m21 = m.m21; ret.m22 = m.m22; ret.m23 = 0.0;
            return ret;
        }

        DMat4 &inverse(DMat4 &ret,const DMat4 &m) {
            f64 idet = Math::rcp(determinant(m));
            ret.m00 =  (m.m11 * m.m22 - m.m12 * m.m21) * idet;
            ret.m10 = -(m.m10 * m.m22 - m.m12 * m.m20) * idet;
            ret.m20 =  (m.m10 * m.m21 - m.m11 * m.m20) * idet;
            ret.m01 = -(m.m01 * m.m22 - m.m02 * m.m21) * idet;
            ret.m11 =  (m.m00 * m.m22 - m.m02 * m.m20) * idet;
            ret.m21 = -(m.m00 * m.m21 - m.m01 * m.m20) * idet;
            ret.m02 =  (m.m01 * m.m12 - m.m02 * m.m11) * idet;
            ret.m12 = -(m.m00 * m.m12 - m.m02 * m.m10) * idet;
            ret.m22 =  (m.m00 * m.m11 - m.m01 * m.m10) * idet;
            ret.m03 = -(ret.m00 * m.m03 + ret.m01 * m.m13 + ret.m02 * m.m23);
            ret.m13 = -(ret.m10 * m.m03 + ret.m11 * m.m13 + ret.m12 * m.m23);
            ret.m23 = -(ret.m20 * m.m03 + ret.m21 * m.m13 + ret.m22 * m.m23);
            return ret;
        }

        DMat4 &lerp(DMat4 &ret,const DMat4 &m0,const DMat4 &m1,f64 k) {
            DVec3 positions[3];
            SVQuat rotations[3];
            FVec3 scales[3];
            decomposeTransform(m0,positions[0],rotations[0],scales[0]);
            decomposeTransform(m1,positions[1],rotations[1],scales[1]);
            lerp(positions[2],positions[0],positions[1],k);
            slerp(rotations[2],rotations[0],rotations[1],(f32)k);
            lerp(scales[2],scales[0],scales[1],(f32)k);
            return composeTransform(ret,positions[2],rotations[2],scales[2]);
        }

        //
        DMat4 orthonormalize(const DMat4 &m) {
            DMat4 ret;
            return orthonormalize(ret,m);
        }

        DMat4 rotation(const DMat4 &m) {
            DMat4 ret;
            return rotation(ret,m);
        }

        DMat4 inverse(const DMat4 &m) {
            DMat4 ret;
            return inverse(ret,m);
        }

        DMat4 lerp(const DMat4 &m0,const DMat4 &m1,f64 k) {
            DMat4 ret;
            return lerp(ret,m0,m1,k);
        }

        //
        DMat4 translate(const DVec3 &v) {
            DMat4 ret;
            ret.setTranslate(v);
            return ret;
        }

        DMat4 translate(f64 x,f64 y,f64 z) {
            DMat4 ret;
            ret.setTranslate(DVec3(x,y,z));
            return ret;
        }

        //
        DMat4 reflect(const DVec4 &plane) {
            DMat4 ret;
            f64 x = plane.x;
            f64 y = plane.y;
            f64 z = plane.z;
            f64 x2 = x * 2.0;
            f64 y2 = y * 2.0;
            f64 z2 = z * 2.0;
            ret.m00 = 1.0 - x * x2; ret.m01 = -y * x2;      ret.m02 = -z * x2;      ret.m03 = -plane.w * x2;
            ret.m10 = -x * y2;      ret.m11 = 1.0 - y * y2; ret.m12 = -z * y2;      ret.m13 = -plane.w * y2;
            ret.m20 = -x * z2;      ret.m21 = -y * z2;      ret.m22 = 1.0 - z * z2; ret.m23 = -plane.w * z2;
            return ret;
        }

        DMat4 setTo(const DVec3 &position,const DVec3 &direction,const FVec3 &up) {
            DMat4 ret;
            FVec3 z = normalize(FVec3(position - direction));
            FVec3 x = normalize(cross(up,z));
            FVec3 y = normalize(cross(z,x));
            ret.m00 = x.x; ret.m01 = y.x; ret.m02 = z.x; ret.m03 = position.x;
            ret.m10 = x.y; ret.m11 = y.y; ret.m12 = z.y; ret.m13 = position.y;
            ret.m20 = x.z; ret.m21 = y.z; ret.m22 = z.z; ret.m23 = position.z;
            return ret;
        }

        DMat4 lookAt(const DVec3 &position,const DVec3 &direction,const FVec3 &up) {
            DMat4 ret,m0,m1;
            FVec3 z = normalize(FVec3(position - direction));
            FVec3 x = normalize(cross(up,z));
            FVec3 y = normalize(cross(z,x));
            m0.m00 = x.x; m0.m01 = x.y; m0.m02 = x.z; m0.m03 = 0.0;
            m0.m10 = y.x; m0.m11 = y.y; m0.m12 = y.z; m0.m13 = 0.0;
            m0.m20 = z.x; m0.m21 = z.y; m0.m22 = z.z; m0.m23 = 0.0;
            m1.setTranslate(-position);
            return mul(ret,m0,m1);
        }

        //
        void decomposeTransform(const DMat4 &m,DVec3 &position,SVQuat &rot,FVec3 &s) {
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

        DMat4 &composeTransform(DMat4 &ret,const DVec3 &position,const SVQuat &rot,const FVec3 &s) {
            FMat3 rotation,scale;
            scale.setDiagonal(s);
            ret.set(mul(rotation,rot.getMat3(),scale),position);
            return ret;
        }

    }//!namesapce util
    
}//!namespace sv
