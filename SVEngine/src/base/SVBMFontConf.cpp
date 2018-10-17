//
// SVBMFontConf.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBMFontConf.h"
SVBMFontConf::SVBMFontConf(SVInst *_app): SVFontConf(_app) {
}

SVBMFontConf::~SVBMFontConf() {
    m_fontDefMap.clear();
    m_characterSet.clear();
}
