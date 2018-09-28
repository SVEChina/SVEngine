//
// SVFacePointNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FACESHAPENODE_H
#define SV_FACESHAPENODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        class SVFaceShapeNode : public SVNode {
            public :
            SVFaceShapeNode(SVInst *_app);
            
            ~SVFaceShapeNode();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            void render();
            
            bool procEvent(SVEventPtr _event);
            
            void addPass(SVPassPtr _pass);
            
            void clearPass();
        protected:
            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            SVMtlFaceShapePtr m_pMtl;
        protected:
            typedef SVArray<SVPassPtr> PASSPOOL;
            PASSPOOL m_passPool;
            SVRenderTexturePtr m_fbo;
        };
    }
}

#endif /* SV_FACESHAPENODE_H */
