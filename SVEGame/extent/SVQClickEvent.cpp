//
// Created by lidm on 17/9/5.
//

#include "SVQClickEvent.h"

//增加分数
SVEventAddScore::SVEventAddScore(){
    m_score = 10.0f;
    m_playerCode = 0;
}

//设置分数
SVEventSetScore::SVEventSetScore() {
    m_score = 1000.0f;
    m_playerCode = -1;
}

//设置分数
SVEventGameInfo::SVEventGameInfo() {
    m_timelen = 10.0f;
    m_playerCode = 0;
}
