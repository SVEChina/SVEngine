//
// SVLine3d.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
// copy from irr
//

#ifndef SV_LINE_3D_H
#define SV_LINE_3D_H

#include "SVTypes.h"
#include "SVVector3d.h"

namespace sv {
    
    namespace util{
        //! 3D line between two points with intersection methods.
        template<class T>
        class SVLine3d {
        public:
            
            //! Default constructor
            /** line from (0,0,0) to (1,1,1) */
            SVLine3d() : start(0, 0, 0), end(1, 1, 1) {}
            
            //! Constructor with two points
            SVLine3d(T xa, T ya, T za, T xb, T yb, T zb) : start(xa, ya, za), end(xb, yb, zb) {}
            
            //! Constructor with two points as vectors
            SVLine3d(const SVVector3d<T> &start, const SVVector3d<T> &end) : start(start), end(end) {}
            
            // operators
            
            SVLine3d<T> operator+(const SVVector3d<T> &point) const {
                return SVLine3d<T>(start + point, end + point);
            }
            
            SVLine3d<T> &operator+=(const SVVector3d<T> &point) {
                start += point;
                end += point;
                return *this;
            }
            
            SVLine3d<T> operator-(const SVVector3d<T> &point) const {
                return SVLine3d<T>(start - point, end - point);
            }
            
            SVLine3d<T> &operator-=(const SVVector3d<T> &point) {
                start -= point;
                end -= point;
                return *this;
            }
            
            bool operator==(const SVLine3d<T> &other) const {
                return (start == other.start && end == other.end) ||
                (end == other.start && start == other.end);
            }
            
            bool operator!=(const SVLine3d<T> &other) const {
                return !(start == other.start && end == other.end) ||
                (end == other.start && start == other.end);
            }
            
            // functions
            //! Set this line to a new line going through the two points.
            void setLine(const T &xa, const T &ya, const T &za, const T &xb, const T &yb, const T &zb) {
                start.set(xa, ya, za);
                end.set(xb, yb, zb);
            }
            
            //! Set this line to a new line going through the two points.
            void setLine(const SVVector3d<T> &nstart, const SVVector3d<T> &nend) {
                start.set(nstart);
                end.set(nend);
            }
            
            //! Set this line to new line given as parameter.
            void setLine(const SVLine3d<T> &line) {
                start.set(line.start);
                end.set(line.end);
            }
            
            //! Get length of line
            /** \return Length of line. */
            T getLength() const { return start.getDistanceFrom(end); }
            
            //! Get squared length of line
            /** \return Squared length of line. */
            T getLengthSQ() const { return start.getDistanceFromSQ(end); }
            
            //! Get middle of line
            /** \return Center of line. */
            SVVector3d<T> getMiddle() const {
                return (start + end) / (T) 2;
            }
            
            //! Get vector of line
            /** \return vector of line. */
            SVVector3d<T> getVector() const {
                return end - start;
            }
            
            //! Check if the given point is between start and end of the line.
            /** Assumes that the point is already somewhere on the line.
             \param point The point to test.
             \return True if point is on the line between start and end, else false.
             */
            bool isPointBetweenStartAndEnd(const SVVector3d<T> &point) const {
                return point.isBetweenPoints(start, end);
            }
            
            //! Get the closest point on this line to a point
            /** \param point The point to compare to.
             \return The nearest point which is part of the line. */
            SVVector3d<T> getClosestPoint(const SVVector3d<T> &point) const {
                SVVector3d<T> c = point - start;
                SVVector3d<T> v = end - start;
                T d = (T) v.getLength();
                v /= d;
                T t = v.dotProduct(c);
                
                if (t < (T) 0.0)
                    return start;
                if (t > d)
                    return end;
                
                v *= t;
                return start + v;
            }
            
            //! Check if the line intersects with a sphere
            /** \param sorigin: Origin of the sphere.
             \param sradius: Radius of the sphere.
             \param outdistance: The distance to the first intersection point.
             \return True if there is an intersection.
             If there is one, the distance to the first intersection point
             is stored in outdistance. */
            bool getIntersectionWithSphere(const SVVector3d<T> &sorigin, T sradius, f64 &outdistance) const {
                const SVVector3d<T> q = sorigin - start;
                T c = q.getLength();
                T v = q.dotProduct(getVector().normalize());
                T d = sradius * sradius - (c * c - v * v);
                if (d < 0.0)
                    return false;
                outdistance = v - squareroot(d);
                return true;
            }
            
            // member variables
            //! Start point of line
            SVVector3d<T> start;
            //! End point of line
            SVVector3d<T> end;
        };
        
        //! Typedef for an f32 line.
        typedef SVLine3d<f32> line3df;
        //! Typedef for an integer line.
        typedef SVLine3d<s32> line3di;
        
    }//!namespace util
}//!namespace sv

#endif

