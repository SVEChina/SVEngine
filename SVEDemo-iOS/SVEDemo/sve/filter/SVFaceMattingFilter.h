//
//  SVFaceMattingFilter.h
//  SVEngineShow
//
//  Created by 徐子昱 on 2019/10/28.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#ifndef SV_FACEMATTING_FILTER_H
#define SV_FACEMATTING_FILTER_H

#include "basesys/filter/SVFilterBase.h"


namespace sv {
    
    namespace logic {
        
        class SVFaceMattingFilter : public SVFilterBase {
        public:
            SVFaceMattingFilter(SVInst *_app);
            
            ~SVFaceMattingFilter();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
        protected:
         
            f32 m_faceDateExt[800];
            f32 m_texcoord[800];
            SVMtlCorePtr m_mtl;
            SVSpriteNodePtr m_spriteNode;
            
        };
    }
}

#endif /* SV_FACEMATTING_FILTER_H */
