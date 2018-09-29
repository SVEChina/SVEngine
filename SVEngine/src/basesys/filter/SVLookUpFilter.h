//
//  SVLookUpFilter.hpp
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
        
        class SVLookUpFilter : public SVFilterBase {
        public:
            SVLookUpFilter(SVInst *_app);
            
            ~SVLookUpFilter();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            virtual void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            
            virtual f32 getFilterParam(SVFILTERITEMTYPE _type);
            
            bool create(SVTexturePtr _looktex,SVTexturePtr _bgtex);
            
        protected:
            SVMtlCorePtr m_lkMtl;
            SVMtlCorePtr m_mtl_back;
            SVTexturePtr m_tex01;
            SVTexturePtr m_tex02;
        };
        
    };//!namespace logic
    
}//!namespace sv


#endif /* SV_LOOKUPFILTER_H */
