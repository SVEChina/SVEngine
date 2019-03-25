//
//  SVMtlGradientMap.h
//  SVEngine
//
//  Created by 徐子昱 on 2019/3/22.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#ifndef SV_MTL_GRADIENTMAP_H
#define SV_MTL_GRADIENTMAP_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlGradientMap : public SVMtlCore  {
        public:
            SVMtlGradientMap(SVInst *_app);
            
            SVMtlGradientMap(SVMtlGradientMap* _mtl);
            
            ~SVMtlGradientMap();
            
            virtual SVMtlCorePtr clone();
            
            void setSmooth(f32 _smooth);
            
        protected:
            virtual void _submitMtl(SVRendererBasePtr _render);
            //
            f32 m_lSmoothSize;
        };
        
    }//!namespace mtl
    
}//!namespace sv

#endif /* SV_MTL_GRADIENTMAP_H */
