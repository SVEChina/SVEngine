//
// SVActionSys.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActionSys.h"
#include "../base/SVLock.h"
#include "SVActionUnit.h"
SVActionSys::SVActionSys(SVInst *_app)
:SVSysBase(_app) {
    m_subsysType = "SVAnimateSys";
    m_lock = MakeSharedPtr<SVLock>();
}

SVActionSys::~SVActionSys() {
    m_lock = nullptr;
}

void SVActionSys::init() {
}

void SVActionSys::destroy() {
    clearActionUnits();
}

void SVActionSys::update(f32 _dt) {
    m_lock->lock();
    for(s32 i=0;i<m_aniPool.size();){
        if( m_aniPool[i]->isEnd() ){
            m_aniPool.removeForce(i);
        } else {
            m_aniPool[i]->update(_dt);
            i++;
        }
    }
    m_lock->unlock();
}

void SVActionSys::addActionUnit(SVAniBasePtr _actUnit){
    if ( _actUnit && !hasActionUnit(_actUnit) ){
        m_lock->lock();
        m_aniPool.append(_actUnit);
        m_lock->unlock();
    }
}

bool SVActionSys::removeActionUnit(SVAniBasePtr _actUnit){
    bool t_ret = false;
    m_lock->lock();
    for (s32 i = 0; i < m_aniPool.size(); i++) {
        SVAniBasePtr t_aniUnit = m_aniPool[i];
        if ( t_aniUnit == _actUnit) {
            m_aniPool.removeForce(i);
            t_aniUnit = nullptr;
            t_ret = true;
            break;
        }
    }
    m_lock->unlock();
    return t_ret;
}

void SVActionSys::clearActionUnits() {
    m_lock->lock();
    for (s32 i = 0; i < m_aniPool.size(); i++) {
        SVAniBasePtr t_aniUnit = m_aniPool[i];
        t_aniUnit->destroy();
    }
    m_aniPool.destroy();
    m_lock->unlock();
}

bool SVActionSys::hasActionUnit(SVAniBasePtr _actUnit) {
    bool t_ret = false;
    m_lock->lock();
    for (s32 i = 0; i < m_aniPool.size(); i++) {
        if (m_aniPool[i] == _actUnit) {
            t_ret = true;
            break;
        }
    }
    m_lock->unlock();
    return t_ret;
}
