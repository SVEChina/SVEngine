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

SVUIBase::~SVUIBase() {
    
}

void SVUIBase::setArcho(EUIARCHO _cho) {
    m_archo = _cho;
}

EUIARCHO SVUIBase::getArcho() {
    return m_archo;
}

SVRect SVUIBase::getRect(EUIARCHO _archo,f32 _w,f32 _h) {
    f32 t_off_x = 0.0f;
    f32 t_off_y = 0.0f;
    if(_archo == E_ARCHO_LT) {
        t_off_x = _w*0.5f;
        t_off_y = -_h*0.5f;
    }else if(_archo == E_ARCHO_CT) {
        t_off_x = 0.0f;
        t_off_y = -_h*0.5f;
    }else if(_archo == E_ARCHO_RT) {
        t_off_x = -_w*0.5f;
        t_off_y = -_h*0.5f;
    }else if(_archo == E_ARCHO_LC) {
        t_off_x = _w*0.5f;
        t_off_y = 0.0f;
    }else if(_archo == E_ARCHO_CC) {
        t_off_x = 0.0f;
        t_off_y = 0.0f;
    }else if(_archo == E_ARCHO_RC) {
        t_off_x = -_w*0.5f;
        t_off_y = 0.0f;
    }else if(_archo == E_ARCHO_LB) {
        t_off_x = _w*0.5f;
        t_off_y = _h*0.5f;
    }else if(_archo == E_ARCHO_CB) {
        t_off_x = 0.0f;
        t_off_y = _h*0.5f;
    }else if(_archo == E_ARCHO_RB) {
        t_off_x = -_w*0.5f;
        t_off_y = _h*0.5f;
    }
    SVRect t_rc;
    t_rc.m_lb_pt = FVec2(_w*-0.5f+t_off_x, _h*-0.5f+t_off_y);
    t_rc.m_rt_pt = FVec2(_w*0.5f+t_off_x, _h*0.5f+t_off_y);
    return t_rc;
}
