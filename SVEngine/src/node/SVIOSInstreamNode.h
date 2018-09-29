//
// SVIOSInstreamNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_IOSINSTREAMNODE_H
#define SV_IOSINSTREAMNODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        class SVIOSInstreamNode : public SVNode {
        public:
            SVIOSInstreamNode(SVInst *_app);
            
            virtual ~SVIOSInstreamNode();
            
            void init(SVTEXTYPE _tt);
            
            void destroy();
            
            void update(f32 dt);
            
            void render();
            
        protected:
            SVRenderObjectPtr m_pRenderObj;
            SVMtlCorePtr m_pMtl;
            SVTEXTYPE m_tt;
        };

    }//!namespace node
    
}//!namespace sv


#endif //SV_IOSCAMSTREAMNODE_H
