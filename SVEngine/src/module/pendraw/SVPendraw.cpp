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
}

void SVPendraw::open() {
    SVGameBase::open();
}

void SVPendraw::close() {
    SVGameBase::close();
}

bool SVPendraw::procEvent(SVEventPtr _event){
    //
    if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_BEGIN){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        f32 t_mod_x = t_touch->x;
        f32 t_mod_y = t_touch->y;
        m_curStroke = MakeSharedPtr<SVPenStroke>(mApp);
        m_curStroke->begin(0.0f,0.0f,0.0f);
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_END){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        f32 t_mod_x = t_touch->x;
        f32 t_mod_y = t_touch->y;
        if(m_curStroke) {
            m_curStroke->end(0.0f,0.0f,0.0f);
        }
        m_curStroke = nullptr;
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_MOVE){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        f32 t_mod_x = t_touch->x;
        f32 t_mod_y = t_touch->y;
        if(m_curStroke) {
            m_curStroke->draw(0.0f,0.0f,0.0f);
        }
    }
    return true;
}


