//
// SVBillboardNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BILLBOARD_H
#define SV_BILLBOARD_H

#include "SVSpriteNode.h"

namespace sv {
    
    namespace node{
        
        class SVBillboardNode : public SVSpriteNode {
        public:
            SVBillboardNode(SVInst *_app);
            
            SVBillboardNode(SVInst *_app,f32 _w,f32 _h);
            
            ~SVBillboardNode();
            
            void update(f32 _dt);
            
            void render();
            
            void setSize(f32 _w,f32 _h);
            
            void syncTexSize();
            
            void setTexcoord(f32 x,f32 y);
            
            inline f32 getWidth(){ return m_width; }
            
            inline f32 getHeight(){ return m_height; }
            
            cptr8 getTexturePath();
            
            virtual void setTexture(cptr8 _path, bool enableMipMap = false);
            
            virtual void setTexture(SVTEXTYPE _textype);
            
            virtual void setTexture(SVTexturePtr _tex);
            
            void setMesh(SVRenderMeshPtr _mesh);
            
            void setMtl(SVMtlCorePtr _mtl);
            
            void setViewPos(FVec3 &_pos);
            
            void setUp(FVec3 &_up);
            
            //序列化接口
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        protected:
            f32 m_width;
            f32 m_height;
            f32 m_texcoordX;
            f32 m_texcoordY;
            FVec3 m_viewPos;
            FVec3 m_up;
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


#endif //SV_BILLBOARD_H
