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
            
            SVBackGroundNode(SVInst *_app,f32 _w,f32 _h);
            
            virtual ~SVBackGroundNode();
            
            void update(f32 dt);
            
            void render();
            
            void setTexture(cptr8 _path);
            
            void setTexture(SVTEXTYPE _textype);
            
            SVTEXTYPE getUseInnerTex(){ return m_useTexType; }
            
            bool enableDeform(SVTEXTYPE _textype);
            
            void disableDeform();
            
            bool isDeform();
            
            SVDeformImageMovePtr getDeform();
            
        protected:
            SVTEXTYPE m_useTexType;
            SVDeformImageMovePtr m_pDeform;
            
        public:
            //序列化接口
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        };
        
    }//!namespace node
    
}//!namespace sv

#endif /* SV_BACKGROUNDNODE_H */
