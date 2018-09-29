#include "SVMat3.h"
#include "SVMat4d.h"
#include "SVMat2.h"
#include "SVMat4.h"
#include "SVQuat.h"

namespace sv {
    
    namespace util {

        /******************************************************************************\
        *
        * FMat3
        *
        \******************************************************************************/

        //
        static const f32 FMat3_identity_values[12] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
        };

        //
        const FMat3 FMat3_zero(0.0f);
        const FMat3 FMat3_one(1.0f);
        const FMat3 FMat3_identity(FMat3_identity_values);

        //
        FMat3::FMat3(f32 v) {
            m00 = v;    m01 = v;    m02 = v;
            m10 = v;    m11 = v;    m12 = v;
            m20 = v;    m21 = v;    m22 = v;
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f;
        }

        FMat3::FMat3(const FMat2 &m) {
            m00 = m.m00; m01 = m.m01; m02 = 0.0f;
            m10 = m.m10; m11 = m.m11; m12 = 0.0f;
            m20 = 0.0f;  m21 = 0.0f;  m22 = 1.0f;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;
        }

        FMat3::FMat3(const FMat3 &m) {
            #if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                col0 = m.col0;
                col1 = m.col1;
                col2 = m.col2;
            #else
                m00 = m.m00; m01 = m.m01; m02 = m.m02;
                m10 = m.m10; m11 = m.m11; m12 = m.m12;
                m20 = m.m20; m21 = m.m21; m22 = m.m22;
                m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;
            #endif
        }

        FMat3::FMat3(const FMat4 &m) {
            #if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                col0 = m.col0;
                col1 = m.col1;
                col2 = m.col2;
            #else
                m00 = m.m00; m01 = m.m01; m02 = m.m02;
                m10 = m.m10; m11 = m.m11; m12 = m.m12;
                m20 = m.m20; m21 = m.m21; m22 = m.m22;
                m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;
            #endif
        }

        FMat3::FMat3(const DMat4 &m) {
            m00 = (f32)m.m00; m01 = (f32)m.m01; m02 = (f32)m.m02;
            m10 = (f32)m.m10; m11 = (f32)m.m11; m12 = (f32)m.m12;
            m20 = (f32)m.m20; m21 = (f32)m.m21; m22 = (f32)m.m22;
            m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;
        }

        FMat3::FMat3(const SVQuat &q) {
            FMat3 m = q.getMat3();
            #if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                col0 = m.col0;
                col1 = m.col1;
                col2 = m.col2;
            #else
                m00 = m.m00; m01 = m.m01; m02 = m.m02;
                m10 = m.m10; m11 = m.m11; m12 = m.m12;
                m20 = m.m20; m21 = m.m21; m22 = m.m22;
                m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;
            #endif
        }

        FMat3::FMat3(const f32 *m) {
            m00 = m[0]; m01 = m[4]; m02 = m[8];
            m10 = m[1]; m11 = m[5]; m12 = m[9];
            m20 = m[2]; m21 = m[6]; m22 = m[10];
            m30 = 0.0f; m31 = 0.0f; m32 = 0.0f;
        }

        //
        FMat3 FMat3::operator-() const {
            FMat3 ret;
            ret.m00 = -m00; ret.m01 = -m01; ret.m02 = -m02;
            ret.m10 = -m10; ret.m11 = -m11; ret.m12 = -m12;
            ret.m20 = -m20; ret.m21 = -m21; ret.m22 = -m22;
            return ret;
        }

        FMat3 &FMat3::operator*=(f32 v) {
            return mul(*this,*this,v);
        }

        FMat3 &FMat3::operator*=(const FMat3 &m) {
            return mul(*this,FMat3(*this),m);
        }

        FMat3 &FMat3::operator+=(const FMat3 &m) {
            return add(*this,*this,m);
        }

        FMat3 &FMat3::operator-=(const FMat3 &m) {
            return sub(*this,*this,m);
        }

        //
        void FMat3::setRow(s32 row,const FVec3 &v) {
            assert((u32)row < 3 && "FMat3::setRow(): bad row");
            mat[row + 0] = v.x;
            mat[row + 4] = v.y;
            mat[row + 8] = v.z;
        }

        FVec3 FMat3::getRow(s32 row) const {
            assert((u32)row < 3 && "FMat3::getRow(): bad row");
            return FVec3(mat[row + 0],mat[row + 4],mat[row + 8]);
        }

        //
        void FMat3::setColumn(s32 column,const FVec3 &v) {
            assert((u32)column < 3 && "FMat3::setColumn(): bad column");
            column *= 4;
            mat[column + 0] = v.x;
            mat[column + 1] = v.y;
            mat[column + 2] = v.z;
        }

        FVec3 FMat3::getColumn(s32 column) const {
            assert((u32)column < 3 && "FMat3::getColumn(): bad column");
            return FVec3(mat + column * 4);
        }

        //
        void FMat3::setDiagonal(const FVec3 &v) {
            m00 = v.x;  m01 = 0.0f; m02 = 0.0f;
            m10 = 0.0f; m11 = v.y;  m12 = 0.0f;
            m20 = 0.0f; m21 = 0.0f; m22 = v.z;
        }

        FVec3 FMat3::getDiagonal() const {
            return FVec3(m00,m11,m22);
        }

        //
        void FMat3::set(const FMat2 &m) {
            m00 = m.m00; m01 = m.m01; m02 = 0.0f;
            m10 = m.m10; m11 = m.m11; m12 = 0.0f;
            m20 = 0.0f;  m21 = 0.0f;  m22 = 1.0f;
        }

        void FMat3::set(const FMat3 &m) {
            #if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                col0 = m.col0;
                col1 = m.col1;
                col2 = m.col2;
            #else
                m00 = m.m00; m01 = m.m01; m02 = m.m02;
                m10 = m.m10; m11 = m.m11; m12 = m.m12;
                m20 = m.m20; m21 = m.m21; m22 = m.m22;
            #endif
        }

        void FMat3::set(const FMat4 &m) {
            #if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                col0 = m.col0;
                col1 = m.col1;
                col2 = m.col2;
            #else
                m00 = m.m00; m01 = m.m01; m02 = m.m02;
                m10 = m.m10; m11 = m.m11; m12 = m.m12;
                m20 = m.m20; m21 = m.m21; m22 = m.m22;
            #endif
        }

        void FMat3::set(const DMat4 &m) {
            m00 = (f32)m.m00; m01 = (f32)m.m01; m02 = (f32)m.m02;
            m10 = (f32)m.m10; m11 = (f32)m.m11; m12 = (f32)m.m12;
            m20 = (f32)m.m20; m21 = (f32)m.m21; m22 = (f32)m.m22;
        }

        void FMat3::set(const SVQuat &q) {
            FMat3 m = q.getMat3();
            #if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
                col0 = m.col0;
                col1 = m.col1;
                col2 = m.col2;
            #else
                m00 = m.m00; m01 = m.m01; m02 = m.m02;
                m10 = m.m10; m11 = m.m11; m12 = m.m12;
                m20 = m.m20; m21 = m.m21; m22 = m.m22;
            #endif
        }

        void FMat3::set(const f32 *m) {
            m00 = m[0]; m01 = m[4]; m02 = m[8];
            m10 = m[1]; m11 = m[5]; m12 = m[9];
            m20 = m[2]; m21 = m[6]; m22 = m[10];
        }

        void FMat3::get(f32 *m) const {
            m[0] = m00; m[4] = m01; m[8] = m02;
            m[1] = m10; m[5] = m11; m[9] = m12;
            m[2] = m20; m[6] = m21; m[10] = m22;
        }

        //
        void FMat3::setZero() {
            m00 = 0.0f; m01 = 0.0f; m02 = 0.0f;
            m10 = 0.0f; m11 = 0.0f; m12 = 0.0f;
            m20 = 0.0f; m21 = 0.0f; m22 = 0.0f;
        }

        void FMat3::setIdentity() {
            m00 = 1.0f; m01 = 0.0f; m02 = 0.0f;
            m10 = 0.0f; m11 = 1.0f; m12 = 0.0f;
            m20 = 0.0f; m21 = 0.0f; m22 = 1.0f;
        }

        void FMat3::setSkewSymmetric(const FVec3 &v) {
            m00 = 0.0f; m01 = -v.z; m02 =  v.y;
            m10 =  v.z; m11 =  0.0; m12 = -v.x;
            m20 = -v.y; m21 =  v.x; m22 =  0.0;
        }

        void FMat3::setRotate(const FVec3 &axis,f32 angle) {
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
            m00 = (1.0f - c) * xx + c;  m01 = (1.0f - c) * xy - zs; m02 = (1.0f - c) * zx + ys;
            m10 = (1.0f - c) * xy + zs; m11 = (1.0f - c) * yy + c;  m12 = (1.0f - c) * yz - xs;
            m20 = (1.0f - c) * zx - ys; m21 = (1.0f - c) * yz + xs; m22 = (1.0f - c) * zz + c;
        }

        void FMat3::setRotateX(f32 angle) {
            f32 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            m00 = 1.0f; m01 = 0.0f; m02 = 0.0f;
            m10 = 0.0f; m11 = c;    m12 = -s;
            m20 = 0.0f; m21 = s;    m22 = c;
        }

        void FMat3::setRotateY(f32 angle) {
            f32 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            m00 = c;    m01 = 0.0f; m02 = s;
            m10 = 0.0f; m11 = 1.0f; m12 = 0.0f;
            m20 = -s;   m21 = 0.0f; m22 = c;
        }

        void FMat3::setRotateZ(f32 angle) {
            f32 s,c;
            Math::sincos(angle * DEG2RAD,s,c);
            m00 = c;    m01 = -s;   m02 = 0.0f;
            m10 = s;    m11 = c;    m12 = 0.0f;
            m20 = 0.0f; m21 = 0.0f; m22 = 1.0f;
        }

        void FMat3::setScale(const FVec3 &v) {
            m00 = v.x;  m01 = 0.0f; m02 = 0.0f;
            m10 = 0.0f; m11 = v.y;  m12 = 0.0f;
            m20 = 0.0f; m21 = 0.0f; m22 = v.z;
        }

        //
        SVQuat FMat3::getQuat() const {
            SVQuat ret;
            f32 s,trace = m00 + m11 + m22;
            if(trace > 0.0f) {
                s = Math::sqrt(trace + 1.0f);
                ret.w = s * 0.5f;
                s = Math::rcp(s) * 0.5f;
                ret.x = (m21 - m12) * s;
                ret.y = (m02 - m20) * s;
                ret.z = (m10 - m01) * s;
            } else {
                s32 i = 0;
                if(m11 > m00) i = 1;
                if(m22 > mat[4 * i + i]) i = 2;
                switch(i) {
                    case 0:
                        s = Math::sqrt(m00 - m11 - m22 + 1.0f);
                        ret.x = s * 0.5f;
                        s = Math::rcp(s) * 0.5f;
                        ret.y = (m10 + m01) * s;
                        ret.z = (m02 + m20) * s;
                        ret.w = (m21 - m12) * s;
                        break;
                    case 1:
                        s = Math::sqrt(m11 - m00 - m22 + 1.0f);
                        ret.y = s * 0.5f;
                        s = Math::rcp(s) * 0.5f;
                        ret.z = (m21 + m12) * s;
                        ret.x = (m10 + m01) * s;
                        ret.w = (m02 - m20) * s;
                        break;
                    case 2:
                        s = Math::sqrt(m22 - m00 - m11 + 1.0f);
                        ret.z = s * 0.5f;
                        s = Math::rcp(s) * 0.5f;
                        ret.x = (m02 + m20) * s;
                        ret.y = (m21 + m12) * s;
                        ret.w = (m10 - m01) * s;
                        break;
                }
            }
            return ret;
        }

        //
        s32 operator==(const FMat3 &m0,const FMat3 &m1) {
            return compare(m0,m1);
        }

        s32 operator!=(const FMat3 &m0,const FMat3 &m1) {
            return !compare(m0,m1);
        }

        FMat3 operator*(const FMat3 &m,f32 v) {
            FMat3 ret;
            return mul(ret,m,v);
        }

        FVec2 operator*(const FMat3 &m,const FVec2 &v) {
            FVec2 ret;
            return mul(ret,m,v);
        }

        FVec2 operator*(const FVec2 &v,const FMat3 &m) {
            FVec2 ret;
            return mul(ret,v,m);
        }

        FVec3 operator*(const FMat3 &m,const FVec3 &v) {
            FVec3 ret;
            return mul(ret,m,v);
        }

        FVec3 operator*(const FVec3 &v,const FMat3 &m) {
            FVec3 ret;
            return mul(ret,v,m);
        }

        DVec2 operator*(const FMat3 &m,const DVec2 &v) {
            DVec2 ret;
            return mul(ret,m,v);
        }

        DVec2 operator*(const DVec2 &v,const FMat3 &m) {
            DVec2 ret;
            return mul(ret,v,m);
        }

        DVec3 operator*(const FMat3 &m,const DVec3 &v) {
            DVec3 ret;
            return mul(ret,m,v);
        }

        DVec3 operator*(const DVec3 &v,const FMat3 &m) {
            DVec3 ret;
            return mul(ret,v,m);
        }

        FMat3 operator*(const FMat3 &m0,const FMat3 &m1) {
            FMat3 ret;
            return mul(ret,m0,m1);
        }

        FMat3 operator+(const FMat3 &m0,const FMat3 &m1) {
            FMat3 ret;
            return add(ret,m0,m1);
        }

        FMat3 operator-(const FMat3 &m0,const FMat3 &m1) {
            FMat3 ret;
            return sub(ret,m0,m1);
        }

        //
        s32 compare(const FMat3 &m0,const FMat3 &m1) {
            return (compare(m0.m00,m1.m00) && compare(m0.m10,m1.m10) && compare(m0.m20,m1.m20) &&
                compare(m0.m01,m1.m01) && compare(m0.m11,m1.m11) && compare(m0.m21,m1.m21) &&
                compare(m0.m02,m1.m02) && compare(m0.m12,m1.m12) && compare(m0.m22,m1.m22));
        }

        s32 compare(const FMat3 &m0,const FMat3 &m1,f32 epsilon) {
            return (compare(m0.m00,m1.m00,epsilon) && compare(m0.m10,m1.m10,epsilon) && compare(m0.m20,m1.m20,epsilon) &&
                compare(m0.m01,m1.m01,epsilon) && compare(m0.m11,m1.m11,epsilon) && compare(m0.m21,m1.m21,epsilon) &&
                compare(m0.m02,m1.m02,epsilon) && compare(m0.m12,m1.m12,epsilon) && compare(m0.m22,m1.m22,epsilon));
        }

        f32 trace(const FMat3 &m) {
            return m.m00 + m.m11 + m.m22;
        }

        f32 determinant(const FMat3 &m) {
            f32 det = 0.0f;
            det =  m.m00 * (m.m11 * m.m22 - m.m12 * m.m21);
            det -= m.m01 * (m.m10 * m.m22 - m.m12 * m.m20);
            det += m.m02 * (m.m10 * m.m21 - m.m11 * m.m20);
            return det;
        }

        FMat3 &mul(FMat3 &ret,const FMat3 &m,f32 v) {
            #ifdef USE_SSE
                __m128 temp = _mm_set1_ps(v);
                ret.col0 = _mm_mul_ps(m.col0,temp);
                ret.col1 = _mm_mul_ps(m.col1,temp);
                ret.col2 = _mm_mul_ps(m.col2,temp);
            #elif USE_ALTIVEC
                vec_float4 temp = vec_splats(v);
                vec_float4 zero = vec_splats(0.0f);
                ret.col0 = vec_madd(m.col0,temp,zero);
                ret.col1 = vec_madd(m.col1,temp,zero);
                ret.col2 = vec_madd(m.col2,temp,zero);
            #elif USE_NEON
                ret.col0 = vmulq_n_f32(m.col0,v);
                ret.col1 = vmulq_n_f32(m.col1,v);
                ret.col2 = vmulq_n_f32(m.col2,v);
            #else
                ret.m00 = m.m00 * v; ret.m01 = m.m01 * v; ret.m02 = m.m02 * v;
                ret.m10 = m.m10 * v; ret.m11 = m.m11 * v; ret.m12 = m.m12 * v;
                ret.m20 = m.m20 * v; ret.m21 = m.m21 * v; ret.m22 = m.m22 * v;
            #endif
            return ret;
        }

        FVec2 &mul(FVec2 &ret,const FMat3 &m,const FVec2 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m02;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m12;
            return ret;
        }

        FVec2 &mul(FVec2 &ret,const FVec2 &v,const FMat3 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21;
            return ret;
        }

        FVec3 &mul(FVec3 &ret,const FMat3 &m,const FVec3 &v) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(m.col0,_MM_SWIZZLE(v.vec,X,X,X,W));
                __m128 res_1 = _mm_mul_ps(m.col1,_MM_SWIZZLE(v.vec,Y,Y,Y,W));
                __m128 res_2 = _mm_mul_ps(m.col2,_MM_SWIZZLE(v.vec,Z,Z,Z,W));
                ret.vec = _mm_add_ps(_mm_add_ps(res_0,res_1),res_2);
            #elif USE_ALTIVEC
                vec_float4 zero = vec_splats(0.0f);
                vec_float4 res_0 = vec_madd(m.col0,VEC_SWIZZLE(v.vec,X,X,X,W),zero);
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

        FVec3 &mul(FVec3 &ret,const FVec3 &v,const FMat3 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
            return ret;
        }

        DVec2 &mul(DVec2 &ret,const FMat3 &m,const DVec2 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m02;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m12;
            return ret;
        }

        DVec2 &mul(DVec2 &ret,const DVec2 &v,const FMat3 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21;
            return ret;
        }

        DVec3 &mul(DVec3 &ret,const FMat3 &m,const DVec3 &v) {
            ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z;
            ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
            ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;
            return ret;
        }

        DVec3 &mul(DVec3 &ret,const DVec3 &v,const FMat3 &m) {
            ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
            ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
            ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
            return ret;
        }

        FMat3 &mul(FMat3 &ret,const FMat3 &m,const FVec3 &v) {
            ret.m00 =  m.m01 * v.z - m.m02 * v.y;
            ret.m10 =  m.m11 * v.z - m.m12 * v.y;
            ret.m20 =  m.m21 * v.z - m.m22 * v.y;
            ret.m01 = -m.m00 * v.z + m.m02 * v.x;
            ret.m11 = -m.m10 * v.z + m.m12 * v.x;
            ret.m21 = -m.m20 * v.z + m.m22 * v.x;
            ret.m02 =  m.m00 * v.y - m.m01 * v.x;
            ret.m12 =  m.m10 * v.y - m.m11 * v.x;
            ret.m22 =  m.m20 * v.y - m.m21 * v.x;
            return ret;
        }

        FMat3 &mul(FMat3 &ret,const FVec3 &v,const FMat3 &m) {
            ret.m00 = -v.z * m.m10 + v.y * m.m20;
            ret.m10 =  v.z * m.m00 - v.x * m.m20;
            ret.m20 = -v.y * m.m00 + v.x * m.m10;
            ret.m01 = -v.z * m.m11 + v.y * m.m21;
            ret.m11 =  v.z * m.m01 - v.x * m.m21;
            ret.m21 = -v.y * m.m01 + v.x * m.m11;
            ret.m02 = -v.z * m.m12 + v.y * m.m22;
            ret.m12 =  v.z * m.m02 - v.x * m.m22;
            ret.m22 = -v.y * m.m02 + v.x * m.m12;
            return ret;
        }

        FMat3 &mul(FMat3 &ret,const FMat3 &m0,const FMat3 &m1) {
            #ifdef USE_SSE
                __m128 res_0 = _mm_mul_ps(m0.col0,_MM_SWIZZLE(m1.col0,X,X,X,W));
                __m128 res_1 = _mm_mul_ps(m0.col1,_MM_SWIZZLE(m1.col0,Y,Y,Y,W));
                __m128 res_2 = _mm_mul_ps(m0.col2,_MM_SWIZZLE(m1.col0,Z,Z,Z,W));
                ret.col0 = _mm_add_ps(_mm_add_ps(res_0,res_1),res_2);
                res_0 = _mm_mul_ps(m0.col0,_MM_SWIZZLE(m1.col1,X,X,X,W));
                res_1 = _mm_mul_ps(m0.col1,_MM_SWIZZLE(m1.col1,Y,Y,Y,W));
                res_2 = _mm_mul_ps(m0.col2,_MM_SWIZZLE(m1.col1,Z,Z,Z,W));
                ret.col1 = _mm_add_ps(_mm_add_ps(res_0,res_1),res_2);
                res_0 = _mm_mul_ps(m0.col0,_MM_SWIZZLE(m1.col2,X,X,X,W));
                res_1 = _mm_mul_ps(m0.col1,_MM_SWIZZLE(m1.col2,Y,Y,Y,W));
                res_2 = _mm_mul_ps(m0.col2,_MM_SWIZZLE(m1.col2,Z,Z,Z,W));
                ret.col2 = _mm_add_ps(_mm_add_ps(res_0,res_1),res_2);
            #elif USE_ALTIVEC
                vec_float4 zero = vec_splats(0.0f);
                vec_uchar16 xxxw = VEC_PERM2(X,X,X,W);
                vec_uchar16 yyyw = VEC_PERM2(Y,Y,Y,W);
                vec_uchar16 zzzw = VEC_PERM2(Z,Z,Z,W);
                ret.col0 = vec_madd(m0.col0,vec_perm(m1.col0,m1.col0,xxxw),zero);
                ret.col1 = vec_madd(m0.col0,vec_perm(m1.col1,m1.col1,xxxw),zero);
                ret.col2 = vec_madd(m0.col0,vec_perm(m1.col2,m1.col2,xxxw),zero);
                ret.col0 = vec_madd(m0.col1,vec_perm(m1.col0,m1.col0,yyyw),ret.col0);
                ret.col1 = vec_madd(m0.col1,vec_perm(m1.col1,m1.col1,yyyw),ret.col1);
                ret.col2 = vec_madd(m0.col1,vec_perm(m1.col2,m1.col2,yyyw),ret.col2);
                ret.col0 = vec_madd(m0.col2,vec_perm(m1.col0,m1.col0,zzzw),ret.col0);
                ret.col1 = vec_madd(m0.col2,vec_perm(m1.col1,m1.col1,zzzw),ret.col1);
                ret.col2 = vec_madd(m0.col2,vec_perm(m1.col2,m1.col2,zzzw),ret.col2);
            #elif USE_NEON
                float32x2_t low_0 = vget_low_f32(m1.col0);
                float32x2_t low_1 = vget_low_f32(m1.col1);
                float32x2_t low_2 = vget_low_f32(m1.col2);
                ret.col0 = vmulq_lane_f32(m0.col0,low_0,0);
                ret.col1 = vmulq_lane_f32(m0.col0,low_1,0);
                ret.col2 = vmulq_lane_f32(m0.col0,low_2,0);
                ret.col0 = vmlaq_lane_f32(ret.col0,m0.col1,low_0,1);
                ret.col1 = vmlaq_lane_f32(ret.col1,m0.col1,low_1,1);
                ret.col2 = vmlaq_lane_f32(ret.col2,m0.col1,low_2,1);
                float32x2_t high_0 = vget_high_f32(m1.col0);
                float32x2_t high_1 = vget_high_f32(m1.col1);
                float32x2_t high_2 = vget_high_f32(m1.col2);
                ret.col0 = vmlaq_lane_f32(ret.col0,m0.col2,high_0,0);
                ret.col1 = vmlaq_lane_f32(ret.col1,m0.col2,high_1,0);
                ret.col2 = vmlaq_lane_f32(ret.col2,m0.col2,high_2,0);
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

        FMat3 &add(FMat3 &ret,const FMat3 &m0,const FMat3 &m1) {
            #ifdef USE_SSE
                ret.col0 = _mm_add_ps(m0.col0,m1.col0);
                ret.col1 = _mm_add_ps(m0.col1,m1.col1);
                ret.col2 = _mm_add_ps(m0.col2,m1.col2);
            #elif USE_ALTIVEC
                ret.col0 = vec_add(m0.col0,m1.col0);
                ret.col1 = vec_add(m0.col1,m1.col1);
                ret.col2 = vec_add(m0.col2,m1.col2);
            #elif USE_NEON
                ret.col0 = vaddq_f32(m0.col0,m1.col0);
                ret.col1 = vaddq_f32(m0.col1,m1.col1);
                ret.col2 = vaddq_f32(m0.col2,m1.col2);
            #else
                ret.m00 = m0.m00 + m1.m00; ret.m01 = m0.m01 + m1.m01; ret.m02 = m0.m02 + m1.m02;
                ret.m10 = m0.m10 + m1.m10; ret.m11 = m0.m11 + m1.m11; ret.m12 = m0.m12 + m1.m12;
                ret.m20 = m0.m20 + m1.m20; ret.m21 = m0.m21 + m1.m21; ret.m22 = m0.m22 + m1.m22;
            #endif
            return ret;
        }

        FMat3 &sub(FMat3 &ret,const FMat3 &m0,const FMat3 &m1) {
            #ifdef USE_SSE
                ret.col0 = _mm_sub_ps(m0.col0,m1.col0);
                ret.col1 = _mm_sub_ps(m0.col1,m1.col1);
                ret.col2 = _mm_sub_ps(m0.col2,m1.col2);
            #elif USE_ALTIVEC
                ret.col0 = vec_sub(m0.col0,m1.col0);
                ret.col1 = vec_sub(m0.col1,m1.col1);
                ret.col2 = vec_sub(m0.col2,m1.col2);
            #elif USE_NEON
                ret.col0 = vsubq_f32(m0.col0,m1.col0);
                ret.col1 = vsubq_f32(m0.col1,m1.col1);
                ret.col2 = vsubq_f32(m0.col2,m1.col2);
            #else
                ret.m00 = m0.m00 - m1.m00; ret.m01 = m0.m01 - m1.m01; ret.m02 = m0.m02 - m1.m02;
                ret.m10 = m0.m10 - m1.m10; ret.m11 = m0.m11 - m1.m11; ret.m12 = m0.m12 - m1.m12;
                ret.m20 = m0.m20 - m1.m20; ret.m21 = m0.m21 - m1.m21; ret.m22 = m0.m22 - m1.m22;
            #endif
            return ret;
        }

        FMat3 &orthonormalize(FMat3 &ret,const FMat3 &m) {
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
            #elif USE_NEON
                float32x4_t z = vcrossq_f32(m.col0,m.col1);
                float32x4_t y = vcrossq_f32(z,m.col0);
                ret.col0 = vnormalize3q_f32(m.col0);
                ret.col1 = vnormalize3q_f32(y);
                ret.col2 = vnormalize3q_f32(z);
            #else
                FVec3 x = FVec3(m.m00,m.m10,m.m20);
                FVec3 y = FVec3(m.m01,m.m11,m.m21);
                FVec3 z = cross(x,y);
                cross(y,z,x);
                x.normalize();
                y.normalize();
                z.normalize();
                ret.m00 = x.x; ret.m01 = y.x; ret.m02 = z.x;
                ret.m10 = x.y; ret.m11 = y.y; ret.m12 = z.y;
                ret.m20 = x.z; ret.m21 = y.z; ret.m22 = z.z;
            #endif
            return ret;
        }

        FMat3 &transpose(FMat3 &ret,const FMat3 &m) {
            ret.m00 = m.m00; ret.m01 = m.m10; ret.m02 = m.m20;
            ret.m10 = m.m01; ret.m11 = m.m11; ret.m12 = m.m21;
            ret.m20 = m.m02; ret.m21 = m.m12; ret.m22 = m.m22;
            return ret;
        }

        FMat3 &inverse(FMat3 &ret,const FMat3 &m) {
            f32 idet = Math::rcp(determinant(m));
            ret.m00 =  (m.m11 * m.m22 - m.m12 * m.m21) * idet;
            ret.m10 = -(m.m10 * m.m22 - m.m12 * m.m20) * idet;
            ret.m20 =  (m.m10 * m.m21 - m.m11 * m.m20) * idet;
            ret.m01 = -(m.m01 * m.m22 - m.m02 * m.m21) * idet;
            ret.m11 =  (m.m00 * m.m22 - m.m02 * m.m20) * idet;
            ret.m21 = -(m.m00 * m.m21 - m.m01 * m.m20) * idet;
            ret.m02 =  (m.m01 * m.m12 - m.m02 * m.m11) * idet;
            ret.m12 = -(m.m00 * m.m12 - m.m02 * m.m10) * idet;
            ret.m22 =  (m.m00 * m.m11 - m.m01 * m.m10) * idet;
            return ret;
        }

        FMat3 &inverse(FMat3 &ret,const FMat3 &m,f32 det) {
            f32 idet = Math::rcp(det);
            ret.m00 =  (m.m11 * m.m22 - m.m12 * m.m21) * idet;
            ret.m10 = -(m.m10 * m.m22 - m.m12 * m.m20) * idet;
            ret.m20 =  (m.m10 * m.m21 - m.m11 * m.m20) * idet;
            ret.m01 = -(m.m01 * m.m22 - m.m02 * m.m21) * idet;
            ret.m11 =  (m.m00 * m.m22 - m.m02 * m.m20) * idet;
            ret.m21 = -(m.m00 * m.m21 - m.m01 * m.m20) * idet;
            ret.m02 =  (m.m01 * m.m12 - m.m02 * m.m11) * idet;
            ret.m12 = -(m.m00 * m.m12 - m.m02 * m.m10) * idet;
            ret.m22 =  (m.m00 * m.m11 - m.m01 * m.m10) * idet;
            return ret;
        }

        //
        FMat3 orthonormalize(const FMat3 &m) {
            FMat3 ret;
            return orthonormalize(ret,m);
        }

        FMat3 transpose(const FMat3 &m) {
            FMat3 ret;
            return transpose(ret,m);
        }

        FMat3 inverse(const FMat3 &m) {
            FMat3 ret;
            return inverse(ret,m);
        }

        FMat3 inverse(const FMat3 &m,f32 det) {
            FMat3 ret;
            return inverse(ret,m,det);
        }

        FMat3 rotate3(const FVec3 &axis,f32 angle) {
            FMat3 ret;
            ret.setRotate(axis,angle);
            return ret;
        }

        FMat3 rotate3(f32 x,f32 y,f32 z,f32 angle) {
            return rotate3(FVec3(x,y,z),angle);
        }

        FMat3 rotate3(const SVQuat &q) {
            return q.getMat3();
        }

        FMat3 rotateX3(f32 angle) {
            FMat3 ret;
            ret.setRotateX(angle);
            return ret;
        }

        FMat3 rotateY3(f32 angle) {
            FMat3 ret;
            ret.setRotateY(angle);
            return ret;
        }

        FMat3 rotateZ3(f32 angle) {
            FMat3 ret;
            ret.setRotateZ(angle);
            return ret;
        }

        FMat3 scale3(const FVec3 &v) {
            FMat3 ret;
            ret.setScale(v);
            return ret;
        }

        FMat3 scale3(f32 x,f32 y,f32 z) {
            return scale3(FVec3(x,y,z));
        }

        //
        FMat3 jacobi(const FMat3 &m,FMat3 &v) {
            FMat3 j,ret = m;
            v.setIdentity();
            f32 prev = INFINITY;
            const s32 num_iterations = 50;
            for(s32 n = 0; n < num_iterations; n++) {
                s32 p = 0;
                s32 q = 1;
                for(s32 i = 0; i < 3; i++) {
                    for(s32 j = 0; j < 3; j++) {
                        if(i == j) continue;
                        if(Math::abs(ret.get(i,j)) > Math::abs(ret.get(p,q))) {
                            p = i;
                            q = j;
                        }
                    }
                }
                f32 c = 1.0f;
                f32 s = 0.0f;
                if(Math::abs(ret.get(p,q)) > EPSILON) {
                    f32 r = (ret.get(q,q) - ret.get(p,p)) / (ret.get(p,q) * 2.0f);
                    if(r >= 0.0f) r = Math::rcp(r + Math::sqrt(1.0f + r * r));
                    else r = -Math::rcp(-r + Math::sqrt(1.0f + r * r));
                    c = Math::rsqrt(1.0f + r * r);
                    s = r * c;
                }
                j.setIdentity();
                j.set(p,p,c);
                j.set(p,q,s);
                j.set(q,p,-s);
                j.set(q,q,c);
                v = v * j;
                ret = transpose(j) * ret * j;
                f32 sum = 0.0f;
                for(s32 i = 0; i < 3; i++) {
                    for(s32 j = 0; j < 3; j++) {
                        if(i == j) continue;
                        sum += ret.get(i,j) * ret.get(i,j);
                    }
                }
                if(prev <= sum) break;
                prev = sum;
            }
            return ret;
        }

    }//!namesapce util
    
}//!namespace sv
