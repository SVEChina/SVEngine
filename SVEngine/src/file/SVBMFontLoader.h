//
// SVBMFontLoader.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BMFONTLOADER_H
#define SV_BMFONTLOADER_H

#include "SVFileLoader.h"
#include "../core/SVBMFontConf.h"
namespace sv {
    class SVBMFontLoader : public SVFileLoader {
    public:
        SVBMFontLoader(SVInst *_app);
        
        ~SVBMFontLoader();
        
        void loadData(cptr8 _fontFile, SVBMFontConfPtr _fontConf);
    public:
        SVMap<u64, s32> m_kerningDictionary;
        SVBMFONTPADDING m_padding;
        SVString m_atlasName;
    private:
        u32  _parseCharacterDefinition(SVBMFontConfPtr _fontConf, cptr8 line);
        void _parseInfoArguments(cptr8 line);
        void _parseCommonArguments(cptr8 line);
        void _parseImageFileName(cptr8 line, cptr8 fntFile);
        void _parseKerningEntry(cptr8 line);
        void _purgeKerningDictionary();
        void _purgeFontDefDictionary();
    };
    
    //
    class SVBMFontParseBinaryFormat {
    public:
        SVBMFontParseBinaryFormat();
        
        ~SVBMFontParseBinaryFormat();
        
        void parseConfigFile(SVBMFontConfPtr _fontConf, u8 *_pData, u64 _size, cptr8 _fontFile);
    private:
        void _readInfoBlock(SVBMFontConfPtr _fontConf, u8 *_pData, u32 _blockSize);
        void _readCommonBlock(SVBMFontConfPtr _fontConf, u8 *_pData, u32 _blockSize);
        void _readPagesBlock(SVBMFontConfPtr _fontConf, u8 *_pData, u32 _blockSize);
        void _readCharsBlock(SVBMFontConfPtr _fontConf, u8 *_pData, u32 _blockSize);
        void _readKerningPairsBlock(SVBMFontConfPtr _fontConf, u8 *_pData, u32 _blockSize);
    };
    //
    class SVBMFontParseTextFormat {
    public:
        SVBMFontParseTextFormat();
        
        ~SVBMFontParseTextFormat();
        
        void parseConfigFile(SVBMFontConfPtr _fontConf, u8 *_pData, u64 _size, cptr8 _fontFile);
    private:
        void _interpretInfo(SVBMFontConfPtr _fontConf, cptr8 line);
        void _interpretCommon(SVBMFontConfPtr _fontConf, cptr8 line);
        void _interpretChar(SVBMFontConfPtr _fontConf, cptr8 line);
        void _interpretKerning(SVBMFontConfPtr _fontConf, cptr8 line);
        void _interpretPage(SVBMFontConfPtr _fontConf, cptr8 line);
    };
    
}//!namespace sv




#endif //SV_BMFONTLOADER_H
