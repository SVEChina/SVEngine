//
//  SVTimeLineEvent.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVTimeLineEvent.h"

SVTimeLineEvent::SVTimeLineEvent(SVInst* _app,f32 _time)
:SVTimeLine(_app,_time){
    m_type = E_TL_T_EVENT;
    m_startKey = MakeSharedPtr<SVKeyEvent>(mApp,0.0f);
    m_endKey = MakeSharedPtr<SVKeyEvent>(mApp,_time);
}

SVTimeLineEvent::~SVTimeLineEvent() {
}

void SVTimeLineEvent::enter(SVNodePtr _nodePtr) {
    SVTimeLine::enter(_nodePtr);
}

void SVTimeLineEvent::exit(SVNodePtr _nodePtr) {
    SVTimeLine::exit(_nodePtr);
}

void SVTimeLineEvent::update(SVNodePtr _nodePtr,f32 _dt) {
    SVTimeLine::update(_nodePtr,_dt);
    f32 t_start_t = m_accTime - _dt;
    f32 t_end_t = m_accTime;
    //
    for(s32 i=0;i<m_keyPool.size();i++) {
        if(m_keyPool[i]->m_time >= t_start_t && m_keyPool[i]->m_time < t_end_t) {
            //执行key
            SVKeyEventPtr t_event_key = std::dynamic_pointer_cast<SVKeyEvent>(m_keyPool[i]);
        }
    }
}


