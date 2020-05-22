//
// SVActionMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActionMgr.h"
#include "../work/SVTdCore.h"
#include "SVActionUnit.h"

SVActionMgr::SVActionMgr(SVInst *_app)
:SVSysBase(_app) {
    m_subsysType = "SVActionMgr";
    m_lock = MakeSharedPtr<SVLock>();
}

SVActionMgr::~SVActionMgr() {
    m_lock = nullptr;
}

void SVActionMgr::init() {
}

void SVActionMgr::destroy() {
    removeAllAni();
}

void SVActionMgr::update(f32 _dt) {
    m_lock->lock();
    for(s32 i=0;i<m_aniPool.size(); i++){
        m_aniPool[i]->update(_dt);
    }
    m_lock->unlock();
}

SVActionUnitPtr SVActionMgr::addAction(SVActBasePtr _action, SVNodePtr _node){
    if (_action && _node) {
        SVActionUnitPtr actionUnit = MakeSharedPtr<SVActionUnit>(mApp, _action, _node);
        actionUnit->init();
        m_lock->lock();
        m_aniPool.append(actionUnit);
        m_lock->unlock();
        return actionUnit;
    }
    return nullptr;
}

void SVActionMgr::addAni(SVAniBasePtr _aniBase){
    if (_aniBase) {
        m_lock->lock();
        m_aniPool.append(_aniBase);
        m_lock->unlock();
    }
}

bool SVActionMgr::removeAni(SVAniBasePtr _aniBase){
    bool t_ret = false;
    m_lock->lock();
    for (s32 i = 0; i < m_aniPool.size(); i++) {
        SVAniBasePtr t_aniBase = m_aniPool[i];
        if (t_aniBase == _aniBase) {
            t_aniBase->destroy();
            m_aniPool.removeForce(i);
            t_aniBase = nullptr;
            t_ret = true;
            break;
        }
    }
    m_lock->unlock();
    return t_ret;
}

bool SVActionMgr::removeAllAni() {
    m_lock->lock();
    for (s32 i = 0; i < m_aniPool.size(); i++) {
        SVAniBasePtr t_aniBase = m_aniPool[i];
        t_aniBase->destroy();
    }
    m_aniPool.destroy();
    m_lock->unlock();
    return true;
}

