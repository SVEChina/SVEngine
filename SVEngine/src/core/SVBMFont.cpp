//
// SVBMFont.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBMFont.h"
#include "SVUnicode.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../app/SVInst.h"
#include "../file/SVFileMgr.h"
#include "../file/SVBMFontLoader.h"

SVBMFontPtr SVBMFont::creatFnt(cptr8 _filePath, SVInst *_app){
    SVBMFontPtr font = MakeSharedPtr<SVBMFont>(_app);
    SVBMFontLoader t_loder(_app);
    bool t_res = t_loder.loadData(_filePath, font);
    if (t_res) {
        return font;
    }
    return nullptr;
}

SVBMFont::SVBMFont(SVInst *_app)
:SVFont(_app) {
    m_defChar.charID = 10000000;
    m_defChar.x = 0;
    m_defChar.y = 0;
    m_defChar.width = 0;
    m_defChar.height = 0;
    m_defChar.xOffset = 0;
    m_defChar.yOffset = 0;
    m_defChar.xAdvance = 0;
    m_defChar.page = 0;
    //
    m_scale = 1.0f;
    m_fontHeight = 0;
    m_fontScaleW = 0;
    m_fontScaleH = 0;
    m_base = 0;
    m_outlineThickness = 0;
    m_hasOutline = false;
    m_encoding = UTF8;
    m_enableMipMap = true;
}

SVBMFont::~SVBMFont() {
    m_charsMap.clear();
}

void SVBMFont::init(){

}

f32 SVBMFont::getTextWidth(cptr8 _text){
    s32 count = _getTextLength(_text);

    f32 x = 0;

    for( s32 n = 0; n < count; )
    {
        s32 charId = getTextChar(_text,n,&n);

        SVBMFont::SVBMFONTCHARINFO ch = getChar(charId);
        x += m_scale * (ch.xAdvance);

        if( n < count )
            x += _adjustForKerningPairs(charId, getTextChar(_text,n));
    }

    return x;
}

f32 SVBMFont::getTextHeight(cptr8 _text){
    return m_scale*m_fontHeight;
}

s32 SVBMFont::getTextLength(cptr8 _text){
    return _getTextLength(_text);
}

s32 SVBMFont::getTextChar(cptr8 _text, s32 _pos, s32 *_nextPos){
    s32 ch;
    u32 len;
    if( m_encoding == UTF8 )
    {
        ch = SVUnicode::decodeUTF8(&_text[_pos], &len);
        if( ch == -1 ) len = 1;
    }
    else if( m_encoding == UTF16 )
    {
        ch = SVUnicode::decodeUTF16(&_text[_pos], &len);
        if( ch == -1 ) len = 2;
    }
    else
    {
        len = 1;
        ch = (u8)_text[_pos];
    }
    
    if( _nextPos ) *_nextPos = _pos + len;
    return ch;
}

s32 SVBMFont::_getTextLength(cptr8 _text){
    if( m_encoding == UTF16 )
    {
        s32 textLen = 0;
        for(;;)
        {
            u32 len;
            int r = SVUnicode::decodeUTF16(&_text[textLen], &len);
            if( r > 0 )
                textLen += len;
            else if( r < 0 )
                textLen++;
            else
                return textLen;
        }
    }
    // Both UTF8 and standard ASCII strings can use strlen
    return (s32)strlen(_text);
}

SVBMFont::SVBMFONTCHARINFO SVBMFont::getChar(s32 _charID){
    SVMap<u32, SVBMFont::SVBMFONTCHARINFO>::Iterator it = m_charsMap.find(_charID);
    if( it == m_charsMap.end() ) return m_defChar;
    return it->data;
    
}

f32 SVBMFont::_adjustForKerningPairs(s32 _first, s32 _second){
    SVBMFont::SVBMFONTCHARINFO ch = getChar(_first);
    if( ch.charID == 10000000 ) return 0;
    for( u32 n = 0; n < ch.kerningPairs.size(); n += 2 )
    {
        if( ch.kerningPairs[n] == _second )
            return ch.kerningPairs[n+1] * m_scale;
    }
    return 0;
}

s32 SVBMFont::_findTextChar(cptr8 _text, s32 _start, s32 _length, s32 _ch){
    s32 pos = _start;
    s32 nextPos;
    s32 currChar = -1;
    while( pos < _length )
    {
        currChar = getTextChar(_text, pos, &nextPos);
        if( currChar == _ch )
            return pos;
        pos = nextPos;
    }
    return -1;
}

void SVBMFont::setFontInfo(s32 _outlineThickness){
    m_outlineThickness = _outlineThickness;
}

void SVBMFont::setCommonInfo(s32 _fontHeight, s32 _base, s32 _scaleW, s32 _scaleH, s32 _pages, bool _isPacked){
    m_fontHeight = _fontHeight;
    m_base = _base;
    m_fontScaleW = _scaleW;
    m_fontScaleH = _scaleH;
//    font->pages.resize(pages);
//    for( int n = 0; n < pages; n++ )
//        font->pages[n] = 0;
    
    if( _isPacked && m_outlineThickness )
        m_hasOutline = true;
}

void SVBMFont::addChar(s32 _charID, s32 _x, s32 _y, s32 _w, s32 _h, s32 _xoffset, s32 _yoffset, s32 _xadvance, s32 _page, s32 _chnl){
    // Convert to a 4 element vector
    // TODO: Does this depend on hardware? It probably does
    if     ( _chnl == 1 ) _chnl = 0x00010000;  // Blue channel
    else if( _chnl == 2 ) _chnl = 0x00000100;  // Green channel
    else if( _chnl == 4 ) _chnl = 0x00000001;  // Red channel
    else if( _chnl == 8 ) _chnl = 0x01000000;  // Alpha c¯≤hannel
    else _chnl = 0;
    
    if( _charID >= 0 )
    {
        SVBMFONTCHARINFO ch;
        ch.x = _x;
        ch.y = _y;
        ch.width = _w;
        ch.height = _h;
        ch.xOffset = _xoffset;
        ch.yOffset = _yoffset;
        ch.xAdvance = _xadvance;
        ch.page = _page;
        ch.chnl = _chnl;
        m_charsMap.append(_charID, ch);
    }
    
    if( _charID == -1 )
    {
        m_defChar.x = _x;
        m_defChar.y = _y;
        m_defChar.width = _w;
        m_defChar.height = _h;
        m_defChar.xOffset = _xoffset;
        m_defChar.yOffset = _yoffset;
        m_defChar.xAdvance = _xadvance;
        m_defChar.page = _page;
        m_defChar.chnl = _chnl;
    }
}

void SVBMFont::addKerningPair(s32 _first, s32 _second, s32 _amount){
    SVMap<u32, SVBMFont::SVBMFONTCHARINFO>::Iterator it = m_charsMap.find(_first);
    if( _first >= 0 && _first < 256 && it!=m_charsMap.end() ){
        m_charsMap[_first].kerningPairs.append(_second);
        m_charsMap[_first].kerningPairs.append(_amount);
    }
}

void SVBMFont::loadPage(s32 _pageID, cptr8 _pageFile){
    SVString rootFile = m_fntFilePath;
    s32 pos = rootFile.rfind('/');
    rootFile = SVString::substr(rootFile.c_str(), 0, pos);
    SVString path = rootFile + "/" + _pageFile;
    SVTexturePtr texture = mApp->getTexMgr()->getTexture(path, true, true);
    m_textures.append(_pageID, texture);
}
