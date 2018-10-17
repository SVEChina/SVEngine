//
// SVBMFontConf.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BMFONTCONF_H
#define SV_BMFONTCONF_H

#include "SVFontConf.h"
#include "SVRect.h"
#include "SVMap.h"
#include "SVSet.h"
namespace sv {
    typedef struct _SVBMFontDef {
        u32 charID;
        SVRect rect;
        s16 xOffset;
        s16 yOffset;
        s16 xAdvance;
    } SVBMFONTDEF;
    
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
        
        SVMap<s32, SVBMFONTDEF> m_fontDefMap;
        
        SVSet<u32> m_characterSet;
        
        s32 m_commonHeight;
        
        s32 m_fontSize;
    };
    
}//!namespace sv



#endif //SV_BMFONTCONF_H
