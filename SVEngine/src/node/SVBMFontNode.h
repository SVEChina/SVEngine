//
// SVBMFontNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BMFONTNODE_H
#define SV_BMFONTNODE_H

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
            
            f32 getFontW();
            
            f32 getFontH();
            
            void setText(cptr8 _text);
            
            cptr8 getText();
            
            void setSpacing(f32 _spacing);
            
            f32 getSpacing();
            
            void setAtcPt(BITFONT_ATCH_PT _type);
            
            BITFONT_ATCH_PT getAtcPt();
            
            virtual void setAlpha(f32 _alpha);
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
            BITFONT_ATCH_PT m_atchType;    //布局位置 9个方向
            u32 m_textSize;
            f32 m_fontW;
            f32 m_fontH;
            f32 m_spacing;
            f32 m_alpha;
            SVString m_text;
            bool m_textDirty;
        };
    }//!namespace node
    
}//!namespace sv



#endif //SV_BMFONTNODE_H
