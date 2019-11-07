//
// SVUIButton.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVUIButton.h"
#include "../event/SVOpEvent.h"
#include "../event/SVEventMgr.h"
//
SVUIButton::SVUIButton(SVInst *_app)
:SVUIPanel(_app) {
    ntype = "SVUIButton";
    m_canSelect = true;
    m_p_cb_obj = nullptr;
    m_callback = nullptr;
}

SVUIButton::SVUIButton(SVInst *_app,f32 _w,f32 _h)
:SVUIPanel(_app) {
    ntype = "SVUIButton";
    m_canSelect = true;
    m_visible = true;
    m_p_cb_obj = nullptr;
    m_callback = nullptr;
}

SVUIButton::~SVUIButton() {
}

void SVUIButton::setCallBack(sv_uibutton_callback _cb, void* _obj){
    m_callback = _cb;
    m_p_cb_obj = _obj;
}

bool SVUIButton::procEvent(SVEventPtr _event){
    SVPickGetUIEventPtr t_tmp = DYN_TO_SHAREPTR(SVPickGetUIEvent, _event);
    if (t_tmp && t_tmp->m_pNode) {
        if (t_tmp->m_pNode == THIS_TO_SHAREPTR(SVUIButton)) {
            t_tmp->m_pNode->setbeSelect(false);
            if (m_callback) {
                (*m_callback)(THIS_TO_SHAREPTR(SVUIButton), m_p_cb_obj);
            }
        }
    }
    return true;
}
