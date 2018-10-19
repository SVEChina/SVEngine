//
// SVBMFont.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBMFont.h"
#include "SVUnicode.h"
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
    m_fontSize = 0;
    m_fontHeight = 0;
    m_base = 0;
    m_scaleW = 0;
    m_scaleH = 0;
}

SVBMFont::~SVBMFont() {
    m_charsMap.clear();
}

void SVBMFont::init(){

}


f32 SVBMFont::getTextWidth(cptr8 _text, s32 _count){
    if( _count <= 0 )
        _count = _getTextLength(_text);

    f32 x = 0;

    for( s32 n = 0; n < _count; )
    {
        s32 charId = getTextChar(_text,n,&n);

        SVBMFont::SVBMFONTCHARINFO ch = getChar(charId);
        x += m_scale * (ch.xAdvance);

        if( n < _count )
            x += _adjustForKerningPairs(charId, getTextChar(_text,n));
    }

    return x;
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
