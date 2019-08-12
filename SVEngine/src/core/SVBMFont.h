//
// SVBMFont.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BMFONT_H
#define SV_BMFONT_H

#include "SVFont.h"
#include "../mtl/SVMtlDeclare.h"
#include "../base/SVMap.h"
#include "../base/SVSet.h"
namespace sv {
    namespace util{
        class SVBMFont : public SVFont {
        public:
            typedef struct _SVBMFontCharInfo {
                u32 charID;
                u16 x;
                u16 y;
                u16 width;
                u16 height;
                s16 xOffset;
                s16 yOffset;
                s16 xAdvance;
                u16 page;
                u16 chnl;
                SVArray<s32> kerningPairs;
            } SVBMFONTCHARINFO;
            
            typedef struct _SVBMFontPadding {
                s32 left;
                s32 top;
                s32 right;
                s32 bottom;
            } SVBMFONTPADDING;
            
            static SVBMFontPtr creatFnt(cptr8 _filePath, SVInst *_app);
            
            SVBMFont(SVInst *_app);
            
            ~SVBMFont();
            
            void init();
            
            f32 getTextWidth(cptr8 _text);
            
            f32 getTextHeight(cptr8 _text);
            
            s32 getTextLength(cptr8 _text);
            
            s32 getTextChar(cptr8 _text, s32 _pos, s32 *_nextPos = 0);
            
            SVBMFONTCHARINFO getChar(s32 _charID);
            ////
            void setFontInfo(s32 _outlineThickness);
            
            void setCommonInfo(s32 _fontHeight, s32 _base, s32 _scaleW, s32 _scaleH, s32 _pages, bool _isPacked);
            
            void addChar(s32 _charID, s32 _x, s32 _y, s32 _w, s32 _h, s32 _xoffset, s32 _yoffset, s32 _xadvance, s32 _page, s32 _chnl);
            
            void addKerningPair(s32 _first, s32 _second, s32 _amount);
            
            void loadPage(s32 _pageID, cptr8 _pageFile, cptr8 _fontFile);
        public:
            SVMap<u32, SVTexturePtr> m_textures;
            
            f32 m_scale;
            
            s16 m_base;// y of base line
            
            s16 m_fontScaleW;
            
            s16 m_fontScaleH;
            
            s16 m_pages;
            
            SVString m_fntName;
            
            bool m_enableMipMap;
        protected:
            s32 _getTextLength(cptr8 _text);
            f32 _adjustForKerningPairs(s32 _first, s32 _second);
            s32 _findTextChar(cptr8 _text, s32 _start, s32 _length, s32 _ch);
        protected:
            SVMap<u32, SVBMFONTCHARINFO> m_charsMap;
            SVBMFONTCHARINFO m_defChar;
            s32 m_outlineThickness;
            bool m_hasOutline;
            s32 m_fontHeight;// total height of the font
        };
        
    }//!namespace util
    
}//!namespace sv



#endif //SV_BMFONT_H
