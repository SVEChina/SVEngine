//
// SVBMFontLoader.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBMFontLoader.h"
#include "SVFileMgr.h"
#include "../base/SVDataChunk.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"
SVBMFontLoader::SVBMFontLoader(SVInst *_app)
:SVFileLoader(_app) {

}

SVBMFontLoader::~SVBMFontLoader() {
    m_kerningDictionary.clear();
}

void SVBMFontLoader::loadData(cptr8 _fontFile, SVBMFontConfPtr _fontConf) {
    SVDataChunk tSVDataChunk;
    bool t_flag = mApp->getFileMgr()->loadFileContentStr(&tSVDataChunk, _fontFile);
    if (!t_flag) {
        return;
    }
    if (tSVDataChunk.m_size == 0){
        return;
    }
    u8* fontData = (u8 *)tSVDataChunk.m_data;
    if (std::memcmp("BMF", fontData, 3) == 0) {
        SVBMFontParseBinaryFormat parseBinaryFormat;
        parseBinaryFormat.parseConfigFile(_fontConf, fontData, tSVDataChunk.m_size, _fontFile);
    }else {
        SVBMFontParseTextFormat parseTextFormat;
        parseTextFormat.parseConfigFile(_fontConf, fontData, tSVDataChunk.m_size, _fontFile);
    }
}

//parse font binary format
SVBMFontParseBinaryFormat::SVBMFontParseBinaryFormat(){
    
}

SVBMFontParseBinaryFormat::~SVBMFontParseBinaryFormat(){
    
}

void SVBMFontParseBinaryFormat::parseConfigFile(SVBMFontConfPtr _fontConf, u8 *_pData, u64 _size, cptr8 _fontFile){
    /* based on http://www.angelcode.com/products/bmfont/doc/file_format.html file format */
    u64 remains = _size;
    _pData += 4; remains -= 4;
    while (remains > 0)
    {
        u8 blockId = _pData[0]; _pData += 1; remains -= 1;
        u32 blockSize = 0; memcpy(&blockSize, _pData, 4);
        _pData += 4; remains -= 4;
        if (blockId == 1){
            _readInfoBlock(_fontConf, _pData, blockSize);
        }else if (blockId == 2){
            _readCommonBlock(_fontConf, _pData, blockSize);
        }else if (blockId == 3){
            _readPagesBlock(_fontConf, _pData, blockSize);
        }else if (blockId == 4){
            _readCharsBlock(_fontConf, _pData, blockSize);
        }else if (blockId == 5) {
            _readKerningPairsBlock(_fontConf, _pData, blockSize);
        }
        _pData += blockSize; remains -= blockSize;
    }
}


void SVBMFontParseBinaryFormat::_readInfoBlock(SVBMFontConfPtr _fontConf, u8 *_pData, u32 _blockSize){
    /*
     fontSize       2   int      0
     bitField       1   bits     2  bit 0: smooth, bit 1: unicode, bit 2: italic, bit 3: bold, bit 4: fixedHeight, bits 5-7: reserved
     charSet        1   uint     3
     stretchH       2   uint     4
     aa             1   uint     6
     paddingUp      1   uint     7
     paddingRight   1   uint     8
     paddingDown    1   uint     9
     paddingLeft    1   uint     10
     spacingHoriz   1   uint     11
     spacingVert    1   uint     12
     outline        1   uint     13 added with version 2
     fontName       n+1 string   14 null terminated string with length n
     */
    memcpy(&_fontConf->m_fontSize, _pData, 2);
    //            m_padding.top = (u8)_pData[7];
    //            m_padding.right = (u8)_pData[8];
    //            m_padding.bottom = (u8)_pData[9];
    //            m_padding.left = (u8)_pData[10];
}

void SVBMFontParseBinaryFormat::_readCommonBlock(SVBMFontConfPtr _fontConf, u8 *_pData, u32 _blockSize){
    /*
     lineHeight 2   uint    0
     base       2   uint    2
     scaleW     2   uint    4
     scaleH     2   uint    6
     pages      2   uint    8
     bitField   1   bits    10  bits 0-6: reserved, bit 7: packed
     alphaChnl  1   uint    11
     redChnl    1   uint    12
     greenChnl  1   uint    13
     blueChnl   1   uint    14
     */
    memcpy(&_fontConf->m_fontHeight, _pData, 2);
    memcpy(&_fontConf->m_scaleW, _pData + 4, 2);
    memcpy(&_fontConf->m_scaleH, _pData + 6, 2);
    memcpy(&_fontConf->m_pages, _pData + 8, 2);
}

void SVBMFontParseBinaryFormat::_readPagesBlock(SVBMFontConfPtr _fontConf, u8 *_pData, u32 _blockSize){
    /*
     pageNames     p*(n+1)     strings     0     p null terminated strings, each with length n
     */
    cptr8 *value = (cptr8 *)_pData;
    //            assert(std::strlen(value) < blockSize && "Block size should be less then string");
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //            _atlasName = FileUtils::getInstance()->fullPathFromRelativeFile(value, controlFile);
}

void SVBMFontParseBinaryFormat::_readCharsBlock(SVBMFontConfPtr _fontConf, u8 *_pData, u32 _blockSize){
    /*
     id         4   uint    0+c*20  These fields are repeated until all characters have been described
     x          2   uint    4+c*20
     y          2   uint    6+c*20
     width      2   uint    8+c*20
     height     2   uint    10+c*20
     xoffset    2   int     12+c*20
     yoffset    2   int     14+c*20
     xadvance   2   int     16+c*20
     page       1   uint    18+c*20
     chnl       1   uint    19+c*20
     */
    u64 count = _blockSize / 20;
    for (u64 i = 0; i < count; i++){
        SVBMFONTCHARINFO charInfo;
        u32 charId = 0; memcpy(&charId, _pData + (i * 20), 4);
        charInfo.charID = charId;
        memcpy(&charInfo.x, _pData + (i * 20) + 4, 2);
        memcpy(&charInfo.y, _pData + (i * 20) + 6, 2);
        memcpy(&charInfo.width, _pData + (i * 20) + 8, 2);
        memcpy(&charInfo.height, _pData + (i * 20) + 10, 2);
        memcpy(&charInfo.xOffset, _pData + (i * 20) + 12, 2);
        memcpy(&charInfo.yOffset, _pData + (i * 20) + 14, 2);
        memcpy(&charInfo.xAdvance, _pData + (i * 20) + 16, 2);
        memcpy(&charInfo.page, _pData + (i * 20) + 18, 2);
        memcpy(&charInfo.chnl, _pData + (i * 20) + 19, 2);
        _fontConf->m_charsMap.append(charId, charInfo);
    }
}

void SVBMFontParseBinaryFormat::_readKerningPairsBlock(SVBMFontConfPtr _fontConf, u8 *_pData, u32 _blockSize){
    /*
     first  4   uint    0+c*10     These fields are repeated until all kerning pairs have been described
     second 4   uint    4+c*10
     amount 2   int     8+c*10
     */
    u64 count = _blockSize / 20;
    for (u64 i = 0; i < count; i++)
    {
        s32 first = 0; memcpy(&first, _pData + (i * 10), 4);
        s32 second = 0; memcpy(&second, _pData + (i * 10) + 4, 4);
        s16 amount = 0; memcpy(&amount, _pData + (i * 10) + 8, 2);
        SVMap<u32, SVBMFONTCHARINFO>::Iterator it = _fontConf->m_charsMap.find(first);
        if( first >= 0 && first < 256 && it!=_fontConf->m_charsMap.end() ){
            _fontConf->m_charsMap[first].kerningPairs.append(second);
            _fontConf->m_charsMap[first].kerningPairs.append(amount);
        }
    }
}

//parse font text format
SVBMFontParseTextFormat::SVBMFontParseTextFormat(){
    
}

SVBMFontParseTextFormat::~SVBMFontParseTextFormat(){
    
}

void SVBMFontParseTextFormat::parseConfigFile(SVBMFontConfPtr _fontConf, u8 *_pData, u64 _size, cptr8 _fontFile){
    //    if (data[0] == 0)
    //    {
    //        SV_LOG_ERROR("Error: parsing BMFontFile %s", _fontFile);
    //        return;
    //    }
    //    cptr8 contents = (cptr8)data;
    //    u64 contentsLen = (u64)std::strlen(contents);
    //    c8 line[512] = {0};
    //
    //    u8 *next = (u8 *)std::strchr(contents, '\n');
    //    u8 *base = data;
    //    u64 lineLength = 0;
    //    u64 parseCount = 0;
    //    u32 basePos = 0;
    //    u32 nextPos = SVString(contents).find('\n');
    //    while (next)
    //    {
    //        lineLength = ((u32)(nextPos - basePos));
    //        memcpy(line, contents + parseCount, lineLength);
    //        line[lineLength] = 0;
    //
    //        parseCount += lineLength + 1;
    //        if (parseCount < contentsLen)
    //        {
    //            base = next + 1;
    //            basePos = basePos + 1;
    //
    //            next = (u8 *)std::strchr((cptr8)base, '\n');
    //            nextPos = SVString((cptr8)next).find('\n');
    //        }
    //        else
    //        {
    //            next = nullptr;
    //        }
    //
    //        if (memcmp(line, "info face", 9) == 0)
    //        {
    //            // FIXME: info parsing is incomplete
    //            // Not needed for the Hiero editors, but needed for the AngelCode editor
    //            //            [self parseInfoArguments:line];
    //            _parseInfoArguments(line);
    //        }
    //        // Check to see if the start of the line is something we are interested in
    //        else if (memcmp(line, "common lineHeight", 17) == 0)
    //        {
    //            _parseCommonArguments(line);
    //        }
    //        else if (memcmp(line, "page id", 7) == 0)
    //        {
    //            _parseImageFileName(line, _fontFile);
    //        }
    //        else if (memcmp(line, "chars c", 7) == 0)
    //        {
    //            // Ignore this line
    //        }
    //        else if (memcmp(line, "char", 4) == 0)
    //        {
    //            // Parse the current line and create a new CharDef
    //            u32 charID = _parseCharacterDefinition(_fontConf,line);
    //            _fontConf->m_characterSet.append(charID);
    //        }
    //        else if (memcmp(line, "kerning first", 13) == 0)
    //        {
    //            _parseKerningEntry(line);
    //        }
    //    }
}

void SVBMFontParseTextFormat::_interpretInfo(SVBMFontConfPtr _fontConf, cptr8 line){
    
}

void SVBMFontParseTextFormat::_interpretCommon(SVBMFontConfPtr _fontConf, cptr8 line){
    
}

void SVBMFontParseTextFormat::_interpretChar(SVBMFontConfPtr _fontConf, cptr8 line){
    
}
void SVBMFontParseTextFormat::_interpretKerning(SVBMFontConfPtr _fontConf, cptr8 line){
    
}
void SVBMFontParseTextFormat::_interpretPage(SVBMFontConfPtr _fontConf, cptr8 line){
    
}
