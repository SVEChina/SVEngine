//
// SVMtlBasedOn.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlBasedOn : public SVMtlCore {
        public:
            SVMtlBasedOn(SVInst *_app);
            
            SVMtlBasedOn(SVMtlBasedOn* _mtl);
            
            ~SVMtlBasedOn();
            
            virtual SVMtlCorePtr clone();
            
            void setShadows(f32 _shadows);
            
            void setHighlights(f32 _highlights);
            
            void setContrast(f32 _contrast);
            
            void setSaturation(f32 _saturation);
            
            void setBrightness(f32 _brightness);
            
            void setWhitening(f32 _whitening);
            
            void setGamma(f32 _gamma);
            
            void setRedShift(f32 redShift);
            
            void setGreenShift(f32 _greenShift);
            
            void setBlueShift(f32 _blueShift);
            
            void setSDRedShift(f32 redShift);
            
            void setSDGreenShift(f32 _greenShift);
            
            void setSDBlueShift(f32 _blueShift);
            
            void setHHRedShift(f32 redShift);
            
            void setHHGreenShift(f32 _greenShift);
            
            void setHHBlueShift(f32 _blueShift);
            
            void setTemperature(f32 _temperature);
            
            void setTint(f32 _tint);
            
        protected:
            void _submitMtl(SVRendererBasePtr _render);
            
            s32 m_BaseOnFlag;
            //
            f32 m_shadows;
            f32 m_highlights;
            f32 m_contrast;
            f32 m_saturation;
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
        
        class SVMtlAcutance : public SVMtlCore {
        public:
            SVMtlAcutance(SVInst *_app);
            
            SVMtlAcutance(SVMtlAcutance *_mtl);
            
            ~SVMtlAcutance();
            
            virtual SVMtlCorePtr clone();
            
            void setAcutance(f32 _acutance);
            
        protected:
            void _submitMtl(SVRendererBasePtr _render);
            
        protected:
            s32 m_BaseOnFlag;
            //
            f32 m_acutance;
        };
    }
}
