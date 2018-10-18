//
// SVBMFont.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BMFONT_H
#define SV_BMFONT_H

#include "SVFont.h"
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
            
            SVBMFont(SVInst *_app);
            
            ~SVBMFont();
            
            void init();
        public:
            SVMap<u32, SVBMFONTCHARINFO> m_charsMap;
            
            s32 m_fontSize;
            
            s16 m_fontHeight;
            
            s16 m_base;
            
            s16 m_scaleW;
            
            s16 m_scaleH;
            
            s16 m_pages;
            
            SVString m_atlasName;
        protected:
            
        };
        
    }//!namespace util
    
}//!namespace sv



#endif //SV_BMFONT_H
