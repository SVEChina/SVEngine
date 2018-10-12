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
            
            inline void setHSLSaturation(f32 _saturation){
                m_HSLSaturation=_saturation;
            }
            
            inline void setHSLLightness(f32 _lightness){
                m_HSLLightness=_lightness;
            }
            
            inline void setHSLHue(f32 _hue){
                m_HSLHue=_hue;
            }
            
            inline void setHSLHueRaduis(f32 _hueRaduis){
                m_HSLHueRaduis=_hueRaduis;
            }
            
            inline SVTexturePtr getOutTex(){
                return outTex;
            }
            
        protected:
            SVMtlBrightnessContrastPtr m_BCMtl;//brightness contrast
            SVMtlSaturationPtr m_SaturationMtl;
            SVMtlVibrancePtr m_VibranceMtl;
            SVMtlHSLPtr m_hslMtl;
            
            SVPassPtr m_passSaturation;
            SVPassPtr m_passVibrance;
            SVPassPtr m_passHSL;
            SVPassPtr m_passBC;
            SVPassPtr m_passBack;
            SVTexturePtr outTex;
            SVTexturePtr inTex;
            f32 m_brightness;
            f32 m_contrast;
            f32 m_saturation;
            f32 m_vibrance;
            f32 m_HSLSaturation;
            f32 m_HSLLightness;
            f32 m_HSLHue;
            f32 m_HSLHueRaduis;
            
        };
    };//!namespace logic
}//!namespace sv


#endif /* SV_FILTERLOOKUPTABLEADJUST_H */
