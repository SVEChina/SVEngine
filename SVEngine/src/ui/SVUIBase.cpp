//
// SVUIBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVUIBase.h"

//
SVUIBase::SVUIBase(SVInst *_app)
:SVNode(_app) {
    ntype = "SVUIBase";
    m_archo = E_ARCHO_CC;
    m_rsType = RST_UI;
}

void SVUIBase::setArcho(EUIARCHO _cho) {
    m_archo = _cho;
}

EUIARCHO SVUIBase::getArcho() {
    return m_archo;
}

SVRect SVUIBase::getRect(EUIARCHO _archo,f32 _w,f32 _h) {
    SVRect t_rc;
    
    return t_rc;
}
