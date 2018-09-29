//
// Created by 付一洲 on 2017/5/5.
//

#include "SVOpGame.h"
#include "../app/SVInst.h"
#include "../logicsys/SVLogicMgr.h"
#include "../event/SVEventMgr.h"
#include "../parse/SVParseGameMain.h"
#include "../game/SVGameBase.h"
#include "../game/SVQClickMain.h"
#include "../game/SVQClickUnitMgr.h"
#include "../game/SVGameCommonEvent.h"

//加载游戏
SVOpLoadGame::SVOpLoadGame(SVInst *_app,cptr8 _packname,cb_func_game _cb)
:SVOpBase(_app)
,m_packName(_packname)
,m_pCB(_cb){
}

void SVOpLoadGame::_process(f32 dt) {
    stringc _path = m_packName + "/";
    SVParseGameMain tt_parseGame(mApp);
    if( tt_parseGame.parse(_path.c_str()) ){
        SVGameBase* t_game = tt_parseGame.generate();
        t_game->setCallback(m_pCB);
        mApp->m_pGlobalMgr->m_pLogicMgr->setGame(t_game);
        t_game->release();
    }
}

//
SVOpStartGame::SVOpStartGame(SVInst *_app)
:SVOpBase(_app) {
}

void SVOpStartGame::_process(f32 dt) {
    SVEventGameStart* t_gamestart = new SVEventGameStart();
    mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_gamestart);
}

//
SVOpCloseGame::SVOpCloseGame(SVInst *_app)
:SVOpBase(_app) {
}

void SVOpCloseGame::_process(f32 dt) {
    mApp->m_pGlobalMgr->m_pLogicMgr->setGame(nullptr);
}

//设置数据
SVOpSetGameData::SVOpSetGameData(SVInst *_app,cptr8 _info)
:SVOpBase(_app) {
    m_info = _info;
}

void SVOpSetGameData::_process(f32 dt) {
    SVGameBase* t_game = mApp->m_pGlobalMgr->m_pLogicMgr->getGame();
    if(t_game){
        t_game->pushData(m_info.c_str());
    }
}

//设置数据
SVOpSetGameHead::SVOpSetGameHead(SVInst *_app,s32 _playercode,s32 _w,s32 _h,void* _data)
:SVOpBase(_app) {
    m_playercode = _playercode;
    m_width = _w;
    m_height = _h;
    s32 t_len = m_width*m_height*4;
    m_pData = malloc(t_len);
    memcpy(m_pData,_data,t_len);
}

//设置头像数据
SVOpSetGameHead::~SVOpSetGameHead(){
    free(m_pData);
    m_pData = nullptr;
}

void SVOpSetGameHead::_process(f32 dt) {
    SVGameBase* t_game = mApp->m_pGlobalMgr->m_pLogicMgr->getGame();
    if(t_game){
        t_game->setHead(m_playercode,m_width,m_height,m_pData);
    }
}

