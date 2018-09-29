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
            
            inline SVTexturePtr getOutTex(){
                return outTex;
            }
            
            
        protected:
            SVMtlBrightnessContrastPtr m_BCMtl;//brightness contrast
            SVMtlSaturationPtr m_SaturationMtl;
            SVMtlVibrancePtr m_VibranceMtl;
            
            SVPassPtr m_passSaturation;
            SVPassPtr m_passVibrance;
            SVPassPtr m_passBC;
            SVPassPtr m_passBack;
            SVTexturePtr outTex;
            SVTexturePtr inTex;
            f32 m_brightness;
            f32 m_contrast;
            f32 m_saturation;
            f32 m_vibrance;
            
        };
    };//!namespace logic
}//!namespace sv


#endif /* SV_FILTERLOOKUPTABLEADJUST_H */
