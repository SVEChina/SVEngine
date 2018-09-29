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
        
    }//!namespace mtl
    
}//!namespace sv


#endif /* SV_MTLBRIGHTNESSCONTRAST_H */

