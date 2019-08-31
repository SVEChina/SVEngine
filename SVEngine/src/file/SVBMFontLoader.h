//
// SVBMFontLoader.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BMFONTLOADER_H
#define SV_BMFONTLOADER_H

#include "SVFileLoader.h"
namespace sv {
    class SVBMFontLoader : public SVFileLoader {
    public:
        SVBMFontLoader(SVInst *_app);
        
        ~SVBMFontLoader();
        
        void loadData(cptr8 _fontFile, SVBMFontPtr _font);
    private:
        u32  _parseCharacterDefinition(SVBMFontPtr _font, cptr8 line);
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
        
        void parseConfigFile(SVBMFontPtr _font, FILE *_f, c8 *_pData, u64 _size, cptr8 _fontFile);
    private:
        void _readInfoBlock(SVBMFontPtr _font, c8 *_pData, u32 _blockSize);
        void _readCommonBlock(SVBMFontPtr _font, c8 *_pData, u32 _blockSize);
        void _readPagesBlock(SVBMFontPtr _font, c8 *_pData, u32 _blockSize);
        void _readCharsBlock(SVBMFontPtr _font, c8 *_pData, u32 _blockSize);
        void _readKerningPairsBlock(SVBMFontPtr _font, c8 *_pData, u32 _blockSize);
    };
    //
    class SVBMFontParseTextFormat {
    public:
        SVBMFontParseTextFormat();
        
        ~SVBMFontParseTextFormat();
        
        void parseConfigFile(SVBMFontPtr _font, FILE *_f, cptr8 _fontFile);
    private:
        s32 _skipWhiteSpace(cptr8 _str, s32 _start);
        s32 _findEndOfToken(cptr8 _str, s32 _start);
        void _interpretInfo(SVBMFontPtr _font, cptr8 _str, s32 _start);
        void _interpretCommon(SVBMFontPtr _font, cptr8 _str, s32 _start);
        void _interpretChar(SVBMFontPtr _font, cptr8 _str, s32 _start);
        void _interpretKerning(SVBMFontPtr _font, cptr8 _str, s32 _start);
        void _interpretPage(SVBMFontPtr _font, cptr8 _str, s32 _start, cptr8 _fontFile);
    };
    
}//!namespace sv




#endif //SV_BMFONTLOADER_H
