//
// SVPendraw.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPendraw.h"
#include "SVPenStroke.h"
#include "../../event/SVOpEvent.h"
#include "../SVGameReady.h"
#include "../SVGameRun.h"
#include "../SVGameEnd.h"
#include "../../app/SVInst.h"
#include "../../base/SVDataSwap.h"
SVPendraw::SVPendraw(SVInst *_app)
:SVGameBase(_app)
,m_curStroke(nullptr){
    
}

SVPendraw::~SVPendraw() {
    m_curStroke = nullptr;
}

void SVPendraw::init(SVGameReadyPtr _ready,SVGameRunPtr _run,SVGameEndPtr _end) {
    SVGameBase::init(_ready,_run,_end);
}

void SVPendraw::destroy() {
    SVGameBase::destroy();
}

void SVPendraw::update(f32 _dt) {
    SVGameBase::update(_dt);
    if (m_curStroke) {
        m_curStroke->update(_dt);
    }
}

void SVPendraw::open() {
    SVGameBase::open();
}

void SVPendraw::close() {
    SVGameBase::close();
}

bool SVPendraw::procEvent(SVEventPtr _event){
    //
    f32 t_camera_w = mApp->m_pGlobalParam->m_inner_width*1.0;
    f32 t_camera_h = mApp->m_pGlobalParam->m_inner_height*1.0;
    if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_BEGIN){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        f32 t_mod_x = t_touch->x;
        f32 t_mod_y = t_touch->y;
        f32 t_n_targetX = t_mod_x - t_camera_w*0.5;
        f32 t_n_targetY = t_mod_y - t_camera_h*0.5;
        if (!m_curStroke) {
            m_curStroke = MakeSharedPtr<SVPenStroke>(mApp);
            m_curStroke->begin(t_n_targetX,t_n_targetY,0.0f);
        }
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_END){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        f32 t_mod_x = t_touch->x;
        f32 t_mod_y = t_touch->y;
        f32 t_n_targetX = t_mod_x - t_camera_w*0.5;
        f32 t_n_targetY = t_mod_y - t_camera_h*0.5;
//        if(m_curStroke) {
//            m_curStroke->end(t_n_targetX,t_n_targetY,0.0f);
//        }
//        m_curStroke = nullptr;
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_MOVE){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        f32 t_mod_x = t_touch->x;
        f32 t_mod_y = t_touch->y;
        f32 t_n_targetX = t_mod_x - t_camera_w*0.5;
        f32 t_n_targetY = t_mod_y - t_camera_h*0.5;
        if(m_curStroke) {
            m_curStroke->draw(t_n_targetX,t_n_targetY,0.0f);
        }
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_ANCHOR_AR){
        SVARAnchorEventPtr anchor = DYN_TO_SHAREPTR(SVARAnchorEvent, _event);
        if (anchor && m_curStroke) {
            FMat4 localMat = FMat4((f32 *)anchor->m_matData->getData());
            m_curStroke->setModelMatrix(localMat);
        }
    }
    return true;
}


