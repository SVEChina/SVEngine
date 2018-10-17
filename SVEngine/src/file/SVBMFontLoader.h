//
// SVBMFontLoader.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BMFONTLOADER_H
#define SV_BMFONTLOADER_H

#include "SVFileLoader.h"
#include "../base/SVRect.h"
#include "../base/SVMap.h"
#include "../base/SVSet.h"
#include "../base/SVBMFontConf.h"
namespace sv {

    
    class SVBMFontLoader : public SVFileLoader {
    public:
        SVBMFontLoader(SVInst *_app);
        
        ~SVBMFontLoader();
        
        void loadData(cptr8 _name, SVBMFontConf *_fontConf);
    public:
        SVMap<u64, s32> m_kerningDictionary;
        SVBMFONTPADDING m_padding;
        SVString m_atlasName;
    private:
        void _parseBinaryConfigFile(SVBMFontConf *_fontConf, u8 *_pData, u64 _size, cptr8 _name);
        u32  _parseCharacterDefinition(SVBMFontConf *_fontConf, cptr8 line);
        void _parseInfoArguments(cptr8 line);
        void _parseCommonArguments(cptr8 line);
        void _parseImageFileName(cptr8 line, cptr8 fntFile);
        void _parseKerningEntry(cptr8 line);
        void _purgeKerningDictionary();
        void _purgeFontDefDictionary();
    };
    
}//!namespace sv




#endif //SV_BMFONTLOADER_H
