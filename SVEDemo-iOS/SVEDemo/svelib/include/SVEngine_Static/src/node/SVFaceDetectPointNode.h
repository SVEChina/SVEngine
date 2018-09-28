//
// SVFaceDetectPointNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#ifndef SV_FACEDETECTPOINTNODE_H
#define SV_FACEDETECTPOINTNODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        class SVFaceDetectPointNode : public SVNode {
        public:
            SVFaceDetectPointNode(SVInst *_app);
            
            ~SVFaceDetectPointNode();
            
            void update(f32 _dt);
            
            void render();
            
            inline f32 getWidth(){ return m_width; }
            
            inline f32 getHeight(){ return m_height; }
            
            inline void setWidth(f32 _width){ m_width=_width; }
            
            inline void setHeight(f32 _height){ m_height=_height; }
            
            inline void setinw(f32 _w){  m_inw=_w; }
            
            inline void setinh(f32 _h){  m_inh=_h; }
            
            void setTexture(SVTexturePtr _tex);
            
            void createMesh(V2* _facepoint);
            
        protected:
            f32 m_width;
            f32 m_height;
            f32 m_inw;
            f32 m_inh;
            SVRenderMeshPtr m_pMesh;
            SVRenderObjectPtr m_pRenderObj;
            SVTexturePtr m_pTex;
           
        };
        
    }//!namespace node
    
}//!namespace sv

#endif /* SV_FACEDETECTPOINTNODE_H */
