#include "SVMat2.h"
#include "SVMat3.h"
#include "SVMat4d.h"
#include "SVMat4.h"
#include "SVQuat.h"

namespace sv {
    
    namespace util {

        /******************************************************************************\
        *
        * FMat2
        *
        \******************************************************************************/

        //
        static const f32 FMat2_identity_values[4] = {
            1.0f, 0.0f,
            0.0f, 1.0f,
        };

        //
        const FMat2 FMat2_zero(0.0f);
        const FMat2 FMat2_one(1.0f);
        const FMat2 FMat2_identity(FMat2_identity_values);

        //
        FMat2::FMat2(f32 v) {
            m00 = v; m01 = v;
            m10 = v; m11 = v;
        }

        FMat2::FMat2(const FMat2 &m) {
            m00 = m.m00; m01 = m.m01;
            m10 = m.m10; m11 = m.m11;
        }

        FMat2::FMat2(const FMat3 &m) {
            m00 = m.m00; m01 = m.m01;
            m10 = m.m10; m11 = m.m11;
        }

        FMat2::FMat2(const FMat4 &m) {
            m00 = m.m00; m01 = m.m01;
            m10 = m.m10; m11 = m.m11;
        }

        FMat2::FMat2(const DMat4 &m) {
            m00 = (f32)m.m00; m01 = (f32)m.m01;
            m10 = (f32)m.m10; m11 = (f32)m.m11;
        }

        FMat2::FMat2(const f32 *m) {
            m00 = m[0]; m01 = m[2];
            m10 = m[1]; m11 = m[3];
        }

        //
        FMat2 FMat2::operator-() const {
            FMat2 ret;
            ret.m00 = -m00; ret.m01 = -m01;
            ret.m10 = -m10; ret.m11 = -m11;
            return ret;
        }

        FMat2 &FMat2::operator*=(f32 v) {
            return mul(*this,*this,v);
        }

        FMat2 &FMat2::operator*=(const FMat2 &m) {
            return mul(*this,FMat2(*this),m);
        }

        FMat2 &FMat2::operator+=(const FMat2 &m) {
            return add(*this,*this,m);
        }

        FMat2 &FMat2::operator-=(const FMat2 &m) {
            return sub(*this,*this,m);
        }

        //
        void FMat2::setRow(s32 row,const FVec2 &v) {
            assert((u32)row < 2 && "FMat2::setRow(): bad row");
            mat[row + 0] = v.x;
            mat[row + 2] = v.y;
        }

        FVec2 FMat2::getRow(s32 row) const {
            assert((u32)row < 2 && "FMat2::getRow(): bad row");
            return FVec2(mat[row + 0],mat[row + 2]);
        }

        //
        void FMat2::setColumn(s32 column,const FVec2 &v) {
            assert((u32)column < 2 && "FMat2::setColumn(): bad column");
            column *= 2;
            mat[column + 0] = v.x;
            mat[column + 1] = v.y;
        }

        FVec2 FMat2::getColumn(s32 column) const {
            assert((u32)column < 2 && "FMat2::getColumn(): bad column");
            return FVec2(mat + column * 2);
        }

        //
        void FMat2::set(const FMat2 &m) {
            m00 = m.m00; m01 = m.m01;
            m10 = m.m10; m11 = m.m11;
        }

        void FMat2::set(const FMat3 &m) {
            m00 = m.m00; m01 = m.m01;
            m10 = m.m10; m11 = m.m11;
        }

        void FMat2::set(const FMat4 &m) {
            m00 = m.m00; m01 = m.m01;
            m10 = m.m10; m11 = m.m11;
        }

        void FMat2::set(const DMat4 &m) {
            m00 = (f32)m.m00; m01 = (f32)m.m01;
            m10 = (f32)m.m10; m11 = (f32)m.m11;
        }

        void FMat2::set(const f32 *m) {
            m00 = m[0]; m01 = m[2];
            m10 = m[1]; m11 = m[3];
        }

        void FMat2::get(f32 *m) const {
            m[0] = m00; m[2] = m01;
            m[1] = m10; m[3] = m11;
        }

        //
        void FMat2::setZero() {
            m00 = 0.0f; m01 = 0.0f;
            m10 = 0.0f; m11 = 0.0f;
        }

        void FMat2::setIdentity() {
            m00 = 1.0f; m01 = 0.0f;
            m10 = 0.0f; m11 = 1.0f;
        }

        void FMat2::setRotate(f32 angle) {
            f32 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            m00 = c; m01 = -s;
            m10 = s; m11 = c;
        }

        void FMat2::setScale(const FVec2 &v) {
            m00 = v.x;  m01 = 0.0f;
            m10 = 0.0f; m11 = v.y;
        }

        //
        s32 operator==(const FMat2 &m0,const FMat2 &m1) {
            return compare(m0,m1);
        }

        s32 operator!=(const FMat2 &m0,const FMat2 &m1) {
            return !compare(m0,m1);
        }

        FMat2 operator*(const FMat2 &m,f32 v) {
            FMat2 ret;
            return mul(ret,m,v);
        }

        FVec2 operator*(const FMat2 &m,const FVec2 &v) {
            FVec2 ret;
            return mul(ret,m,v);
        }

        FVec2 operator*(const FVec2 &v,const FMat2 &m) {
            FVec2 ret;
            return mul(ret,v,m);
        }

        DVec2 operator*(const FMat2 &m,const DVec2 &v) {
            DVec2 ret;
            return mul(ret,m,v);
        }

        DVec2 operator*(const DVec2 &v,const FMat2 &m) {
            DVec2 ret;
            return mul(ret,v,m);
        }

        FMat2 operator*(const FMat2 &m0,const FMat2 &m1) {
            FMat2 ret;
            return mul(ret,m0,m1);
        }

        FMat2 operator+(const FMat2 &m0,const FMat2 &m1) {
            FMat2 ret;
            return add(ret,m0,m1);
        }

        FMat2 operator-(const FMat2 &m0,const FMat2 &m1) {
            FMat2 ret;
            return sub(ret,m0,m1);
        }

        //
        s32 compare(const FMat2 &m0,const FMat2 &m1) {
            return (compare(m0.m00,m1.m00) && compare(m0.m10,m1.m10)) &&
                compare(m0.m01,m1.m01) && compare(m0.m11,m1.m11);
        }

        s32 compare(const FMat2 &m0,const FMat2 &m1,f32 epsilon) {
            return (compare(m0.m00,m1.m00,epsilon) && compare(m0.m10,m1.m10,epsilon)) &&
                compare(m0.m01,m1.m01,epsilon) && compare(m0.m11,m1.m11,epsilon);
        }

        f32 trace(const FMat2 &m) {
            return m.m00 + m.m11;
        }

        f32 determinant(const FMat2 &m) {
            return m.m00 * m.m11 - m.m10 * m.m01;
        }

        FMat2 &mul(FMat2 &ret,const FMat2 &m,f32 v) {
            ret.m00 = m.m00 * v; ret.m01 = m.m01 * v;
            ret.m10 = m.m10 * v; ret.m11 = m.m11 * v;
            return ret;
        }

        FVec2 &mul(FVec2 &ret,const FMat2 &m,const FVec2 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y;
            ret.y = m.m10 * v.x + m.m11 * v.y;
            return ret;
        }

        FVec2 &mul(FVec2 &ret,const FVec2 &v,const FMat2 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y;
            ret.y = m.m01 * v.x + m.m11 * v.y;
            return ret;
        }

        DVec2 &mul(DVec2 &ret,const FMat2 &m,const DVec2 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y;
            ret.y = m.m10 * v.x + m.m11 * v.y;
            return ret;
        }

        DVec2 &mul(DVec2 &ret,const DVec2 &v,const FMat2 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y;
            ret.y = m.m01 * v.x + m.m11 * v.y;
            return ret;
        }

        FMat2 &mul(FMat2 &ret,const FMat2 &m0,const FMat2 &m1) {
            ret.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10;
            ret.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10;
            ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11;
            ret.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11;
            return ret;
        }

        FMat2 &add(FMat2 &ret,const FMat2 &m0,const FMat2 &m1) {
            ret.m00 = m0.m00 + m1.m00; ret.m01 = m0.m01 + m1.m01;
            ret.m10 = m0.m10 + m1.m10; ret.m11 = m0.m11 + m1.m11;
            return ret;
        }

        FMat2 &sub(FMat2 &ret,const FMat2 &m0,const FMat2 &m1) {
            ret.m00 = m0.m00 - m1.m00; ret.m01 = m0.m01 - m1.m01;
            ret.m10 = m0.m10 - m1.m10; ret.m11 = m0.m11 - m1.m11;
            return ret;
        }

        FMat2 &transpose(FMat2 &ret,const FMat2 &m) {
            ret.m00 = m.m00; ret.m01 = m.m10;
            ret.m10 = m.m01; ret.m11 = m.m11;
            return ret;
        }

        FMat2 &inverse(FMat2 &ret,const FMat2 &m) {
            f32 idet = Math::rcp(determinant(m));
            ret.m00 =  m.m11 * idet;
            ret.m10 = -m.m10 * idet;
            ret.m01 = -m.m01 * idet;
            ret.m11 =  m.m00 * idet;
            return ret;
        }

        FMat2 &inverse(FMat2 &ret,const FMat2 &m,f32 det) {
            f32 idet = Math::rcp(det);
            ret.m00 =  m.m11 * idet;
            ret.m10 = -m.m10 * idet;
            ret.m01 = -m.m01 * idet;
            ret.m11 =  m.m00 * idet;
            return ret;
        }

        //
        FMat2 transpose(const FMat2 &m) {
            FMat2 ret;
            return transpose(ret,m);
        }

        FMat2 inverse(const FMat2 &m) {
            FMat2 ret;
            return inverse(ret,m);
        }

        FMat2 inverse(const FMat2 &m,f32 det) {
            FMat2 ret;
            return inverse(ret,m,det);
        }

    }//!namesapce util
    
}//!namespace sv
