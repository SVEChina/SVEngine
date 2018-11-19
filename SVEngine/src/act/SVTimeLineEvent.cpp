//
//  SVTimeLineEvent.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVTimeLineEvent.h"

SVTimeLineEvent::SVTimeLineEvent(SVInst* _app,f32 _time,s32 _rate)
:SVTimeLine(_app,_time,_rate){
    m_type = E_TL_T_EVENT;
}

SVTimeLineEvent::~SVTimeLineEvent() {
}

void SVTimeLineEvent::initKey() {
    SVKeyEventPtr m_startKey = MakeSharedPtr<SVKeyEvent>(mApp,0);
    m_keyPool.append(m_startKey);
    SVKeyEventPtr m_endKey = MakeSharedPtr<SVKeyEvent>(mApp,m_maxFrame);
    m_keyPool.append(m_endKey);
}


void SVTimeLineEvent::enter(SVNodePtr _nodePtr) {
    SVTimeLine::enter(_nodePtr);
}

void SVTimeLineEvent::exit(SVNodePtr _nodePtr) {
    SVTimeLine::exit(_nodePtr);
}

void SVTimeLineEvent::update(SVNodePtr _nodePtr,f32 _dt) {
    SVTimeLine::update(_nodePtr,_dt);
}

