//
// SVRecycleProcess.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRecycleProcess.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"

SVRecycleProcess::SVRecycleProcess(SVInst *_app)
:SVProcess(_app) {
}

SVRecycleProcess::~SVRecycleProcess() {
}

void SVRecycleProcess::update(f32 _dt){
    
}

bool SVRecycleProcess::procEvent(SVEventPtr _event){
    SVRecycleEventPtr t_event = DYN_TO_SHAREPTR(SVRecycleEvent,_event);
    if(t_event && t_event->m_obj){
        t_event->m_obj = nullptr;
        return false;
    }
    return true;
}
