//
// SVFreeTypeNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FREETYPENODE_H
#define SV_FREETYPENODE_H

#include "SVNode.h"

#ifdef CONFIG_IS_LOAD_FREETYPE

namespace sv {
    
    namespace node{
        
        class SVFreeTypeNode : public SVNode {
        public:
            SVFreeTypeNode(SVInst *_app);
            
            virtual ~SVFreeTypeNode();
            
            virtual void update(f32 dt);
            
            virtual void render();
            
            void setText(cptr8 _text);
            
            cptr8 getText();
            
        private:
            void _updateTextTexture();  //更新文字纹理
            
            s32 m_fontPixelSize;        //字体标准像素尺寸
            s32 m_DefTextPixelSize;
            SVTexturePtr m_texture;
            SVRenderMeshPtr m_pMesh;    //模型
            SVRenderObjectPtr m_renderObject;
            bool m_textChange;
            SVString m_Text;
        };

    }//!namespace node
    
}//!namespace sv

#endif //CONFIG_IS_LOAD_FREETYPE

#endif //SV_FREETYPENODE_H
