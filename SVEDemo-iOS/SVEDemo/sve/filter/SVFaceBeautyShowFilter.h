//
// SVFaceBeautyShowFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FACEBEAUTYSHOWFILTER_H
#define SV_FACEBEAUTYSHOWFILTER_H


#include "basesys/filter/SVFilterBase.h"
#include "mtl/SVMtlDeclare.h"
#include "node/SVNodeDeclare.h"

namespace sv {

    namespace logic {

        class SVFairData : public SVFilterBase {
        public:
            SVFairData(SVInst *_app);

            ~SVFairData();
            
            virtual bool create();
            
            virtual void destroy();

            virtual void update(f32 dt);
            
            virtual void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            
            virtual f32 getFilterParam(SVFILTERITEMTYPE _type);

            SVMtlFilterBrilaterlPtr m_mtl_a;
            SVMtlFilterBrilaterl2Ptr m_mtl_b;
            SVMtlGaussianPtr m_mtl_c;
            SVMtlSmoothPtr m_mtl_blur01;
            SVMtlSmoothPtr m_mtl_blur02;
            SVMtlSmoothPtr m_mtl_mix;
            SVMtlCorePtr m_mtl_back;

            inline void setRadius(f32 _radius) {
                m_radius = _radius;
            }
            
            inline f32 getRadius() {
                return m_radius;
            }

            inline void setDistanceV(f32 _distanceV) {
                m_distanceV = _distanceV;
            }

            inline void setDistanceH(f32 _distanceH) {
                m_distanceH = _distanceH;
            }

            inline void setWidth_factor(f32 _width_factor) {
                m_width_factor = _width_factor;
            }

            inline void setHeight_factor(f32 _height_factor) {
                m_height_factor = _height_factor;
            }

        protected:
            f32 m_radius;

            f32 m_distanceV;

            f32 m_distanceH;

            f32 m_width_factor;

            f32 m_height_factor;
        };

        class SVFairDataLows : public SVFilterBase {
        public:
            SVFairDataLows(SVInst *_app);

            ~SVFairDataLows();

            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            virtual void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            
            virtual f32 getFilterParam(SVFILTERITEMTYPE _type);

            SVMtlFilterBrilaterlPtr m_mtl_a;
            SVMtlFilterBrilaterlPtr m_mtl_b;
            SVMtlBlurPtr m_mtl_c;
            SVMtlCorePtr m_mtl_back;

            inline void setRadius(f32 _radius) {
                m_radius = _radius;
            }
            
            inline f32 getRadius() {
                return m_radius;
            }

            inline void setDistanceV(f32 _distanceV) {
                m_distanceV = _distanceV;
            }

            inline void setDistanceH(f32 _distanceH) {
                m_distanceH = _distanceH;
            }

            inline void setWidth_factor(f32 _width_factor) {
                m_width_factor = _width_factor;
            }

            inline void setHeight_factor(f32 _height_factor) {
                m_height_factor = _height_factor;
            }

        protected:
            f32 m_radius;

            f32 m_distanceV;

            f32 m_distanceH;

            f32 m_width_factor;

            f32 m_height_factor;
        };
        
    }
}

#endif /* SV_FACEBEAUTYSHOWFILTER_H */
