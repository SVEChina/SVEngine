//
// SVStreamOut.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVStreamOut.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../base/SVLock.h"
#include "../node/SVFrameOutIOS.h"
#include "../node/SVScene.h"
#include "../node/SVFrameOutTex.h"
#include "../node/SVFrameOutRead.h"
#include "../basesys/SVSceneMgr.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderCmdOut.h"


SVStreamOut::SVStreamOut(SVInst *_app)
:SVProcess(_app){
    m_lock = MakeSharedPtr<SVLock>();
    m_missionlock = MakeSharedPtr<SVLock>();
    m_pFrameOut = nullptr;
#if defined(SV_IOS)
    m_outMethod = E_OUT_M_IOS;
#elif defined(SV_ANDROID)
    m_outMethod = E_OUT_M_ANDRIOD;
#else
     m_outMethod = E_OUT_M_READPIEXL;
#endif
    m_outWidth = 0;
    m_outHeight = 0;
    m_outFormat = SV_OUT_STEAM_RGB2YUVI420;
}

SVStreamOut::~SVStreamOut() {
    m_lock = nullptr;
    m_missionlock = nullptr;
    m_pFrameOut = nullptr;
    m_mission.destroy();
}

//打开输出流
bool SVStreamOut::openOutStream() {
    if(m_pFrameOut) {
        return false;
    }
    if(m_outWidth == 0) {
        m_outWidth = mApp->m_pGlobalParam->m_inner_width;
    }
    if(m_outHeight == 0) {
        m_outHeight = mApp->m_pGlobalParam->m_inner_height;
    }
    //根据当前类型 打开输出流
    if( m_outMethod == E_OUT_M_NULL) {
        SV_LOG_INFO("please set outstream type! \n");
        return false;
    }else if( m_outMethod == E_OUT_M_READPIEXL) {
        SVFrameOutReadPtr t_streamNode = MakeSharedPtr<SVFrameOutRead>(mApp);
        if(t_streamNode) {
            t_streamNode->create(m_outWidth,m_outHeight);
            t_streamNode->setFormat(m_outFormat);
        }
        m_pFrameOut = t_streamNode;
    }else if( m_outMethod == E_OUT_M_MAP) {
        
    }else if( m_outMethod == E_OUT_M_PBO) {
        
    }else if( m_outMethod == E_OUT_M_IOS) {
#ifdef SV_IOS
        SVFrameOutIOSPtr t_streamNode = MakeSharedPtr<SVFrameOutIOS>(mApp);
        if( t_streamNode ) {
            t_streamNode->init(m_outFormat,m_outWidth,m_outHeight);
        }
        m_pFrameOut = t_streamNode;
#endif
    }else if( m_outMethod == E_OUT_M_ANDRIOD) {
#ifdef SV_ANDROID
        m_pFrameOut = MakeSharedPtr<SVFrameOutTex>(mApp);
#endif
    }
    if(m_pFrameOut) {
        return true;
    }
    return false;
}

//关闭输出输出流
void SVStreamOut::closeOutStream() {
    if(m_pFrameOut) {
        m_pFrameOut->removeFromParent();
        m_pFrameOut = nullptr;
    }
}

bool SVStreamOut::isOpen() {
    if(m_pFrameOut) {
        return true;
    }
    return false;
}

s32 SVStreamOut::getOutWidth() {
    return m_outWidth;
}

s32 SVStreamOut::getOutHeight() {
    return m_outHeight;
}

s32 SVStreamOut::getOutFormat() {
    return m_outFormat;
}

void SVStreamOut::lockData() {
    if(m_pFrameOut) {
        m_pFrameOut->lockData();
    }
}

void SVStreamOut::unlockData() {
    if(m_pFrameOut) {
        m_pFrameOut->unlockData();
    }
}

void* SVStreamOut::getOutData() {
    if(m_pFrameOut) {
        return m_pFrameOut->getData();
    }
    return nullptr;
}

//改变输出流方式
void SVStreamOut::setOutMethod(OUTMETHOD _method) {
    //如果是打开状态
    if( isOpen() ) {
        changeOutMethod(_method);
    }else {
        m_outMethod = _method;
    }
}

void SVStreamOut::setOutSize(s32 _width,s32 _height) {
    m_outWidth = _width;
    m_outHeight = _height;
}

void SVStreamOut::setOutFormat(SV_OUTSTEAMFORMATE _format) {
    m_outFormat = _format;
}

//改变输出流方式
void SVStreamOut::changeOutMethod(OUTMETHOD _method) {
    if( isOpen() ) {
        m_outMethod = _method;
    }
}

//
void SVStreamOut::_refreshOutStream() {
    
}

//
void SVStreamOut::createOutStream(cptr8 _name, s32 _type, s32 _format, s32 _width, s32 _height, RENDERSTREAMTYPE _steamType) {
    if(_type == 0){
#ifdef SV_IOS
        m_pFrameOut = MakeSharedPtr<SVFrameOutIOS>(mApp);
        SVFrameOutIOSPtr t_streamNode = std::dynamic_pointer_cast<SVFrameOutIOS>(m_pFrameOut);
        if( t_streamNode ) {
            s32 _w = mApp->m_pGlobalParam->m_inner_width;
            s32 _h = mApp->m_pGlobalParam->m_inner_height;
            if (_width !=0 && _height != 0) {
                _w = _width;
                _h = _height;
            }
            t_streamNode->init(SV_OUTSTEAMFORMATE(_format),_w,_h);
            t_streamNode->setRSType(_steamType);
        }
#endif
    }else if(_type == 1){
#ifdef SV_ANDROID
        m_pFrameOut = MakeSharedPtr<SVFrameOutTex>(mApp);
        SVFrameOutTexPtr t_streamNode = std::dynamic_pointer_cast<SVFrameOutTex>(m_pFrameOut);
#endif
    }
}

void SVStreamOut::destroyOutStream() {
    if(m_pFrameOut) {
        m_pFrameOut->removeFromParent();
        m_pFrameOut = nullptr; 
    }
}

void SVStreamOut::active() {
    SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
    if(t_sc && m_pFrameOut){
        t_sc->addNode(m_pFrameOut);
    }
}

void SVStreamOut::unactive() {
    if(m_pFrameOut){
        m_pFrameOut->removeFromParent();
    }
}

/*************************输出任务***************************/
s32 SVStreamOut::getTexId(){
    SVFrameOutTexPtr t_streamNode = std::dynamic_pointer_cast<SVFrameOutTex>(m_pFrameOut);
    if(t_streamNode){
        return  t_streamNode->getTexId();
    }
    return 0;
}

void SVStreamOut::setStreamOutCB(cb_out_stream _cb) {
#ifdef SV_IOS
    m_lock->lock();
    m_pOutStreamCB = _cb;
    if( m_pFrameOut && m_pOutStreamCB) {
        SVFrameOutIOSPtr t_streamNode = std::dynamic_pointer_cast<SVFrameOutIOS>(m_pFrameOut);
        t_streamNode->setCallback(m_pOutStreamCB);
    }
    m_lock->unlock();
#endif
}

//输出
void SVStreamOut::output() {
    m_missionlock->lock();
    for(s32 i=0; i<m_mission.size();) {
        m_mission[i]->output();
        if( m_mission[i]->isEnd() ) {
            m_mission[i]->sendEnd();
            m_mission.removeForce(i);
        }else{
            i++;
        }
    }
    m_missionlock->unlock();
}

void SVStreamOut::addOutMission(SVOutMissionPtr _mission) {
    m_missionlock->lock();
    for(s32 i=0; i<m_mission.size(); i++) {
        SVString t_name = m_mission[i]->getMissionName();
        if( t_name == _mission->getMissionName() ) {
            m_mission.remove(i);
            goto addOutMission_end;
        }
    }
    //
    m_mission.append(_mission);
    //
    addOutMission_end:
    m_missionlock->unlock();
}

void SVStreamOut::delOutMission(cptr8 _name) {
    m_missionlock->lock();
    for(s32 i=0; i<m_mission.size(); i++) {
        SVString t_name = m_mission[i]->getMissionName();
        if( t_name == _name ) {
            m_mission.remove(i);
            goto delOutMission_end;
        }
    }
delOutMission_end:
    m_missionlock->unlock();
}

void SVStreamOut::clearAllMission() {
    m_missionlock->lock();
    m_mission.clear();
    m_mission.destroy();
    m_missionlock->unlock();
}
