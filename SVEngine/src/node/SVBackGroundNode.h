//
// SVBackGroundNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BACKGROUNDNODE_H
#define SV_BACKGROUNDNODE_H

#include "SVSpriteNode.h"
#include "../base/SVMap.h"


namespace sv {
    
    namespace node {
        
        class SVBackGroundNode : public SVSpriteNode {
        public:
            SVBackGroundNode(SVInst *_app);
            
            virtual ~SVBackGroundNode();
            
            void destroy();
            
            void update(f32 dt);
            
            void render();
            
            void setTexture(SVTexturePtr _tex);
            
            SVTexturePtr getOutTex();
            
            SVDeformImageMovePtr getDeform();
        protected:
            SVTexturePtr  m_texout;
            SVDeformImageMovePtr m_Deform;
            
        };
        
    }//!namespace node
    
}//!namespace sv

#endif /* SV_BACKGROUNDNODE_H */
