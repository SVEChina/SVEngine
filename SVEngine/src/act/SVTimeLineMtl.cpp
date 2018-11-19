//
//  SVTimeLineMtl.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVTimeLineMtl.h"

SVTimeLineMtl::SVTimeLineMtl(SVInst* _app,f32 _time,s32 _rate)
:SVTimeLine(_app,_time,_rate){
    m_type = E_TL_T_MTL;
}

SVTimeLineMtl::~SVTimeLineMtl() {
}

void SVTimeLineMtl::initKey() {
    SVKeyMtlPtr m_startKey = MakeSharedPtr<SVKeyMtl>(mApp,0);
    m_keyPool.append(m_startKey);
    SVKeyMtlPtr m_endKey = MakeSharedPtr<SVKeyMtl>(mApp,m_maxFrame);
    m_keyPool.append(m_endKey);
}

void SVTimeLineMtl::enter(SVNodePtr _nodePtr) {
    SVTimeLine::enter(_nodePtr);
}

void SVTimeLineMtl::exit(SVNodePtr _nodePtr) {
    SVTimeLine::exit(_nodePtr);
}

void SVTimeLineMtl::update(SVNodePtr _nodePtr,f32 _dt) {
    SVTimeLine::update(_nodePtr,_dt);
}


