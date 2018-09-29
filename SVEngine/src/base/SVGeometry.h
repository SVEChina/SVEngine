#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "SVMathLib.h"
#include "SVVec3.h"
#include "SVVec4.h"
#include "SVMat4.h"
#include "SVMat4d.h"

namespace sv {
    
    namespace util {
        
        //******************************** Angle normalization **********************************************\
        //
        sv_inline f32 normalizeAngle(f32 angle) {
            if(angle >= 180.0f) {
                angle = Math::mod(angle + 180.0f,360.0f) - 180.0f;
            } else if(angle <= -180.0f) {
                angle = -Math::mod(-angle - 180.0f,360.0f) + 180.0f;
            }
            return angle;
        }
        
        //*********************************** Orthonormal basis *******************************************\
        //
        sv_inline void orthoBasis(const FVec3 &v,FVec3 &tangent,FVec3 &binormal) {
            if(Math::abs(v.z) > 0.7f) {
                f32 length2 = v.y * v.y + v.z * v.z;
                f32 ilength = Math::rsqrt(length2);
                tangent.x = 0.0f;
                tangent.y = -v.z * ilength;
                tangent.z = v.y * ilength;
                binormal.x = length2 * ilength;
                binormal.y = -v.x * tangent.z;
                binormal.z = v.x * tangent.y;
            } else {
                f32 length2 = v.x * v.x + v.y * v.y;
                f32 ilength = Math::rsqrt(length2);
                tangent.x = -v.y * ilength;
                tangent.y = v.x * ilength;
                tangent.z = 0.0f;
                binormal.x = -v.z * tangent.y;
                binormal.y = v.z * tangent.x;
                binormal.z = length2 * ilength;
            }
        }
        
        sv_inline void orthoBasis(const DVec3 &v,DVec3 &tangent,DVec3 &binormal) {
            if(Math::abs(v.z) > 0.7) {
                f64 length2 = v.y * v.y + v.z * v.z;
                f64 ilength = Math::rsqrt(length2);
                tangent.x = 0.0;
                tangent.y = -v.z * ilength;
                tangent.z = v.y * ilength;
                binormal.x = length2 * ilength;
                binormal.y = -v.x * tangent.z;
                binormal.z = v.x * tangent.y;
            } else {
                f64 length2 = v.x * v.x + v.y * v.y;
                f64 ilength = Math::rsqrt(length2);
                tangent.x = -v.y * ilength;
                tangent.y = v.x * ilength;
                tangent.z = 0.0;
                binormal.x = -v.z * tangent.y;
                binormal.y = v.z * tangent.x;
                binormal.z = length2 * ilength;
            }
        }
        
        //
        void orthoTransform(const FVec3 &v,FMat4 &transform);
        void orthoTransform(const DVec3 &v,DMat4 &transform);
        
        //******************************** Triangle parameters **********************************************\
        //
        f32 triangleArea(const FVec3 &v0,const FVec3 &v1,const FVec3 &v2);
        f64 triangleArea(const DVec3 &v0,const DVec3 &v1,const DVec3 &v2);
        //
        FVec3 triangleNormal(const FVec3 &v0,const FVec3 &v1,const FVec3 &v2);
        DVec3 triangleNormal(const DVec3 &v0,const DVec3 &v1,const DVec3 &v2);
        //
        FVec4 trianglePlane(const FVec3 &v0,const FVec3 &v1,const FVec3 &v2);
        DVec4 trianglePlane(const DVec3 &v0,const DVec3 &v1,const DVec3 &v2);
        //******************************* Polygon parameters ***********************************************\
        //
        FVec4 polygonPlane(const FVec3 *vertex,const u16 *indices,s32 num_indices);
        DVec4 polygonPlane(const DVec3 *vertex,const u16 *indices,s32 num_indices);
        //****************************** Point tringle intersections *********************************************\
        //
        s32 pointTriangleInside(const FVec3 &point,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2);
        s32 pointTriangleInside(const DVec3 &point,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2);
        s32 pointTriangleInside(const FVec3 &point,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2,const FVec3 &normal);
        s32 pointTriangleInside(const DVec3 &point,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2,const DVec3 &normal);
        //
        f32 pointTriangleDistance(const FVec3 &point,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2,const FVec4 &plane);
        f64 pointTriangleDistance(const DVec3 &point,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2,const DVec4 &plane);
        //
        void pointTriangleCoordinates(const FVec3 &point,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2,f32 &a,f32 &b);
        void pointTriangleCoordinates(const DVec3 &point,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2,f64 &a,f64 &b);
        //******************************* Point polygon intersections ***********************************************\
        //
        s32 pointPolygonInside(const FVec3 &point,const FVec3 *vertex,const u16 *indices,s32 num_indices);
        s32 pointPolygonInside(const DVec3 &point,const DVec3 *vertex,const u16 *indices,s32 num_indices);
        s32 pointPolygonInside(const FVec3 &point,const FVec3 *vertex,const u16 *indices,s32 num_indices,const FVec3 &normal);
        s32 pointPolygonInside(const DVec3 &point,const DVec3 *vertex,const u16 *indices,s32 num_indices,const DVec3 &normal);
        //
        f32 pointPolygonDistance(const FVec3 &point,const FVec3 *vertex,const u16 *indices,s32 num_indices,const FVec4 &plane);
        f64 pointPolygonDistance(const DVec3 &point,const DVec3 *vertex,const u16 *indices,s32 num_indices,const DVec4 &plane);
        //******************************* Bounding box intersections ***********************************************\
        //
        s32 rayBoundBoxIntersection(const FVec3 &point,const FVec3 &direction,const FVec3 &min,const FVec3 &max);
        s32 rayBoundBoxIntersection(const DVec3 &point,const DVec3 &direction,const DVec3 &min,const DVec3 &max);
        //
        s32 irayBoundBoxIntersection(const FVec3 &point,const FVec3 &idirection,const FVec3 &min,const FVec3 &max);
        s32 irayBoundBoxIntersection(const DVec3 &point,const DVec3 &idirection,const DVec3 &min,const DVec3 &max);
        //
        s32 rayTriangleIntersection(const FVec3 &point,const FVec3 &direction,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2);
        s32 rayTriangleIntersection(const DVec3 &point,const DVec3 &direction,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2);
        //
        s32 rayPlaneIntersection(FVec3 &ret,const FVec3 &point,const FVec3 &direction,const FVec4 &plane);
        s32 rayPlaneIntersection(DVec3 &ret,const DVec3 &point,const DVec3 &direction,const DVec3 &plane);
        //******************************** Closest points **********************************************\
        //
        void getClosestPointOnLine(const FVec3 &point,const FVec3 &p0,const FVec3 &p1,FVec3 &ret);
        void getClosestPointOnLine(const DVec3 &point,const DVec3 &p0,const DVec3 &p1,DVec3 &ret);
        //
        s32 getClosestPointOnTriangle(const FVec3 &point,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2,FVec3 &ret);
        s32 getClosestPointOnTriangle(const DVec3 &point,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2,DVec3 &ret);
        //
        s32 getClosestPointsOnLines(const FVec3 &p00,const FVec3 &p01,const FVec3 &p10,const FVec3 &p11,FVec3 &ret_0,FVec3 &ret_1);
        s32 getClosestPointsOnLines(const DVec3 &p00,const DVec3 &p01,const DVec3 &p10,const DVec3 &p11,DVec3 &ret_0,DVec3 &ret_1);
        
    } //!namespace util
    
}//!namespace sv



#endif /* __GEOMETRY_H__ */
