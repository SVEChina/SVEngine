//
// SVActBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVActBase.h"
#include "../node/SVNode.h"
SVActParam::SVActParam(){
    
}

SVActBase::SVActBase(SVInst* _app):SVGBase(_app) {
    m_act_callback = nullptr;
    m_p_cb_obj = nullptr;
    m_isEnd = false;
}

SVActBase::~SVActBase() {
}

void SVActBase::initParam(SVActParamPtr _paramPtr){
}

void SVActBase::run(SVNodePtr _nodePtr, f32 dt) {
}

void SVActBase::enter(SVNodePtr _nodePtr) {
}

void SVActBase::exit(SVNodePtr _nodePtr) {
}

bool SVActBase::isEnd() {
    return m_isEnd;
}

void SVActBase::reset(){
    
}

void SVActBase::setActCallBack(sv_act_callback _act_callback, void *_obj){
    m_act_callback = _act_callback;
    m_p_cb_obj = _obj;
}
