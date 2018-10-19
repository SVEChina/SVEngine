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

