//
// SVBasicSys.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBasicSys.h"
#include "SVRecycleProcess.h"
#include "SVPickProcess.h"
#include "SVFontProcess.h"
#include "SVStreamIn.h"
#include "SVStreamOut.h"
#include "SVPictureProcess.h"

SVBasicSys::SVBasicSys(SVInst *_app)
:SVSysBase(_app) {
    m_subsysType = "SVBasicSys";
    m_pRecycleModule = nullptr;
    m_pPickModule = nullptr;
    m_pFontModule = nullptr;
    m_pStreamIn = nullptr;
    m_pStreamOut = nullptr;
}

SVBasicSys::~SVBasicSys() {
    m_pStreamIn = nullptr;
    m_pStreamOut = nullptr;
}

void SVBasicSys::init() {
    //
    m_pRecycleModule = MakeSharedPtr<SVRecycleProcess>(mApp);
    m_pRecycleModule->startListen();
    //
    m_pPickModule = MakeSharedPtr<SVPickProcess>(mApp);
    m_pPickModule->disablePick();
    m_pPickModule->startListen();
    //
    m_pFontModule = MakeSharedPtr<SVFontProcess>(mApp);
    //
    m_pStreamIn = MakeSharedPtr<SVStreamIn>(mApp);
    //
    m_pStreamOut = MakeSharedPtr<SVStreamOut>(mApp);
    //
    m_pPicProc = MakeSharedPtr<SVPictureProcess>(mApp);
    m_pPicProc->init();
}

void SVBasicSys::destroy() {
    //
    m_pFontModule = nullptr;
    //
    m_pPickModule->stopListen();
    m_pPickModule = nullptr;
    //
    m_pRecycleModule->stopListen();
    m_pRecycleModule = nullptr;
    //
    m_pStreamIn = nullptr;
    //
    m_pStreamOut = nullptr;
    //
    if(m_pPicProc){
        m_pPicProc->destroy();
        m_pPicProc = nullptr;
    }
}

void SVBasicSys::update(f32 dt) {
    //输入流系统更新
    if(m_pStreamIn){
        m_pStreamIn->update(dt);
    }
    //输出流系统更新
    if(m_pStreamOut){
        m_pStreamOut->update(dt);
    }
    //相片处理更新
    if(m_pPicProc){
        m_pPicProc->update(dt);
    }
    //回收系统
    if (m_pRecycleModule) {
        m_pRecycleModule->update(dt);
    }
}

SVRecycleProcessPtr SVBasicSys::getRecycleModule(){
    return m_pRecycleModule;
}

SVPickProcessPtr SVBasicSys::getPickModule(){
    return m_pPickModule;
}

SVFontProcessPtr SVBasicSys::getFontModule(){
    return m_pFontModule;
}

SVStreamInPtr SVBasicSys::getStreamIn(){
    return m_pStreamIn;
}

SVStreamOutPtr SVBasicSys::getStreamOut(){
    return m_pStreamOut;
}

SVPictureProcessPtr SVBasicSys::getPicProc() {
    return m_pPicProc;
}

