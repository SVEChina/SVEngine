//
// SVGameBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVGameBase.h"
#include "SVGameReady.h"
#include "SVGameRun.h"
#include "SVGameEnd.h"


SVGameBase::SVGameBase(SVInst *_app)
:SVModuleBase(_app) {
    m_pGameReady = nullptr;
    m_pGameRun = nullptr;
    m_pGameEnd = nullptr;
    m_stage = E_G_STAGE_WAIT;
}

SVGameBase::~SVGameBase() {
    m_pGameReady = nullptr;
    m_pGameRun = nullptr;
    m_pGameEnd = nullptr;
}

void SVGameBase::init(SVGameReadyPtr _ready,SVGameRunPtr _run,SVGameEndPtr _end) {
    SVModuleBase::init();
    startListen();
    m_pGameReady = _ready;
    m_pGameRun = _run;
    m_pGameEnd = _end;
    //
    if(m_pGameReady) {
        m_pGameReady->init();
    }
    if( m_pGameRun ) {
        m_pGameRun->init();
    }
    if(m_pGameEnd) {
        m_pGameEnd->init();
    }
}

void SVGameBase::destroy() {
    stopListen();
    if (m_pGameReady) {
        m_pGameReady->destroy();
    }
    if (m_pGameRun) {
        m_pGameRun->destroy();
    }
    if (m_pGameEnd) {
        m_pGameEnd->destroy();
    }
    SVModuleBase::destroy();
}

void SVGameBase::update(f32 _dt) {
    if(m_stage == E_G_STAGE_WAIT) {
    }
    //开始
    if(m_stage == E_G_STAGE_BEGIN) {
        if(m_pGameReady) {
            //准备逻辑
            if( m_pGameReady->isEnd() ) {
                m_stage = E_G_STAGE_RUN;
                m_pGameReady->exit();
                if(m_pGameRun) {
                    m_pGameRun->enter();
                }
            }else{
                m_pGameReady->update(_dt);
            }
        }else{
            //无准备
            m_stage = E_G_STAGE_RUN;
            if(m_pGameRun) {
                m_pGameRun->enter();
            }
        }
    }
    //循环
    if(m_stage == E_G_STAGE_RUN) {
        if(m_pGameRun) {
            if( m_pGameRun->isEnd() ) {
                m_stage = E_G_STAGE_END;
                m_pGameRun->exit();
                if(m_pGameEnd){
                    m_pGameEnd->enter();
                }
            }else{
                m_pGameRun->update(_dt);
            }
        }else{
            m_stage = E_G_STAGE_END;
            if(m_pGameEnd){
                m_pGameEnd->enter();
            }
        }
    }
    //结束
    if(m_stage == E_G_STAGE_END) {
        if(m_pGameEnd){
            if( m_pGameEnd->isEnd() ) {
                m_stage = E_G_STAGE_WAIT;
                m_pGameEnd->exit();
            }else{
                m_pGameEnd->update(_dt);
            }
        }else{
            m_stage = E_G_STAGE_WAIT;
        }
    }
}

void SVGameBase::open() {
    SVModuleBase::open();
    if(m_stage == E_G_STAGE_WAIT) {
        m_stage = E_G_STAGE_BEGIN;
        if(m_pGameReady){
            m_pGameReady->enter();
        }
    }
}

void SVGameBase::close() {
    SVModuleBase::close();
    if(m_stage != E_G_STAGE_WAIT) {
        m_stage = E_G_STAGE_WAIT;
        if(m_pGameReady) {
            m_pGameReady->exit();
        }
        if(m_pGameRun){
            m_pGameRun->exit();
        }
        if(m_pGameEnd){
            m_pGameEnd->exit();
        }
    }
}

bool SVGameBase::procEvent(SVEventPtr _event){
    return true;
}


