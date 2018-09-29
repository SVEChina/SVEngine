//
// Created by lidm on 17/9/5.
//

#include "SVFaceDanceEvent.h"

//单个死亡
SVEventFDUnitDead::SVEventFDUnitDead(cptr8 _sceneName){
    m_gameTime = 0.0f; //系统时间
    m_score = 0.0f;    //分数
    m_energy = 0.0f;   //能量值
    m_iscrit = false;;
}

//清屏
SVEventFDUnitAllDead::SVEventFDUnitAllDead(cptr8 _sceneName) {
}

SVEventFDTimeChange::SVEventFDTimeChange(cptr8 _sceneName){
}

SVEventFDScoreChange::SVEventFDScoreChange(cptr8 _sceneName){
}

