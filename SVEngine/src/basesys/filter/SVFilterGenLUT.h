//
//  SVFilterGenLUT.hpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/9/25.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_FILTERLOOKUPTABLEADJUST_H
#define SV_FILTERLOOKUPTABLEADJUST_H

#include "SVFilterBase.h"

namespace sv {
    namespace logic {
        
        class SVGenLUTParam :public SVObject {
        public:
            SVGenLUTParam(){
                reset();
            };
            
            void reset(){
                m_saturation=0.0f;
                m_brightness=0.0f;
                m_contrast=0.0f;
                m_vibrance=0.0f;
                
                m_HSLSaturationRed=1.0f;
                m_HSLLightnessRed=1.0f;
                m_HSLHueRed=0.0f;
                
                m_HSLSaturationYellow=1.0f;
                m_HSLLightnessYellow=1.0f;
                m_HSLHueYellow=0.0f;
                
                m_HSLSaturationGreen=1.0f;
                m_HSLLightnessGreen=1.0f;
                m_HSLHueGreen=0.0f;
                
                m_HSLSaturationMagenta=1.0f;
                m_HSLLightnessMagenta=1.0f;
                m_HSLHueMagenta=0.0f;
                
                m_HSLSaturationBlue=1.0f;
                m_HSLLightnessBlue=1.0f;
                m_HSLHueBlue=0.0f;
                
                m_HSLSaturationCyan=1.0f;
                m_HSLLightnessCyan=1.0f;
                m_HSLHueCyan=0.0f;
                
                m_redShift=0.0f;
                m_greenShift=0.0f;
                m_blueShift=0.0f;
                m_sdredShift=0.0f;
                m_sdgreenShift=0.0f;
                m_sdblueShift=0.0f;
                m_hhredShift=0.0f;
                m_hhgreenShift=0.0f;
                m_hhblueShift=0.0f;
                
                m_shadow=0.0;
                m_Highlight=0.0;
                
                m_gamma=1.0;
                
                m_temperature=5000.0;
                
                m_exposure=0.0;
            }
            
            void copy(SVGenLUTParamPtr _p){
                m_saturation = _p->m_saturation;
                m_brightness = _p->m_brightness;
                m_contrast = _p->m_contrast;
                m_vibrance = _p->m_vibrance;
                
                m_HSLSaturationRed=_p->m_HSLSaturationRed;
                m_HSLLightnessRed=_p->m_HSLLightnessRed;
                m_HSLHueRed=_p->m_HSLHueRed;
                
                m_HSLSaturationYellow=_p->m_HSLSaturationYellow;
                m_HSLLightnessYellow=_p->m_HSLLightnessYellow;
                m_HSLHueYellow=_p->m_HSLHueYellow;
                
                m_HSLSaturationGreen=_p->m_HSLSaturationGreen;
                m_HSLLightnessGreen=_p->m_HSLLightnessGreen;
                m_HSLHueGreen=_p->m_HSLHueGreen;
                
                m_HSLSaturationMagenta=_p->m_HSLSaturationMagenta;
                m_HSLLightnessMagenta=_p->m_HSLLightnessMagenta;
                m_HSLHueMagenta=_p->m_HSLHueMagenta;
                
                m_HSLSaturationBlue=_p->m_HSLSaturationBlue;
                m_HSLLightnessBlue=_p->m_HSLLightnessBlue;
                m_HSLHueBlue=_p->m_HSLHueBlue;
                
                m_HSLSaturationCyan=_p->m_HSLSaturationCyan;
                m_HSLLightnessCyan=_p->m_HSLLightnessCyan;
                m_HSLHueCyan=_p->m_HSLHueCyan;
                
                m_redShift=_p->m_redShift;
                m_greenShift=_p->m_greenShift;
                m_blueShift=_p->m_blueShift;
                m_sdredShift=_p->m_sdredShift;
                m_sdgreenShift=_p->m_sdgreenShift;
                m_sdblueShift=_p->m_sdblueShift;
                m_hhredShift=_p->m_hhredShift;
                m_hhgreenShift=_p->m_hhgreenShift;
                m_hhblueShift=_p->m_hhblueShift;
                
                m_shadow=_p->m_shadow;
                m_Highlight=_p->m_Highlight;
                
                m_gamma=_p->m_gamma;
                
                m_temperature=_p->m_temperature;
                m_tint=_p->m_tint;
                
                m_exposure=_p->m_exposure;
                
            }
            
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
            
            f32 m_gamma;
            
            f32 m_temperature;
            f32 m_tint;
            
            f32 m_exposure;
            
            SVString m_data;
            
        };
        
        class SVFilterGenLUT : public SVFilterBase {
        public:
            SVFilterGenLUT(SVInst *_app);
            
            ~SVFilterGenLUT();
            
            virtual  bool create();
            
            virtual  void destroy();
            
            virtual  void update(f32 dt);
            
            inline void setBrightness(f32 _brightness){
                 m_genParam->m_brightness=_brightness;
            }
            
            inline void setContrast(f32 _contrast){
                 m_genParam->m_contrast=_contrast;
            }
            
            inline void setSaturation(f32 _saturation){
                 m_genParam->m_saturation=_saturation;
            }
            
            inline void setVibrance(f32 _vibrance){
                 m_genParam->m_vibrance=_vibrance;
            }
            
            inline void setGamma(f32 _gamma){
                m_genParam->m_gamma=_gamma;
            }
            
            inline void setTemperature(f32 _temperature){
                m_genParam->m_temperature=_temperature;
            }
            
            inline void setTint(f32 _tint){
                m_genParam->m_tint=_tint;
            }
            
            inline void setExposure(f32 _exposure){
                m_genParam->m_exposure=_exposure;
            }
            
            inline void setHSLSaturationRed(f32 _saturation){
                 m_genParam->m_HSLSaturationRed=_saturation;
            }
            
            inline void setHSLLightnessRed(f32 _lightness){
                 m_genParam->m_HSLLightnessRed=_lightness;
            }
            
            inline void setHSLHueRed(f32 _hue){
                 m_genParam->m_HSLHueRed=_hue;
            }
            
            inline void setHSLSaturationYellow(f32 _saturation){
                 m_genParam->m_HSLSaturationYellow=_saturation;
            }
            
            inline void setHSLLightnessYellow(f32 _lightness){
                 m_genParam->m_HSLLightnessYellow=_lightness;
            }
            
            inline void setHSLHueYellow(f32 _hue){
                 m_genParam->m_HSLHueYellow=_hue;
            }
            
            inline void setHSLSaturationGreen(f32 _saturation){
                 m_genParam->m_HSLSaturationGreen=_saturation;
            }
            
            inline void setHSLLightnessGreen(f32 _lightness){
                 m_genParam->m_HSLLightnessGreen=_lightness;
            }
            
            inline void setHSLHueGreen(f32 _hue){
                 m_genParam->m_HSLHueGreen=_hue;
            }
            
            inline void setHSLSaturationMagenta(f32 _saturation){
                 m_genParam->m_HSLSaturationMagenta=_saturation;
            }
            
            inline void setHSLLightnessMagenta(f32 _lightness){
                 m_genParam->m_HSLLightnessMagenta=_lightness;
            }
            
            inline void setHSLHueMagenta(f32 _hue){
                 m_genParam->m_HSLHueMagenta=_hue;
            }
            
            inline void setHSLSaturationBlue(f32 _saturation){
                 m_genParam->m_HSLSaturationBlue=_saturation;
            }
            
            inline void setHSLLightnessBlue(f32 _lightness){
                 m_genParam->m_HSLLightnessBlue=_lightness;
            }
            
            inline void setHSLHueBlue(f32 _hue){
                 m_genParam->m_HSLHueBlue=_hue;
            }
            
            inline void setHSLSaturationCyan(f32 _saturation){
                 m_genParam->m_HSLSaturationCyan=_saturation;
            }
            
            inline void setHSLLightnessCyan(f32 _lightness){
                 m_genParam->m_HSLLightnessCyan=_lightness;
            }
            
            inline void setHSLHueCyan(f32 _hue){
                 m_genParam->m_HSLHueCyan=_hue;
            }
            
            inline void setRedShift(f32 _redShift){
                 m_genParam->m_redShift=_redShift;
            }
            inline void setBlueShift(f32 _blueShift){
                 m_genParam->m_blueShift=_blueShift;
            }
            inline void setGreenShift(f32 _greenShift){
                 m_genParam->m_greenShift=_greenShift;
            }
            inline void setSDRedShift(f32 _sdredShift){
                 m_genParam->m_sdredShift=_sdredShift;
            }
            inline void setSDGreenShift(f32 _sdgreenShift){
                 m_genParam->m_sdgreenShift=_sdgreenShift;
            }
            inline void setSDBlueShift(f32 _sdblueShift){
                 m_genParam->m_sdblueShift=_sdblueShift;
            }
            inline void setHHRedShift(f32 _hhredShift){
                 m_genParam->m_hhredShift=_hhredShift;
            }
            inline void setHHGreenShift(f32 _hhgreenShift){
                 m_genParam->m_hhgreenShift=_hhgreenShift;
            }
            inline void setHHBlueShift(f32 _hhblueShift){
                 m_genParam->m_hhblueShift=_hhblueShift;
            }
            
            inline void setShadow(f32 _shadow){
                 m_genParam->m_shadow=_shadow;
            }
            
            inline void setHighlight(f32 _highlight){
                 m_genParam->m_Highlight=_highlight;
            }
            
            void setLUTData(ptr8  data,u32 size);
            
            SVTexturePtr getOutTex();
            
            void refreshFData(SVGenLUTParamPtr _param);
            
            SVGenLUTParamPtr getLUTparam(){
                return m_genParam;
            }
            
        protected:
            SVMtlBrightnessContrastPtr m_BCMtl;//brightness contrast
            SVMtlSaturationPtr m_SaturationMtl;
            SVMtlVibrancePtr m_VibranceMtl;
            SVMtlColorBalancePtr m_colorBalanceMtl;
            SVMtlShadowHighlightPtr m_shadowHighlightMtl;
            SVMtlWhiteBalancePtr m_whiteBalanceMtl;
            SVMtlGammaPtr m_gammaMtl;
            SVMtlExposurePtr m_exposureMtl;
            SVMtlHSLPtr m_hslMtl;
            SVGenLUTParamPtr m_genParam;
            
        public:
            //序列化
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                                RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        };
        
    };//!namespace logic
}//!namespace sv


#endif /* SV_FILTERLOOKUPTABLEADJUST_H */
