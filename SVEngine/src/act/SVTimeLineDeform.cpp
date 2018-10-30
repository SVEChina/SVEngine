//
//  SVTimeLineDeform.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVTimeLineDeform.h"

SVTimeLineDeform::SVTimeLineDeform(SVInst* _app,f32 _time,s32 _rate)
:SVTimeLine(_app,_time,_rate){
    m_type = E_TL_T_DEFORM;
    m_startKey = MakeSharedPtr<SVKeyDeform>(mApp,0);
    u32 t_maxFrame = SVTimeLine::maxFrame(_time,_rate);
    m_endKey = MakeSharedPtr<SVKeyDeform>(mApp,t_maxFrame);
}

SVTimeLineDeform::~SVTimeLineDeform() {
}

void SVTimeLineDeform::enter(SVNodePtr _nodePtr) {
    SVTimeLine::enter(_nodePtr);
}

void SVTimeLineDeform::exit(SVNodePtr _nodePtr) {
    SVTimeLine::exit(_nodePtr);
}

void SVTimeLineDeform::update(SVNodePtr _nodePtr,f32 _dt) {
    SVTimeLine::update(_nodePtr,_dt);
    //插值
}


