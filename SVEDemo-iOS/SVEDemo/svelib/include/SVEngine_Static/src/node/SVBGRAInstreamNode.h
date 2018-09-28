//
// SVBGRAInstreamNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BGRAINSTREAMNODE_H
#define SV_BGRAINSTREAMNODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        //
        class SVBGRAInstreamNode : public SVNode {
        public:
            SVBGRAInstreamNode(SVInst *_app);
            
            SVBGRAInstreamNode(SVInst *_app,f32 _w,f32 _h);
            
            ~SVBGRAInstreamNode();
            
            void update(f32 _dt);
            
            void render();
            
            void setSpriteSize(f32 _w,f32 _h);
            
            inline f32 getWidth(){ return m_width; }
            
            inline f32 getHeight(){ return m_height; }
            
            void setMaterial(SVMtlCorePtr _mtl);
            
            void setTexture(SVTexturePtr _tex);
            
        protected:
            f32 m_width;
            f32 m_height;
            SVRenderMeshPtr m_pMesh;
            SVRenderObjectPtr m_pRenderObj;
            SVTexturePtr m_pTex;
        };
        
    }//!namespace node
    
}//!namespace sv


#endif //SV_SPRITE_H
