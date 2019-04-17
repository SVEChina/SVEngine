#include "SVGeometry.h"

namespace sv {
    
    namespace util {
        
        //************************************** Orthonormal transformation ****************************************\
        //
        void orthoTransform(const FVec3 &v,FMat4 &transform) {
            if(Math::abs(v.z) > 0.7f) {
                f32 length2 = v.y * v.y + v.z * v.z;
                f32 ilength = Math::rsqrt(length2);
                transform.m00 = 0.0f;
                transform.m10 = -v.z * ilength;
                transform.m20 = v.y * ilength;
                transform.m01 = length2 * ilength;
                transform.m11 = -v.x * transform.m20;
                transform.m21 = v.x * transform.m10;
            } else {
                f32 length2 = v.x * v.x + v.y * v.y;
                f32 ilength = Math::rsqrt(length2);
                transform.m00 = -v.y * ilength;
                transform.m10 = v.x * ilength;
                transform.m20 = 0.0f;
                transform.m01 = -v.z * transform.m10;
                transform.m11 = v.z * transform.m00;
                transform.m21 = length2 * ilength;
            }
            transform.m02 = v.x;
            transform.m12 = v.y;
            transform.m22 = v.z;
        }

        void orthoTransform(const DVec3 &v,DMat4 &transform) {
            if(Math::abs(v.z) > 0.7) {
                f64 length2 = v.y * v.y + v.z * v.z;
                f64 ilength = Math::rsqrt(length2);
                transform.m00 = 0.0;
                transform.m10 = -v.z * ilength;
                transform.m20 = v.y * ilength;
                transform.m01 = length2 * ilength;
                transform.m11 = -v.x * transform.m20;
                transform.m21 = v.x * transform.m10;
            } else {
                f64 length2 = v.x * v.x + v.y * v.y;
                f64 ilength = Math::rsqrt(length2);
                transform.m00 = -v.y * ilength;
                transform.m10 = v.x * ilength;
                transform.m20 = 0.0;
                transform.m01 = -v.z * transform.m10;
                transform.m11 = v.z * transform.m00;
                transform.m21 = length2 * ilength;
            }
            transform.m02 = v.x;
            transform.m12 = v.y;
            transform.m22 = v.z;
        }

        //********************************** Triangle parameters********************************************\
        //
        f32 triangleArea(const FVec3 &v0,const FVec3 &v1,const FVec3 &v2) {
            f32 a = length(v1 - v2);
            f32 b = length(v2 - v0);
            f32 c = length(v0 - v1);
            f32 s = (a + b + c) * 0.5f;
            return Math::sqrt(s * (s - a) * (s - b) * (s - c));
        }

        f64 triangleArea(const DVec3 &v0,const DVec3 &v1,const DVec3 &v2) {
            f64 a = length(v1 - v2);
            f64 b = length(v2 - v0);
            f64 c = length(v0 - v1);
            f64 s = (a + b + c) * 0.5;
            return Math::sqrt(s * (s - a) * (s - b) * (s - c));
        }

        //
        FVec3 triangleNormal(const FVec3 &v0,const FVec3 &v1,const FVec3 &v2) {
            FVec3 v10,v20,normal;
            cross(normal,sub(v10,v1,v0),sub(v20,v2,v0));
            normal.normalize();
            return normal;
        }

        DVec3 triangleNormal(const DVec3 &v0,const DVec3 &v1,const DVec3 &v2) {
            DVec3 v10,v20,normal;
            cross(normal,sub(v10,v1,v0),sub(v20,v2,v0));
            normal.normalize();
            return normal;
        }

        //
        FVec4 trianglePlane(const FVec3 &v0,const FVec3 &v1,const FVec3 &v2) {
            FVec3 v10,v20,normal;
            cross(normal,sub(v10,v1,v0),sub(v20,v2,v0));
            normal.normalize();
            return FVec4(normal,-dot(normal,v0));
        }

        DVec4 trianglePlane(const DVec3 &v0,const DVec3 &v1,const DVec3 &v2) {
            DVec3 v10,v20,normal;
            cross(normal,sub(v10,v1,v0),sub(v20,v2,v0));
            normal.normalize();
            return DVec4(normal,-dot(normal,v0));
        }

        //*********************************** Polygon parameters*******************************************\
        //
        FVec4 polygonPlane(const FVec3 *vertex,const u16 *indices,s32 num_indices) {
            f32 area = 0.0f;
            FVec3 v10,v20,normal;
            FVec4 plane = FVec4_zero;
            for(s32 i = 2; i < num_indices; i++) {
                const FVec3 &v0 = vertex[indices[i - 2]];
                const FVec3 &v1 = vertex[indices[i - 1]];
                const FVec3 &v2 = vertex[indices[i]];
                cross(normal,sub(v10,v1,v0),sub(v20,v2,v0));
                f32 a = length(normal);
                if(area < a) {
                    normal *= Math::rcp(a);
                    plane.set(normal,-dot(normal,v0));
                    area = a;
                }
            }
            return plane;
        }

        DVec4 polygonPlane(const DVec3 *vertex,const u16 *indices,s32 num_indices) {
            f64 area = 0.0;
            DVec3 v10,v20,normal;
            DVec4 plane = DVec4_zero;
            for(s32 i = 2; i < num_indices; i++) {
                const DVec3 &v0 = vertex[indices[i - 2]];
                const DVec3 &v1 = vertex[indices[i - 1]];
                const DVec3 &v2 = vertex[indices[i]];
                cross(normal,sub(v10,v1,v0),sub(v20,v2,v0));
                f64 a = length(normal);
                if(area < a) {
                    normal *= Math::rcp(a);
                    plane.set(normal,-dot(normal,v0));
                    area = a;
                }
            }
            return plane;
        }

        //******************************** Point tringle intersections **********************************************\
        //
        s32 pointTriangleInside(const FVec3 &point,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2) {
            FVec3 axis,normal;
            FVec3 v10,v20,pv0;
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(normal,v10,v20);
            cross(axis,normal,v20);
            f32 det = dot(v10,axis);
            if(det > 0.0f) {
                sub(pv0,point,v0);
                f32 s = dot(pv0,axis);
                if(s < 0.0f || s > det) return 0;
                cross(axis,pv0,v10);
                f32 t = dot(normal,axis);
                if(t < 0.0f || t + s > det) return 0;
                return 1;
            } else if(det < 0.0f) {
                sub(pv0,point,v0);
                f32 s = dot(pv0,axis);
                if(s > 0.0f || s < det) return 0;
                cross(axis,pv0,v10);
                f32 t = dot(normal,axis);
                if(t > 0.0f || t + s < det) return 0;
                return 1;
            }
            return 0;
        }

        s32 pointTriangleInside(const DVec3 &point,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2) {
            DVec3 axis,normal;
            DVec3 v10,v20,pv0;
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(normal,v10,v20);
            cross(axis,normal,v20);
            f64 det = dot(v10,axis);
            if(det > 0.0) {
                sub(pv0,point,v0);
                f64 s = dot(pv0,axis);
                if(s < 0.0 || s > det) return 0;
                cross(axis,pv0,v10);
                f64 t = dot(normal,axis);
                if(t < 0.0 || t + s > det) return 0;
                return 1;
            } else if(det < 0.0) {
                sub(pv0,point,v0);
                f64 s = dot(pv0,axis);
                if(s > 0.0 || s < det) return 0;
                cross(axis,pv0,v10);
                f64 t = dot(normal,axis);
                if(t > 0.0 || t + s < det) return 0;
                return 1;
            }
            return 0;
        }

        //
        s32 pointTriangleInside(const FVec3 &point,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2,const FVec3 &normal) {
            FVec3 axis;
            FVec3 v10,v20,pv0;
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(axis,normal,v20);
            f32 det = dot(v10,axis);
            if(det > 0.0f) {
                sub(pv0,point,v0);
                f32 s = dot(pv0,axis);
                if(s < 0.0f || s > det) return 0;
                cross(axis,pv0,v10);
                f32 t = dot(normal,axis);
                if(t < 0.0f || t + s > det) return 0;
                return 1;
            } else if(det < 0.0f) {
                sub(pv0,point,v0);
                f32 s = dot(pv0,axis);
                if(s > 0.0f || s < det) return 0;
                cross(axis,pv0,v10);
                f32 t = dot(normal,axis);
                if(t > 0.0f || t + s < det) return 0;
                return 1;
            }
            return 0;
        }

        s32 pointTriangleInside(const DVec3 &point,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2,const DVec3 &normal) {
            DVec3 axis;
            DVec3 v10,v20,pv0;
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(axis,normal,v20);
            f64 det = dot(v10,axis);
            if(det > 0.0) {
                sub(pv0,point,v0);
                f64 s = dot(pv0,axis);
                if(s < 0.0 || s > det) return 0;
                cross(axis,pv0,v10);
                f64 t = dot(normal,axis);
                if(t < 0.0 || t + s > det) return 0;
                return 1;
            } else if(det < 0.0) {
                sub(pv0,point,v0);
                f64 s = dot(pv0,axis);
                if(s > 0.0 || s < det) return 0;
                cross(axis,pv0,v10);
                f64 t = dot(normal,axis);
                if(t > 0.0 || t + s < det) return 0;
                return 1;
            }
            return 0;
        }

        //
        f32 pointTriangleDistance(const FVec3 &point,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2,const FVec4 &plane) {
            FVec3 axis,normal = FVec3(plane);
            FVec3 v10,v20,pv0;
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(axis,normal,v20);
            f32 det = dot(v10,axis);
            if(det > 0.0f) {
                sub(pv0,point,v0);
                f32 s = dot(pv0,axis);
                if(s >= 0.0f && s <= det) {
                    cross(axis,pv0,v10);
                    f32 t = dot(normal,axis);
                    if(t >= 0.0f && t + s <= det) {
                        return Math::abs(dot(plane,point));
                    }
                }
            } else if(det < 0.0f) {
                sub(pv0,point,v0);
                f32 s = dot(pv0,axis);
                if(s <= 0.0f && s >= det) {
                    cross(axis,pv0,v10);
                    f32 t = dot(normal,axis);
                    if(t <= 0.0f && t + s >= det) {
                        return Math::abs(dot(plane,point));
                    }
                }
            }
            FVec3 v21;
            sub(v21,v2,v1);
            f32 k0 = saturate(dot(v10,sub(pv0,point,v0)) * Math::rcp(length2(v10)));
            f32 k1 = saturate(dot(v20,sub(pv0,point,v0)) * Math::rcp(length2(v20)));
            f32 k2 = saturate(dot(v21,sub(pv0,point,v1)) * Math::rcp(length2(v21)));
            k0 = length2(sub(pv0,point,mad(pv0,v10,k0,v0)));
            k1 = length2(sub(pv0,point,mad(pv0,v20,k1,v0)));
            k2 = length2(sub(pv0,point,mad(pv0,v21,k2,v1)));
            return Math::sqrt(min(k0,min(k1,k2)));
        }

        f64 pointTriangleDistance(const DVec3 &point,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2,const DVec4 &plane) {
            DVec3 axis,normal = DVec3(plane);
            DVec3 v10,v20,pv0;
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(axis,normal,v20);
            f64 det = dot(v10,axis);
            if(det > 0.0) {
                sub(pv0,point,v0);
                f64 s = dot(pv0,axis);
                if(s >= 0.0 && s <= det) {
                    cross(axis,pv0,v10);
                    f64 t = dot(normal,axis);
                    if(t >= 0.0 && t + s <= det) {
                        return Math::abs(dot(plane,point));
                    }
                }
            } else if(det < 0.0) {
                sub(pv0,point,v0);
                f64 s = dot(pv0,axis);
                if(s <= 0.0 && s >= det) {
                    cross(axis,pv0,v10);
                    f64 t = dot(normal,axis);
                    if(t <= 0.0 && t + s >= det) {
                        return Math::abs(dot(plane,point));
                    }
                }
            }
            DVec3 v21;
            sub(v21,v2,v1);
            f64 k0 = saturate(dot(v10,sub(pv0,point,v0)) * Math::rcp(length2(v10)));
            f64 k1 = saturate(dot(v20,sub(pv0,point,v0)) * Math::rcp(length2(v20)));
            f64 k2 = saturate(dot(v21,sub(pv0,point,v1)) * Math::rcp(length2(v21)));
            k0 = length2(sub(pv0,point,mad(pv0,v10,k0,v0)));
            k1 = length2(sub(pv0,point,mad(pv0,v20,k1,v0)));
            k2 = length2(sub(pv0,point,mad(pv0,v21,k2,v1)));
            return Math::sqrt(min(k0,min(k1,k2)));
        }

        //
        void pointTriangleCoordinates(const FVec3 &point,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2,f32 &a,f32 &b) {
            FVec3 area;
            FVec3 v20,v10,v0p;
            sub(v20,v2,v0);
            sub(v10,v1,v0);
            f32 iarea = Math::rsqrt(length2(cross(area,v20,v10)));
            sub(v20,v2,point);
            sub(v10,v1,point);
            sub(v0p,v0,point);
            a = length(cross(area,v20,v0p)) * iarea;
            b = length(cross(area,v10,v0p)) * iarea;
        }

        void pointTriangleCoordinates(const DVec3 &point,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2,f64 &a,f64 &b) {
            DVec3 area;
            DVec3 v20,v10,v0p;
            sub(v20,v2,v0);
            sub(v10,v1,v0);
            f64 iarea = Math::rsqrt(length2(cross(area,v20,v10)));
            sub(v20,v2,point);
            sub(v10,v1,point);
            sub(v0p,v0,point);
            a = length(cross(area,v20,v0p)) * iarea;
            b = length(cross(area,v10,v0p)) * iarea;
        }

        //************************************* Point polygon intersections *****************************************\
        //
        s32 pointPolygonInside(const FVec3 &point,const FVec3 *vertex,const u16 *indices,s32 num_indices) {
            FVec3 clip,normal;
            FVec3 v10,v20;
            const FVec3 &v0 = vertex[indices[0]];
            const FVec3 &v1 = vertex[indices[1]];
            const FVec3 &v2 = vertex[indices[2]];
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(normal,v10,v20);
            for(s32 i = 0, j = 1; i < num_indices; i++, j++) {
                if(j == num_indices) j = 0;
                const FVec3 &v0 = vertex[indices[i]];
                const FVec3 &v1 = vertex[indices[j]];
                sub(v10,v1,v0);
                sub(v20,point,v0);
                cross(clip,v10,normal);
                f32 d = dot(clip,v20);
                if(d > 0.0f) return 0;
            }
            return 1;
        }

        s32 pointPolygonInside(const DVec3 &point,const DVec3 *vertex,const u16 *indices,s32 num_indices) {
            DVec3 clip,normal;
            DVec3 v10,v20;
            const DVec3 &v0 = vertex[indices[0]];
            const DVec3 &v1 = vertex[indices[1]];
            const DVec3 &v2 = vertex[indices[2]];
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(normal,v10,v20);
            for(s32 i = 0, j = 1; i < num_indices; i++, j++) {
                if(j == num_indices) j = 0;
                const DVec3 &v0 = vertex[indices[i]];
                const DVec3 &v1 = vertex[indices[j]];
                sub(v10,v1,v0);
                sub(v20,point,v0);
                cross(clip,v10,normal);
                f64 d = dot(clip,v20);
                if(d > 0.0) return 0;
            }
            return 1;
        }

        //
        s32 pointPolygonInside(const FVec3 &point,const FVec3 *vertex,const u16 *indices,s32 num_indices,const FVec3 &normal) {
            FVec3 clip;
            FVec3 v10,v20;
            for(s32 i = 0, j = 1; i < num_indices; i++, j++) {
                if(j == num_indices) j = 0;
                const FVec3 &v0 = vertex[indices[i]];
                const FVec3 &v1 = vertex[indices[j]];
                sub(v10,v1,v0);
                sub(v20,point,v0);
                cross(clip,v10,normal);
                f32 d = dot(clip,v20);
                if(d > 0.0f) return 0;
            }
            return 1;
        }

        s32 pointPolygonInside(const DVec3 &point,const DVec3 *vertex,const u16 *indices,s32 num_indices,const DVec3 &normal) {
            DVec3 clip;
            DVec3 v10,v20;
            for(s32 i = 0, j = 1; i < num_indices; i++, j++) {
                if(j == num_indices) j = 0;
                const DVec3 &v0 = vertex[indices[i]];
                const DVec3 &v1 = vertex[indices[j]];
                sub(v10,v1,v0);
                sub(v20,point,v0);
                cross(clip,v10,normal);
                f64 d = dot(clip,v20);
                if(d > 0.0) return 0;
            }
            return 1;
        }

        //
        f32 pointPolygonDistance(const FVec3 &point,const FVec3 *vertex,const u16 *indices,s32 num_indices,const FVec4 &plane) {
            FVec3 clip,normal = FVec3(plane);
            FVec3 v10,v20;
            for(s32 i = 0, j = 1; i < num_indices; i++, j++) {
                if(j == num_indices) j = 0;
                const FVec3 &v0 = vertex[indices[i]];
                const FVec3 &v1 = vertex[indices[j]];
                sub(v10,v1,v0);
                sub(v20,point,v0);
                cross(clip,v10,normal);
                f32 d = dot(clip,v20);
                if(d > 0.0f) {
                    f32 distance = INFINITY;
                    for(; i < num_indices; i++, j++) {
                        if(j == num_indices) j = 0;
                        const FVec3 &v0 = vertex[indices[i]];
                        const FVec3 &v1 = vertex[indices[j]];
                        sub(v10,v1,v0);
                        sub(v20,point,v0);
                        f32 k = saturate(dot(v10,v20) * Math::rcp(length2(v10)));
                        f32 d = length2(sub(v20,point,mad(v20,v10,k,v0)));
                        if(distance > d) distance = d;
                    }
                    return Math::sqrt(distance);
                }
            }
            return Math::abs(dot(plane,point));
        }

        f64 pointPolygonDistance(const DVec3 &point,const DVec3 *vertex,const u16 *indices,s32 num_indices,const DVec4 &plane) {
            DVec3 clip,normal = DVec3(plane);
            DVec3 v10,v20;
            for(s32 i = 0, j = 1; i < num_indices; i++, j++) {
                if(j == num_indices) j = 0;
                const DVec3 &v0 = vertex[indices[i]];
                const DVec3 &v1 = vertex[indices[j]];
                sub(v10,v1,v0);
                sub(v20,point,v0);
                cross(clip,v10,normal);
                f64 d = dot(clip,v20);
                if(d > 0.0) {
                    f64 distance = INFINITY;
                    for(; i < num_indices; i++, j++) {
                        if(j == num_indices) j = 0;
                        const DVec3 &v0 = vertex[indices[i]];
                        const DVec3 &v1 = vertex[indices[j]];
                        sub(v10,v1,v0);
                        sub(v20,point,v0);
                        f64 k = saturate(dot(v10,v20) * Math::rcp(length2(v10)));
                        f64 d = length2(sub(v20,point,mad(v20,v10,k,v0)));
                        if(distance > d) distance = d;
                    }
                    return Math::sqrt(distance);
                }
            }
            return Math::abs(dot(plane,point));
        }

        //******************************* Bounding box intersections **************************************
        //
        s32 rayBoundBoxIntersection(const FVec3 &point,const FVec3 &direction,const FVec3 &min,const FVec3 &max) {
            f32 tmin,tmax;
            f32 idirectionx = Math::rcp(direction.x);
            f32 idirectiony = Math::rcp(direction.y);
            if(direction.x >= 0.0f) {
                tmin = (min.x - point.x) * idirectionx;
                tmax = (max.x - point.x) * idirectionx;
            } else {
                tmin = (max.x - point.x) * idirectionx;
                tmax = (min.x - point.x) * idirectionx;
            }
            f32 tymin,tymax;
            if(direction.y >= 0.0f) {
                tymin = (min.y - point.y) * idirectiony;
                tymax = (max.y - point.y) * idirectiony;
            } else {
                tymin = (max.y - point.y) * idirectiony;
                tymax = (min.y - point.y) * idirectiony;
            }
            if((tmin > tymax) || (tmax < tymin)) return 0;
            if(tmin < tymin) tmin = tymin;
            if(tmax > tymax) tmax = tymax;
            f32 tzmin,tzmax;
            f32 idirectionz = Math::rcp(direction.z);
            if(direction.z >= 0.0f) {
                tzmin = (min.z - point.z) * idirectionz;
                tzmax = (max.z - point.z) * idirectionz;
            } else {
                tzmin = (max.z - point.z) * idirectionz;
                tzmax = (min.z - point.z) * idirectionz;
            }
            if((tmin > tzmax) || (tmax < tzmin)) return 0;
            if(tmin < tzmin) tmin = tzmin;
            if(tmax > tzmax) tmax = tzmax;
            return (tmax > 0.0f && tmin < 1.0f);
        }

        s32 rayBoundBoxIntersection(const DVec3 &point,const DVec3 &direction,const DVec3 &min,const DVec3 &max) {
            f64 tmin,tmax;
            f64 idirectionx = Math::rcp(direction.x);
            f64 idirectiony = Math::rcp(direction.y);
            if(direction.x >= 0.0) {
                tmin = (min.x - point.x) * idirectionx;
                tmax = (max.x - point.x) * idirectionx;
            } else {
                tmin = (max.x - point.x) * idirectionx;
                tmax = (min.x - point.x) * idirectionx;
            }
            f64 tymin,tymax;
            if(direction.y >= 0.0) {
                tymin = (min.y - point.y) * idirectiony;
                tymax = (max.y - point.y) * idirectiony;
            } else {
                tymin = (max.y - point.y) * idirectiony;
                tymax = (min.y - point.y) * idirectiony;
            }
            if((tmin > tymax) || (tmax < tymin)) return 0;
            if(tmin < tymin) tmin = tymin;
            if(tmax > tymax) tmax = tymax;
            f64 tzmin,tzmax;
            f64 idirectionz = Math::rcp(direction.z);
            if(direction.z >= 0.0) {
                tzmin = (min.z - point.z) * idirectionz;
                tzmax = (max.z - point.z) * idirectionz;
            } else {
                tzmin = (max.z - point.z) * idirectionz;
                tzmax = (min.z - point.z) * idirectionz;
            }
            if((tmin > tzmax) || (tmax < tzmin)) return 0;
            if(tmin < tzmin) tmin = tzmin;
            if(tmax > tzmax) tmax = tzmax;
            return (tmax > 0.0 && tmin < 1.0);
        }

        //
        s32 irayBoundBoxIntersection(const FVec3 &point,const FVec3 &idirection,const FVec3 &min,const FVec3 &max) {
            f32 tmin,tmax;
            if(idirection.x >= 0.0f) {
                tmin = (min.x - point.x) * idirection.x;
                tmax = (max.x - point.x) * idirection.x;
            } else {
                tmin = (max.x - point.x) * idirection.x;
                tmax = (min.x - point.x) * idirection.x;
            }
            f32 tymin,tymax;
            if(idirection.y >= 0.0f) {
                tymin = (min.y - point.y) * idirection.y;
                tymax = (max.y - point.y) * idirection.y;
            } else {
                tymin = (max.y - point.y) * idirection.y;
                tymax = (min.y - point.y) * idirection.y;
            }
            if((tmin > tymax) || (tmax < tymin))
                return 0;
            if(tmin < tymin)
                tmin = tymin;
            if(tmax > tymax)
                tmax = tymax;
            f32 tzmin,tzmax;
            if(idirection.z >= 0.0f) {
                tzmin = (min.z - point.z) * idirection.z;
                tzmax = (max.z - point.z) * idirection.z;
            } else {
                tzmin = (max.z - point.z) * idirection.z;
                tzmax = (min.z - point.z) * idirection.z;
            }
            if((tmin > tzmax) || (tmax < tzmin))
                return 0;
            if(tmin < tzmin)
                tmin = tzmin;
            if(tmax > tzmax)
                tmax = tzmax;
            return (tmax > 0.0f && tmin < 1.0f);
        }

        s32 irayBoundBoxIntersection(const DVec3 &point,const DVec3 &idirection,const DVec3 &min,const DVec3 &max) {
            f64 tmin,tmax;
            if(idirection.x >= 0.0) {
                tmin = (min.x - point.x) * idirection.x;
                tmax = (max.x - point.x) * idirection.x;
            } else {
                tmin = (max.x - point.x) * idirection.x;
                tmax = (min.x - point.x) * idirection.x;
            }
            f64 tymin,tymax;
            if(idirection.y >= 0.0) {
                tymin = (min.y - point.y) * idirection.y;
                tymax = (max.y - point.y) * idirection.y;
            } else {
                tymin = (max.y - point.y) * idirection.y;
                tymax = (min.y - point.y) * idirection.y;
            }
            if((tmin > tymax) || (tmax < tymin))
                return 0;
            if(tmin < tymin)
                tmin = tymin;
            if(tmax > tymax)
                tmax = tymax;
            f64 tzmin,tzmax;
            if(idirection.z >= 0.0) {
                tzmin = (min.z - point.z) * idirection.z;
                tzmax = (max.z - point.z) * idirection.z;
            } else {
                tzmin = (max.z - point.z) * idirection.z;
                tzmax = (min.z - point.z) * idirection.z;
            }
            if((tmin > tzmax) || (tmax < tzmin))
                return 0;
            if(tmin < tzmin)
                tmin = tzmin;
            if(tmax > tzmax)
                tmax = tzmax;
            return (tmax > 0.0 && tmin < 1.0);
        }
        //
        s32 rayTriangleIntersection(const FVec3 &point,const FVec3 &direction,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2) {
            FVec3 axis;
            FVec3 v10,v20,pv0;
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(axis,direction,v20);
            f32 det = dot(v10,axis);
            if(det > 0.0f) {
                sub(pv0,point,v0);
                f32 s = dot(pv0,axis);
                if(s < 0.0f || s > det){
                    return 0;
                }
                cross(axis,pv0,v10);
                f32 t = dot(direction,axis);
                if(t < 0.0f || t + s > det) {
                    return 0;
                }
                return 1;
            } else if(det < 0.0f) {
                sub(pv0,point,v0);
                f32 s = dot(pv0,axis);
                if(s > 0.0f || s < det) {
                    return 0;
                }
                cross(axis,pv0,v10);
                f32 t = dot(direction,axis);
                if(t > 0.0f || t + s < det) {
                    return 0;
                }
                return 1;
            }
            return 0;
        }
        //
        s32 rayTriangleIntersection(const DVec3 &point,const DVec3 &direction,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2) {
            DVec3 axis;
            DVec3 v10,v20,pv0;
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(axis,direction,v20);
            f64 det = dot(v10,axis);
            if(det > 0.0) {
                sub(pv0,point,v0);
                f64 s = dot(pv0,axis);
                if(s < 0.0 || s > det) {
                    return 0;
                }
                cross(axis,pv0,v10);
                f64 t = dot(direction,axis);
                if(t < 0.0 || t + s > det) {
                    return 0;
                }
                return 1;
            } else if(det < 0.0) {
                sub(pv0,point,v0);
                f64 s = dot(pv0,axis);
                if(s > 0.0 || s < det) {
                    return 0;
                }
                cross(axis,pv0,v10);
                f64 t = dot(direction,axis);
                if(t > 0.0 || t + s < det) {
                    return 0;
                }
                return 1;
            }
            return 0;
        }
        //
        s32 rayPlaneIntersection(FVec3 &ret,const FVec3 &point,const FVec3 &direction,const FVec4 &plane) {
            FVec3 t_plane_nor = FVec3(plane);
            f32 t_dis = plane.w;
            f32 t2 = dot(t_plane_nor, direction);
            if (t2 == 0)
                return 0;
            f32 t = -(dot(t_plane_nor,point)+t_dis)/t2;
            ret = point + (direction * t);
            return 1;
        }
        //
        s32 rayPlaneIntersection(DVec3 &ret,const DVec3 &point,const DVec3 &direction,const DVec3 &plane){
            DVec3 t_plane_nor = DVec3(plane);
            f64 t_dis = plane.w;
            f64 t2 = dot(t_plane_nor, direction);
            if (t2 == 0)
                return 0;
            f64 t = -(dot(t_plane_nor,point)+t_dis)/t2;
            ret = point + (direction * t);
            return 1;
        }
        //*******************************  Closest points *****************************************
        //
        void getClosestPointOnLine(const FVec3 &point,const FVec3 &p0,const FVec3 &p1,FVec3 &ret) {
            FVec3 direction;
            sub(direction,p1,p0);
            f32 d = length2(direction);
            if(d < EPSILON) {
                ret = p0;
                return;
            }
            FVec3 v0;
            f32 k = dot(direction,sub(v0,point,p0)) * Math::rcp(d);
            mad(ret,direction,saturate(k),p0);
        }

        void getClosestPointOnLine(const DVec3 &point,const DVec3 &p0,const DVec3 &p1,DVec3 &ret) {
            DVec3 direction;
            sub(direction,p1,p0);
            f64 d = length2(direction);
            if(d < EPSILON) {
                ret = p0;
                return;
            }
            DVec3 v0;
            f64 k = dot(direction,sub(v0,point,p0)) * Math::rcp(d);
            mad(ret,direction,saturate(k),p0);
        }

        //
        s32 getClosestPointOnTriangle(const FVec3 &point,const FVec3 &v0,const FVec3 &v1,const FVec3 &v2,FVec3 &ret) {
            FVec3 axis,normal;
            FVec3 v10,v20,pv0;
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(normal,v10,v20);
            cross(axis,normal,v20);
            f32 det = dot(v10,axis);
            if(det > 0.0f) {
                sub(pv0,point,v0);
                f32 s = dot(pv0,axis);
                if(s >= 0.0f && s <= det) {
                    cross(axis,pv0,v10);
                    f32 t = dot(normal,axis);
                    if(t >= 0.0f && t + s <= det) {
                        mad(ret,normal,dot(normal,v0) - dot(normal,point),point);
                        return 1;
                    }
                }
            } else if(det < 0.0f) {
                sub(pv0,point,v0);
                f32 s = dot(pv0,axis);
                if(s <= 0.0f && s >= det) {
                    cross(axis,pv0,v10);
                    f32 t = dot(normal,axis);
                    if(t <= 0.0f && t + s >= det) {
                        mad(ret,normal,dot(normal,v0) - dot(normal,point),point);
                        return 1;
                    }
                }
            }
            FVec3 v21,p0,p1;
            sub(v21,v2,v1);
            f32 k0 = saturate(dot(v10,sub(pv0,point,v0)) * Math::rcp(length2(v10)));
            f32 k1 = saturate(dot(v20,sub(pv0,point,v0)) * Math::rcp(length2(v20)));
            f32 k2 = saturate(dot(v21,sub(pv0,point,v1)) * Math::rcp(length2(v21)));
            k0 = length2(sub(pv0,point,mad(p0,v10,k0,v0)));
            k1 = length2(sub(pv0,point,mad(p1,v20,k1,v0)));
            k2 = length2(sub(pv0,point,mad(ret,v21,k2,v1)));
            if(k0 < k1) { if(k0 < k2) ret = FVec3(p0); }
            else { if(k1 < k2) ret = FVec3(p1); }
            return 0;
        }

        s32 getClosestPointOnTriangle(const DVec3 &point,const DVec3 &v0,const DVec3 &v1,const DVec3 &v2,DVec3 &ret) {
            DVec3 axis,normal;
            DVec3 v10,v20,pv0;
            sub(v10,v1,v0);
            sub(v20,v2,v0);
            cross(normal,v10,v20);
            cross(axis,normal,v20);
            f64 det = dot(v10,axis);
            if(det > 0.0) {
                sub(pv0,point,v0);
                f64 s = dot(pv0,axis);
                if(s >= 0.0 && s <= det) {
                    cross(axis,pv0,v10);
                    f64 t = dot(normal,axis);
                    if(t >= 0.0 && t + s <= det) {
                        mad(ret,normal,dot(normal,v0) - dot(normal,point),point);
                        return 1;
                    }
                }
            } else if(det < 0.0) {
                sub(pv0,point,v0);
                f64 s = dot(pv0,axis);
                if(s <= 0.0 && s >= det) {
                    cross(axis,pv0,v10);
                    f64 t = dot(normal,axis);
                    if(t <= 0.0 && t + s >= det) {
                        mad(ret,normal,dot(normal,v0) - dot(normal,point),point);
                        return 1;
                    }
                }
            }
            DVec3 v21,p0,p1;
            sub(v21,v2,v1);
            f64 k0 = saturate(dot(v10,sub(pv0,point,v0)) * Math::rcp(length2(v10)));
            f64 k1 = saturate(dot(v20,sub(pv0,point,v0)) * Math::rcp(length2(v20)));
            f64 k2 = saturate(dot(v21,sub(pv0,point,v1)) * Math::rcp(length2(v21)));
            k0 = length2(sub(pv0,point,mad(p0,v10,k0,v0)));
            k1 = length2(sub(pv0,point,mad(p1,v20,k1,v0)));
            k2 = length2(sub(pv0,point,mad(ret,v21,k2,v1)));
            if(k0 < k1) { if(k0 < k2) ret = DVec3(p0); }
            else { if(k1 < k2) ret = DVec3(p1); }
            return 0;
        }

        //
        s32 getClosestPointsOnLines(const FVec3 &p00,const FVec3 &p01,const FVec3 &p10,const FVec3 &p11,FVec3 &ret_0,FVec3 &ret_1) {
            if(p00 == p01) {
                ret_0 = p00;
                getClosestPointOnLine(p00,p10,p11,ret_1);
                return 0;
            }
            if(p10 == p11) {
                ret_1 = p10;
                getClosestPointOnLine(p10,p00,p01,ret_0);
                return 0;
            }
            FVec3 v0,v1,v2;
            sub(v0,p01,p00);
            sub(v1,p11,p10);
            sub(v2,p10,p00);
            f32 d20 = dot(v2,v0);
            f32 d21 = dot(v2,v1);
            if(d20 < EPSILON && d21 > -EPSILON) {
                ret_0 = p00;
                ret_1 = p10;
                return 0;
            }
            FVec3 v3;
            sub(v3,p11,p01);
            f32 d30 = dot(v3,v0);
            f32 d31 = dot(v3,v1);
            if(d30 > -EPSILON && d31 < EPSILON) {
                ret_0 = p01;
                ret_1 = p11;
                return 0;
            }
            FVec3 v4;
            sub(v4,p11,p00);
            f32 d40 = dot(v4,v0);
            f32 d41 = dot(v4,v1);
            if(d40 < EPSILON && d41 < EPSILON) {
                ret_0 = p00;
                ret_1 = p11;
                return 0;
            }
            FVec3 v5;
            sub(v5,p10,p01);
            f32 d50 = dot(v5,v0);
            f32 d51 = dot(v5,v1);
            if(d50 > -EPSILON && d51 > -EPSILON) {
                ret_0 = p01;
                ret_1 = p10;
                return 0;
            }
            FVec3 v6;
            f32 d00 = dot(v0,v0);
            if(d20 > -EPSILON && d50 < EPSILON) {
                f32 k = d20 / d00;
                if(dot(mad(v6,v0,-k,v2),v1) > -EPSILON) {
                    mad(ret_0,v0,k,p00);
                    ret_1 = p10;
                    return 0;
                }
            }
            if(d40 > -EPSILON && d30 < EPSILON) {
                f32 k = d40 / d00;
                if(dot(mad(v6,v0,-k,v4),v1) < EPSILON) {
                    mad(ret_0,v0,k,p00);
                    ret_1 = p11;
                    return 0;
                }
            }
            f32 d11 = dot(v1,v1);
            if(d21 < EPSILON && d41 > -EPSILON) {
                f32 k = -d21 / d11;
                if(dot(mad(v6,v1,k,v2),v0) < EPSILON) {
                    ret_0 = p00;
                    mad(ret_1,v1,k,p10);
                    return 0;
                }
            }
            if(d51 < EPSILON && d31 > -EPSILON) {
                f32 k = -d51 / d11;
                if(dot(mad(v6,v1,k,v5),v0) > -EPSILON) {
                    ret_0 = p01;
                    mad(ret_1,v1,k,p10);
                    return 0;
                }
            }
            f32 d10 = dot(v1,v0);
            f32 det = d10 * d10 - d00 * d11;
            if(Math::abs(det) < EPSILON) {
                f32 distance = INFINITY;
                f32 d2 = length2(v2);
                if(distance > d2) {
                    distance = d2;
                    ret_0 = p00;
                    ret_1 = p10;
                }
                f32 d3 = length2(v3);
                if(distance > d3) {
                    distance = d3;
                    ret_0 = p01;
                    ret_1 = p11;
                }
                f32 d4 = length2(v4);
                if(distance > d4) {
                    distance = d4;
                    ret_0 = p00;
                    ret_1 = p11;
                }
                f32 d5 = length2(v5);
                if(distance > d5) {
                    distance = d5;
                    ret_0 = p01;
                    ret_1 = p10;
                }
                return 0;
            }
            f32 idet = Math::rcp(det);
            mad(ret_0,v0,saturate((d10 * d21 - d11 * d20) * idet),p00);
            mad(ret_1,v1,saturate((d00 * d21 - d10 * d20) * idet),p10);
            return 1;
        }

        s32 getClosestPointsOnLines(const DVec3 &p00,const DVec3 &p01,const DVec3 &p10,const DVec3 &p11,DVec3 &ret_0,DVec3 &ret_1) {
            if(p00 == p01) {
                ret_0 = p00;
                getClosestPointOnLine(p00,p10,p11,ret_1);
                return 0;
            }
            if(p10 == p11) {
                ret_1 = p10;
                getClosestPointOnLine(p10,p00,p01,ret_0);
                return 0;
            }
            DVec3 v0,v1,v2;
            sub(v0,p01,p00);
            sub(v1,p11,p10);
            sub(v2,p10,p00);
            f64 d20 = dot(v2,v0);
            f64 d21 = dot(v2,v1);
            if(d20 < EPSILON && d21 > -EPSILON) {
                ret_0 = p00;
                ret_1 = p10;
                return 0;
            }
            DVec3 v3;
            sub(v3,p11,p01);
            f64 d30 = dot(v3,v0);
            f64 d31 = dot(v3,v1);
            if(d30 > -EPSILON && d31 < EPSILON) {
                ret_0 = p01;
                ret_1 = p11;
                return 0;
            }
            DVec3 v4;
            sub(v4,p11,p00);
            f64 d40 = dot(v4,v0);
            f64 d41 = dot(v4,v1);
            if(d40 < EPSILON && d41 < EPSILON) {
                ret_0 = p00;
                ret_1 = p11;
                return 0;
            }
            DVec3 v5;
            sub(v5,p10,p01);
            f64 d50 = dot(v5,v0);
            f64 d51 = dot(v5,v1);
            if(d50 > -EPSILON && d51 > -EPSILON) {
                ret_0 = p01;
                ret_1 = p10;
                return 0;
            }
            DVec3 v6;
            f64 d00 = dot(v0,v0);
            if(d20 > -EPSILON && d50 < EPSILON) {
                f64 k = d20 / d00;
                if(dot(mad(v6,v0,-k,v2),v1) > -EPSILON) {
                    mad(ret_0,v0,k,p00);
                    ret_1 = p10;
                    return 0;
                }
            }
            if(d40 > -EPSILON && d30 < EPSILON) {
                f64 k = d40 / d00;
                if(dot(mad(v6,v0,-k,v4),v1) < EPSILON) {
                    mad(ret_0,v0,k,p00);
                    ret_1 = p11;
                    return 0;
                }
            }
            f64 d11 = dot(v1,v1);
            if(d21 < EPSILON && d41 > -EPSILON) {
                f64 k = -d21 / d11;
                if(dot(mad(v6,v1,k,v2),v0) < EPSILON) {
                    ret_0 = p00;
                    mad(ret_1,v1,k,p10);
                    return 0;
                }
            }
            if(d51 < EPSILON && d31 > -EPSILON) {
                f64 k = -d51 / d11;
                if(dot(mad(v6,v1,k,v5),v0) > -EPSILON) {
                    ret_0 = p01;
                    mad(ret_1,v1,k,p10);
                    return 0;
                }
            }
            f64 d10 = dot(v1,v0);
            f64 det = d10 * d10 - d00 * d11;
            if(Math::abs(det) < EPSILON) {
                f64 distance = INFINITY;
                f64 d2 = length2(v2);
                if(distance > d2) {
                    distance = d2;
                    ret_0 = p00;
                    ret_1 = p10;
                }
                f64 d3 = length2(v3);
                if(distance > d3) {
                    distance = d3;
                    ret_0 = p01;
                    ret_1 = p11;
                }
                f64 d4 = length2(v4);
                if(distance > d4) {
                    distance = d4;
                    ret_0 = p00;
                    ret_1 = p11;
                }
                f64 d5 = length2(v5);
                if(distance > d5) {
                    distance = d5;
                    ret_0 = p01;
                    ret_1 = p10;
                }
                return 0;
            }
            f64 idet = Math::rcp(det);
            mad(ret_0,v0,saturate((d10 * d21 - d11 * d20) * idet),p00);
            mad(ret_1,v1,saturate((d00 * d21 - d10 * d20) * idet),p10);
            return 1;
        }
        
        s32 getTwoLinesIntersection(const FVec2 &p0, const FVec2 &p1, const FVec2 &p2, const FVec2 &p3){
            //            a p0
            //            b p1
            //            c p2
            //            d p3
            //快速排斥实验
            if (max(p2.x,p3.x)<min(p0.x,p1.x) || max(p0.x,p1.x)<min(p2.x,p3.x) ||max(p2.y,p3.y)<min(p0.y,p1.y) || max(p0.y,p1.y)<min(p2.y,p3.y)) {
                return 0 ;
            }
            //跨立实验
            if (cross(p0-p3,p2-p3)*cross(p1-p3,p2-p3)>0 ||cross(p3-p1,p0-p1)*cross(p2-p1,p0-p1)>0) {
                return 0;
            }
            return 1;
        }
    } //!namespace util
    
}//!namespace sv
