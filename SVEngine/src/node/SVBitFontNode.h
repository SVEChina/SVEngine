//
// SVBitFontNode.h
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
        class SVBitFontNode : public SVNode {
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
            SVBitFontNode(SVInst *_app);
            
            ~SVBitFontNode();
            
            virtual void update(f32 dt);
            
            virtual void render();
            
            void init(cptr8 path, f32 _fontWidth){}
            
            void setFontSize(f32 _w,f32 _h);
            
            f32 getFontW();
            
            f32 getFontH();
            
            void setInnerOff(f32 _offXScale,f32 _offYScale);
            
            f32 getInnerOffXScale();
            
            f32 getInnerOffYScale();
            
            void setText(s32 _number);
            
            void setText(cptr8 _text);
            
            cptr8 getText();
            
            void setAtcPt(BITFONT_ATCH_PT _type);
            
            BITFONT_ATCH_PT getAtcPt();
            
            void setTexture(SVTexturePtr _tex);
            
            void setTexture(cptr8 _path);
            
            cptr8 getTexturePath();
            
            virtual void setAlpha(s32 _alpha);
        private:
            void _genMesh();
            void _genTexcoords();
            void _refresh();
            
            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            SVDataSwapPtr m_pRenderVertex;
            SVTexturePtr m_texture;
            SVString m_Text;
            f32 m_FontWidth;    //物理字体宽度
            f32 m_FontHeight;   //物理字体高度
            f32 m_innerOffXScale;    //四周向里偏移倍数
            f32 m_innerOffYScale;    //四周向里偏移
            BITFONT_ATCH_PT m_atchType;    //布局位置 9个方向
            //字符和字符对应的纹理坐标
            typedef SVArray<c8> CHARTBL;
            CHARTBL m_charTbl;
            typedef SVArray<FontTexcoords> TEXCOORDTBL;
            TEXCOORDTBL m_texcoordsTbl;
            bool m_fontDirty;
            s32 m_alpha;
        };

        
    }//!namespace node
    
}//!namespace sv



#endif //SV_BITFONT_H
