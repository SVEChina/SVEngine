//
//  SVFilterLookUpTableAdjust.hpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/9/25.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_FILTERLOOKUPTABLEADJUST_H
#define SV_FILTERLOOKUPTABLEADJUST_H
#include "SVFilterBase.h"
#include "../../mtl/SVMtlDeclare.h"
#include "../../node/SVNodeDeclare.h"

namespace sv {
    namespace logic {
        class SVFilterLookUpTableAdjust : public SVFilterBase {
        public:
            SVFilterLookUpTableAdjust(SVInst *_app);
            
            ~SVFilterLookUpTableAdjust();
            
            virtual  bool create();
            
            virtual  void destroy();
            
            virtual  void update(f32 dt);
            
            inline void setBrightness(f32 _brightness){
                m_brightness=_brightness;
            }
            
            inline void setContrast(f32 _contrast){
                m_contrast=_contrast;
            }
            
            inline void setSaturation(f32 _saturation){
                m_saturation=_saturation;
            }
            
            inline void setVibrance(f32 _vibrance){
                m_vibrance=_vibrance;
            }
            
            inline void setHSLSaturationRed(f32 _saturation){
                m_HSLSaturationRed=_saturation;
            }
            
            inline void setHSLLightnessRed(f32 _lightness){
                m_HSLLightnessRed=_lightness;
            }
            
            inline void setHSLHueRed(f32 _hue){
                m_HSLHueRed=_hue;
            }
            
            inline void setHSLSaturationYellow(f32 _saturation){
                m_HSLSaturationYellow=_saturation;
            }
            
            inline void setHSLLightnessYellow(f32 _lightness){
                m_HSLLightnessYellow=_lightness;
            }
            
            inline void setHSLHueYellow(f32 _hue){
                m_HSLHueYellow=_hue;
            }
            
            inline void setHSLSaturationGreen(f32 _saturation){
                m_HSLSaturationGreen=_saturation;
            }
            
            inline void setHSLLightnessGreen(f32 _lightness){
                m_HSLLightnessGreen=_lightness;
            }
            
            inline void setHSLHueGreen(f32 _hue){
                m_HSLHueGreen=_hue;
            }
            
            inline void setHSLSaturationMagenta(f32 _saturation){
                m_HSLSaturationMagenta=_saturation;
            }
            
            inline void setHSLLightnessMagenta(f32 _lightness){
                m_HSLLightnessMagenta=_lightness;
            }
            
            inline void setHSLHueMagenta(f32 _hue){
                m_HSLHueMagenta=_hue;
            }
            
            inline void setHSLSaturationBlue(f32 _saturation){
                m_HSLSaturationBlue=_saturation;
            }
            
            inline void setHSLLightnessBlue(f32 _lightness){
                m_HSLLightnessBlue=_lightness;
            }
            
            inline void setHSLHueBlue(f32 _hue){
                m_HSLHueBlue=_hue;
            }
            
            inline void setHSLSaturationCyan(f32 _saturation){
                m_HSLSaturationCyan=_saturation;
            }
            
            inline void setHSLLightnessCyan(f32 _lightness){
                m_HSLLightnessCyan=_lightness;
            }
            
            inline void setHSLHueCyan(f32 _hue){
                m_HSLHueCyan=_hue;
            }
            
            inline void setRedShift(f32 _redShift){
                m_redShift=_redShift;
            }
            inline void setBlueShift(f32 _blueShift){
                m_blueShift=_blueShift;
            }
            inline void setGreenShift(f32 _greenShift){
                m_greenShift=_greenShift;
            }
            inline void setSDRedShift(f32 _sdredShift){
                m_sdredShift=_sdredShift;
            }
            inline void setSDGreenShift(f32 _sdgreenShift){
                m_sdgreenShift=_sdgreenShift;
            }
            inline void setSDBlueShift(f32 _sdblueShift){
                m_sdblueShift=_sdblueShift;
            }
            inline void setHHRedShift(f32 _hhredShift){
                m_hhredShift=_hhredShift;
            }
            inline void setHHGreenShift(f32 _hhgreenShift){
                m_hhgreenShift=_hhgreenShift;
            }
            inline void setHHBlueShift(f32 _hhblueShift){
                m_hhblueShift=_hhblueShift;
            }
            
            inline void setShadow(f32 _shadow){
                m_shadow=_shadow;
            }
            
            inline void setHighlight(f32 _highlight){
                m_Highlight=_highlight;
            }
            
            inline SVTexturePtr getOutTex(){
                return m_outTex;
            }
            
        protected:
            SVMtlBrightnessContrastPtr m_BCMtl;//brightness contrast
            SVMtlSaturationPtr m_SaturationMtl;
            SVMtlVibrancePtr m_VibranceMtl;
            SVMtlColorBalancePtr m_colorBalanceMtl;
            SVMtlShadowHighlightPtr m_shadowHighlightMtl;
            SVMtlHSLPtr m_hslMtl;
            
            SVTexturePtr m_outTex;
            
            f32 m_brightness;
            f32 m_contrast;
            f32 m_saturation;
            f32 m_vibrance;
           
            f32 m_HSLSaturationRed;
            f32 m_HSLLightnessRed;
            f32 m_HSLHueRed;
            
            f32 m_HSLSaturationYellow;
            f32 m_HSLLightnessYellow;
            f32 m_HSLHueYellow;
            
            f32 m_HSLSaturationGreen;
            f32 m_HSLLightnessGreen;
            f32 m_HSLHueGreen;
            
            f32 m_HSLSaturationMagenta;
            f32 m_HSLLightnessMagenta;
            f32 m_HSLHueMagenta;
            
            f32 m_HSLSaturationBlue;
            f32 m_HSLLightnessBlue;
            f32 m_HSLHueBlue;
            
            f32 m_HSLSaturationCyan;
            f32 m_HSLLightnessCyan;
            f32 m_HSLHueCyan;
            
            f32 m_redShift;
            f32 m_greenShift;
            f32 m_blueShift;
            f32 m_sdredShift;
            f32 m_sdgreenShift;
            f32 m_sdblueShift;
            f32 m_hhredShift;
            f32 m_hhgreenShift;
            f32 m_hhblueShift;
            
            f32 m_shadow;
            f32 m_Highlight;
            
            
            
            
        };
    };//!namespace logic
}//!namespace sv


#endif /* SV_FILTERLOOKUPTABLEADJUST_H */
