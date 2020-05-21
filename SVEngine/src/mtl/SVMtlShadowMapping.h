//
//  SVMtlShadowMapping.h
//  SVEngine
//
//  Created by 徐子昱 on 2019/9/13.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#ifndef SV_MtlSHADOWMAPPING_H
#define SV_MtlSHADOWMAPPING_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlShadowMapping : public SVMtlCore  {
        public:
            SVMtlShadowMapping(SVInst *_app);
            
            SVMtlShadowMapping(SVMtlShadowMapping* _mtl);
            
            ~SVMtlShadowMapping();
            
            void setLightVPMat(f32 *_vp);
            
            virtual SVMtlCorePtr clone();
            
        protected:
            virtual void _submitMtl(SVRendererPtr _render);
            f32 m_LightVPMat[16];
    
        };
        
    }//!namespace mtl
    
}//!namespace sv


#endif /* SV_MtlSHADOWMAPPING_H */
