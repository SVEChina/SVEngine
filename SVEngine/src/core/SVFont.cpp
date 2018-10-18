//
// SVFont.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFont.h"
SVFont::SVFont(SVInst *_app)
:SVGBase(_app) {

}

SVFont::~SVFont() {
    
}

void SVFont::init(){
    
}

void SVFont::setTextEncoding(SVFONTTEXTENCODING _encoding){
    m_encoding = _encoding;
}

f32 SVFont::getTextWidth(cptr8 _text, s32 _count){
    return 0;
}

s32 SVFont::_getTextLength(cptr8 _text){
//    if( m_encoding == UTF16 )
//    {
//        int textLen = 0;
//        for(;;)
//        {
//            unsigned int len;
//            int r = acUtility::DecodeUTF16(&text[textLen], &len);
//            if( r > 0 )
//                textLen += len;
//            else if( r < 0 )
//                textLen++;
//            else
//                return textLen;
//        }
//    }
//
//    // Both UTF8 and standard ASCII strings can use strlen
//    return (int)strlen(text);
    return 0;
}
