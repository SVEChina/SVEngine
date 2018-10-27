//
// SVOutMission.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOutMissionNor.h"

SVOutMissionNor::SVOutMissionNor(SVInst* _app,cptr8 _name)
:SVOutMission(_app,_name)
,m_end(false){
}

SVOutMissionNor::~SVOutMissionNor() {
}

void SVOutMissionNor::output() {
    SVOutMission::output();
}

void SVOutMissionNor::setEnd() {
    m_end = true;
}

bool SVOutMissionNor::isEnd() {
    return m_end;
}
