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
    for(s32 i=0;i<m_aniUnitPool.size();){
        if( m_aniUnitPool[i]->isEnd() ){
            m_aniUnitPool.removeForce(i);
        } else {
            m_aniUnitPool[i]->update(_dt);
            i++;
        }
    }
    m_lock->unlock();
}

void SVActionSys::addActionUnit(SVActionUnitPtr _actUnit){
    if ( _actUnit && !hasActionUnit(_actUnit) ){
        m_lock->lock();
        m_aniUnitPool.append(_actUnit);
        m_lock->unlock();
    }
}

bool SVActionSys::removeActionUnit(SVActionUnitPtr _actUnit){
    bool t_ret = false;
    m_lock->lock();
    for (s32 i = 0; i < m_aniUnitPool.size(); i++) {
        SVActionUnitPtr t_aniUnit = m_aniUnitPool[i];
        if ( t_aniUnit == _actUnit) {
            m_aniUnitPool.removeForce(i);
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
    for (s32 i = 0; i < m_aniUnitPool.size(); i++) {
        SVActionUnitPtr t_aniUnit = m_aniUnitPool[i];
        t_aniUnit->destroy();
    }
    m_aniUnitPool.destroy();
    m_lock->unlock();
}

bool SVActionSys::hasActionUnit(SVActionUnitPtr _actUnit) {
    bool t_ret = false;
    m_lock->lock();
    for (s32 i = 0; i < m_aniUnitPool.size(); i++) {
        if (m_aniUnitPool[i] == _actUnit) {
            t_ret = true;
            break;
        }
    }
    m_lock->unlock();
    return t_ret;
}
