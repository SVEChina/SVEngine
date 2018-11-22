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
            
            void setLight(s32 _channel, FVec3 _lightPos, FVec3 _lightColor);
            
            f32 getLightChannel();
            
            FVec3 getLightPos();
            
            FVec3 getLightColor();
        protected:
            GLTFModelPtr m_model;
            
            SVMultMeshMtlRenderObjectPtr m_pRObj;
            
            f32 m_lightChannel;
            
            FVec3 m_lightPos;
            
            FVec3 m_lightColor;
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


