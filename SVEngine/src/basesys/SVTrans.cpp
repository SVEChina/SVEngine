//
// SVTrans.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVTrans.h"

SVTrans::SVTrans(SVInst *_app)
:SVGBase(_app){
    m_width = 0;
    m_height = 0;
    m_picformate = SV_PF_RGBA;
    m_angle = 0;
}

SVTrans::~SVTrans() {
}

void SVTrans::init(s32 _w, s32 _h,f32 _angle,PICFORMATE _picformate,SVTEXTYPE _tt){
    m_width = _w;
    m_height = _h;
    m_picformate = _picformate;
    m_angle = _angle;
    m_texTT = _tt;
}

void SVTrans::setAngle(f32 _angle){
    m_angle=_angle;
}

void SVTrans::destroy(){
}

void SVTrans::update(f32 dt) {
    
}

void SVTrans::render() {
    
}
