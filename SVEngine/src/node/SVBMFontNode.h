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
                f32 w;
                f32 h;
                f32 ox;
                f32 oy;
                f32 a;
                s32 charID;
            };
        public:
            SVBMFontNode(SVInst *_app);
            
            ~SVBMFontNode();
            
            virtual void update(f32 dt);
            
            virtual void render();
            
            void setFont(SVBMFontPtr _font);
            
            void setText(cptr8 _text);
            
            cptr8 getText();
            
            void setSpacing(f32 _spacing);
            
            f32 getSpacing();
            
            SVBMFontPtr getFont();
            
            f32 getFontWidth();
            
            f32 getFontHeight();
            
            virtual void setAlpha(f32 _alpha);
            //序列化接口
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &_item);
        private:
            void _genMesh();
            void _refresh();
        protected:
            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            SVDataSwapPtr m_pRenderVertex;
            SVTexturePtr m_texture;
            SVBMFontPtr m_font;
            typedef SVArray<FontTexcoords> TEXCOORDTBL;
            BITFONT_ATCH_PT m_atchType;
            f32 m_spacing;
            f32 m_alpha;
            SVString m_text;
            bool m_textDirty;
        };
    }//!namespace node
    
}//!namespace sv



#endif //SV_BMFONTNODE_H
