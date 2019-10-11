//
// SVLineNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_NODE_LINENODE_H
#define SV_NODE_LINENODE_H

#include "SVNode.h"
namespace sv {
    
    namespace node{
        
        class SVLineNode : public SVNode {
        public:
            SVLineNode(SVInst *_app);
            
            virtual ~SVLineNode();
            
            virtual void update(f32 _dt);
            
            virtual void render();
            
            void setLineData(f32 *_data, s32 _size, s32 _count);
            
        public:
            SVRenderObjectPtr m_pRenderObj;
            
            SVRenderMeshPtr m_pMesh;
            
            SVMtlGeo3dPtr m_lineMtl;
            
            SVColor m_color;
        };
        
    }//!namespace node
    
}//!namespace sv

#endif //SV_NODE_LINENODE_H
