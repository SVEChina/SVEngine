//
// SVBMFontNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BITFONT_H
#define SV_BITFONT_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        //
        class SVBMFontNode : public SVNode {
            //纹理坐标
            struct FontTexcoords{
                f32 lt_x;
                f32 lt_y;
                f32 rt_x;
                f32 rt_y;
                f32 lb_x;
                f32 lb_y;
                f32 rb_x;
                f32 rb_y;
            };
        public:
            SVBMFontNode(SVInst *_app);
            
            ~SVBMFontNode();
            
            virtual void update(f32 dt);
            
            virtual void render();
            
            void setFont(SVBMFontPtr _font);
            
            void setFontSize(f32 _w,f32 _h);
            
            void setText(cptr8 _text);
            
            void setSpacing(f32 _spacing);
        private:
            void _genMesh();
            void _refreshTexcoords();
            void _refresh();
        protected:
            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            SVDataSwapPtr m_pRenderVertex;
            SVTexturePtr m_texture;
            SVBMFontPtr m_font;
            typedef SVArray<FontTexcoords> TEXCOORDTBL;
            TEXCOORDTBL m_texcoordsTbl;
            u32 m_textSize;
            f32 m_fontW;
            f32 m_fontH;
            f32 m_spacing;
            SVString m_text;
            bool m_textDirty;
        };
    }//!namespace node
    
}//!namespace sv



#endif //SV_BITFONT_H
