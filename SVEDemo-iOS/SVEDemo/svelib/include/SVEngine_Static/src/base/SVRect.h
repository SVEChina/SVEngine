//
// SVRect.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RECT_H
#define SV_RECT_H

#include "SVTypes.h"
#include "SVVec2.h"

namespace sv {

    namespace util {

        struct SVRect {

            SVRect()
            : m_lb_pt(0, 0)
            , m_rt_pt(0, 0) {
                
            }
            
            //! Constructor with two corners
            SVRect(f32 x, f32 y, f32 x2, f32 y2)
            : m_lb_pt(x, y)
            , m_rt_pt(x2, y2) {
                
            }
            
            //! Constructor with two corners
            SVRect(const FVec2 &upperLeft, const FVec2 &lowerRight)
            : m_lb_pt(upperLeft)
            , m_rt_pt(lowerRight) {
                
            }
            
//            //! Constructor with upper left corner and dimension
//            template<class U>
//            SVRect(const SVPosition2d<T> &pos, const SVDimension2d<U> &size)
//            : m_lb_pt(pos),
//            m_rt_pt(pos.X + size.Width, pos.Y + size.Height) {
//
//            }
//
//            //! Constructor with upper left at 0,0 and lower right using dimension
//            template<class U>
//            explicit SVRect(const SVDimension2d<U> &size)
//            : m_lb_pt(0, 0)
//            , m_rt_pt(size.Width, size.Height) {
//
//            }
            
            //! move right by given numbers
            SVRect operator+(const FVec2 &pos) const {
                SVRect ret(*this);
                return ret += pos;
            }
            
            //! move right by given numbers
            SVRect &operator+=(const FVec2 &pos) {
                m_lb_pt += pos;
                m_rt_pt += pos;
                return *this;
            }
            
            //! move left by given numbers
            SVRect operator-(const FVec2 &pos) const {
                SVRect ret(*this);
                return ret -= pos;
            }
            
            //! move left by given numbers
            SVRect &operator-=(const FVec2 &pos) {
                m_lb_pt -= pos;
                m_rt_pt -= pos;
                return *this;
            }
            
            //! equality operator
            bool operator==(const SVRect &other) const {
                return (m_lb_pt == other.m_lb_pt &&
                        m_rt_pt == other.m_rt_pt);
            }
            
            //! inequality operator
            bool operator!=(const SVRect &other) const {
                return (m_lb_pt != other.m_lb_pt ||
                        m_rt_pt != other.m_rt_pt);
            }
            
            bool operator<(const SVRect &other) const {
                return getArea() < other.getArea();
            }
            
            f32 getArea() const {
                return getWidth() * getHeight();
            }
            
            bool isPointInside(const FVec2 &pos) const {
                return (m_lb_pt.x <= pos.x &&
                        m_lb_pt.y <= pos.y &&
                        m_rt_pt.x >= pos.x &&
                        m_rt_pt.y >= pos.y);
            }
            

            bool isRectCollided(const SVRect &other) const {
                return (m_rt_pt.y > other.m_lb_pt.y &&
                        m_lb_pt.y < other.m_rt_pt.y &&
                        m_rt_pt.x > other.m_lb_pt.x &&
                        m_lb_pt.x < other.m_rt_pt.x);
            }
            
            void clipAgainst(const SVRect &other) {
                if (other.m_rt_pt.x < m_rt_pt.x)
                    m_rt_pt.x = other.m_rt_pt.x;
                if (other.m_rt_pt.y < m_rt_pt.y)
                    m_rt_pt.y = other.m_rt_pt.y;
                
                if (other.m_lb_pt.x > m_lb_pt.x)
                    m_lb_pt.x = other.m_lb_pt.x;
                if (other.m_lb_pt.y > m_lb_pt.y)
                    m_lb_pt.y = other.m_lb_pt.y;
                
                // correct possible invalid SVRect resulting from clipping
                if (m_lb_pt.y > m_rt_pt.y)
                    m_lb_pt.y = m_rt_pt.y;
                if (m_lb_pt.x > m_rt_pt.x)
                    m_lb_pt.x = m_rt_pt.x;
            }
            
            bool constrainTo(const SVRect &other) {
                if (other.getWidth() < getWidth() || other.getHeight() < getHeight())
                    return false;
                
                f32 diff = other.m_rt_pt.x - m_rt_pt.x;
                if (diff < 0) {
                    m_rt_pt.x += diff;
                    m_lb_pt.x += diff;
                }
                
                diff = other.m_rt_pt.y - m_rt_pt.y;
                if (diff < 0) {
                    m_rt_pt.y += diff;
                    m_lb_pt.y += diff;
                }
                
                diff = m_lb_pt.x - other.m_lb_pt.x;
                if (diff < 0) {
                    m_lb_pt.x -= diff;
                    m_rt_pt.x -= diff;
                }
                
                diff = m_lb_pt.y - other.m_lb_pt.y;
                if (diff < 0) {
                    m_lb_pt.y -= diff;
                    m_rt_pt.y -= diff;
                }
                
                return true;
            }
            
            //! Get width of rectangle.
            f32 getWidth() const {
                return m_rt_pt.x - m_lb_pt.x;
            }
            
            //! Get height of rectangle.
            f32 getHeight() const {
                return m_rt_pt.y - m_lb_pt.y;
            }
            
            void repair() {
                if (m_rt_pt.x < m_lb_pt.x) {
                    f32 t = m_rt_pt.x;
                    m_rt_pt.x = m_lb_pt.x;
                    m_lb_pt.x = t;
                }
                
                if (m_rt_pt.y < m_lb_pt.y) {
                    f32 t = m_rt_pt.y;
                    m_rt_pt.y = m_lb_pt.y;
                    m_lb_pt.y = t;
                }
            }
            
            bool isValid() const {
                return ((m_rt_pt.x >= m_lb_pt.x) &&
                        (m_rt_pt.y >= m_lb_pt.y));
            }
            
            //! Get the center of the rectangle
            FVec2 getCenter() const {
                return FVec2((m_lb_pt.x + m_rt_pt.x) * 0.5f,
                             (m_lb_pt.y + m_rt_pt.y) * 0.5f);
            }
            
//            //! Get the dimensions of the rectangle
//            SVDimension2d<T> getSize() const {
//                return SVDimension2d<T>(getWidth(), getHeight());
//            }
//
            void addInternalPoint(const FVec2 &p) {
                addInternalPoint(p.x, p.y);
            }
            
            void addInternalPoint(f32 x, f32 y) {
                if (x > m_rt_pt.x)
                    m_rt_pt.x = x;
                if (y > m_rt_pt.y)
                    m_rt_pt.y = y;
                
                if (x < m_lb_pt.x)
                    m_lb_pt.x = x;
                if (y < m_lb_pt.y)
                    m_lb_pt.y = y;
            }
            
            //! Upper left corner
            FVec2 m_lb_pt;
            //! Lower right corner
            FVec2 m_rt_pt;
        };
        
    //!namespace util
    }
}//!namespace sv

#endif
