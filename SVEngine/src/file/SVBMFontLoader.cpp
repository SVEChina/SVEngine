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

void SVBMFontLoader::loadData(cptr8 _name, SVBMFontConf *_fontConf) {
//    std::string data = FileUtils::getInstance()->getStringFromFile(controlFile);
    SVDataChunk tSVDataChunk;
    bool t_flag = mApp->getFileMgr()->loadFileContentStr(&tSVDataChunk, _name);
    if (!t_flag) {
        return;
    }
    u8* data = (u8 *)tSVDataChunk.m_data;
    if (tSVDataChunk.m_size == 0)
    {
        return;
    }
    if (std::strlen((cptr8)data) >= (sizeof("BMP") - 1) && memcmp("BMF", data, sizeof("BMP") - 1) == 0) {
        // Handle fnt file of binary format
        _parseBinaryConfigFile(_fontConf, data, std::strlen((cptr8)data), _name);
        return;
    }
    if (data[0] == 0)
    {
        SV_LOG_ERROR("Error: parsing BMFontFile %s", _name);
        return;
    }
    cptr8 contents = (cptr8)data;
    u64 contentsLen = (u64)std::strlen(contents);
    c8 line[512] = {0};
    
    u8 *next = (u8 *)std::strchr(contents, '\n');
    u8 *base = data;
    u64 lineLength = 0;
    u64 parseCount = 0;
    u32 basePos = 0;
    u32 nextPos = SVString(contents).find('\n');
    while (next)
    {
        lineLength = ((u32)(nextPos - basePos));
        memcpy(line, contents + parseCount, lineLength);
        line[lineLength] = 0;
        
        parseCount += lineLength + 1;
        if (parseCount < contentsLen)
        {
            base = next + 1;
            basePos = basePos + 1;
            
            next = (u8 *)std::strchr((cptr8)base, '\n');
            nextPos = SVString((cptr8)next).find('\n');
        }
        else
        {
            next = nullptr;
        }
        
        if (memcmp(line, "info face", 9) == 0)
        {
            // FIXME: info parsing is incomplete
            // Not needed for the Hiero editors, but needed for the AngelCode editor
            //            [self parseInfoArguments:line];
            _parseInfoArguments(line);
        }
        // Check to see if the start of the line is something we are interested in
        else if (memcmp(line, "common lineHeight", 17) == 0)
        {
            _parseCommonArguments(line);
        }
        else if (memcmp(line, "page id", 7) == 0)
        {
            _parseImageFileName(line, _name);
        }
        else if (memcmp(line, "chars c", 7) == 0)
        {
            // Ignore this line
        }
        else if (memcmp(line, "char", 4) == 0)
        {
            // Parse the current line and create a new CharDef
            u32 charID = _parseCharacterDefinition(_fontConf,line);
            _fontConf->m_characterSet.append(charID);
        }
        else if (memcmp(line, "kerning first", 13) == 0)
        {
            _parseKerningEntry(line);
        }
    }
}

void SVBMFontLoader::_parseBinaryConfigFile(SVBMFontConf *_fontConf, u8 *_pData, u64 _size, cptr8 _name){
    /* based on http://www.angelcode.com/products/bmfont/doc/file_format.html file format */
    u64 remains = _size;
    _pData += 4; remains -= 4;
    while (remains > 0)
    {
        u8 blockId = _pData[0]; _pData += 1; remains -= 1;
        u32 blockSize = 0; memcpy(&blockSize, _pData, 4);
        _pData += 4; remains -= 4;
        if (blockId == 1)
        {
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
            m_padding.top = (u8)_pData[7];
            m_padding.right = (u8)_pData[8];
            m_padding.bottom = (u8)_pData[9];
            m_padding.left = (u8)_pData[10];
        }
        else if (blockId == 2)
        {
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
            u16 lineHeight = 0; memcpy(&lineHeight, _pData, 2);
            _fontConf->m_commonHeight = lineHeight;
            
            u16 scaleW = 0; memcpy(&scaleW, _pData + 4, 2);
            u16 scaleH = 0; memcpy(&scaleH, _pData + 6, 2);
            
//            CCASSERT(scaleW <= Configuration::getInstance()->getMaxTextureSize() && scaleH <= Configuration::getInstance()->getMaxTextureSize(), "CCLabelBMFont: page can't be larger than supported");
            
            u16 pages = 0; memcpy(&pages, _pData + 8, 2);
//            CCASSERT(pages == 1, "CCBitfontAtlas: only supports 1 page");
        }
        else if (blockId == 3)
        {
            /*
             pageNames     p*(n+1)     strings     0     p null terminated strings, each with length n
             */
            
            cptr8 *value = (cptr8 *)_pData;
//            assert(std::strlen(value) < blockSize && "Block size should be less then string");
           // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//            _atlasName = FileUtils::getInstance()->fullPathFromRelativeFile(value, controlFile);
        }
        else if (blockId == 4)
        {
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
            u64 count = blockSize / 20;
            for (u64 i = 0; i < count; i++)
            {
                u32 charId = 0; memcpy(&charId, _pData + (i * 20), 4);
                
                SVBMFONTDEF& fontDef = _fontConf->m_fontDefMap[charId];
                fontDef.charID = charId;
                
                u16 charX = 0; memcpy(&charX, _pData + (i * 20) + 4, 2);
                fontDef.rect.m_lb_pt.x = charX;
                
                u16 charY = 0; memcpy(&charY, _pData + (i * 20) + 6, 2);
                fontDef.rect.m_rt_pt.y = charY;
                
                u16 charWidth = 0; memcpy(&charWidth, _pData + (i * 20) + 8, 2);
                fontDef.rect.m_rt_pt.x = charX + charWidth;
                
                u16 charHeight = 0; memcpy(&charHeight, _pData + (i * 20) + 10, 2);
                fontDef.rect.m_lb_pt.y = charY + charHeight;
                
                u16 xoffset = 0; memcpy(&xoffset, _pData + (i * 20) + 12, 2);
                fontDef.xOffset = xoffset;
                
                u16 yoffset = 0; memcpy(&yoffset, _pData + (i * 20) + 14, 2);
                fontDef.yOffset = yoffset;
                
                u16 xadvance = 0; memcpy(&xadvance, _pData + (i * 20) + 16, 2);
                fontDef.xAdvance = xadvance;
                
                _fontConf->m_characterSet.append(fontDef.charID);
            }
        }
        else if (blockId == 5) {
            /*
             first  4   uint    0+c*10     These fields are repeated until all kerning pairs have been described
             second 4   uint    4+c*10
             amount 2   int     8+c*10
             */
            unsigned long count = blockSize / 20;
            for (unsigned long i = 0; i < count; i++)
            {
                s32 first = 0; memcpy(&first, _pData + (i * 10), 4);
                s32 second = 0; memcpy(&second, _pData + (i * 10) + 4, 4);
                s16 amount = 0; memcpy(&amount, _pData + (i * 10) + 8, 2);
                
                u64 key = ((u64)first<<32) | ((u64)second&0xffffffffll);
                m_kerningDictionary[key] = amount;
            }
        }
        
        _pData += blockSize; remains -= blockSize;
    }
}

u32  SVBMFontLoader::_parseCharacterDefinition(SVBMFontConf *_fontConf, cptr8 line){
    u32 charID = 0;
    //////////////////////////////////////////////////////////////////////////
    // line to parse:
    // char id=32   x=0     y=0     width=0     height=0     xoffset=0     yoffset=44    xadvance=14     page=0  chnl=0
    //////////////////////////////////////////////////////////////////////////
    // Character ID
    auto tmp = strstr(line, "id=") + 3;
    sscanf(tmp, "%u", &charID);
    
    SVBMFONTDEF& characterDefinition = _fontConf->m_fontDefMap[charID];
    characterDefinition.charID = charID;
    
    // Character x
    tmp = strstr(tmp, "x=") + 2;
    sscanf(tmp, "%f", &characterDefinition.rect.m_lb_pt.x);
    // Character y
    tmp = strstr(tmp, "y=") + 2;
    sscanf(tmp, "%f", &characterDefinition.rect.m_rt_pt.y);
    // Character width
    tmp = strstr(tmp, "width=") + 6;
    sscanf(tmp, "%f", &characterDefinition.rect.m_rt_pt.x - &characterDefinition.rect.m_lb_pt.x);
    // Character height
    tmp = strstr(tmp, "height=") + 7;
    sscanf(tmp, "%f", &characterDefinition.rect.m_lb_pt.y - &characterDefinition.rect.m_rt_pt.y);
    // Character xoffset
    tmp = strstr(tmp, "xoffset=") + 8;
    sscanf(tmp, "%hd", &characterDefinition.xOffset);
    // Character yoffset
    tmp = strstr(tmp, "yoffset=") + 8;
    sscanf(tmp, "%hd", &characterDefinition.yOffset);
    // Character xadvance
    tmp = strstr(tmp, "xadvance=") + 9;
    sscanf(tmp, "%hd", &characterDefinition.xAdvance);
    
    return charID;
}
void SVBMFontLoader::_parseInfoArguments(cptr8 line){
    //////////////////////////////////////////////////////////////////////////
    // possible lines to parse:
    // info face="Script" size=32 bold=0 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=1 padding=1,4,3,2 spacing=0,0 outline=0
    // info face="Cracked" size=36 bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=1,1
    //////////////////////////////////////////////////////////////////////////
//    sscanf(strstr(line, "size=") + 5, "%d", &m_fontSize);
    // padding
    sscanf(strstr(line,"padding=") + 8, "%d,%d,%d,%d", &m_padding.top, &m_padding.right, &m_padding.bottom, &m_padding.left);
}
void SVBMFontLoader::_parseCommonArguments(cptr8 line){
    //////////////////////////////////////////////////////////////////////////
    // line to parse:
    // common lineHeight=104 base=26 scaleW=1024 scaleH=512 pages=1 packed=0
    //////////////////////////////////////////////////////////////////////////
    
    // Height
    auto tmp = strstr(line, "lineHeight=") + 11;
//    sscanf(tmp, "%d", &m_commonHeight);
}
void SVBMFontLoader::_parseImageFileName(cptr8 line, cptr8 fntFile){
    
}
void SVBMFontLoader::_parseKerningEntry(cptr8 line){
    //////////////////////////////////////////////////////////////////////////
    // line to parse:
    // kerning first=121  second=44  amount=-7
    //////////////////////////////////////////////////////////////////////////
    
    s32 first, second, amount;
    auto tmp = strstr(line, "first=") + 6;
    sscanf(tmp, "%d", &first);
    
    tmp = strstr(tmp, "second=") + 7;
    sscanf(tmp, "%d", &second);
    
    tmp = strstr(tmp, "amount=") + 7;
    sscanf(tmp, "%d", &amount);
    
    u64 key = ((u64)first<<32) | ((u64)second&0xffffffffll);
    
    m_kerningDictionary[key] = amount;
}
void SVBMFontLoader::_purgeKerningDictionary(){
    
}
void SVBMFontLoader::_purgeFontDefDictionary(){
    
}
