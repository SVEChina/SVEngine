//
// SVBMFont.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBMFont.h"
SVBMFont::SVBMFont(SVInst *_app)
:SVFont(_app) {

}

SVBMFont::~SVBMFont() {
    m_charsMap.clear();
}

void SVBMFont::init(){

}
