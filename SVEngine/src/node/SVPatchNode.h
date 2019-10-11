//
// SVPatchNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_NODE_PATCHNODE_H
#define SV_NODE_PATCHNODE_H

#include "SVNode.h"
namespace sv {
    
    namespace node{
        
        class SVPatchNode : public SVNode {
        public:
            SVPatchNode(SVInst *_app);
            
            virtual ~SVPatchNode();
            
            virtual void update(f32 _dt);
            
            virtual void render();
            
            void setPatchVertexData(SVDataSwapPtr _vertexData, s32 _vertexCount);
            
            void setPatchIndexData(SVDataSwapPtr _indicesData, s32 _indicesCount);
        public:
            SVRenderObjectPtr m_pRenderObj;
            
            SVRenderMeshPtr m_pMesh;
            
            SVMtlGeo3dPtr m_patchMtl;
            
            SVColor m_color;
        };
        
    }//!namespace node
    
}//!namespace sv

#endif //SV_NODE_PATCHNODE_H
