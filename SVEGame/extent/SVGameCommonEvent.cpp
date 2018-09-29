//
// Created by yinlong on 17/10/20.
//

#include "SVGameCommonEvent.h"

SVGameState::SVGameState(const char *_sceneName , int iGameState)
:m_iGameState(iGameState){
}

int SVGameState::getGameState(){
    return m_iGameState;
}

MaGamePreBatFinish::MaGamePreBatFinish(const char *_sceneName){
}

SVGameBatResult::SVGameBatResult(const char *_sceneName)
:m_TotalScore(-1),m_exScore1(-1),m_exScore2(-1){
}

//游戏开始
SVEventGameStart::SVEventGameStart(){
}

SVEventGameEnterBattle::SVEventGameEnterBattle(){
}

//游戏暂停
SVGamePause::SVGamePause(const char *_sceneName){
}


//游戏结束
SVGameOver::SVGameOver(const char *_sceneName)
:m_iScole(0){
}

//连击开启
SVEventComboStart::SVEventComboStart(const char *_sceneName){
}

//连击关闭
SVEventComboStop::SVEventComboStop(const char *_sceneName){
}

//连击值刷新
SVEventComboValue::SVEventComboValue(const char *_sceneName){
    m_comboValue = 0;
    m_iComBoShowType = 0;
    m_bShowComboValue = false;
}
