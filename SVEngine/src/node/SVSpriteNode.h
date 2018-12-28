//
// SVSpriteNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SPRITE_H
#define SV_SPRITE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        class SVSpriteNode : public SVNode {
        public:
            SVSpriteNode(SVInst *_app);
            
            SVSpriteNode(SVInst *_app,f32 _w,f32 _h);
            
            ~SVSpriteNode();
            
            void update(f32 _dt);
            
            void render();
            
            void setSpriteSize(f32 _w,f32 _h);
            
            void syncTexSize();
            
            void setTexcoord(f32 x,f32 y);
            
            inline f32 getWidth(){ return m_width; }
            
            inline f32 getHeight(){ return m_height; }
            
            void setMaterial(SVMtlCorePtr _mtl);
            
            SVMtlCorePtr getMaterial();
            
            cptr8 getTexturePath();
            
            virtual void setTexture(cptr8 _path);
            
            virtual void setTexture(SVTEXTYPE _textype);
            
            virtual void setTexture(SVTexturePtr _tex);
            
            void setMesh(SVRenderMeshPtr _mesh);
            
            //序列化接口
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        protected:
            f32 m_width;
            f32 m_height;
            f32 m_texcoordX;
            f32 m_texcoordY;
            SVString m_pTexPath;
            SVTEXTYPE m_inTexType;
            //运行时属性
            SVTexturePtr m_pTex;
            SVRenderMeshPtr m_pMesh;
            SVRenderObjectPtr m_pRenderObj;
        private:
            SVString m_pTexName;
        };

    }//!namespace node
    
}//!namespace sv


#endif //SV_SPRITE_H
