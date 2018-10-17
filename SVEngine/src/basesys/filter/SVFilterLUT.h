//
//  SVFilterLUT.hpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/9/25.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_LOOKUPFILTER_H
#define SV_LOOKUPFILTER_H

#include "SVFilterBase.h"

namespace sv {
    
    namespace logic {
        
        class SVFilterLUT : public SVFilterBase {
        public:
            SVFilterLUT(SVInst *_app);
            
            ~SVFilterLUT();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            void setLUTTex(SVTexturePtr _looktex);
            
            SVTexturePtr getLUTTex();
            
            virtual void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            
            virtual f32 getFilterParam(SVFILTERITEMTYPE _type);

        protected:
            SVTexturePtr m_texLUT;
            bool m_dirtyLUT;
        };
        
    };//!namespace logic
    
}//!namespace sv


#endif /* SV_LOOKUPFILTER_H */
