//
//  SVBrightnessMtl.hpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/9/25.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_MTLBRIGHTNESSCONTRAST_H
#define SV_MTLBRIGHTNESSCONTRAST_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlBrightnessContrast : public SVMtlCore  {
        public:
            SVMtlBrightnessContrast(SVInst *_app);
            
            SVMtlBrightnessContrast(SVMtlBrightnessContrast* _mtl);
            
            ~SVMtlBrightnessContrast();
            
            virtual SVMtlCorePtr clone();
            
            void setBrightness(f32 _smooth);
            
            void setContrast(f32 _smooth);
            
        protected:
            virtual void _submitMtl(SVRendererBasePtr _render);
            //
            f32 m_brightness;
            f32 m_contrast;
        };
        
        class SVMtlSaturation : public SVMtlCore  {
        public:
            SVMtlSaturation(SVInst *_app);
            
            SVMtlSaturation(SVMtlSaturation* _mtl);
            
            ~SVMtlSaturation();
            
            virtual SVMtlCorePtr clone();
            
            void setSaturation(f32 _smooth);
            
        protected:
            virtual void _submitMtl(SVRendererBasePtr _render);
            //
            f32 m_saturation;
        };
        
        class SVMtlVibrance : public SVMtlCore  {
        public:
            SVMtlVibrance(SVInst *_app);
            
            SVMtlVibrance(SVMtlVibrance* _mtl);
            
            ~SVMtlVibrance();
            
            virtual SVMtlCorePtr clone();
            
            void setVibrance(f32 _smooth);
            
        protected:
            virtual void _submitMtl(SVRendererBasePtr _render);
            //
            f32 m_vibrance;
        };
        
        class SVMtlShadowHighlight : public SVMtlCore  {
        public:
            SVMtlShadowHighlight(SVInst *_app);
            
            SVMtlShadowHighlight(SVMtlShadowHighlight* _mtl);
            
            ~SVMtlShadowHighlight();
            
            virtual SVMtlCorePtr clone();
            
            void setShadow(f32 _smooth){
                m_shadow=_smooth;
            }
            
            void setHighlight(f32 _smooth){
                m_HighLight=_smooth;
            }
            
        protected:
            virtual void _submitMtl(SVRendererBasePtr _render);
            //
            f32 m_shadow;
            f32 m_HighLight;
            
        };
        
        class SVMtlColorBalance: public SVMtlCore  {
        public:
            SVMtlColorBalance(SVInst *_app);
            
            SVMtlColorBalance(SVMtlColorBalance* _mtl);
            
            ~SVMtlColorBalance();
            
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
            
            virtual SVMtlCorePtr clone();
            
        protected:
            virtual void _submitMtl(SVRendererBasePtr _render);
            f32 m_redShift;
            f32 m_greenShift;
            f32 m_blueShift;
            f32 m_sdredShift;
            f32 m_sdgreenShift;
            f32 m_sdblueShift;
            f32 m_hhredShift;
            f32 m_hhgreenShift;
            f32 m_hhblueShift;
        };
        
        
        class SVMtlHSL : public SVMtlCore  {
        public:
            SVMtlHSL(SVInst *_app);
            
            SVMtlHSL(SVMtlHSL* _mtl);
            
            ~SVMtlHSL();
            
            virtual SVMtlCorePtr clone();

            inline void setSaturationRed(f32 _smooth){
                m_saturationRed=_smooth;
            }

            inline  void setHueRed(f32 _smooth){
                m_HueRed=_smooth;
            }

            inline  void setLightnessRed(f32 _smooth){
                m_LightnessRed=_smooth;
            }

            inline void setSaturationYellow(f32 _smooth){
                m_saturationYellow=_smooth;
            }

            inline void setHueYellow(f32 _smooth){
                m_HueYellow=_smooth;
            }

            inline void setLightnessYellow(f32 _smooth){
                m_LightnessYellow=_smooth;
            }

            inline  void setSaturationGreen(f32 _smooth){
                m_saturationGreen=_smooth;
            }

            inline void setHueGreen(f32 _smooth){
                m_HueGreen=_smooth;
            }

            inline void setLightnessGreen(f32 _smooth){
                m_LightnessGreen=_smooth;
            }

            inline void setSaturationMagenta(f32 _smooth){
                m_saturationMagenta=_smooth;
            }

            inline void setHueMagenta(f32 _smooth){
                m_HueMagenta=_smooth;
            }

            inline void setLightnessMagenta(f32 _smooth){
                m_LightnessMagenta=_smooth;
            }

            inline void setSaturationBlue(f32 _smooth){
                m_saturationBlue=_smooth;
            }

            inline void setHueBlue(f32 _smooth){
                m_HueBlue=_smooth;
            }

            inline  void setLightnessBlue(f32 _smooth){
                m_LightnessBlue=_smooth;
            }
            
            inline void  setSaturationCyan(f32 _smooth){
                m_saturationCyan=_smooth;
            }
            
            inline void setHueCyan(f32 _smooth){
                m_HueCyan=_smooth;
            }
            
            inline void setLightnessCyan(f32 _smooth){
                m_LightnessCyan=_smooth;
            }
            
        protected:
            virtual void _submitMtl(SVRendererBasePtr _render);
            //
            f32 m_saturationRed;
            f32 m_HueRed;
            f32 m_LightnessRed;
            
            f32 m_saturationYellow;
            f32 m_HueYellow;
            f32 m_LightnessYellow;
            
            f32 m_saturationGreen;
            f32 m_HueGreen;
            f32 m_LightnessGreen;
            
            f32 m_saturationMagenta;
            f32 m_HueMagenta;
            f32 m_LightnessMagenta;
            
            f32 m_saturationBlue;
            f32 m_HueBlue;
            f32 m_LightnessBlue;
            
            f32 m_saturationCyan;
            f32 m_HueCyan;
            f32 m_LightnessCyan;
            
        };
        
    }//!namespace mtl
    
}//!namespace sv


#endif /* SV_MTLBRIGHTNESSCONTRAST_H */

