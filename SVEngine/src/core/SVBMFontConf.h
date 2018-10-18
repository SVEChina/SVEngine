//
// SVBMFontConf.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BMFONTCONF_H
#define SV_BMFONTCONF_H

#include "SVFontConf.h"
#include "../base/SVRect.h"
#include "../base/SVMap.h"
#include "../base/SVSet.h"
namespace sv {
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
    
    class SVBMFontConf : public SVFontConf {
    public:
        SVBMFontConf(SVInst *_app);
        
        ~SVBMFontConf();
        
        SVMap<u32, SVBMFONTCHARINFO> m_charsMap;
        
        SVSet<u32> m_characterSet;
        
        s32 m_fontSize;
        
        s16 m_fontHeight;
        
        s16 m_base;
        
        s16 m_scaleW;
        
        s16 m_scaleH;
        
        s16 m_pages;
    };
    
}//!namespace sv



#endif //SV_BMFONTCONF_H
