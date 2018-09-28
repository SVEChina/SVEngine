//
// SVBasedonFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BASEDONFILTER_H
#define SV_BASEDONFILTER_H

#include "../../base/SVGBase.h"
#include "../../mtl/SVMtlDeclare.h"

namespace sv {
    namespace logic {
        class SVBasedonFilter : public SVGBase {
        public:
            SVBasedonFilter(SVInst *_app);
            
            ~SVBasedonFilter();
            
            void update(f32 dt);
            
            inline void setShadows(f32 _shadows) {
                m_shadows=_shadows;
            }
            
            inline void setHighlights(f32 _highlights) {
                m_highlights = _highlights;
            }
            
            inline void setContrast(f32 _contrast) {
                m_contrast = _contrast;
            }
            
            inline void setSaturation(f32 _saturation) {
                m_saturation = _saturation;
            }
            
            inline void setAcutance(f32 _acutance) {
                m_acutance = _acutance;
            }
            
            inline void setBrightness(f32 _brightness) {
                m_brightness= _brightness;
            }
            
            inline void setWhitening(f32 _whitening) {
                m_whitening= _whitening;
            }
            
            inline void setGamma(f32 _gamma) {
                m_gamma= _gamma;
            }
            
            inline void setRedShift(f32 redShift) {
                m_redShift= redShift;
            }
            
            inline void setGreenShift(f32 _greenShift) {
                m_greenShift= _greenShift;
            }
            
            inline void setBlueShift(f32 _blueShift) {
                m_blueShift= _blueShift;
            }
            
            inline void setSDRedShift(f32 redShift) {
                m_sdredShift= redShift;
            }
            
            inline void setSDGreenShift(f32 _greenShift) {
                m_sdgreenShift= _greenShift;
            }
            
            inline void setSDBlueShift(f32 _blueShift) {
                m_sdblueShift= _blueShift;
            }
            
            inline void setHHRedShift(f32 redShift) {
                m_hhredShift= redShift;
            }
            
            inline void setHHGreenShift(f32 _greenShift) {
                m_hhgreenShift= _greenShift;
            }
            
            inline void setHHBlueShift(f32 _blueShift) {
                m_hhblueShift= _blueShift;
            }
            
            inline void setTemperature(f32 _temperature) {
                m_temperature= _temperature;
            }
            
            inline void setTint(f32 _tint) {
                m_tint= _tint;
            }
            //
            inline f32 getShadows() {
                return m_shadows;
            }
            
            inline f32 getHighlights() {
                return m_highlights;
            }
            
            inline f32 getContrast() {
                return m_contrast;
            }
            
            inline f32 getSaturation() {
                return m_saturation;
            }
            
            inline f32 getAcutance() {
                return m_acutance;
            }
            
            inline f32 getBrightness() {
                return m_brightness;
            }
            
            inline f32 getWhitening() {
                return m_whitening;
            }
            
            inline f32 getGamma() {
                return m_gamma;
            }
            
            inline f32 getRedShift() {
                return m_redShift;
            }
            
            inline f32 getGreenShift() {
                return m_greenShift;
            }
            
            inline f32 getBlueShift() {
                return m_blueShift;
            }
            
            inline f32 getSDRedShift() {
                return m_sdredShift;
            }
            
            inline f32 getSDGreenShift() {
                return m_sdgreenShift;
            }
            
            inline f32 getSDBlueShift() {
                return m_sdblueShift;
            }
            
            inline f32 getHHRedShift() {
                return m_hhredShift;
            }
            
            inline f32 getHHGreenShift( ) {
                return m_hhgreenShift;
            }
            
            inline f32 getHHBlueShift( ) {
                return m_hhblueShift;
            }
            
            inline f32 getTemperature( ) {
                return m_temperature;
            }
            
            inline f32 getTint() {
                return m_tint;
            }
            
            SVMultPassNodePtr m_BasedonFilterNode;
            SVMtlCorePtr m_baseOnmtl;
            SVMtlCorePtr m_acutancemtl;
            
        protected:
            f32 m_shadows;
            f32 m_highlights;
            f32 m_contrast;
            f32 m_saturation;
            f32 m_acutance;
            f32 m_brightness;
            f32 m_whitening;
            f32 m_gamma;
            f32 m_redShift;
            f32 m_greenShift;
            f32 m_blueShift;
            f32 m_sdredShift;
            f32 m_sdgreenShift;
            f32 m_sdblueShift;
            f32 m_hhredShift;
            f32 m_hhgreenShift;
            f32 m_hhblueShift;
            f32 m_temperature;
            f32 m_tint;
        };
    };//!namespace logic
}//!namespace sv
#endif /* SV_BASEDONFILTER_H */
