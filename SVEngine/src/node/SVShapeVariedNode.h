//
// SVShapeVariedNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SHAPEVARIEDNODE_H
#define SV_SHAPEVARIEDNODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node {
        
        // face shape douyin
        
        class SVShapeVariedNode : public SVNode {
        public:
            SVShapeVariedNode(SVInst *_app);
            
            virtual ~SVShapeVariedNode();
            
            void init();
            
            void destroy();
            
            void update(f32 dt);
            
            void render();
            
            void addPass(SVPassPtr _pass);
            
            void clearPass();
            
        protected:
            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            SVMtlFaceShapeVariedPtr m_pMtl;
        protected:
            typedef SVArray<SVPassPtr> PASSPOOL;
            PASSPOOL m_passPool;
            SVRenderTexturePtr m_fbo;
            
        };
        
    }//!namespace node
    
}//!namespace sv

#endif /* SV_SHAPEVARIEDNODE_H */
