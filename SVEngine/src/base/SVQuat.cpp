#include "SVQuat.h"

namespace sv {
    
    namespace util {


        /******************************************************************************\
        *
        * SVQuat
        *
        \******************************************************************************/

        //
        const SVQuat quat_identity(0.0f,0.0f,0.0f,1.0f);

        //
        SVQuat::SVQuat(const FVec3 &axis,f32 angle) {
            set(axis,angle);
        }

        SVQuat::SVQuat(f32 x,f32 y,f32 z,f32 angle) {
            set(x,y,z,angle);
        }

        SVQuat::SVQuat(f32 angle_x,f32 angle_y,f32 angle_z) {
            set(angle_x,angle_y,angle_z);
        }

        SVQuat::SVQuat(const FMat3 &m) {
            SVQuat q = m.getQuat();
            x = q.x;
            y = q.y;
            z = q.z;
            w = q.w;
        }

        SVQuat::SVQuat(const FMat4 &m) {
            SVQuat q = FMat3(m).getQuat();
            x = q.x;
            y = q.y;
            z = q.z;
            w = q.w;
        }

        SVQuat::SVQuat(const DMat4 &m) {
            SVQuat q = FMat3(m).getQuat();
            x = q.x;
            y = q.y;
            z = q.z;
            w = q.w;
        }

        //
        SVQuat SVQuat::operator-() const {
            SVQuat ret;
            ret.x = -x;
            ret.y = -y;
            ret.z = -z;
            ret.w = -w;
            return ret;
        }

        SVQuat &SVQuat::operator*=(f32 v) {
            return mul(*this,*this,v);
        }

        SVQuat &SVQuat::operator*=(const SVQuat &q) {
            return mul(*this,SVQuat(*this),q);
        }

        SVQuat &SVQuat::operator+=(const SVQuat &q) {
            return add(*this,*this,q);
        }

        SVQuat &SVQuat::operator-=(const SVQuat &q) {
            return sub(*this,*this,q);
        }

        //
        void SVQuat::set(const FVec3 &v) {
            x = v.x;
            y = v.y;
            z = v.z;
            w = 0.0f;
        }

        void SVQuat::set(const FMat3 &m) {
            SVQuat q = m.getQuat();
            x = q.x;
            y = q.y;
            z = q.z;
            w = q.w;
        }

        void SVQuat::set(const FMat4 &m) {
            SVQuat q = FMat3(m).getQuat();
            x = q.x;
            y = q.y;
            z = q.z;
            w = q.w;
        }

        void SVQuat::set(const DMat4 &m) {
            SVQuat q = FMat3(m).getQuat();
            x = q.x;
            y = q.y;
            z = q.z;
            w = q.w;
        }

        void SVQuat::set(const FVec3 &axis,f32 angle) {
            f32 s,c;
            Math::sincos(angle * DEG2RAD * 0.5f,s,c);
            f32 ilength = Math::rsqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
            x = axis.x * ilength * s;
            y = axis.y * ilength * s;
            z = axis.z * ilength * s;
            w = c;
        }

        void SVQuat::set(f32 axis_x,f32 axis_y,f32 axis_z,f32 angle) {
            f32 s,c;
            Math::sincos(angle * DEG2RAD * 0.5f,s,c);
            f32 ilength = Math::rsqrt(axis_x * axis_x + axis_y * axis_y + axis_z * axis_z);
            x = axis_x * ilength * s;
            y = axis_y * ilength * s;
            z = axis_z * ilength * s;
            w = c;
        }

        void SVQuat::set(f32 angle_x,f32 angle_y,f32 angle_z) {
            f32 sx,cx,sy,cy,sz,cz;
            Math::sincos(angle_x * DEG2RAD * 0.5f,sx,cx);
            Math::sincos(angle_y * DEG2RAD * 0.5f,sy,cy);
            Math::sincos(angle_z * DEG2RAD * 0.5f,sz,cz);
            f32 sxcy = sx * cy;
            f32 cxcy = cx * cy;
            f32 sxsy = sx * sy;
            f32 cxsy = cx * sy;
            x = cxsy * sz + sxcy * cz;
            y = cxsy * cz - sxcy * sz;
            z = sxsy * cz + cxcy * sz;
            w = cxcy * cz - sxsy * sz;
        }

        void SVQuat::get(FVec3 &axis,f32 &angle) const {
            f32 ilength = Math::rsqrt(x * x + y * y + z * z);
            axis.x = x * ilength;
            axis.y = y * ilength;
            axis.z = z * ilength;
            angle = Math::acos(clamp(w,-1.0f,1.0f)) * RAD2DEG * 2.0f;
            if(angle > 180.0f) {
                angle -= 360.0f;
            }
        }

        //
        FMat3 SVQuat::getMat3() const {
            FMat3 ret;
            f32 x2 = x + x;
            f32 y2 = y + y;
            f32 z2 = z + z;
            f32 xx2 = x * x2;
            f32 yy2 = y * y2;
            f32 zz2 = z * z2;
            f32 zx2 = z * x2;
            f32 xy2 = x * y2;
            f32 yz2 = y * z2;
            f32 wx2 = w * x2;
            f32 wy2 = w * y2;
            f32 wz2 = w * z2;
            ret.m00 = 1.0f - yy2 - zz2;
            ret.m01 = xy2 - wz2;
            ret.m02 = zx2 + wy2;
            ret.m10 = xy2 + wz2;
            ret.m11 = 1.0f - xx2 - zz2;
            ret.m12 = yz2 - wx2;
            ret.m20 = zx2 - wy2;
            ret.m21 = yz2 + wx2;
            ret.m22 = 1.0f - xx2 - yy2;
            return ret;
        }

        f32 SVQuat::getAngle(const FVec3 &axis) const {
            FVec3 v0,v1;
            mul(v0,*this,axis);
            add(v1,v0,axis);
            v1.normalize();
            f32 angle = dot(axis,v1);
            if(Math::abs(angle) > EPSILON) {
                SVQuat q0,q1;
                q0.set(-cross(v0,axis,v1));
                q0.w = angle;
                mul(q1,q0,*this);
                q1.normalize();
                q1.get(v0,angle);
            } else {
                get(v0,angle);
            }
            if(dot(v0,axis) < 0.0f){
                angle = -angle;
            }
            return angle;
        }

        //
        s32 operator==(const SVQuat &q0,const SVQuat &q1) {
            return compare(q0,q1);
        }

        s32 operator!=(const SVQuat &q0,const SVQuat &q1) {
            return !compare(q0,q1);
        }

        SVQuat operator*(const SVQuat &q,f32 v) {
            SVQuat ret;
            return mul(ret,q,v);
        }

        FVec3 operator*(const SVQuat &q,const FVec3 &v) {
            FVec3 ret;
            return mul(ret,q,v);
        }

        FVec3 operator*(const FVec3 &v,const SVQuat &q) {
            FVec3 ret;
            return mul(ret,v,q);
        }

        DVec3 operator*(const SVQuat &q,const DVec3 &v) {
            DVec3 ret;
            return mul(ret,q,v);
        }

        DVec3 operator*(const DVec3 &v,const SVQuat &q) {
            DVec3 ret;
            return mul(ret,v,q);
        }

        SVQuat operator*(const SVQuat &q0,const SVQuat &q1) {
            SVQuat ret;
            return mul(ret,q0,q1);
        }

        SVQuat operator+(const SVQuat &q0,const SVQuat &q1) {
            SVQuat ret;
            return add(ret,q0,q1);
        }

        SVQuat operator-(const SVQuat &q0,const SVQuat &q1) {
            SVQuat ret;
            return sub(ret,q0,q1);
        }

        //
        FVec3 &mul(FVec3 &ret,const SVQuat &q,const FVec3 &v) {
            f32 x2 = q.x + q.x;
            f32 y2 = q.y + q.y;
            f32 z2 = q.z + q.z;
            f32 xx2 = q.x * x2;
            f32 yy2 = q.y * y2;
            f32 zz2 = q.z * z2;
            f32 zx2 = q.z * x2;
            f32 xy2 = q.x * y2;
            f32 yz2 = q.y * z2;
            f32 wx2 = q.w * x2;
            f32 wy2 = q.w * y2;
            f32 wz2 = q.w * z2;
            ret.x = (1.0f - yy2 - zz2) * v.x + (xy2 - wz2) * v.y + (zx2 + wy2) * v.z;
            ret.y = (xy2 + wz2) * v.x + (1.0f - xx2 - zz2) * v.y + (yz2 - wx2) * v.z;
            ret.z = (zx2 - wy2) * v.x + (yz2 + wx2) * v.y + (1.0f - xx2 - yy2) * v.z;
            return ret;
        }

        FVec3 &mul(FVec3 &ret,const FVec3 &v,const SVQuat &q) {
            f32 x2 = q.x + q.x;
            f32 y2 = q.y + q.y;
            f32 z2 = q.z + q.z;
            f32 xx2 = q.x * x2;
            f32 yy2 = q.y * y2;
            f32 zz2 = q.z * z2;
            f32 zx2 = q.z * x2;
            f32 xy2 = q.x * y2;
            f32 yz2 = q.y * z2;
            f32 wx2 = q.w * x2;
            f32 wy2 = q.w * y2;
            f32 wz2 = q.w * z2;
            ret.x = (1.0f - yy2 - zz2) * v.x + (xy2 + wz2) * v.y + (zx2 - wy2) * v.z;
            ret.y = (xy2 - wz2) * v.x + (1.0f - xx2 - zz2) * v.y + (yz2 + wx2) * v.z;
            ret.z = (zx2 + wy2) * v.x + (yz2 - wx2) * v.y + (1.0f - xx2 - yy2) * v.z;
            return ret;
        }

        DVec3 &mul(DVec3 &ret,const SVQuat &q,const DVec3 &v) {
            f64 x2 = q.x + q.x;
            f64 y2 = q.y + q.y;
            f64 z2 = q.z + q.z;
            f64 xx2 = q.x * x2;
            f64 yy2 = q.y * y2;
            f64 zz2 = q.z * z2;
            f64 zx2 = q.z * x2;
            f64 xy2 = q.x * y2;
            f64 yz2 = q.y * z2;
            f64 wx2 = q.w * x2;
            f64 wy2 = q.w * y2;
            f64 wz2 = q.w * z2;
            ret.x = (1.0 - yy2 - zz2) * v.x + (xy2 - wz2) * v.y + (zx2 + wy2) * v.z;
            ret.y = (xy2 + wz2) * v.x + (1.0 - xx2 - zz2) * v.y + (yz2 - wx2) * v.z;
            ret.z = (zx2 - wy2) * v.x + (yz2 + wx2) * v.y + (1.0 - xx2 - yy2) * v.z;
            return ret;
        }

        DVec3 &mul(DVec3 &ret,const DVec3 &v,const SVQuat &q) {
            f64 x2 = q.x + q.x;
            f64 y2 = q.y + q.y;
            f64 z2 = q.z + q.z;
            f64 xx2 = q.x * x2;
            f64 yy2 = q.y * y2;
            f64 zz2 = q.z * z2;
            f64 zx2 = q.z * x2;
            f64 xy2 = q.x * y2;
            f64 yz2 = q.y * z2;
            f64 wx2 = q.w * x2;
            f64 wy2 = q.w * y2;
            f64 wz2 = q.w * z2;
            ret.x = (1.0 - yy2 - zz2) * v.x + (xy2 + wz2) * v.y + (zx2 - wy2) * v.z;
            ret.y = (xy2 - wz2) * v.x + (1.0 - xx2 - zz2) * v.y + (yz2 + wx2) * v.z;
            ret.z = (zx2 + wy2) * v.x + (yz2 - wx2) * v.y + (1.0 - xx2 - yy2) * v.z;
            return ret;
        }

        SVQuat &mul(SVQuat &ret,const SVQuat &q0,const SVQuat &q1) {
            ret.x = q0.w * q1.x + q0.x * q1.w + q0.y * q1.z - q0.z * q1.y;
            ret.y = q0.w * q1.y + q0.y * q1.w + q0.z * q1.x - q0.x * q1.z;
            ret.z = q0.w * q1.z + q0.z * q1.w + q0.x * q1.y - q0.y * q1.x;
            ret.w = q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
            return ret;
        }

        SVQuat &slerp(SVQuat &ret,const SVQuat &q0,const SVQuat &q1,f32 k) {
            if(k <= 0.0f) {
                ret = q0;
            } else if(k >= 1.0f) {
                ret = q1;
            } else {
                f32 k0,k1;
                f32 c = dot(q0,q1);
                f32 ac = Math::abs(c);
                if(ac < 1.0f - EPSILON) {
                    f32 angle = Math::acos(ac);
                    f32 is = Math::rcp(Math::sin(angle));
                    k0 = Math::sin(angle * (1.0f - k)) * is;
                    k1 = Math::sin(angle * k) * is;
                } else {
                    k0 = 1.0f - k;
                    k1 = k;
                }
                if(c < 0.0f) k1 = -k1;
                ret.x = q0.x * k0 + q1.x * k1;
                ret.y = q0.y * k0 + q1.y * k1;
                ret.z = q0.z * k0 + q1.z * k1;
                ret.w = q0.w * k0 + q1.w * k1;
            }
            return ret;
        }

        //
        SVQuat normalize(const SVQuat &q) {
            SVQuat ret = q;
            return ret.normalize();
        }

        SVQuat inverse(const SVQuat &q) {
            SVQuat ret;
            return inverse(ret,q);
        }

        SVQuat slerp(const SVQuat &q0,const SVQuat &q1,f32 k) {
            SVQuat ret;
            return slerp(ret,q0,q1,k);
        }

        //
        void decomposeTransform(const FMat4 &m,SVQuat &q0,SVQuat &q1) {
            FMat3 rotate;
            q0 = orthonormalize(rotate,FMat3(m)).getQuat();
            q1.x = ( m.m03 * q0.w + m.m13 * q0.z - m.m23 * q0.y) * 0.5f;
            q1.y = (-m.m03 * q0.z + m.m13 * q0.w + m.m23 * q0.x) * 0.5f;
            q1.z = ( m.m03 * q0.y - m.m13 * q0.x + m.m23 * q0.w) * 0.5f;
            q1.w = ( m.m03 * q0.x + m.m13 * q0.y + m.m23 * q0.z) * 0.5f;
            mul(q0,q0,Math::sqrt(m.m00 * m.m00 + m.m01 * m.m01 + m.m02 * m.m02));
        }

        FMat4 &composeTransform(FMat4 &ret,const SVQuat &q0,const SVQuat &q1) {
            f32 ilength = Math::rcp(dot(q0,q1)) * 2.0f;
            f32 nx = q0.x * ilength;
            f32 ny = q0.y * ilength;
            f32 nz = q0.z * ilength;
            f32 nw = q0.w * ilength;
            f32 xx = q0.x * nx;
            f32 yy = q0.y * ny;
            f32 zz = q0.z * nz;
            f32 ww = q0.w * nw;
            f32 xy = q0.x * ny;
            f32 xz = q0.x * nz;
            f32 xw = q0.x * nw;
            f32 yz = q0.y * nz;
            f32 yw = q0.y * nw;
            f32 zw = q0.z * nw;
            ret.m00 = (ww + xx - yy - zz) * 0.5f;
            ret.m10 = xy + zw;
            ret.m20 = xz - yw;
            ret.m30 = 0.0f;
            ret.m01 = xy - zw;
            ret.m11 = (ww + yy - xx - zz) * 0.5f;
            ret.m21 = yz + xw;
            ret.m31 = 0.0f;
            ret.m02 = xz + yw;
            ret.m12 = yz - xw;
            ret.m22 = (ww + zz - xx - yy) * 0.5f;
            ret.m32 = 0.0f;
            ret.m03 = q1.w * nx + q1.x * nw + q1.z * ny - q1.y * nz;
            ret.m13 = q1.w * ny + q1.x * nz + q1.y * nw - q1.z * nx;
            ret.m23 = q1.w * nz + q1.y * nx + q1.z * nw - q1.x * ny;
            ret.m33 = 1.0f;
            return ret;
        }
        
    }//!namesapce util
    
}//!namespace sv
