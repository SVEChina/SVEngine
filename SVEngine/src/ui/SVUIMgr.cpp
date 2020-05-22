//
// SVUIMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVUIMgr.h"
#include "../node/SVNode.h"
#include "../work/SVTdCore.h"

SVUIMgr::SVUIMgr(SVInst *_app)
:SVSysBase(_app) {
    m_subsysType = "SVUIMgr";
}

SVUIMgr::~SVUIMgr() {
    m_uiLock = nullptr;
}

void SVUIMgr::init() {
    m_uiLock = MakeSharedPtr<SVLock>();
    m_pUIRoot = MakeSharedPtr<SVNode>(mApp);
}

void SVUIMgr::destroy() {
    m_pUIRoot = nullptr;
}

//更新
void SVUIMgr::update(f32 dt) {
    m_uiLock->lock();
    if( m_pUIRoot ) {
        m_pUIRoot->deep_update(dt);
    }
    m_uiLock->unlock();
}

void SVUIMgr::visit(SVVisitorBasePtr _visitor) {
    if(m_pUIRoot) {
        m_pUIRoot->select_visit(_visitor);
    }
}

SVNodePtr SVUIMgr::getRoot() {
    return m_pUIRoot;
}

void SVUIMgr::pushUINode(SVNodePtr _node) {
    if(_node && m_pUIRoot) {
        m_pUIRoot->addChild(_node);
    }
}

void SVUIMgr::removeUINode(SVNodePtr _node) {
    if(_node) {
        _node->removeFromParent();
    }
}

void SVUIMgr::clearUINode() {
    if(m_pUIRoot) {
        m_pUIRoot->clearChild();
    }
}
