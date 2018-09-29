//
// SVActMtl.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActMtl.h"
#include "../mtl/SVMtlCore.h"
#include "../node/SVNode.h"
SVActMtl::SVActMtl(SVInst *_app):SVActTime(_app) {
    m_pNewMtl = nullptr;
    m_pOldMtl = nullptr;
}

SVActMtl::~SVActMtl() {
    m_pNewMtl = nullptr;
    m_pOldMtl = nullptr;
}

void SVActMtl::enter(SVNodePtr _nodePtr) {
    if (m_pNewMtl && _nodePtr) {
        m_pOldMtl = _nodePtr->getMtl();
        _nodePtr->setMtl(m_pNewMtl);
    }
}

void SVActMtl::exit(SVNodePtr _nodePtr) {
    if (m_pNewMtl && _nodePtr) {
        _nodePtr->setMtl(m_pOldMtl);
    }
}
