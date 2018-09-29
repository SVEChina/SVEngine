//
// Created by yinlong on 17/10/20.
//

#ifndef SV_GAMECOMMONE_EVENT_H
#define SV_GAMECOMMONE_EVENT_H

#include "../event/SVEvent.h"

//事件

//游戏暂停
class SVGameState : public SVEvent {
public:
    SVGameState(const char *_sceneName , int iGameState);
    
    int getGameState();
private:
    int m_iGameState;
};

class MaGamePreBatFinish : public SVEvent{
public:
    MaGamePreBatFinish(const char *_sceneName);
};

class SVGameBatResult : public SVEvent{
public:
    SVGameBatResult(const char *_sceneName);
    
    int m_TotalScore;
    int m_exScore1;
    int m_exScore2;
};

//游戏开始
class SVEventGameStart : public SVEvent {
public:
    SVEventGameStart();
};

class SVEventGameEnterBattle : public SVEvent {
public:
    SVEventGameEnterBattle();
};

//游戏暂停
class SVGamePause : public SVEvent {
public:
    SVGamePause(const char *_sceneName);
};

//游戏结束
class SVGameOver : public SVEvent {
public:
    SVGameOver(const char *_sceneName);
    
    int m_iScole;
};

//连击开启
class SVEventComboStart : public SVEvent {
public:
    SVEventComboStart(const char *_sceneName);
};

//连击关闭
class SVEventComboStop : public SVEvent {
public:
    SVEventComboStop(const char *_sceneName);
};

//连击值刷新
class SVEventComboValue : public SVEvent {
public:
    SVEventComboValue(const char *_sceneName);
    
    s32 m_comboValue;
    
    s32 m_iComBoShowType;
    bool m_bShowComboValue;
};


#endif //SV_GAMECOMMONE_EVENT_H
