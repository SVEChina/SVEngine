//
// SVglTF.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#ifndef SVglTF_h
#define SVglTF_h
#include "../base/SVVec2.h"
#include "../base/SVVec3.h"
#include "../base/SVVec4.h"
#include "../base/SVMat2.h"
#include "../base/SVMat3.h"
#include "../base/SVMat4.h"
#include "../base/svstr.h"
#include "../base/SVBounds.h"
#ifndef SVGLTF_EMBREE
#define SVGLTF_EMBREE 1
#endif

#ifndef SVGLTF_QUADS_AS_TRIANGLES
#define SVGLTF_QUADS_AS_TRIANGLES 1
#endif

// -----------------------------------------------------------------------------
// INCLUDES
// -----------------------------------------------------------------------------

#include <algorithm>  // for std::upper_bound
#include <array>
#include <atomic>
#include <cctype>
#include <cfloat>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>  // for std::hash
#include <string>
#include <vector>

// -----------------------------------------------------------------------------
// MATH CONSTANTS AND FUNCTIONS
// -----------------------------------------------------------------------------
namespace sv {
    namespace util{
        /*
        // -----------------------------------------------------------------------------
        // RIGID BODY TRANSFORMS/FRAMES
        // -----------------------------------------------------------------------------
        
        // Rigid frames stored as a column-major affine transform matrix.
        struct frame2f{
            FVec2 x;
            FVec2 y;
            FVec2 o;
        };
        struct frame3f {
            FVec3 x;
            FVec3 y;
            FVec3 z;
            FVec3 o;
        };
        
        // Indentity frames.
        const frame2f identity_frame2f = frame2f{FVec2(1, 0), FVec2(0, 1), FVec2(0, 0)};
        const frame3f identity_frame3f = frame3f{FVec3(1, 0, 0), FVec3(0, 1, 0), FVec3(0, 0, 1), FVec3(0, 0, 0)};
        
        // Frame construction from axis.
        sv_inline frame3f make_frame_fromz(const FVec3& _o, const FVec3& _v) {
            FVec3 z = normalize(_v);
            FVec3 x;
            normalize(orthogonal(x, z));
            FVec3 y = normalize(cross(z, x));
            return {x, y, z, _o};
        }
        sv_inline frame3f make_frame_fromzx(const FVec3& _o, const FVec3& _z, const FVec3& _x) {
            FVec3 z = normalize(_z);
            FVec3 x = orthonormalize(_x, z);
            FVec3 y = normalize(cross(z, x));
            return {x, y, z, _o};
        }
        
        // Frame to matrix conversion.
        template <typename T>
        sv_inline FMat3 frame_to_mat(const frame2f& a) {
            FMat3 ret;
            ret.m00 = a.x.x;
            return ret;
//            return {
//                {a.x.x, a.x.y, 0},
//                {a.y.x, a.y.y, 0},
//                {a.o.x, a.o.y, 1},
//            };
        }
        template <typename T>
        constexpr inline frame<T, 2> mat_to_frame(const mat<T, 3, 3>& a) {
            return {
                {a.x.x, a.x.y},
                {a.y.x, a.y.y},
                {a.z.x, a.z.y},
            };
        }
        template <typename T>
        constexpr inline mat<T, 4, 4> frame_to_mat(const frame<T, 3>& a) {
            return {
                {a.x.x, a.x.y, a.x.z, 0},
                {a.y.x, a.y.y, a.y.z, 0},
                {a.z.x, a.z.y, a.z.z, 0},
                {a.o.x, a.o.y, a.o.z, 1},
            };
        }
        template <typename T>
        constexpr inline frame<T, 3> mat_to_frame(const mat<T, 4, 4>& a) {
            return {
                {a.x.x, a.x.y, a.x.z},
                {a.y.x, a.y.y, a.y.z},
                {a.z.x, a.z.y, a.z.z},
                {a.w.x, a.w.y, a.w.z},
            };
        }
        
        // Frame comparisons.
        template <typename T>
        constexpr inline bool operator==(const frame<T, 2>& a, const frame<T, 2>& b) {
            return a.x == b.x && a.y == b.y && a.o == b.o;
        }
        template <typename T>
        constexpr inline bool operator!=(const frame<T, 2>& a, const frame<T, 2>& b) {
            return !(a == b);
        }
        template <typename T>
        constexpr inline bool operator==(const frame<T, 3>& a, const frame<T, 3>& b) {
            return a.x == b.x && a.y == b.y && a.z == b.z && a.o == b.o;
        }
        template <typename T>
        constexpr inline bool operator!=(const frame<T, 3>& a, const frame<T, 3>& b) {
            return !(a == b);
        }
        
        // Frame composition, equivalent to affine matrix product.
        template <typename T>
        constexpr inline frame<T, 2> operator*(
                                               const frame<T, 2>& a, const frame<T, 2>& b) {
            auto rot = mat<T, 2, 2>{a.x, a.y} * mat<T, 2, 2>{b.x, b.y};
            auto pos = mat<T, 2, 2>{a.x, a.y} * b.o + a.o;
            return {rot.x, rot.y, pos};
        }
        template <typename T>
        constexpr inline frame<T, 3> operator*(
                                               const frame<T, 3>& a, const frame<T, 3>& b) {
            auto rot = mat<T, 3, 3>{a.x, a.y, a.z} * mat<T, 3, 3>{b.x, b.y, b.z};
            auto pos = mat<T, 3, 3>{a.x, a.y, a.z} * b.o + a.o;
            return {rot.x, rot.y, rot.z, pos};
        }
        // Frame inverse, equivalent to rigid affine inverse.
        template <typename T>
        constexpr inline frame<T, 2> inverse(const frame<T, 2>& a, bool is_rigid = true) {
            auto minv = (is_rigid) ? transpose(mat<T, 2, 2>{a.x, a.y}) :
            inverse(mat<T, 2, 2>{a.x, a.y});
            return {minv.x, minv.y, -(minv * a.o)};
        }
        template <typename T>
        constexpr inline frame<T, 3> inverse(const frame<T, 3>& a, bool is_rigid = true) {
            auto minv = (is_rigid) ? transpose(mat<T, 3, 3>{a.x, a.y, a.z}) :
            inverse(mat<T, 3, 3>{a.x, a.y, a.z});
            return {minv.x, minv.y, minv.z, -(minv * a.o)};
        }
        
        
        
        // -----------------------------------------------------------------------------
        // RAYS
        // -----------------------------------------------------------------------------
        
        
        // Rays with origin, direction and min/max t value.
        template <typename T, int N>
        struct ray;
        
        // Rays with origin, direction and min/max t value.
        template <typename T>
        struct ray<T, 2> {
            vec<T, 2> o    = {0, 0};
            vec<T, 2> d    = {0, 1};
            T         tmin = 0;
            T         tmax = maxt<T>();
        };
        template <typename T>
        struct ray<T, 3> {
            vec<T, 3> o    = {0, 0, 0};
            vec<T, 3> d    = {0, 0, 1};
            T         tmin = 0;
            T         tmax = maxt<T>();
        };
        
        // Type aliases.
        using ray2f = ray<float, 2>;
        using ray3f = ray<float, 3>;
        
        // Construct a ray from direction or segments using a default epsilon.
        template <typename T, int N>
        constexpr inline ray<T, N> make_ray(
                                            const vec<T, N>& o, const vec<T, N>& d, T eps = 1e-4f) {
            return {o, d, eps, maxt<T>()};
        }
        template <typename T, int N>
        constexpr inline ray<T, N> make_segment(
                                                const vec<T, N>& p1, const vec<T, N>& p2, T eps = 1e-4f) {
            return {p1, normalize(p2 - p1), eps, length(p2 - p1) - 2 * eps};
        }
        
        
        
        // -----------------------------------------------------------------------------
        // TRANSFORMS
        // -----------------------------------------------------------------------------
        
        
        // Transforms points, vectors and directions by matrices.
        template <typename T>
        constexpr inline vec<T, 2> transform_point(
                                                   const mat<T, 3, 3>& a, const vec<T, 2>& b) {
            auto tvb = a * vec3f{b.x, b.y, 1};
            return vec<T, 2>{tvb.x, tvb.y} / tvb.z;
        }
        template <typename T>
        constexpr inline vec<T, 3> transform_point(
                                                   const mat<T, 4, 4>& a, const vec<T, 3>& b) {
            auto tvb = a * vec<T, 4>{b.x, b.y, b.z, 1};
            return vec<T, 3>{tvb.x, tvb.y, tvb.z} / tvb.w;
        }
        template <typename T>
        constexpr inline vec<T, 2> transform_vector(
                                                    const mat<T, 3, 3>& a, const vec<T, 2>& b) {
            auto tvb = a * vec<T, 3>{b.x, b.y, 0};
            return vec<T, 2>{tvb.x, tvb.y} / tvb.z;
        }
        template <typename T>
        constexpr inline vec<T, 3> transform_vector(
                                                    const mat<T, 3, 3>& a, const vec<T, 3>& b) {
            return a * b;
        }
        template <typename T>
        constexpr inline vec<T, 3> transform_vector(
                                                    const mat<T, 4, 4>& a, const vec<T, 3>& b) {
            auto tvb = a * vec4f{b.x, b.y, b.z, 0};
            return vec<T, 3>{tvb.x, tvb.y, tvb.z};
        }
        template <typename T, int N>
        constexpr inline vec<T, N> transform_direction(
                                                       const mat<T, 4, 4>& a, const vec<T, N>& b) {
            return normalize(transform_vector(a, b));
        }
        
        // Transforms points, vectors and directions by frames.
        template <typename T>
        constexpr inline vec<T, 2> transform_point(
                                                   const frame<T, 2>& a, const vec<T, 2>& b) {
            return a.x * b.x + a.y * b.y + a.o;
        }
        template <typename T>
        constexpr inline vec<T, 3> transform_point(
                                                   const frame<T, 3>& a, const vec<T, 3>& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.o;
        }
        template <typename T>
        constexpr inline vec<T, 2> transform_vector(
                                                    const frame<T, 2>& a, const vec<T, 2>& b) {
            return a.x * b.x + a.y * b.y;
        }
        template <typename T>
        constexpr inline vec<T, 3> transform_vector(
                                                    const frame<T, 3>& a, const vec<T, 3>& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }
        template <typename T, int N>
        constexpr inline vec<T, N> transform_direction(
                                                       const frame<T, N>& a, const vec<T, N>& b) {
            return normalize(transform_vector(a, b));
        }
        
        // Transforms rays and bounding boxes by matrices.
        template <typename T, int N>
        constexpr inline ray<T, N> transform_ray(
                                                 const frame<T, N>& a, const ray<T, N>& b) {
            return {transform_point(a, b.o), transform_vector(a, b.d), b.tmin, b.tmax};
        }
        template <typename T, int N>
        constexpr inline ray<T, N> transform_ray(
                                                 const mat<T, N + 1, N + 1>& a, const ray<T, N>& b) {
            return {transform_point(a, b.o), transform_vector(a, b.d), b.tmin, b.tmax};
        }
        template <typename T>
        constexpr inline bbox<T, 3> transform_bbox(
                                                   const frame<T, 3>& a, const bbox<T, 3>& b) {
            auto corners = {vec3f{b.min.x, b.min.y, b.min.z},
                vec3f{b.min.x, b.min.y, b.max.z}, vec3f{b.min.x, b.max.y, b.min.z},
                vec3f{b.min.x, b.max.y, b.max.z}, vec3f{b.max.x, b.min.y, b.min.z},
                vec3f{b.max.x, b.min.y, b.max.z}, vec3f{b.max.x, b.max.y, b.min.z},
                vec3f{b.max.x, b.max.y, b.max.z}};
            auto xformed = bbox<T, 3>();
            for (auto& corner : corners) xformed += transform_point(a, corner);
            return xformed;
        }
        template <typename T>
        constexpr inline bbox<T, 3> transform_bbox(
                                                   const mat<T, 4, 4>& a, const bbox<T, 3>& b) {
            auto corners = {vec3f{b.min.x, b.min.y, b.min.z},
                vec3f{b.min.x, b.min.y, b.max.z}, vec3f{b.min.x, b.max.y, b.min.z},
                vec3f{b.min.x, b.max.y, b.max.z}, vec3f{b.max.x, b.min.y, b.min.z},
                vec3f{b.max.x, b.min.y, b.max.z}, vec3f{b.max.x, b.max.y, b.min.z},
                vec3f{b.max.x, b.max.y, b.max.z}};
            auto xformed = bbox<T, 3>();
            for (auto& corner : corners) xformed += transform_point(a, corner);
            return xformed;
        }
        
        // Inverse transforms by frames, assuming they are rigid transforms.
        template <typename T>
        constexpr inline vec<T, 2> transform_point_inverse(
                                                           const frame<T, 2>& a, const vec<T, 2>& b) {
            return {dot(b - a.o, a.x), dot(b - a.o, a.y)};
        }
        template <typename T>
        constexpr inline vec3f transform_point_inverse(
                                                       const frame<T, 3>& a, const vec<T, 3>& b) {
            return {dot(b - a.o, a.x), dot(b - a.o, a.y), dot(b - a.o, a.z)};
        }
        template <typename T>
        constexpr inline vec<T, 2> transform_vector_inverse(
                                                            const frame<T, 2>& a, const vec<T, 2>& b) {
            return {dot(b, a.x), dot(b, a.y)};
        }
        template <typename T>
        constexpr inline vec3f transform_vector_inverse(
                                                        const frame<T, 3>& a, const vec<T, 3>& b) {
            return {dot(b, a.x), dot(b, a.y), dot(b, a.z)};
        }
        template <typename T, int N>
        constexpr inline vec3f transform_direction_inverse(
                                                           const frame<T, N>& a, const vec<T, N>& b) {
            return normalize(transform_vector_inverse(a, b));
        }
        template <typename T, int N>
        constexpr inline ray<T, N> transform_ray_inverse(
                                                         const frame<T, N>& a, const ray<T, N>& b) {
            return {transform_point_inverse(a, b.o),
                transform_direction_inverse(a, b.d), b.tmin, b.tmax};
        }
        template <typename T>
        constexpr inline bbox<T, 3> transform_bbox_inverse(
                                                           const frame<T, 3>& a, const bbox<T, 3>& b) {
            return transform_bbox(inverse(a), b);
        }
        
        // Translation, scaling and rotations transforms.
        template <typename T>
        constexpr inline frame<T, 3> translation_frame(const vec<T, 3>& a) {
            return {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, a};
        }
        template <typename T>
        constexpr inline frame<T, 3> scaling_frame(const vec<T, 3>& a) {
            return {{a.x, 0, 0}, {0, a.y, 0}, {0, 0, a.z}, {0, 0, 0}};
        }
        template <typename T>
        constexpr inline frame<T, 3> rotation_frame(const vec<T, 3>& axis, T angle) {
            auto s = sin(angle), c = cos(angle);
            auto vv = normalize(axis);
            return {{c + (1 - c) * vv.x * vv.x, (1 - c) * vv.x * vv.y + s * vv.z,
                (1 - c) * vv.x * vv.z - s * vv.y},
                {(1 - c) * vv.x * vv.y - s * vv.z, c + (1 - c) * vv.y * vv.y,
                    (1 - c) * vv.y * vv.z + s * vv.x},
                {(1 - c) * vv.x * vv.z + s * vv.y, (1 - c) * vv.y * vv.z - s * vv.x,
                    c + (1 - c) * vv.z * vv.z},
                {0, 0, 0}};
        }
        template <typename T>
        constexpr inline frame<T, 3> rotation_frame(const vec<T, 4>& quat) {
            auto v = quat;
            return {{v.w * v.w + v.x * v.x - v.y * v.y - v.z * v.z,
                (v.x * v.y + v.z * v.w) * 2, (v.z * v.x - v.y * v.w) * 2},
                {(v.x * v.y - v.z * v.w) * 2,
                    v.w * v.w - v.x * v.x + v.y * v.y - v.z * v.z,
                    (v.y * v.z + v.x * v.w) * 2},
                {(v.z * v.x + v.y * v.w) * 2, (v.y * v.z - v.x * v.w) * 2,
                    v.w * v.w - v.x * v.x - v.y * v.y + v.z * v.z},
                {0, 0, 0}};
        }
        template <typename T>
        constexpr inline frame<T, 3> rotation_frame(const mat<T, 3, 3>& rot) {
            return {rot.x, rot.y, rot.z, {0, 0, 0}};
        }
        
        // Lookat frame. Z-axis can be inverted with inv_xz.
        template <typename T>
        constexpr inline frame<T, 3> lookat_frame(const vec<T, 3>& eye,
                                                  const vec<T, 3>& center, const vec<T, 3>& up, bool inv_xz = false) {
            auto w = normalize(eye - center);
            auto u = normalize(cross(up, w));
            auto v = normalize(cross(w, u));
            if (inv_xz) {
                w = -w;
                u = -u;
            }
            return {u, v, w, eye};
        }
        
        // OpenGL frustum, ortho and perspecgive matrices.
        template <typename T>
        constexpr inline mat<T, 4, 4> frustum_mat(T l, T r, T b, T t, T n, T f) {
            return {{2 * n / (r - l), 0, 0, 0}, {0, 2 * n / (t - b), 0, 0},
                {(r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1},
                {0, 0, -2 * f * n / (f - n), 0}};
        }
        template <typename T>
        constexpr inline mat<T, 4, 4> orthographic_mat(T l, T r, T b, T t, T n, T f) {
            return {{2 / (r - l), 0, 0, 0}, {0, 2 / (t - b), 0, 0},
                {0, 0, -2 / (f - n), 0},
                {-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1}};
        }
        template <typename T>
        constexpr inline mat<T, 4, 4> orthographic2d_mat(
                                                         T left, T right, T bottom, T top) {
            return orthographic_mat(left, right, bottom, top, -1, 1);
        }
        template <typename T>
        constexpr inline mat<T, 4, 4> orthographic_mat(T xmag, T ymag, T near, T far) {
            return {{1 / xmag, 0, 0, 0}, {0, 1 / ymag, 0, 0},
                {0, 0, 2 / (near - far), 0}, {0, 0, (far + near) / (near - far), 1}};
        }
        template <typename T>
        constexpr inline mat<T, 4, 4> perspective_mat(T fovy, T aspect, T near, T far) {
            auto tg = tan(fovy / 2);
            return {{1 / (aspect * tg), 0, 0, 0}, {0, 1 / tg, 0, 0},
                {0, 0, (far + near) / (near - far), -1},
                {0, 0, 2 * far * near / (near - far), 0}};
        }
        template <typename T>
        constexpr inline mat<T, 4, 4> perspective_mat(T fovy, T aspect, T near) {
            auto tg = tan(fovy / 2);
            return {{1 / (aspect * tg), 0, 0, 0}, {0, 1 / tg, 0, 0}, {0, 0, -1, -1},
                {0, 0, 2 * near, 0}};
        }
        
        // Rotation conversions.
        template <typename T>
        constexpr inline tuple<vec<T, 3>, T> rotation_axisangle(const vec<T, 4>& quat) {
            return {normalize(vec3f{quat.x, quat.y, quat.z}), 2 * acos(quat.w)};
        }
        template <typename T>
        constexpr inline vec<T, 4> rotation_quat(const vec<T, 3>& axis, T angle) {
            auto len = length(axis);
            if (!len) return {0, 0, 0, 1};
            return vec4f{sin(angle / 2) * axis.x / len, sin(angle / 2) * axis.y / len,
                sin(angle / 2) * axis.z / len, cos(angle / 2)};
        }
        template <typename T>
        constexpr inline vec<T, 4> rotation_quat(const vec<T, 4>& axisangle) {
            return rotation_quat(
                                 vec3f{axisangle.x, axisangle.y, axisangle.z}, axisangle.w);
        }
        
        // Turntable and FPS Camera navigation.
        inline void camera_turntable(vec3f& from, vec3f& to, vec3f& up,
                                     const vec2f& rotate, float dolly, const vec2f& pan);
        inline void camera_turntable(frame3f& frame, float& focus, const vec2f& rotate,
                                     float dolly, const vec2f& pan);
        inline void camera_fps(frame3f& frame, const vec3f& transl, const vec2f& rotate);
        
        // Computes the image uv coordinates corresponding to the view parameters.
        // Returns negative coordinates if out of the image.
        inline vec2i get_image_coords(const vec2f& mouse_pos, const vec2f& center,
                                      float scale, const vec2i& txt_size) {
            auto xyf = (mouse_pos - center) / scale;
            return vec2i{(int)round(xyf.x + txt_size.x / 2.0f),
                (int)round(xyf.y + txt_size.y / 2.0f)};
        }
        
        // Center image and autofit.
        inline void center_image(vec2f& center, float& scale, const vec2i& imsize,
                                 const vec2i& winsize, bool zoom_to_fit) {
            if (zoom_to_fit) {
                scale  = min(winsize.x / (float)imsize.x, winsize.y / (float)imsize.y);
                center = {(float)winsize.x / 2, (float)winsize.y / 2};
            } else {
                if (winsize.x >= imsize.x * scale) center.x = winsize.x / 2;
                if (winsize.y >= imsize.y * scale) center.y = winsize.y / 2;
            }
        }
        
        
        // -----------------------------------------------------------------------------
        // MONETACARLO SAMPLING FUNCTIONS
        // -----------------------------------------------------------------------------
        
        
        // Sample an hemispherical direction with uniform distribution.
        inline vec3f sample_hemisphere_direction(const vec2f& ruv) {
            auto z   = ruv.y;
            auto r   = sqrt(1 - z * z);
            auto phi = 2 * pif * ruv.x;
            return {r * cos(phi), r * sin(phi), z};
        }
        inline float sample_hemisphere_direction_pdf(const vec3f& direction) {
            return (direction.z <= 0) ? 0 : 1 / (2 * pif);
        }
        
        // Sample a spherical direction with uniform distribution.
        inline vec3f sample_sphere_direction(const vec2f& ruv) {
            auto z   = 2 * ruv.y - 1;
            auto r   = sqrt(1 - z * z);
            auto phi = 2 * pif * ruv.x;
            return {r * cos(phi), r * sin(phi), z};
        }
        inline float sample_sphere_direction_pdf(const vec3f& w) {
            return 1 / (4 * pif);
        }
        
        // Sample an hemispherical direction with cosine distribution.
        inline vec3f sample_hemisphere_direction_cosine(const vec2f& ruv) {
            auto z   = sqrt(ruv.y);
            auto r   = sqrt(1 - z * z);
            auto phi = 2 * pif * ruv.x;
            return {r * cos(phi), r * sin(phi), z};
        }
        inline float sample_hemisphere_direction_cosine_pdf(const vec3f& direction) {
            return (direction.z <= 0) ? 0 : direction.z / pif;
        }
        
        // Sample an hemispherical direction with cosine power distribution.
        inline vec3f sample_hemisphere_direction_cospower(
                                                          float exponent, const vec2f& ruv) {
            auto z   = pow(ruv.y, 1 / (exponent + 1));
            auto r   = sqrt(1 - z * z);
            auto phi = 2 * pif * ruv.x;
            return {r * cos(phi), r * sin(phi), z};
        }
        inline float sample_hemisphere_direction_cospower_pdf(
                                                              float exponent, const vec3f& direction) {
            return (direction.z <= 0) ?
            0 :
            pow(direction.z, exponent) * (exponent + 1) / (2 * pif);
        }
        
        // Sample a point uniformly on a disk.
        inline vec3f sample_disk_point(const vec2f& ruv) {
            auto r   = sqrt(ruv.y);
            auto phi = 2 * pif * ruv.x;
            return {cos(phi) * r, sin(phi) * r, 0};
        }
        inline float sample_disk_point_pdf() { return 1 / pif; }
        
        // Sample a point uniformly on a cylinder, without caps.
        inline vec3f sample_cylinder_point(const vec2f& ruv) {
            auto phi = 2 * pif * ruv.x;
            return {sin(phi), cos(phi), ruv.y * 2 - 1};
        }
        inline float sample_cylinder_point_pdf() { return 1 / pif; }
        
        // Sample a point uniformly on a triangle returning the baricentric coordinates.
        inline vec2f sample_triangle_coordinates(const vec2f& ruv) {
            return {1 - sqrt(ruv.x), ruv.y * sqrt(ruv.x)};
        }
        
        // Sample a point uniformly on a triangle.
        inline vec3f sample_triangle_point(
                                           const vec3f& p0, const vec3f& p1, const vec3f& p2, const vec2f& ruv) {
            auto uv = sample_triangle_coordinates(ruv);
            return p0 * (1 - uv.x - uv.y) + p1 * uv.x + p2 * uv.y;
        }
        // Pdf for uniform triangle sampling, i.e. triangle area.
        inline float sample_triangle_point_pdf(
                                               const vec3f& p0, const vec3f& p1, const vec3f& p2) {
            return 2 / length(cross(p1 - p0, p2 - p0));
        }
        
        // Sample an index with uniform distribution.
        inline int sample_uniform_index(int size, float r) {
            return clamp((int)(r * size), 0, size - 1);
        }
        inline float sample_uniform_index_pdf(int size) { return 1.0f / size; }
        
        // Sample an index with uniform distribution.
        template <typename T>
        inline T sample_uniform_element(const vector<T>& elements, float r) {
            if (elements.empty()) return {};
            auto size = (int)elements.size();
            return elements[clamp((int)(r * size), 0, size - 1)];
        }
        template <typename T>
        inline float sample_uniform_element_pdf(const vector<T>& elements) {
            if (elements.empty()) return 0;
            return 1.0f / (int)elements.size();
        }
        
        // Sample a discrete distribution represented by its cdf.
        inline int sample_discrete_distribution(const vector<float>& cdf, float r) {
            r        = clamp(r * cdf.back(), 0.0f, cdf.back() - 0.00001f);
            auto idx = (int)(std::upper_bound(cdf.data(), cdf.data() + cdf.size(), r) -
                             cdf.data());
            return clamp(idx, 0, (int)cdf.size() - 1);
        }
        // Pdf for uniform discrete distribution sampling.
        inline float sample_discrete_distribution_pdf(const vector<float>& cdf, int idx) {
            if (idx == 0) return cdf.at(0);
            return cdf.at(idx) - cdf.at(idx - 1);
        }
        
        
        
        // -----------------------------------------------------------------------------
        // PERLIN NOISE FUNCTION
        // -----------------------------------------------------------------------------
        
        
        // Compute the revised Perlin noise function. Wrap provides a wrapping noise
        // but must be power of two (wraps at 256 anyway). For octave based noise,
        // good values are obtained with octaves=6 (numerber of noise calls),
        // lacunarity=~2.0 (spacing between successive octaves: 2.0 for warpping
        // output), gain=0.5 (relative weighting applied to each successive octave),
        // offset=1.0 (used to invert the ridges).
        float perlin_noise(const vec3f& p, const vec3i& wrap = zero3i);
        float perlin_ridge_noise(const vec3f& p, float lacunarity = 2.0f,
                                 float gain = 0.5f, float offset = 1.0f, int octaves = 6,
                                 const vec3i& wrap = zero3i);
        float perlin_fbm_noise(const vec3f& p, float lacunarity = 2.0f,
                               float gain = 0.5f, int octaves = 6, const vec3i& wrap = zero3i);
        float perlin_turbulence_noise(const vec3f& p, float lacunarity = 2.0f,
                                      float gain = 0.5f, int octaves = 6, const vec3i& wrap = zero3i);
        
        
        
        // -----------------------------------------------------------------------------
        // GEOMETRY UTILITIES
        // -----------------------------------------------------------------------------
        
        
        // Line properties.
        inline vec3f line_tangent(const vec3f& p0, const vec3f& p1) {
            return normalize(p1 - p0);
        }
        inline float line_length(const vec3f& p0, const vec3f& p1) {
            return length(p1 - p0);
        }
        
        // Triangle properties.
        inline vec3f triangle_normal(const vec3f& p0, const vec3f& p1, const vec3f& p2) {
            return normalize(cross(p1 - p0, p2 - p0));
        }
        inline float triangle_area(const vec3f& p0, const vec3f& p1, const vec3f& p2) {
            return length(cross(p1 - p0, p2 - p0)) / 2;
        }
        
        // Quad propeties.
        inline vec3f quad_normal(
                                 const vec3f& p0, const vec3f& p1, const vec3f& p2, const vec3f& p3) {
            return normalize(triangle_normal(p0, p1, p3) + triangle_normal(p2, p3, p1));
        }
        inline float quad_area(
                               const vec3f& p0, const vec3f& p1, const vec3f& p2, const vec3f& p3) {
            return triangle_area(p0, p1, p3) + triangle_area(p2, p3, p1);
        }
        
        // Triangle tangent and bitangent from uv
        inline tuple<vec3f, vec3f> triangle_tangents_fromuv(const vec3f& p0,
                                                            const vec3f& p1, const vec3f& p2, const vec2f& uv0, const vec2f& uv1,
                                                            const vec2f& uv2) {
            // Follows the definition in http://www.terathon.com/code/tangent.html and
            // https://gist.github.com/aras-p/2843984
            // normal points up from texture space
            auto p   = p1 - p0;
            auto q   = p2 - p0;
            auto s   = vec2f{uv1.x - uv0.x, uv2.x - uv0.x};
            auto t   = vec2f{uv1.y - uv0.y, uv2.y - uv0.y};
            auto div = s.x * t.y - s.y * t.x;
            
            if (div != 0) {
                auto tu = vec3f{t.y * p.x - t.x * q.x, t.y * p.y - t.x * q.y,
                    t.y * p.z - t.x * q.z} /
                div;
                auto tv = vec3f{s.x * q.x - s.y * p.x, s.x * q.y - s.y * p.y,
                    s.x * q.z - s.y * p.z} /
                div;
                return {tu, tv};
            } else {
                return {{1, 0, 0}, {0, 1, 0}};
            }
        }
        
        // Interpolates values over a line parameterized from a to b by u. Same as lerp.
        template <typename T>
        inline T interpolate_line(const T& p0, const T& p1, float u) {
            return p0 * (1 - u) + p1 * u;
        }
        // Interpolates values over a triangle parameterized by u and v along the
        // (p1-p0) and (p2-p0) directions. Same as barycentric interpolation.
        template <typename T>
        inline T interpolate_triangle(
                                      const T& p0, const T& p1, const T& p2, const vec2f& uv) {
            return p0 * (1 - uv.x - uv.y) + p1 * uv.x + p2 * uv.y;
        }
        // Interpolates values over a quad parameterized by u and v along the
        // (p1-p0) and (p2-p1) directions. Same as bilinear interpolation.
        template <typename T>
        inline T interpolate_quad(
                                  const T& p0, const T& p1, const T& p2, const T& p3, const vec2f& uv) {
#if YGL_QUADS_AS_TRIANGLES
            if (uv.x + uv.y <= 1) {
                return interpolate_triangle(p0, p1, p3, uv);
            } else {
                return interpolate_triangle(p2, p3, p1, 1 - uv);
            }
#else
            return p0 * (1 - uv.x) * (1 - uv.y) + p1 * uv.x * (1 - uv.y) +
            p2 * uv.x * uv.y + p3 * (1 - uv.x) * uv.y;
#endif
        }
        
        // Interpolates values along a cubic Bezier segment parametrized by u.
        template <typename T>
        inline T interpolate_bezier(
                                    const T& p0, const T& p1, const T& p2, const T& p3, float u) {
            return p0 * (1 - u) * (1 - u) * (1 - u) + p1 * 3 * u * (1 - u) * (1 - u) +
            p2 * 3 * u * u * (1 - u) + p3 * u * u * u;
        }
        // Computes the derivative of a cubic Bezier segment parametrized by u.
        template <typename T>
        inline T interpolate_bezier_derivative(
                                               const T& p0, const T& p1, const T& p2, const T& p3, float u) {
            return (p1 - p0) * 3 * (1 - u) * (1 - u) + (p2 - p1) * 6 * u * (1 - u) +
            (p3 - p2) * 3 * u * u;
        }
        
        
        
        // -----------------------------------------------------------------------------
        // SHAPE UTILITIES
        // -----------------------------------------------------------------------------
        
        
        // Compute per-vertex normals/tangents for lines/triangles/quads.
        vector<vec3f> compute_vertex_tangents(
                                              const vector<vec2i>& lines, const vector<vec3f>& positions);
        vector<vec3f> compute_vertex_normals(
                                             const vector<vec3i>& triangles, const vector<vec3f>& positions);
        vector<vec3f> compute_vertex_normals(
                                             const vector<vec4i>& quads, const vector<vec3f>& positions);
        
        // Compute per-vertex tangent space for triangle meshes.
        // Tangent space is defined by a four component vector.
        // The first three components are the tangent with respect to the u texcoord.
        // The fourth component is the sign of the tangent wrt the v texcoord.
        // Tangent frame is useful in normal mapping.
        vector<vec4f> compute_tangent_spaces(const vector<vec3i>& triangles,
                                             const vector<vec3f>& positions, const vector<vec3f>& normals,
                                             const vector<vec2f>& texturecoords);
        
        // Apply skinning to vertex position and normals.
        tuple<vector<vec3f>, vector<vec3f>> compute_skinning(
                                                             const vector<vec3f>& positions, const vector<vec3f>& normals,
                                                             const vector<vec4f>& weights, const vector<vec4i>& joints,
                                                             const vector<frame3f>& xforms);
        // Apply skinning as specified in Khronos glTF.
        tuple<vector<vec3f>, vector<vec3f>> compute_matrix_skinning(
                                                                    const vector<vec3f>& positions, const vector<vec3f>& normals,
                                                                    const vector<vec4f>& weights, const vector<vec4i>& joints,
                                                                    const vector<mat4f>& xforms);
        
        // Dictionary to store edge information.
        using edge_map = unordered_map<vec2i, vec2i>;
        
        // Initialize an edge map with elements.
        edge_map make_edge_map(const vector<vec3i>& triangles);
        edge_map make_edge_map(const vector<vec4i>& quads);
        // Insert an edge and return its index
        int insert_edge(edge_map& emap, const vec2i& edge);
        // Get the edge index / insertion count
        int get_edge_index(const edge_map& emap, const vec2i& edge);
        int get_edge_count(const edge_map& emap, const vec2i& edge);
        // Get list of edges / boundary edges
        vector<vec2i> get_edges(const edge_map& emap);
        vector<vec2i> get_boundary(const edge_map& emap);
        
        // Create an array of edges.
        inline vector<vec2i> get_edges(const vector<vec3i>& triangles) {
            return get_edges(make_edge_map(triangles));
        }
        inline vector<vec2i> get_edges(const vector<vec4i>& quads) {
            return get_edges(make_edge_map(quads));
        }
        
        // Convert quads to triangles
        vector<vec3i> convert_quads_to_triangles(const vector<vec4i>& quads);
        // Convert quads to triangles with a diamond-like topology.
        // Quads have to be consecutive one row after another.
        vector<vec3i> convert_quads_to_triangles(
                                                 const vector<vec4i>& quads, int row_length);
        // Convert triangles to quads by creating degenerate quads
        vector<vec4i> convert_triangles_to_quads(const vector<vec3i>& triangles);
        
        // Convert beziers to lines using 3 lines for each bezier.
        vector<vec2i> convert_bezier_to_lines(const vector<vec4i>& beziers);
        
        // Convert face-varying data to single primitives. Returns the quads indices
        // and face ids and filled vectors for pos, norm and texcoord.
        tuple<vector<vec4i>, vector<vec3f>, vector<vec3f>, vector<vec2f>> convert_face_varying(
                                                                                               const vector<vec4i>& quads_positions, const vector<vec4i>& quads_normals,
                                                                                               const vector<vec4i>& quads_texturecoords, const vector<vec3f>& positions,
                                                                                               const vector<vec3f>& normals, const vector<vec2f>& texturecoords);
        
        // Subdivide lines by splitting each line in half.
        template <typename T>
        tuple<vector<vec2i>, vector<T>> subdivide_lines(
                                                        const vector<vec2i>& lines, const vector<T>& vert);
        // Subdivide triangle by splitting each triangle in four, creating new
        // vertices for each edge.
        template <typename T>
        tuple<vector<vec3i>, vector<T>> subdivide_triangles(
                                                            const vector<vec3i>& triangles, const vector<T>& vert);
        // Subdivide quads by splitting each quads in four, creating new
        // vertices for each edge and for each face.
        template <typename T>
        tuple<vector<vec4i>, vector<T>> subdivide_quads(
                                                        const vector<vec4i>& quads, const vector<T>& vert);
        // Subdivide beziers by splitting each segment in two.
        template <typename T>
        tuple<vector<vec4i>, vector<T>> subdivide_beziers(
                                                          const vector<vec4i>& beziers, const vector<T>& vert);
        // Subdivide quads using Carmull-Clark subdivision rules.
        template <typename T>
        tuple<vector<vec4i>, vector<T>> subdivide_catmullclark(const vector<vec4i>& quads,
                                                               const vector<T>& vert, bool lock_boundary = false);
        
        // Weld vertices within a threshold. For noe the implementation is O(n^2).
        tuple<vector<vec3f>, vector<int>> weld_vertices(
                                                        const vector<vec3f>& positions, float threshold);
        tuple<vector<vec3i>, vector<vec3f>> weld_triangles(const vector<vec3i>& triangles,
                                                           const vector<vec3f>& positions, float threshold);
        tuple<vector<vec4i>, vector<vec3f>> weld_quads(const vector<vec4i>& quads,
                                                       const vector<vec3f>& positions, float threshold);
        
        // Pick a point in a point set uniformly.
        inline int sample_points_element(int npoints, float re) {
            return sample_uniform_index(npoints, re);
        }
        inline vector<float> sample_points_element_cdf(int npoints) {
            auto cdf = vector<float>(npoints);
            for (auto i = 0; i < cdf.size(); i++) cdf[i] = 1 + (i ? cdf[i - 1] : 0);
            return cdf;
        }
        inline int sample_points_element(const vector<float>& cdf, float re) {
            return sample_discrete_distribution(cdf, re);
        }
        
        // Pick a point on lines uniformly.
        inline vector<float> sample_lines_element_cdf(
                                                      const vector<vec2i>& lines, const vector<vec3f>& positions) {
            auto cdf = vector<float>(lines.size());
            for (auto i = 0; i < cdf.size(); i++) {
                auto l = lines[i];
                auto w = line_length(positions[l.x], positions[l.y]);
                cdf[i] = w + (i ? cdf[i - 1] : 0);
            }
            return cdf;
        }
        inline tuple<int, float> sample_lines_element(
                                                      const vector<float>& cdf, float re, float ru) {
            return {sample_discrete_distribution(cdf, re), ru};
        }
        
        // Pick a point on a triangle mesh uniformly.
        inline vector<float> sample_triangles_element_cdf(
                                                          const vector<vec3i>& triangles, const vector<vec3f>& positions) {
            auto cdf = vector<float>(triangles.size());
            for (auto i = 0; i < cdf.size(); i++) {
                auto t = triangles[i];
                auto w = triangle_area(positions[t.x], positions[t.y], positions[t.z]);
                cdf[i] = w + (i ? cdf[i - 1] : 0);
            }
            return cdf;
        }
        inline tuple<int, vec2f> sample_triangles_element(
                                                          const vector<float>& cdf, float re, const vec2f& ruv) {
            return {sample_discrete_distribution(cdf, re),
                sample_triangle_coordinates(ruv)};
        }
        
        // Pick a point on a quad mesh uniformly.
        inline vector<float> sample_quads_element_cdf(
                                                      const vector<vec4i>& quads, const vector<vec3f>& positions) {
            auto cdf = vector<float>(quads.size());
            for (auto i = 0; i < cdf.size(); i++) {
                auto q = quads[i];
                auto w = quad_area(
                                   positions[q.x], positions[q.y], positions[q.z], positions[q.w]);
                cdf[i] = w + (i ? cdf[i - 1] : 0);
            }
            return cdf;
        }
        inline tuple<int, vec2f> sample_quads_element(
                                                      const vector<float>& cdf, float re, const vec2f& ruv) {
            return {sample_discrete_distribution(cdf, re), ruv};
        }
        inline tuple<int, vec2f> sample_quads_element(const vector<vec4i>& quads,
                                                      const vector<float>& cdf, float re, const vec2f& ruv) {
            auto ei = sample_discrete_distribution(cdf, re);
            if (quads[ei].z == quads[ei].w) {
                return {ei, sample_triangle_coordinates(ruv)};
            } else {
                return {ei, ruv};
            }
        }
        
        // Samples a set of points over a triangle/quad mesh uniformly. Returns pos,
        // norm and texcoord of the sampled points.
        tuple<vector<vec3f>, vector<vec3f>, vector<vec2f>> sample_triangles_points(
                                                                                   const vector<vec3i>& triangles, const vector<vec3f>& positions,
                                                                                   const vector<vec3f>& normals, const vector<vec2f>& texturecoords,
                                                                                   int npoints, int seed = 7);
        tuple<vector<vec3f>, vector<vec3f>, vector<vec2f>> sample_quads_points(
                                                                               const vector<vec4i>& quads, const vector<vec3f>& positions,
                                                                               const vector<vec3f>& normals, const vector<vec2f>& texturecoords,
                                                                               int npoints, int seed = 7);
        
        
        // -----------------------------------------------------------------------------
        // RAY INTERSECTION AND CLOSEST POINT FUNCTIONS
        // -----------------------------------------------------------------------------
        
        
        // Intersect a ray with a point (approximate).
        // Based on http://geomalgorithms.com/a02-lines.html.
        bool intersect_point(
                             const ray3f& ray, const vec3f& p, float r, float& dist, vec2f& uv);
        
        // Intersect a ray with a line (approximate).
        // Based on http://geomalgorithms.com/a05-intersect-1.html and
        // http://geomalgorithms.com/a07-distance.html#
        //     dist3D_Segment_to_Segment
        bool intersect_line(const ray3f& ray, const vec3f& p0, const vec3f& p1,
                            float r0, float r1, float& dist, vec2f& uv);
        
        // Intersect a ray with a triangle.
        bool intersect_triangle(const ray3f& ray, const vec3f& p0, const vec3f& p1,
                                const vec3f& p2, float& dist, vec2f& uv);
        
        // Intersect a ray with a quad represented as two triangles (0,1,3) and
        // (2,3,1), with the uv coordinates of the second triangle corrected by u =
        // 1-u' and v = 1-v' to produce a quad parametrization where u and v go from 0
        // to 1. This is equivalent to Intel's Embree.
        bool intersect_quad(const ray3f& ray, const vec3f& p0, const vec3f& p1,
                            const vec3f& p2, const vec3f& p3, float& dist, vec2f& uv);
        
        // Intersect a ray with a axis-aligned bounding box.
        bool intersect_bbox(const ray3f& ray, const bbox3f& bbox);
        
        // Intersect a ray with a axis-aligned bounding box, implemented as
        // "Robust BVH Ray Traversal" by T. Ize published at
        // http://jcgt.org/published/0002/02/02/paper.pdf
        bool intersect_bbox(const ray3f& ray, const vec3f& ray_dinv,
                            const vec3i& ray_dsign, const bbox3f& bbox);
        
        // Check if a point overlaps a position within a max distance.
        bool overlap_point(const vec3f& pos, float dist_max, const vec3f& p0, float r0,
                           float& dist, vec2f& uv);
        
        // Find closest line point to a position.
        float closestuv_line(const vec3f& pos, const vec3f& p0, const vec3f& p1);
        
        // Check if a line overlaps a position within a max distance.
        bool overlap_line(const vec3f& pos, float dist_max, const vec3f& p0,
                          const vec3f& p1, float r0, float r1, float& dist, vec2f& uv);
        
        // Find closest triangle point to a position.
        vec2f closestuv_triangle(
                                 const vec3f& pos, const vec3f& p0, const vec3f& p1, const vec3f& p2);
        
        // Check if a triangle overlaps a position within a max distance.
        bool overlap_triangle(const vec3f& pos, float dist_max, const vec3f& p0,
                              const vec3f& p1, const vec3f& p2, float r0, float r1, float r2, float& dist,
                              vec2f& uv);
        
        // Check if a quad overlaps a position within a max distance.
        bool overlap_quad(const vec3f& pos, float dist_max, const vec3f& p0,
                          const vec3f& p1, const vec3f& p2, const vec3f& p3, float r0, float r1,
                          float r2, float r3, float& dist, vec2f& uv);
        
        // Check if a bounding box overlaps a position within a max distance.
        bool overlap_bbox(const vec3f& pos, float dist_max, const bbox3f& bbox);
        
        // Check if two bounding boxes overlap.
        bool overlap_bbox(const bbox3f& bbox1, const bbox3f& bbox2);
        
        
        
        // -----------------------------------------------------------------------------
        // BVH FOR RAY INTERSECTION AND CLOSEST ELEMENT
        // -----------------------------------------------------------------------------
        
        
        // Maximum number of primitives per BVH node.
        const int bvh_max_prims = 4;
        
        // BVH tree node containing its bounds, indices to the BVH arrays of either
        // primitives or internal nodes, the node element type,
        // and the split axis. Leaf and internal nodes are identical, except that
        // indices refer to primitives for leaf nodes or other nodes for internal
        // nodes. See bvh_tree for more details.
        struct bvh_node {
            bbox3f   bbox;
            uint16_t num_primitives;
            bool     is_internal;
            uint8_t  split_axis;
            uint32_t primitive_ids[bvh_max_prims];
        };
        
        // Instance for a scene BVH.
        struct bvh_instance {
            frame3f frame         = identity_frame3f;
            frame3f frame_inverse = identity_frame3f;
            int     shape_id      = -1;
        };
        
        // BVH tree, stored as a node array. The tree structure is encoded using array
        // indices instead of pointers, both for speed but also to simplify code.
        // BVH nodes indices refer to either the node array, for internal nodes,
        // or the primitive arrays, for leaf nodes. BVH trees may contain only one type
        // of geometric primitive, like points, lines, triangle or instances of other
        // BVHs. To handle multiple primitive types and transformed primitives, build
        // a two-level hierarchy with the outer BVH, the scene BVH, containing inner
        // BVHs, shape BVHs, each of which of a uniform primitive type.
        // To build a BVH, first fill in either the shape or instance data, then
        // call `build_bvh()`.
        struct bvh_tree {
            // data for shape BVH
            vector<vec3f> positions;
            vector<float> radius;
            vector<int>   points;
            vector<vec2i> lines;
            vector<vec3i> triangles;
            vector<vec4i> quads;
            
            // data for instance BVH
            vector<bvh_instance> instances;
            vector<bvh_tree*>    shape_bvhs;
            
            // optional application specific data to key from a pointer to internal ids
            unordered_map<void*, int> instance_ids;
            unordered_map<void*, int> shape_ids;
            
            // bvh internal nodes
            vector<bvh_node> nodes;
            
            // Embree opaque data
            void* embree_bvh = nullptr;
            
            // cleanup
            ~bvh_tree();
        };
        
        // Build a BVH from the given set of primitives.
        void build_bvh(bvh_tree* bvh, bool high_quality = false);
        // Update the node bounds for a shape bvh.
        void refit_bvh(bvh_tree* bvh);
        
        // Build a BVH from the given set of primitives.
        // Uses Embree if available and requested, otherwise the standard build.
        void build_bvh_embree(bvh_tree* bvh, bool high_quality = false);
        
        // Intersect ray with a bvh returning either the first or any intersection
        // depending on `find_any`. Returns the ray distance `dist`, the instance
        // id `iid`, the shape id `sid`, the shape element index `eid` and the
        // shape barycentric coordinates `uv`.
        bool intersect_bvh(const bvh_tree* bvh, const ray3f& ray, bool find_any,
                           float& dist, int& iid, int& eid, vec2f& uv);
        
        // Find a shape element that overlaps a point within a given distance
        // `max_dist`, returning either the closest or any overlap depending on
        // `find_any`. Returns the point distance `dist`, the instance id `iid`, the
        // shape id `sid`, the shape element index `eid` and the shape barycentric
        // coordinates `uv`.
        bool overlap_bvh(const bvh_tree* bvh, const vec3f& pos, float max_dist,
                         bool find_any, float& dist, int& iid, int& eid, vec2f& uv);
        
        
        
        // -----------------------------------------------------------------------------
        // SHAPE EXAMPLES
        // -----------------------------------------------------------------------------
        
        
        // Shape data returned by make_<shape> functions.
        struct make_shape_data {
            // vertex data
            vector<vec3f> positions;
            vector<vec3f> normals;
            vector<vec2f> texturecoords;
            vector<float> radius;
            
            // elements data
            vector<int>   points;
            vector<vec2i> lines;
            vector<vec3i> triangles;
            vector<vec4i> quads;
            vector<vec4i> beziers;
            
            // face-varying elements data
            vector<vec4i> quads_positions;
            vector<vec4i> quads_normals;
            vector<vec4i> quads_texturecoords;
        };
        
        // Make examples shapes that are not watertight (besides quads).
        // Return (triangles, quads, pos, norm, texcoord)
        make_shape_data make_quad_shape(const vec2i& steps, const vec2f& size,
                                        const vec2f& uvsize, bool as_triangles);
        make_shape_data make_quad_stack_shape(const vec3i& steps, const vec3f& size,
                                              const vec2f& uvsize, bool as_triangles);
        make_shape_data make_floor_shape(const vec2i& steps, const vec2f& size,
                                         const vec2f& uvsize, bool as_triangles);
        make_shape_data make_cube_shape(const vec3i& steps, const vec3f& size,
                                        const vec3f& uvsize, bool as_triangles);
        make_shape_data make_cube_rounded_shape(const vec3i& steps, const vec3f& size,
                                                const vec3f& uvsize, float radius, bool as_triangles);
        make_shape_data make_sphere_shape(
                                          const vec2i& steps, float size, const vec2f& uvsize, bool as_triangles);
        make_shape_data make_sphere_cube_shape(
                                               int steps, float size, float uvsize, bool as_triangles);
        make_shape_data make_sphere_flipcap_shape(const vec2i& steps, float size,
                                                  const vec2f& uvsize, const vec2f& zflip, bool as_triangles);
        make_shape_data make_disk_shape(
                                        const vec2i& steps, float size, const vec2f& uvsize, bool as_triangles);
        make_shape_data make_disk_quad_shape(
                                             int steps, float size, float uvsize, bool as_triangles);
        make_shape_data make_disk_bulged_shape(
                                               int steps, float size, float uvsize, float height, bool as_triangles);
        make_shape_data make_cylinder_side_shape(const vec2i& steps, const vec2f& size,
                                                 const vec2f& uvsize, bool as_triangles);
        make_shape_data make_cylinder_shape(const vec3i& steps, const vec2f& size,
                                            const vec3f& uvsize, bool as_triangles);
        make_shape_data make_cylinder_rounded_shape(const vec3i& steps,
                                                    const vec2f& size, const vec3f& uvsize, float radius, bool as_triangles);
        make_shape_data make_geodesic_sphere_shape(
                                                   int tesselation, float size, bool as_triangles);
        
        // Make examples shapes with are watertight (good for subdivs).
        // Returns (triangles, quads, pos)
        make_shape_data make_suzanne_shape(float size, bool as_triangles);
        make_shape_data make_cube_shape(const vec3f& size, bool as_triangles);
        
        // Make facevarying example shapes that are watertight (good for subdivs).
        make_shape_data make_fvcube_shape(
                                          const vec3i& steps, const vec3f& size, const vec3f& uvsize);
        
        // Generate lines set along a quad. Returns lines, pos, norm, texcoord, radius.
        make_shape_data make_lines_shape(const vec2i& steps, const vec2f& size,
                                         const vec2f& uvsize, const vec2f& line_radius = {0.001f, 0.001f});
        
        // Make point primitives. Returns points, pos, norm, texcoord, radius.
        make_shape_data make_point_shape(float point_radius = 0.001f);
        make_shape_data make_points_shape(
                                          int num, float uvsize, float point_radius = 0.001f);
        make_shape_data make_random_points_shape(int num, const vec3f& size,
                                                 float uvsize, float point_radius = 0.001f, uint64_t seed = 0);
        
        // Make a bezier circle. Returns bezier, pos.
        make_shape_data make_bezier_circle_shape(
                                                 vector<vec4i>& beziers, vector<vec3f>& pos);
        
        // Make a hair ball around a shape.  Returns lines, pos, norm, texcoord, radius.
        // length: minimum and maximum length
        // rad: minimum and maximum radius from base to tip
        // noise: noise added to hair (strength/scale)
        // clump: clump added to hair (number/strength)
        // rotation: rotation added to hair (angle/strength)
        make_shape_data make_hair_shape(const vec2i& steps,
                                        const vector<vec3i>& striangles, const vector<vec4i>& squads,
                                        const vector<vec3f>& spos, const vector<vec3f>& snorm,
                                        const vector<vec2f>& stexcoord, const vec2f& length = {0.1f, 0.1f},
                                        const vec2f& rad = {0.001f, 0.001f}, const vec2f& noise = zero2f,
                                        const vec2f& clump = zero2f, const vec2f& rotation = zero2f, int seed = 7);
        
        // Helper to concatenated shape data for non-facevarying shapes.
        make_shape_data merge_shape_data(const vector<make_shape_data>& shapes);
        
        
        
        // -----------------------------------------------------------------------------
        // COLOR CONVERSION UTILITIES
        // -----------------------------------------------------------------------------
        
        // Element-wise float to byte conversion.
        inline vec4b float_to_byte(const vec4f& a) {
            return {(byte)clamp(int(a.x * 256), 0, 255),
                (byte)clamp(int(a.y * 256), 0, 255), (byte)clamp(int(a.z * 256), 0, 255),
                (byte)clamp(int(a.w * 256), 0, 255)};
        }
        inline vec4f byte_to_float(const vec4b& a) {
            return {a.x / 255.0f, a.y / 255.0f, a.z / 255.0f, a.w / 255.0f};
        }
        
        // Conversion between linear and gamma-encoded colors.
        inline vec3f gamma_to_linear(const vec3f& srgb, float gamma = 2.2f) {
            return {pow(srgb.x, gamma), pow(srgb.y, gamma), pow(srgb.z, gamma)};
        }
        inline vec3f linear_to_gamma(const vec3f& lin, float gamma = 2.2f) {
            return {pow(lin.x, 1 / gamma), pow(lin.y, 1 / gamma), pow(lin.z, 1 / gamma)};
        }
        inline vec4f gamma_to_linear(const vec4f& srgb, float gamma = 2.2f) {
            return {pow(srgb.x, gamma), pow(srgb.y, gamma), pow(srgb.z, gamma), srgb.w};
        }
        inline vec4f linear_to_gamma(const vec4f& lin, float gamma = 2.2f) {
            return {pow(lin.x, 1 / gamma), pow(lin.y, 1 / gamma), pow(lin.z, 1 / gamma),
                lin.w};
        }
        
        // sRGB non-linear curve
        inline float srgb_to_linear(float srgb) {
            if (srgb <= 0.04045) {
                return srgb / 12.92f;
            } else {
                return pow((srgb + 0.055f) / (1.0f + 0.055f), 2.4f);
            }
        }
        inline float linear_to_srgb(float lin) {
            if (lin <= 0.0031308f) {
                return 12.92f * lin;
            } else {
                return (1 + 0.055f) * pow(lin, 1 / 2.4f) - 0.055f;
            }
        }
        
        // Conversion between linear and srgb colors.
        inline vec3f srgb_to_linear(const vec3f& srgb) {
            return {
                srgb_to_linear(srgb.x), srgb_to_linear(srgb.y), srgb_to_linear(srgb.z)};
        }
        inline vec3f linear_to_srgb(const vec3f& lin) {
            return {linear_to_srgb(lin.x), linear_to_srgb(lin.y), linear_to_srgb(lin.z)};
        }
        inline vec4f srgb_to_linear(const vec4f& srgb) {
            return {srgb_to_linear(srgb.x), srgb_to_linear(srgb.y),
                srgb_to_linear(srgb.z), srgb.w};
        }
        inline vec4f linear_to_srgb(const vec4f& lin) {
            return {linear_to_srgb(lin.x), linear_to_srgb(lin.y), linear_to_srgb(lin.z),
                lin.w};
        }
        
        // Approximate luminance estimate for sRGB primaries (better relative luminance)
        inline float luminance(const vec3f& a) {
            return (0.2126f * a.x + 0.7152f * a.y + 0.0722 * a.z);
        }
        inline float luminance(const vec4f& a) {
            return (0.2126f * a.x + 0.7152f * a.y + 0.0722 * a.z);
        }
        
        // Fitted ACES tonemapping curve.
        inline float tonemap_filmic(float hdr) {
            // https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
            // hdr *= 0.6; // brings it back to ACES range
            return (hdr * hdr * 2.51f + hdr * 0.03f) /
            (hdr * hdr * 2.43f + hdr * 0.59f + 0.14f);
        }
        // Apply ACES fitted curve.
        inline vec4f tonemap_filmic(const vec4f& hdr) {
            return {tonemap_filmic(hdr.x), tonemap_filmic(hdr.y), tonemap_filmic(hdr.z),
                hdr.w};
        }
        
        // Tonemap a color value according to an exposure-gamma tone mapper, with
        // an optional filmic curve.
        inline vec4f tonemap_filmic(
                                    const vec4f& hdr, float exposure, bool filmic, bool srgb) {
            auto scale = pow(2.0f, exposure);
            auto ldr   = vec4f{hdr.x * scale, hdr.y * scale, hdr.z * scale, hdr.w};
            if (filmic) ldr = tonemap_filmic(ldr);
            if (srgb) ldr = linear_to_srgb(ldr);
            return ldr;
        }
        
        // Converts HSV to RGB.
        vec3f hsv_to_rgb(const vec3f& hsv);
        vec3f rgb_to_hsv(const vec3f& rgb);
        // Convert between CIE XYZ and xyY
        vec3f xyz_to_xyY(const vec3f& xyz);
        vec3f xyY_to_xyz(const vec3f& xyY);
        // Convert between CIE XYZ and RGB
        vec3f xyz_to_rgb(const vec3f& xyz);
        vec3f rgb_to_xyz(const vec3f& rgb);
        */
        
        // -----------------------------------------------------------------------------
        // ANIMATION UTILITIES
        // -----------------------------------------------------------------------------
        
        // Find the first keyframe value that is greater than the argument.
        sv_inline s32 evaluate_keyframed_index(
                                               const std::vector<f32>& times, const f32& time) {
            for (auto i = 0; i < times.size(); i++)
                if (times[i] > time) return i;
            return (s32)times.size();
        }
        
        // Evaluates a keyframed value using step interpolation.
        template <typename T>
        sv_inline T evaluate_keyframed_step(
                                            const std::vector<f32>& times, const std::vector<T>& vals, f32 time) {
            if (time <= times.front()) return vals.front();
            if (time >= times.back()) return vals.back();
            time     = clamp(time, times.front(), times.back() - 0.001f);
            auto idx = evaluate_keyframed_index(times, time);
            return vals.at(idx - 1);
        }
        
        // Evaluates a keyframed value using linear interpolation.
//        template <typename T>
//        sv_inline FVec4 evaluate_keyframed_slerp(
//                                                 const std::vector<f32>& times, const std::vector<FVec4>& vals, f32 time) {
//            if (time <= times.front()) return vals.front();
//            if (time >= times.back()) return vals.back();
//            time     = clamp(time, times.front(), times.back() - 0.001f);
//            auto idx = evaluate_keyframed_index(times, time);
//            auto t   = (time - times.at(idx - 1)) / (times.at(idx) - times.at(idx - 1));
//            return FVec4::slerp(vals.at(idx - 1), vals.at(idx), t);
//        }
        
        // Evaluates a keyframed value using linear interpolation.
        template <typename T>
        sv_inline T evaluate_keyframed_linear(
                                              const std::vector<f32>& times, const std::vector<T>& vals, f32 time) {
            if (time <= times.front()) return vals.front();
            if (time >= times.back()) return vals.back();
            time     = clamp(time, times.front(), times.back() - 0.001f);
            auto idx = evaluate_keyframed_index(times, time);
            auto t   = (time - times.at(idx - 1)) / (times.at(idx) - times.at(idx - 1));
            return vals.at(idx - 1) * (1 - t) + vals.at(idx) * t;
        }
        
        // Evaluates a keyframed value using Bezier interpolation.
//        template <typename T>
//        sv_inline T evaluate_keyframed_bezier(
//                                           const std::vector<f32>& times, const std::vector<T>& vals, f32 time) {
//            if (time <= times.front()) return vals.front();
//            if (time >= times.back()) return vals.back();
//            time     = clamp(time, times.front(), times.back() - 0.001f);
//            auto idx = evaluate_keyframed_index(times, time);
//            auto t   = (time - times.at(idx - 1)) / (times.at(idx) - times.at(idx - 1));
//            return interpolate_bezier(
//                                      vals.at(idx - 3), vals.at(idx - 2), vals.at(idx - 1), vals.at(idx), t);
//        }
        
        // -----------------------------------------------------------------------------
        // VOLUME TYPE
        // -----------------------------------------------------------------------------
        
        // Volume container.
        template <typename T>
        struct volume {
            int       width  = 0;
            int       height = 0;
            int       depth  = 0;
            std::vector<T> voxels = {};
            
            // constructors
            volume() : width{0}, height{0}, depth{0}, voxels() {}
            volume(int w, int h, int d, const T& v = T{})
            : width{w}, height{h}, depth{d}, voxels(w * h * d, v) {}
            volume(int w, int h, int d, const T* v)
            : width{w}, height{h}, depth{d}, voxels(v, v + w * h * d) {}
        };
        
        // Element access
        template <typename T>
        T& at(volume<T>& vol, int i, int j, int k) {
            return vol.voxels[k * vol.width * vol.height + j * vol.width + i];
        }
        template <typename T>
        const T& at(const volume<T>& vol, int i, int j, int k) {
            return vol.voxels[k * vol.width * vol.height + j * vol.width + i];
        }
        
        
        // -----------------------------------------------------------------------------
        // SCENE DATA
        // -----------------------------------------------------------------------------
        
        // make a simple example volume
        volume<float> make_test_volume1f(
                                         int width, int height, int depth, float scale = 10, float exponent = 6);
        
        
        // -----------------------------------------------------------------------------
        // SCENE DATA
        // -----------------------------------------------------------------------------
        
        
        // forward declaration
        struct bvh_tree;
        
        // Camera based on a simple lens model. The camera is placed using a frame.
        // Camera projection is described in photorgaphics terms. In particular,
        // we specify fil size (35mm by default), the focal lengthm the focus
        // distance and the lens_aperture. All values are in meters.
        struct svgltf_camera {
            SVString  name           = "";
//            frame3f frame          = identity_frame3f;
            bool    orthographic   = false;
//            vec2f   film_size      = {0.036f, 0.024f};
            float   focal_length   = 0.050f;
//            float   focus_distance = maxf;
            float   lens_aperture  = 0;
        };
        
        // Texture containing either an LDR or HDR image. Textures are rendered
        // using linear interpolation (unless `no_interoilation` is set) and
        // weith tiling (unless `clamp_to_edge` is set). HdR images are encoded
        // in linear color space, while LDRs are encoded as sRGB. The latter
        // conversion can be disabled with `ldr_as_linear` for example to render
        // normal maps.
        struct svgltf_texture {
            SVString       name             = "";
            SVString       filename         = "";
//            image<vec4f> hdr_image        = {};
//            image<vec4b> ldr_image        = {};
            bool         clamp_to_edge    = false;
            bool         no_interpolation = false;
            float        height_scale     = 1;
            bool         ldr_as_linear    = false;
            bool         has_opacity      = false;
        };
        
        // Volumetric texture containing a float only volume data. See texture
        // above for other propoerties.
        struct svgltf_voltexture {
//            string        name             = "";
//            string        filename         = "";
//            volume<float> volume_data      = {};
            bool          clamp_to_edge    = false;
            bool          no_interpolation = false;
        };
        
        // Material for surfaces, lines and triangles.
        // For surfaces, uses a microfacet model with thin sheet transmission.
        // The model is based on OBJ, but contains glTF compatibility.
        // For the documentation on the values, please see the OBJ format.
        struct svgltf_material {
            SVString name          = "";
            bool   base_metallic = false;  // base-metallic parametrization
            bool   gltf_textures = false;  // glTF packed textures
            bool   double_sided  = false;  // double sided rendering

            // base values
            FVec3 emission;
            FVec3 diffuse;
            FVec3 specular;
            FVec3 transmission;
            float roughness    = 0.0001;
            float opacity      = 1;
            bool  fresnel      = true;
            bool  refract      = false;
            
            // textures
            svgltf_texture* emission_texture     = nullptr;
            svgltf_texture* diffuse_texture      = nullptr;
            svgltf_texture* specular_texture     = nullptr;
            svgltf_texture* transmission_texture = nullptr;
            svgltf_texture* roughness_texture    = nullptr;
            svgltf_texture* opacity_texture      = nullptr;
            svgltf_texture* occlusion_texture    = nullptr;
            svgltf_texture* bump_texture         = nullptr;
            svgltf_texture* displacement_texture = nullptr;
            svgltf_texture* normal_texture       = nullptr;
            
            // volume properties
            // albedo = scattering / (absorption + scattering)
            // density = absorption + scattering
            FVec3 volume_emission;
            FVec3 volume_albedo;
            FVec3 volume_density;
            f32 volume_phaseg   = 0;
            
            // volume textures
            svgltf_voltexture* volume_density_texture = nullptr;
        };
        
        // Shape data represented as an indexed meshes of elements.
        // May contain either points, lines, triangles and quads.
        struct svgltf_shape {
            // shape data
            SVString          name     = "";
            SVString          filename = "";
            svgltf_material* material = nullptr;
            
            // subdision properties
            int  subdivision_level      = 0;
            bool catmull_clark          = false;
            bool compute_vertex_normals = false;
            
            // primitives
            std::vector<s32>   points    = {};
            std::vector<FVec2> lines     = {};
            std::vector<FVec3> triangles = {};
            std::vector<FVec4> quads     = {};
            
            // face-varying primitives
            std::vector<FVec4> quads_positions     = {};
            std::vector<FVec4> quads_normals       = {};
            std::vector<FVec4> quads_texturecoords = {};
            
            // vertex data
            std::vector<FVec3> positions     = {};
            std::vector<FVec3> normals       = {};
            std::vector<FVec2> texturecoords = {};
            std::vector<FVec4> colors        = {};
            std::vector<f32> radius        = {};
            std::vector<FVec4> tangentspaces = {};
        };
        
        // Shape instance.
        struct svgltf_instance {
            SVString       name  = "";
//            frame3f      frame = identity_frame3f;
            svgltf_shape* shape = nullptr;
        };
        
        // Environment map.
        struct svgltf_environment {
            SVString         name             = "";
//            frame3f        frame            = identity_frame3f;
            FVec3          emission ;
            svgltf_texture* emission_texture = nullptr;
        };
        
        // Node in a transform hierarchy.
        struct svgltf_scene_node {
            SVString             name        = "";
            svgltf_scene_node*  parent      = nullptr;
//            frame3f            local       = identity_frame3f;
            FVec3              translation;
            FVec4              rotation;
            FVec3              scale;
            std::vector<f32>      weights     = {};
            svgltf_camera*      camera      = nullptr;
            svgltf_instance*    instance    = nullptr;
            svgltf_environment* environment = nullptr;
            
            // compute properties
            std::vector<svgltf_scene_node*> children = {};
        };
        
        // Keyframe type.
        enum struct svgltf_interpolation_type { linear, step, bezier };
        
        // Keyframe data.
        struct svgltf_animation {
            SVString                   name               = "";
            SVString                   filename           = "";
            SVString                   animation_group    = "";
            svgltf_interpolation_type interpolation_type = svgltf_interpolation_type::linear;
            std::vector<f32>            keyframes_times    = {};
            std::vector<FVec3>            translation_keyframes   = {};
            std::vector<FVec4>            rotation_keyframes      = {};
            std::vector<FVec3>            scale_keyframes         = {};
            std::vector<std::vector<f32>>    morph_weights_keyframes = {};
            std::vector<svgltf_scene_node*> node_targets           = {};
        };
        
        // Scene comprised an array of objects whose memory is owened by the scene.
        // All members are optional,Scene objects (camera, instances, environments)
        // have transforms defined internally. A scene can optionally contain a
        // node hierarchy where each node might point to a camera, instance or
        // environment. In that case, the element transforms are computed from
        // the hierarchy. Animation is also optional, with keyframe data that
        // updates node transformations only if defined.
        struct svgltf_scene {
            SVString                     name         = "";
            std::vector<svgltf_camera*>      cameras      = {};
            std::vector<svgltf_shape*>       shapes       = {};
            std::vector<svgltf_instance*>    instances    = {};
            std::vector<svgltf_material*>    materials    = {};
            std::vector<svgltf_texture*>     textures     = {};
            std::vector<svgltf_environment*> environments = {};
            std::vector<svgltf_voltexture*>  voltextures  = {};
            std::vector<svgltf_scene_node*>  nodes        = {};
            std::vector<svgltf_animation*>   animations   = {};
            
            // cleanup
            ~svgltf_scene();
        };
        
        
        // -----------------------------------------------------------------------------
        // SCENE UTILITIES
        // -----------------------------------------------------------------------------
        
        
        // Print scene statistics.
        void print_stats(const svgltf_scene* scene);
        
        // Merge scene into one another. Note that the objects are _moved_ from
        // merge_from to merged_into, so merge_from will be empty after this function.
        void merge_scene(svgltf_scene* merge_into, svgltf_scene* merge_from);
        
        // make camera
        svgltf_camera* make_bbox_camera(cptr8 name, const SVBoundBox& bbox,
                                       const FVec2& film = FVec2(0.036f, 0.024f), f32 focal = 0.050f);
        // make default material
        sv_inline svgltf_material* make_default_material(cptr8 name) {
            auto mat     = new svgltf_material();
//            mat->name    = name;
//            mat->diffuse = {0.2f, 0.2f, 0.2f};
            return mat;
        }
        
        // Add a sky environment
//        sv_inline svgltf_environment* make_sky_environment(
//                                                       cptr8 name, f32 sun_angle = pif / 4) {
//            auto texture                  = new svgltf_texture();
////            texture->name                 = name;
////            texture->filename             = "textures/" + name + ".hdr";
////            texture->hdr_image            = make_sunsky_image4f(1024, 512, sun_angle);
//            auto environment              = new svgltf_environment();
//            environment->name             = name;
//            environment->emission         = {1, 1, 1};
//            environment->emission_texture = texture;
//            return environment;
//        }
        
        
        
        // -----------------------------------------------------------------------------
        // EVALUATION OF SCENE PROPERTIES
        // -----------------------------------------------------------------------------
        
        
//        // Update node transforms.
//        void update_transforms(
//                               svgltf_scene* scene, f32 time = 0, cptr8 anim_group = "");
//        
//        // Compute animation range.
//        FVec2 compute_animation_range(
//                                      const svgltf_scene* scene, cptr8 anim_group = "");
//        
//        // Computes shape/scene approximate bounds.
//        bbox3f compute_shape_bounds(const svgltf_shape* shape);
//        bbox3f compute_scene_bounds(const svgltf_scene* scene);
//        
//        // Updates/refits bvh.
//        bvh_tree* make_shape_bvh(
//                                 const yocto_shape* shape, bool high_quality, bool embree = false);
//        bvh_tree* make_scene_bvh(
//                                 const yocto_scene* scene, bool high_quality, bool embree = false);
//        void refit_shape_bvh(const yocto_shape* shape, bvh_tree* bvh);
//        void refit_scene_bvh(const yocto_scene* scene, bvh_tree* bvh);
//        
//        // Apply subdivision and displacement rules.
//        yocto_shape* tesselate_shape(const yocto_shape* shape);
//        void         tesselate_shapes(yocto_scene* scene);
//        
//        // Add missing names, normals, tangents and hierarchy.
//        void add_missing_names(yocto_scene* scene);
//        void add_missing_normals(yocto_scene* scene);
//        void add_missing_tangent_space(yocto_scene* scene);
//        void add_missing_materials(yocto_scene* scene);
//        void add_missing_cameras(svgltf_scene* scene);
//        // Checks for validity of the scene.
//        vector<string> validate_scene(
//                                      const yocto_scene* scene, bool skip_textures = false);
//        void log_validation_errors(const yocto_scene* scene, bool skip_textures = false);
//        
//        // Queries on objects
//        bool is_shape_face_varying(const yocto_shape* shape);
//        
//        // Scene intersection. Upron intersection we set the instance pointer,
//        // the shape element_id and element_uv and the inetrsection distance.
//        struct scene_intersection {
//            yocto_instance* instance   = nullptr;
//            int             element_id = 0;
//            vec2f           element_uv = zero2f;
//            float           distance   = maxf;
//        };
//        
//        // Intersects a ray with an instance. The bvh refers is the shape bvh.
//        scene_intersection intersect_scene(const yocto_instance* instance,
//                                           const bvh_tree* sbvh, const ray3f& ray, bool find_any = false);
//        // Intersects a ray with the scene.
//        scene_intersection intersect_scene(const yocto_scene* scene,
//                                           const bvh_tree* bvh, const ray3f& ray, bool find_any = false);
//        
//        // Shape values interpolated using barycentric coordinates.
//        vec3f evaluate_position(const yocto_shape* shape, int ei, const vec2f& uv);
//        vec3f evaluate_normal(const yocto_shape* shape, int ei, const vec2f& uv);
//        vec2f evaluate_texturecoord(const yocto_shape* shape, int ei, const vec2f& uv);
//        vec4f evaluate_color(const yocto_shape* shape, int ei, const vec2f& uv);
//        float evaluate_radius(const yocto_shape* shape, int ei, const vec2f& uv);
//        vec4f evaluate_tangentspace(const yocto_shape* shape, int ei, const vec2f& uv);
//        vec3f evaluate_tangentspace(
//                                    const yocto_shape* shape, int ei, const vec2f& uv, bool& left_handed);
//        // Shape element values.
//        vec3f evaluate_element_normal(const yocto_shape* shape, int ei);
//        vec4f evaluate_element_tangentspace(const yocto_shape* shape, int ei);
//        
//        // Instance values interpolated using barycentric coordinates.
//        // Handles defaults if data is missing.
//        vec3f evaluate_position(const yocto_instance* instance, int ei, const vec2f& uv);
//        vec3f evaluate_normal(const yocto_instance* instance, int ei, const vec2f& uv);
//        vec3f evaluate_tangentspace(
//                                    const yocto_instance* instance, int ei, const vec2f& uv, bool& left_handed);
//        // Instance element values.
//        vec3f evaluate_element_normal(const yocto_instance* instance, int ei);
//        // Shading normals including material perturbations.
//        vec3f evaluate_shading_normal(
//                                      const yocto_instance* instance, int ei, const vec2f& uv, const vec3f& o);
//        
//        // Environment texture coordinates from the incoming direction.
//        vec2f evaluate_texturecoord(const yocto_environment* environment, const vec3f& i);
//        // Evaluate the incoming direction from the uv.
//        vec3f evaluate_direction(const yocto_environment* environment, const vec2f& uv);
//        // Evaluate the environment emission.
//        vec3f evaluate_emission(const yocto_environment* environment, const vec3f& i);
//        // Evaluate all environment emission.
//        vec3f evaluate_emission(const yocto_scene* scene, const vec3f& i);
//        
//        // Evaluate a texture.
//        vec2i evaluate_texture_size(const yocto_texture* texture);
//        vec4f lookup_texture(const yocto_texture* texture, int i, int j);
//        vec4f evaluate_texture(const yocto_texture* texture, const vec2f& texcoord);
//        float lookup_voltexture(const yocto_voltexture* texture, int i, int j, int k);
//        float evaluate_voltexture(const yocto_voltexture* texture, const vec3f& texcoord);
//        
//        // Set and evaluate camera parameters. Setters take zeros as default values.
//        float evaluate_camera_fovy(const yocto_camera* camera);
//        float evaluate_camera_aspect(const yocto_camera* camera);
//        void  set_camera_fovy(
//                              yocto_camera* camera, float fovy, float aspect, float width = 0.036f);
//        vec2i evaluate_image_size(const yocto_camera* camera, int yresolution);
//        
//        // Generates a ray from a camera image coordinate `uv` and lens coordinates
//        // `luv`.
//        ray3f evaluate_camera_ray(
//                                  const yocto_camera* camera, const vec2f& uv, const vec2f& luv);
//        // Generates a ray from a camera for pixel coordinates `ij`, the image size
//        // `imsize`, the sub-pixel coordinates `puv` and the lens coordinates `luv` and
//        // the image resolution `res`.
//        ray3f evaluate_camera_ray(const yocto_camera* camera, const vec2i& ij,
//                                  const vec2i& imsize, const vec2f& puv, const vec2f& luv);
//        // Generates a ray from a camera for pixel index `idx`, the image size
//        // `imsize`, the sub-pixel coordinates `puv` and the lens coordinates `luv` and
//        // the image resolution `res`.
//        ray3f evaluate_camera_ray(const yocto_camera* camera, int idx,
//                                  const vec2i& imsize, const vec2f& puv, const vec2f& luv);
//        
//        // Evaluates material parameters: emission, diffuse, specular, transmission,
//        // roughness and opacity.
//        vec3f evaluate_emission(const yocto_material* material,
//                                const vec2f& texturecoord, const vec4f& shape_color = {1, 1, 1, 1});
//        vec3f evaluate_diffuse(const yocto_material* material,
//                               const vec2f& texturecoord, const vec4f& shape_color = {1, 1, 1, 1});
//        vec3f evaluate_specular(const yocto_material* material,
//                                const vec2f& texturecoord, const vec4f& shape_color = {1, 1, 1, 1});
//        vec3f evaluate_transmission(const yocto_material* material,
//                                    const vec2f& texturecoord, const vec4f& shape_color = {1, 1, 1, 1});
//        float evaluate_roughness(const yocto_material* material,
//                                 const vec2f& texturecoord, const vec4f& shape_color = {1, 1, 1, 1});
//        float evaluate_opacity(const yocto_material* material,
//                               const vec2f& texturecoord, const vec4f& shape_color = {1, 1, 1, 1});
//        
//        // Material values packed into a convenience structure.
//        struct microfacet_brdf {
//            vec3f diffuse      = zero3f;
//            vec3f specular     = zero3f;
//            vec3f transmission = zero3f;
//            float roughness    = 1;
//            bool  refract      = false;
//        };
//        microfacet_brdf evaluate_brdf(const yocto_material* material,
//                                      const vec2f& texturecoord, const vec4f& shape_color = {1, 1, 1, 1});
//        bool            is_bsdf_delta(const microfacet_brdf& f);
//        
//        // Check volume properties.
//        bool is_volume_homogeneus(const yocto_material* vol);
//        bool is_volume_colored(const yocto_material* vol);
//        
//        // Sample a shape element based on area/length.
//        vector<float>     compute_shape_elements_cdf(const yocto_shape* shape);
//        tuple<int, vec2f> sample_shape_element(const yocto_shape* shape,
//                                               const vector<float>& elem_cdf, float re, const vec2f& ruv);
//        float             sample_shape_element_pdf(const yocto_shape* shape,
//                                                   const vector<float>& elem_cdf, int element_id, const vec2f& element_uv);
//        
//        // Sample an environment based on either texel values of uniform
//        vector<float> compute_environment_texels_cdf(
//                                                     const yocto_environment* environment);
//        vec3f sample_environment_direction(const yocto_environment* environment,
//                                           const vector<float>& texels_cdf, float re, const vec2f& ruv);
//        float sample_environment_direction_pdf(const yocto_environment* environment,
//                                               const vector<float>& texels_cdf, const vec3f& direction);
//        
//        
//        
//        // -----------------------------------------------------------------------------
//        // PATH TRACING
//        // -----------------------------------------------------------------------------
//        
//        
//        // Default trace seed
//        const auto trace_default_seed = 961748941;
//        
//        // Type of tracing algorithm to use
//        enum struct trace_type {
//            path,               // path tracing
//            volpath,            // volumetric path tracing
//            direct,             // direct illumination
//            environment,        // environment illumination only
//            eyelight,           // eyelight rendering
//            path_nomis,         // path tracer without mis
//            path_naive,         // naive path tracing
//            direct_nomis,       // direct illumition without mis
//            debug_normal,       // debug - normal
//            debug_albedo,       // debug - albedo
//            debug_texcoord,     // debug - texcoord
//            debug_frontfacing,  // debug - faceforward
//            debug_diffuse,      // debug - diffuse
//            debug_specular,     // debug - specular
//            debug_roughness,    // debug - roughness
//        };
//        
//        const auto trace_type_names = vector<string>{"path", "volpath", "direct",
//            "environment", "eyelight", "path_nomis", "path_naive", "direct_nomis",
//            "debug_normal", "debug_albedo", "debug_texcoord", "debug_frontfacing",
//            "debug_diffuse", "debug_specular", "debug_roughness"};
//        
//        // Trace options
//        struct trace_params {
//            int        camera_id           = 0;
//            int        vertical_resolution = 256;
//            trace_type sample_tracer       = trace_type::path;
//            int        num_samples         = 256;
//            int        max_bounces         = 8;
//            float      pixel_clamp         = 100;
//            int        samples_per_batch   = 16;
//            bool       no_parallel         = false;
//            int        preview_ratio       = 8;
//            float      display_exposure    = 0;
//            bool       display_filmic      = false;
//            bool       display_srgb        = true;
//            int        random_seed         = trace_default_seed;
//        };
//        
//        // Trace lights used during rendering.
//        struct trace_lights {
//            vector<yocto_instance*>                      instances               = {};
//            vector<yocto_environment*>                   environments            = {};
//            unordered_map<yocto_shape*, vector<float>>   shape_elements_cdf      = {};
//            unordered_map<yocto_texture*, vector<float>> environment_texture_cdf = {};
//        };
//        
//        // Trace data used during rendering. Initialize with `make_trace_state()`
//        struct trace_state {
//            image<vec4f> rendered_image = {};
//            image<vec4f> display_image  = {};
//            
//            // internal data used during rendering
//            image<vec4f>     accumulation_buffer      = {};
//            image<int>       samples_per_pixel        = {};
//            image<rng_state> random_number_generators = {};
//            int              current_sample           = 0;
//            vector<thread>   async_threads;
//            bool             async_stop_flag = false;
//        };
//        
//        // Initialize lights.
//        trace_lights* make_trace_lights(
//                                        const yocto_scene* scene, const trace_params& params);
//        
//        // Initialize state of the renderer.
//        trace_state* make_trace_state(
//                                      const yocto_scene* scene, const trace_params& params);
//        
//        // Progressively compute an image by calling trace_samples multiple times.
//        image<vec4f> trace_image4f(const yocto_scene* scene, const bvh_tree* bvh,
//                                   const trace_lights* lights, const trace_params& params);
//        
//        // Progressively compute an image by calling trace_samples multiple times.
//        // Start with an empty state and then successively call this function to
//        // render the next batch of samples.
//        bool trace_samples(trace_state* state, const yocto_scene* scene,
//                           const bvh_tree* bvh, const trace_lights* lights, const trace_params& params);
//        
//        // Starts an anyncrhounous renderer. The function will keep a reference to
//        // params.
//        void trace_async_start(trace_state* state, const yocto_scene* scene,
//                               const bvh_tree* bvh, const trace_lights* lights, const trace_params& params);
//        // Stop the asynchronous renderer.
//        void trace_async_stop(trace_state* state);
//        
//        // Trace statistics for last run used for fine tuning implementation.
//        // For now returns number of paths and number of rays.
//        tuple<uint64_t, uint64_t> get_trace_stats();
//        void                      reset_trace_stats();
//        
//        
//        
//        // -----------------------------------------------------------------------------
//        // PATH TRACING SUPPORT FUNCTION
//        // -----------------------------------------------------------------------------
//        
//        
//        // Phong exponent to roughness.
//        float specular_exponent_to_roughness(float n);
//        
//        // Specular to fresnel eta.
//        void  specular_fresnel_from_ks(const vec3f& ks, vec3f& es, vec3f& esk);
//        float specular_to_eta(const vec3f& ks);
//        // Compute the fresnel term for dielectrics.
//        vec3f fresnel_dielectric(float cosw, const vec3f& eta_);
//        // Compute the fresnel term for metals.
//        vec3f fresnel_metal(float cosw, const vec3f& eta, const vec3f& etak);
//        // Schlick approximation of Fresnel term, optionally weighted by rs;
//        vec3f fresnel_schlick(const vec3f& ks, float cosw);
//        vec3f fresnel_schlick(const vec3f& ks, float cosw, float rs);
//        vec3f fresnel_schlick(const vec3f& ks, const vec3f& h, const vec3f& o);
//        vec3f fresnel_schlick(const vec3f& ks, const vec3f& h, const vec3f& o, float rs);
//        
//        // Evaluates the GGX distribution and geometric term.
//        float evaluate_ggx(float rs, float ndh, float ndi, float ndo);
//        // Sample the GGX distribution.
//        vec3f sample_ggx(float rs, const vec2f& rn);
//        float sample_ggx_pdf(float rs, float ndh);
//        
//        // Evaluates the GGX distribution and geometric term.
//        float evaluate_ggx_dist(float rs, const vec3f& n, const vec3f& h);
//        float evaluate_ggx_sm(float rs, const vec3f& n, const vec3f& o, const vec3f& i);
//        
//        // Evaluate and sample volume phase function.
//        vec3f sample_phase_function(float vg, const vec2f& u);
//        float evaluate_phase_function(float cos_theta, float vg);
//        
//        
//        
//        // -----------------------------------------------------------------------------
//        // IMPLEMENTATION FOR MATRICES
//        // -----------------------------------------------------------------------------
//        
//        
//        // Matrix diagonals and transposes.
//        template <typename T>
//        constexpr inline mat<T, 1, 1> transpose(const mat<T, 1, 1>& a) {
//            return {{a.x}};
//        }
//        template <typename T>
//        constexpr inline mat<T, 2, 2> transpose(const mat<T, 2, 2>& a) {
//            return {{a.x.x, a.y.x}, {a.x.y, a.y.y}};
//        }
//        template <typename T>
//        constexpr inline mat<T, 3, 3> transpose(const mat<T, 3, 3>& a) {
//            return {
//                {a.x.x, a.y.x, a.z.x},
//                {a.x.y, a.y.y, a.z.y},
//                {a.x.z, a.y.z, a.z.z},
//            };
//        }
//        template <typename T>
//        constexpr inline mat<T, 4, 4> transpose(const mat<T, 4, 4>& a) {
//            return {
//                {a.x.x, a.y.x, a.z.x, a.w.x},
//                {a.x.y, a.y.y, a.z.y, a.w.y},
//                {a.x.z, a.y.z, a.z.z, a.w.z},
//                {a.x.w, a.y.w, a.z.w, a.w.w},
//            };
//        }
//        
//        // Matrix adjugates, determinant and inverses.
//        template <typename T>
//        constexpr inline mat<T, 1, 1> adjugate(const mat<T, 1, 1>& a) {
//            return {{a.x}};
//        }
//        template <typename T>
//        constexpr inline mat<T, 2, 2> adjugate(const mat<T, 2, 2>& a) {
//            return {{a.y.y, -a.x.y}, {-a.y.x, a.x.x}};
//        }
//        template <typename T>
//        constexpr inline mat<T, 3, 3> adjugate(const mat<T, 3, 3>& a) {
//            return {
//                {
//                    a.y.y * a.z.z - a.z.y * a.y.z,
//                    a.z.y * a.x.z - a.x.y * a.z.z,
//                    a.x.y * a.y.z - a.y.y * a.x.z,
//                },
//                {
//                    a.y.z * a.z.x - a.z.z * a.y.x,
//                    a.z.z * a.x.x - a.x.z * a.z.x,
//                    a.x.z * a.y.x - a.y.z * a.x.x,
//                },
//                {
//                    a.y.x * a.z.y - a.z.x * a.y.y,
//                    a.z.x * a.x.y - a.x.x * a.z.y,
//                    a.x.x * a.y.y - a.y.x * a.x.y,
//                },
//            };
//        }
//        template <typename T>
//        constexpr inline mat<T, 4, 4> adjugate(const mat<T, 4, 4>& a) {
//            return {
//                {
//                    a.y.y * a.z.z * a.w.w + a.w.y * a.y.z * a.z.w +
//                    a.z.y * a.w.z * a.y.w - a.y.y * a.w.z * a.z.w -
//                    a.z.y * a.y.z * a.w.w - a.w.y * a.z.z * a.y.w,
//                    a.x.y * a.w.z * a.z.w + a.z.y * a.x.z * a.w.w +
//                    a.w.y * a.z.z * a.x.w - a.w.y * a.x.z * a.z.w -
//                    a.z.y * a.w.z * a.x.w - a.x.y * a.z.z * a.w.w,
//                    a.x.y * a.y.z * a.w.w + a.w.y * a.x.z * a.y.w +
//                    a.y.y * a.w.z * a.x.w - a.x.y * a.w.z * a.y.w -
//                    a.y.y * a.x.z * a.w.w - a.w.y * a.y.z * a.x.w,
//                    a.x.y * a.z.z * a.y.w + a.y.y * a.x.z * a.z.w +
//                    a.z.y * a.y.z * a.x.w - a.x.y * a.y.z * a.z.w -
//                    a.z.y * a.x.z * a.y.w - a.y.y * a.z.z * a.x.w,
//                },
//                {
//                    a.y.z * a.w.w * a.z.x + a.z.z * a.y.w * a.w.x +
//                    a.w.z * a.z.w * a.y.x - a.y.z * a.z.w * a.w.x -
//                    a.w.z * a.y.w * a.z.x - a.z.z * a.w.w * a.y.x,
//                    a.x.z * a.z.w * a.w.x + a.w.z * a.x.w * a.z.x +
//                    a.z.z * a.w.w * a.x.x - a.x.z * a.w.w * a.z.x -
//                    a.z.z * a.x.w * a.w.x - a.w.z * a.z.w * a.x.x,
//                    a.x.z * a.w.w * a.y.x + a.y.z * a.x.w * a.w.x +
//                    a.w.z * a.y.w * a.x.x - a.x.z * a.y.w * a.w.x -
//                    a.w.z * a.x.w * a.y.x - a.y.z * a.w.w * a.x.x,
//                    a.x.z * a.y.w * a.z.x + a.z.z * a.x.w * a.y.x +
//                    a.y.z * a.z.w * a.x.x - a.x.z * a.z.w * a.y.x -
//                    a.y.z * a.x.w * a.z.x - a.z.z * a.y.w * a.x.x,
//                },
//                {
//                    a.y.w * a.z.x * a.w.y + a.w.w * a.y.x * a.z.y +
//                    a.z.w * a.w.x * a.y.y - a.y.w * a.w.x * a.z.y -
//                    a.z.w * a.y.x * a.w.y - a.w.w * a.z.x * a.y.y,
//                    a.x.w * a.w.x * a.z.y + a.z.w * a.x.x * a.w.y +
//                    a.w.w * a.z.x * a.x.y - a.x.w * a.z.x * a.w.y -
//                    a.w.w * a.x.x * a.z.y - a.z.w * a.w.x * a.x.y,
//                    a.x.w * a.y.x * a.w.y + a.w.w * a.x.x * a.y.y +
//                    a.y.w * a.w.x * a.x.y - a.x.w * a.w.x * a.y.y -
//                    a.y.w * a.x.x * a.w.y - a.w.w * a.y.x * a.x.y,
//                    a.x.w * a.z.x * a.y.y + a.y.w * a.x.x * a.z.y +
//                    a.z.w * a.y.x * a.x.y - a.x.w * a.y.x * a.z.y -
//                    a.z.w * a.x.x * a.y.y - a.y.w * a.z.x * a.x.y,
//                },
//                {
//                    a.y.x * a.w.y * a.z.z + a.z.x * a.y.y * a.w.z +
//                    a.w.x * a.z.y * a.y.z - a.y.x * a.z.y * a.w.z -
//                    a.w.x * a.y.y * a.z.z - a.z.x * a.w.y * a.y.z,
//                    a.x.x * a.z.y * a.w.z + a.w.x * a.x.y * a.z.z +
//                    a.z.x * a.w.y * a.x.z - a.x.x * a.w.y * a.z.z -
//                    a.z.x * a.x.y * a.w.z - a.w.x * a.z.y * a.x.z,
//                    a.x.x * a.w.y * a.y.z + a.y.x * a.x.y * a.w.z +
//                    a.w.x * a.y.y * a.x.z - a.x.x * a.y.y * a.w.z -
//                    a.w.x * a.x.y * a.y.z - a.y.x * a.w.y * a.x.z,
//                    a.x.x * a.y.y * a.z.z + a.z.x * a.x.y * a.y.z +
//                    a.y.x * a.z.y * a.x.z - a.x.x * a.z.y * a.y.z -
//                    a.y.x * a.x.y * a.z.z - a.z.x * a.y.y * a.x.z,
//                },
//            };
//        }
//        template <typename T>
//        constexpr inline T determinant(const mat<T, 1, 1>& a) {
//            return a.x;
//        }
//        template <typename T>
//        constexpr inline T determinant(const mat<T, 2, 2>& a) {
//            return a.x.x * a.y.y - a.x.y * a.y.x;
//        }
//        template <typename T>
//        constexpr inline T determinant(const mat<T, 3, 3>& a) {
//            return a.x.x * (a.y.y * a.z.z - a.z.y * a.y.z) +
//            a.x.y * (a.y.z * a.z.x - a.z.z * a.y.x) +
//            a.x.z * (a.y.x * a.z.y - a.z.x * a.y.y);
//        }
//        template <typename T>
//        constexpr inline T determinant(const mat<T, 4, 4>& a) {
//            return a.x.x * (a.y.y * a.z.z * a.w.w + a.w.y * a.y.z * a.z.w +
//                            a.z.y * a.w.z * a.y.w - a.y.y * a.w.z * a.z.w -
//                            a.z.y * a.y.z * a.w.w - a.w.y * a.z.z * a.y.w) +
//            a.x.y * (a.y.z * a.w.w * a.z.x + a.z.z * a.y.w * a.w.x +
//                     a.w.z * a.z.w * a.y.x - a.y.z * a.z.w * a.w.x -
//                     a.w.z * a.y.w * a.z.x - a.z.z * a.w.w * a.y.x) +
//            a.x.z * (a.y.w * a.z.x * a.w.y + a.w.w * a.y.x * a.z.y +
//                     a.z.w * a.w.x * a.y.y - a.y.w * a.w.x * a.z.y -
//                     a.z.w * a.y.x * a.w.y - a.w.w * a.z.x * a.y.y) +
//            a.x.w * (a.y.x * a.w.y * a.z.z + a.z.x * a.y.y * a.w.z +
//                     a.w.x * a.z.y * a.y.z - a.y.x * a.z.y * a.w.z -
//                     a.w.x * a.y.y * a.z.z - a.z.x * a.w.y * a.y.z);
//        }
//        
//        
//        // -----------------------------------------------------------------------------
//        // IMPLEMENTATION FOR UI UTILITIES
//        // -----------------------------------------------------------------------------
//        
//        
//        // Turntable for UI navigation.
//        inline void camera_turntable(vec3f& from, vec3f& to, vec3f& up,
//                                     const vec2f& rotate, float dolly, const vec2f& pan) {
//            // rotate if necessary
//            if (rotate.x || rotate.y) {
//                auto z     = normalize(to - from);
//                auto lz    = length(to - from);
//                auto phi   = atan2(z.z, z.x) + rotate.x;
//                auto theta = acos(z.y) + rotate.y;
//                theta      = clamp(theta, 0.001f, pif - 0.001f);
//                auto nz    = vec3f{sin(theta) * cos(phi) * lz, cos(theta) * lz,
//                    sin(theta) * sin(phi) * lz};
//                from       = to - nz;
//            }
//            
//            // dolly if necessary
//            if (dolly) {
//                auto z  = normalize(to - from);
//                auto lz = max(0.001f, length(to - from) * (1 + dolly));
//                z *= lz;
//                from = to - z;
//            }
//            
//            // pan if necessary
//            if (pan.x || pan.y) {
//                auto z = normalize(to - from);
//                auto x = normalize(cross(up, z));
//                auto y = normalize(cross(z, x));
//                auto t = vec3f{pan.x * x.x + pan.y * y.x, pan.x * x.y + pan.y * y.y,
//                    pan.x * x.z + pan.y * y.z};
//                from += t;
//                to += t;
//            }
//        }
//        
//        // Turntable for UI navigation.
//        inline void camera_turntable(frame3f& frame, float& focus, const vec2f& rotate,
//                                     float dolly, const vec2f& pan) {
//            // rotate if necessary
//            if (rotate != zero2f) {
//                auto phi   = atan2(frame.z.z, frame.z.x) + rotate.x;
//                auto theta = acos(frame.z.y) + rotate.y;
//                theta      = clamp(theta, 0.001f, pif - 0.001f);
//                auto new_z = vec3f{
//                    sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi)};
//                auto new_center = frame.o - frame.z * focus;
//                auto new_o      = new_center + new_z * focus;
//                frame           = lookat_frame(new_o, new_center, {0, 1, 0});
//                focus           = length(new_o - new_center);
//            }
//            
//            // pan if necessary
//            if (dolly) {
//                auto c  = frame.o - frame.z * focus;
//                focus   = max(focus * (1 + dolly), 0.001f);
//                frame.o = c + frame.z * focus;
//            }
//            
//            // pan if necessary
//            if (pan.x || pan.y) {
//                frame.o += frame.x * pan.x + frame.y * pan.y;
//            }
//        }
//        
//        // FPS camera for UI navigation for a frame parametrization.
//        inline void camera_fps(frame3f& frame, vec3f transl, vec2f rotate) {
//            // https://gamedev.stackexchange.com/questions/30644/how-to-keep-my-quaternion-using-fps-camera-from-tilting-and-messing-up
//            auto y = vec3f{0, 1, 0};
//            auto z = orthonormalize(frame.z, y);
//            auto x = cross(y, z);
//            
//            auto rot = rotation_frame(vec3f{1, 0, 0}, rotate.y) *
//            frame3f{frame.x, frame.y, frame.z, vec3f{0, 0, 0}} *
//            rotation_frame(vec3f{0, 1, 0}, rotate.x);
//            auto pos = frame.o + transl.x * x + transl.y * y + transl.z * z;
//            
//            frame = {rot.x, rot.y, rot.z, pos};
//        }
//        
//        
//        // -----------------------------------------------------------------------------
//        // IMPLEMENTATION OF STRING/TIME UTILITIES FOR CLI APPLICATIONS
//        // -----------------------------------------------------------------------------
//        
//        // Prints basic types
//        inline bool print_value(string& str, const string& val) {
//            str += val;
//            return true;
//        }
//        inline bool print_value(string& str, const char* val) {
//            str += val;
//            return true;
//        }
//        inline bool print_value(string& str, int val) {
//            str += std::to_string(val);
//            return true;
//        }
//        inline bool print_value(string& str, float val) {
//            str += std::to_string(val);
//            return true;
//        }
//        inline bool print_value(string& str, double val) {
//            str += std::to_string(val);
//            return true;
//        }
//        
//        template <typename T, size_t N>
//        inline bool print_value(string& str, const array<T, N>& val) {
//            for (auto i = 0; i < N; i++) {
//                if (i) str += " ";
//                str += std::to_string(val[i]);
//            }
//            return true;
//        }
//        
//        // Print compound types.
//        template <typename T, int N>
//        inline bool print_value(string& str, const vec<T, N>& v) {
//            return print_value(str, (const array<T, N>&)v);
//        }
//        template <typename T, int N, int M>
//        inline bool print_value(string& str, const mat<T, N, M>& v) {
//            return print_value(str, (const array<T, N * M>&)v);
//        }
//        template <typename T, int N>
//        inline bool print_value(string& str, const frame<T, N>& v) {
//            return print_value(str, (const array<T, N*(N + 1)>&)v);
//        }
//        template <typename T, int N>
//        inline bool print_value(string& str, const bbox<T, N>& v) {
//            return print_value(str, (const array<T, N * 2>&)v);
//        }
//        template <typename T, int N>
//        inline bool print_value(string& str, const ray<T, N>& v) {
//            return print_value(str, (const array<T, N * 2 + 2>&)v);
//        }
//        
//        // Prints a string.
//        inline bool print_next(string& str, const string& fmt) {
//            return print_value(str, fmt);
//        }
//        template <typename Arg, typename... Args>
//        inline bool print_next(
//                               string& str, const string& fmt, const Arg& arg, const Args&... args) {
//            auto pos = fmt.find("{}");
//            if (pos == string::npos) return print_value(str, fmt);
//            if (!print_value(str, fmt.substr(0, pos))) return false;
//            if (!print_value(str, arg)) return false;
//            return print_next(str, fmt.substr(pos + 2), args...);
//        }
//        
//        // Formats a string `fmt` with values taken from `args`. Uses `{}` as
//        // placeholder.
//        template <typename... Args>
//        inline string format(const string& fmt, const Args&... args) {
//            auto str = string();
//            print_next(str, fmt, args...);
//            return str;
//        }
//        
//        // Prints a string.
//        template <typename... Args>
//        inline bool print(FILE* fs, const string& fmt, const Args&... args) {
//            auto str = format(fmt, args...);
//            return fprintf(fs, "%s", str.c_str()) >= 0;
//        }
//        
//        // Converts to string.
//        template <typename T>
//        inline string to_string(const T& val) {
//            auto str = string();
//            print_value(str, val);
//            return str;
//        }
//        
//        // Prints basic types to string
//        inline bool _parse(const char*& str, string& val) {
//            auto n = 0;
//            char buf[4096];
//            if (sscanf(str, "%4095s%n", buf, &n) != 1) return false;
//            val = buf;
//            str += n;
//            return true;
//        }
//        inline bool _parse(const char*& str, int& val) {
//            auto n = 0;
//            if (sscanf(str, "%d%n", &val, &n) != 1) return false;
//            str += n;
//            return true;
//        }
//        inline bool _parse(const char*& str, float& val) {
//            auto n = 0;
//            if (sscanf(str, "%f%n", &val, &n) != 1) return false;
//            str += n;
//            return true;
//        }
//        inline bool _parse(const char*& str, double& val) {
//            auto n = 0;
//            if (sscanf(str, "%lf%n", &val, &n) != 1) return false;
//            str += n;
//            return true;
//        }
//        
//        // Print compound types
//        template <typename T, size_t N>
//        inline bool _parse(const char*& str, array<T, N>& val) {
//            for (auto i = 0; i < N; i++) {
//                if (!_parse(str, val[i])) return false;
//            }
//            return true;
//        }
//        // Data acess
//        template <typename T, int N>
//        inline bool _parse(const char*& str, vec<T, N>& v) {
//            return _parse(str, (array<T, N>&)v);
//        }
//        template <typename T, int N, int M>
//        inline bool _parse(const char*& str, mat<T, N, M>& v) {
//            return _parse(str, (array<T, N * M>&)v);
//        }
//        template <typename T, int N>
//        inline bool _parse(const char*& str, frame<T, N>& v) {
//            return _parse(str, (array<T, N*(N + 1)>&)v);
//        }
//        template <typename T, int N>
//        inline bool _parse(const char*& str, bbox<T, N>& v) {
//            return _parse(str, (array<T, N * 2>&)v);
//        }
//        template <typename T, int N>
//        inline bool _parse(const char*& str, ray<T, N>& v) {
//            return _parse(str, (array<T, N * 2 + 2>&)v);
//        }
//        
//        // Prints a string.
//        inline bool _parse_next(const char*& str) { return true; }
//        template <typename Arg, typename... Args>
//        inline bool _parse_next(const char*& str, Arg& arg, Args&... args) {
//            if (!_parse(str, arg)) return false;
//            return _parse_next(str, args...);
//        }
//        
//        // Returns trus if this is white space
//        inline bool _is_whitespace(const char* str) {
//            while (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n') str++;
//            return *str == 0;
//        }
//        
//        // Parse a list of space separated values.
//        template <typename... Args>
//        inline bool parse(const string& str, Args&... args) {
//            auto str_ = str.c_str();
//            if (!_parse_next(str_, args...)) return false;
//            return _is_whitespace(str_);
//        }
//        
//        
//        // -----------------------------------------------------------------------------
//        // IMPLEMENTATION OF LOGGING UTILITIES
//        // -----------------------------------------------------------------------------
//        
//        // Logging configutation
//        inline bool& _log_console() {
//            static auto _log_console = true;
//            return _log_console;
//        }
//        inline FILE*& _log_filestream() {
//            static auto _log_filestream = (FILE*)nullptr;
//            return _log_filestream;
//        }
//        
//        // Logs a message
//        inline void log_message(const char* lbl, const char* msg) {
//            if (_log_console()) {
//                printf("%s\n", msg);
//                fflush(stdout);
//            }
//            if (_log_filestream()) {
//                fprintf(_log_filestream(), "%s %s\n", lbl, msg);
//                fflush(_log_filestream());
//            }
//        }
//        
//        // Log info/error/fatal/trace message
//        template <typename... Args>
//        inline void log_info(const string& fmt, const Args&... args) {
//            log_message("INFO ", format(fmt, args...).c_str());
//        }
//        template <typename... Args>
//        inline void log_error(const string& fmt, const Args&... args) {
//            log_message("ERROR", format(fmt, args...).c_str());
//        }
//        template <typename... Args>
//        inline void log_fatal(const string& fmt, const Args&... args) {
//            log_message("FATAL", format(fmt, args...).c_str());
//            exit(1);
//        }
//        
//        // Log traces for timing and program debugging
//        struct log_scope {
//            string  message    = "";
//            int64_t start_time = -1;
//            bool    scoped     = false;
//            ~log_scope();
//        };
//        template <typename... Args>
//        inline void log_trace(const string& fmt, const Args&... args) {
//            log_message("TRACE", format(fmt, args...).c_str());
//        }
//        template <typename... Args>
//        inline log_scope log_trace_begin(const string& fmt, const Args&... args) {
//            auto message = format(fmt, args...);
//            log_trace(message + " [started]");
//            return {message, get_time(), false};
//        }
//        template <typename... Args>
//        inline void log_trace_end(log_scope& scope) {
//            if (scope.start_time >= 0) {
//                log_trace(scope.message + " [ended: " +
//                          format_duration(get_time() - scope.start_time) + "]");
//            } else {
//                log_trace(scope.message + " [ended]");
//            }
//        }
//        template <typename... Args>
//        inline log_scope log_trace_scoped(const string& fmt, const Args&... args) {
//            auto message = format(fmt, args...);
//            log_trace(message + " [started]");
//            return {message, get_time(), true};
//        }
//        inline log_scope::~log_scope() {
//            if (scoped) log_trace_end(*this);
//        }
//        
//        // Configure the logging
//        inline void set_log_console(bool enabled) { _log_console() = enabled; }
//        inline void set_log_file(const string& filename, bool append) {
//            if (_log_filestream()) {
//                fclose(_log_filestream());
//                _log_filestream() = nullptr;
//            }
//            if (filename.empty()) return;
//            _log_filestream() = fopen(filename.c_str(), append ? "at" : "wt");
//        }
//        
//        
//        // -----------------------------------------------------------------------------
//        // IMPLEMENTATION OF STRING FORMAT UTILITIES
//        // -----------------------------------------------------------------------------
//        
//        // Format duration string from nanoseconds
//        inline string format_duration(int64_t duration) {
//            auto elapsed = duration / 1000000;  // milliseconds
//            auto hours   = (int)(elapsed / 3600000);
//            elapsed %= 3600000;
//            auto mins = (int)(elapsed / 60000);
//            elapsed %= 60000;
//            auto secs  = (int)(elapsed / 1000);
//            auto msecs = (int)(elapsed % 1000);
//            char buf[256];
//            sprintf(buf, "%02d:%02d:%02d.%03d", hours, mins, secs, msecs);
//            return buf;
//        }
//        // Format a large integer number in human readable form
//        inline string format_num(uint64_t num) {
//            auto rem = num % 1000;
//            auto div = num / 1000;
//            if (div > 0) return format_num(div) + "," + std::to_string(rem);
//            return std::to_string(rem);
//        }
    } // util
}  // namespace sv

#endif
