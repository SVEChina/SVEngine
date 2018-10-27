//
// SVOutMission.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOutMission.h"

SVOutMission::SVOutMission(SVInst* _app,cptr8 _name)
:SVEventProc(_app)
,m_name(_name)
,m_pOutStreamStateCB(nullptr){
}

SVOutMission::~SVOutMission() {
    m_pOutStreamStateCB = nullptr;
}

cptr8 SVOutMission::getMissionName() {
    return m_name.c_str();
}

void SVOutMission::output(){
    if(m_pOutStreamStateCB) {
        (*m_pOutStreamStateCB)(1);
    }
}

bool SVOutMission::isEnd() {
    return true;
}
