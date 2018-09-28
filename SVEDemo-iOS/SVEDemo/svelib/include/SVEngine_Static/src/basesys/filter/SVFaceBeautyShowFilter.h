//
// SVFaceBeautyShowFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FACEBEAUTYSHOWFILTER_H
#define SV_FACEBEAUTYSHOWFILTER_H

#include "../../base/SVGBase.h"
#include "../../mtl/SVMtlDeclare.h"

namespace sv {

    namespace logic {

        class SVFairData : public SVGBase {
        public:
            SVFairData(SVInst *_app);

            ~SVFairData();

            void update(f32 dt);

            SVTexturePtr m_tex_a;
            SVTexturePtr m_tex_b;
            SVMtlCorePtr m_mtl_a;
            SVMtlCorePtr m_mtl_b;
            SVMtlCorePtr m_mtl_c;
            SVMtlCorePtr m_mtl_d;
            SVMultPassNodePtr m_fairNode;

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

        class SVFairDataLows : public SVGBase {
        public:
            SVFairDataLows(SVInst *_app);

            ~SVFairDataLows();

            void update(f32 dt);

            SVTexturePtr m_tex_a;
            SVTexturePtr m_tex_b;
            SVMtlCorePtr m_mtl_a;
            SVMtlCorePtr m_mtl_b;
            SVMtlCorePtr m_mtl_c;
            SVMtlCorePtr m_mtl_d;
            SVMtlCorePtr m_mtl_e;
            SVMultPassNodePtr m_fairNode;

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
        
        class SVFairDataBlur : public SVGBase {
        public:
            SVFairDataBlur(SVInst *_app);
            
            ~SVFairDataBlur();
            
            void update(f32 dt);
            
            SVTexturePtr m_tex_a;
            SVTexturePtr m_tex_b;
            SVTexturePtr m_tex_c;
            SVMtlCorePtr m_mtl_a;
            SVMtlCorePtr m_mtl_b;
            SVMtlCorePtr m_mtl_c;
            SVMtlCorePtr m_mtl_d;
            SVMtlCorePtr m_mtl_e;
            SVMtlCorePtr m_mtl_f;
            SVMultPassNodePtr m_fairNode;
            
            inline void setSmooth(f32 _smooth) {
                m_smooth = _smooth;
            }
            
            inline f32 getSmooth() {
                return m_smooth;
            }
            
        protected:
            f32 m_smooth;
            
        };
        
        class SVFairLtraLow : public SVGBase {
        public:
            SVFairLtraLow(SVInst *_app);
            
            ~SVFairLtraLow();
            
            void update(f32 dt);
            
            SVMtlCorePtr m_mtl_a;

            SVMultPassNodePtr m_fairNode;
            
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
