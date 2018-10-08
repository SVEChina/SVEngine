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
        
        class SVFairDataBlur : public SVFilterBase {
        public:
            SVFairDataBlur(SVInst *_app);
            
            ~SVFairDataBlur();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            virtual void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            
            virtual f32 getFilterParam(SVFILTERITEMTYPE _type);
            
            SVMtlCorePtr m_mtl_a;
            SVMtlCorePtr m_mtl_b;
            SVMtlCorePtr m_mtl_c;
            SVMtlBlurFairPtr m_mtl_d;
            SVMtlCorePtr m_mtl_e;
            SVMtlCorePtr m_mtl_f;
            SVMtlCorePtr m_mtl_back;
            
            inline void setSmooth(f32 _smooth) {
                m_smooth = _smooth;
            }
            
            inline f32 getSmooth() {
                return m_smooth;
            }
            
        protected:
            f32 m_smooth;
            
        };
        
        class SVFairLtraLow : public SVFilterBase {
        public:
            SVFairLtraLow(SVInst *_app);
            
            ~SVFairLtraLow();
            
            virtual  bool create();
            
            virtual void destroy();
            
            virtual  void update(f32 dt);
            
            virtual void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            
            virtual f32 getFilterParam(SVFILTERITEMTYPE _type);
            
            SVMtlCorePtr m_mtl_a;
            SVMtlCorePtr m_mtl_back;

            
            inline void setSmooth(f32 _smooth) {
                m_smooth = _smooth;
            }
            
            inline f32 getSmooth() {
               return m_smooth;
            }
            
        protected:
            f32 m_smooth;
            
        };
    }
}

#endif /* SV_FACEBEAUTYSHOWFILTER_H */
