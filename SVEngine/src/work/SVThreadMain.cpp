// SVThreadMain.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include <unistd.h>
#include "SVThreadMain.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderMgr.h"
#include "../base/SVLock.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVBasicSys.h"

SVThreadMain::SVThreadMain(SVInst *_app)
: SVThreadWork(_app,"SVThreadMain")
,m_first(true) {
    m_lock = MakeSharedPtr<SVLock>();
}

SVThreadMain::~SVThreadMain() {
    m_lock= nullptr;
    SV_LOG_INFO("SVThreadMain::~SVThreadMain\n");
}

void SVThreadMain::_innerUpdate(){
    m_lock->lock();
    SVThreadWork::_innerUpdate();
    //逻辑更新
    mApp->m_pGlobalMgr->update(_getDert());
    //逻辑数据交换到渲染数据
    mApp->getRenderMgr()->swapData();
    //渲染
    mApp->getRenderMgr()->render();
    //输出
    mApp->getBasicSys()->output();
    m_lock->unlock();
}

void SVThreadMain::_innerDestroy(){
//    //回收GL资源
//    mApp->getRenderMgr()->recycleRes();
    SVThreadWork::_innerDestroy();
}

f32 SVThreadMain::_getDert() {
    f32 dt = 0;
    if(mApp->getTimeState() == ENG_TS_NOR ){
        if (m_first) {
            m_first = false;
            timeval t_curT;
            gettimeofday(&t_curT, NULL);
            m_lastT = t_curT;
            dt = 0.33f;    //按照30帧算的
        } else {
            timeval t_curT;
            gettimeofday(&t_curT, NULL);
            s32 t_ms = s32( (t_curT.tv_sec - m_lastT.tv_sec)*1000+(t_curT.tv_usec - m_lastT.tv_usec) * 0.001f);
            dt = t_ms * 0.001f;
            m_lastT = t_curT;
            SV_LOG_INFO("time dert %f \n",dt);
        }
    }else if(mApp->getTimeState() == ENG_TS_FIX ){
        dt = 1.0f/30.0f;
    }else if(mApp->getTimeState() == ENG_TS_STOP ){
        dt = 0.0f;
    }
    return dt;
}

void SVThreadMain::resetTime(){
    m_first = true;
}


void SVThreadMain::clearThreadCache(){
    m_lock->lock();
    SVThreadWork::_innerUpdate();
    //逻辑更新
    mApp->m_pGlobalMgr->update(0.033f);
    //清楚渲染数据
    mApp->getRenderMgr()->clearData();
    //清屏
    mApp->getRenderMgr()->clearScreen();
    m_lock->unlock();
}


SVThreadSync::SVThreadSync(SVInst *_app)
: SVThreadMain(_app) {
    m_name = "SVThreadSync";
}

SVThreadSync::~SVThreadSync() {
    SV_LOG_INFO("SVThreadSync::~SVThreadSync\n");
}

void SVThreadSync::startThread(){
    
}

void SVThreadSync::stopThread(){
    
}

void SVThreadSync::syncUpdate(f32 _dt){
    m_lock->lock();
    SVThreadWork::_innerUpdate();
    //逻辑更新
    mApp->m_pGlobalMgr->update(_dt);
    //逻辑数据交换到渲染数据
    mApp->getRenderMgr()->swapData();
    //渲染
    mApp->getRenderMgr()->render();
    //输出
    mApp->getBasicSys()->output();
    m_lock->unlock();
}
