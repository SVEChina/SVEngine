//
// SVGLTFModelNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GLTFMODELNODE_H
#define SV_GLTFMODELNODE_H

#include "SVNode.h"
#include "../core/SVglTF.h"

namespace sv {
    
    namespace node{
        //
        class SVGLTFModelNode : public SVNode {
        public:
            SVGLTFModelNode(SVInst *_app);
            
            ~SVGLTFModelNode();
            
            void update(f32 dt);
            
            void render();
            
            void setModel(GLTFModelPtr _model);
            
            GLTFModelPtr getModel();
            
            void destroyModel();
            
        protected:
            void _loadData();
            
            void _refreshModelMatrix();
            
            void _refreshMeshGlobalMat(Node _node, FMat4 _mat4);
            
            GLTFModelPtr m_model;
            
            SVArray<ModelMeshDataPtr>  m_renderMeshData;
            
            SVArray<ModelMeshDataPtr>  m_readyRenderMeshData;
            
            SVMultMeshMtlRenderObjectPtr m_pRObj;
        public:
            //序列化接口
//            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
//
//            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        };
        
    }//!namespace node
    
}//!namespace sv



//
#endif //SV_GLTFMODELNODE_H


