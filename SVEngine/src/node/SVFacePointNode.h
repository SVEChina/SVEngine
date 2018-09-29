//
// SVFacePointNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#ifndef SV_FACEPOINTNODE_H
#define SV_FACEPOINTNODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        class SVFacePointNode : public SVNode {
            public :
            SVFacePointNode(SVInst *_app);
            
            ~SVFacePointNode();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            void render();
        protected:
            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            SVMtlCorePtr m_pMtl;
        };
   
    
    }
}

#endif /* SV_FACEPOINTNODE_H */
