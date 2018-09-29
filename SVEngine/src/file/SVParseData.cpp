//
// SVParseData.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseData.h"
#include "../app/SVInst.h"
#include "../node/SVNode.h"
#include "../node/SVScene.h"
#include "../basesys/SVSceneMgr.h"

//解析基本数据
SVParseData::SVParseData(SVInst *_app)
: SVGBase(_app) {
    personid = 0;
}

SVParseData::~SVParseData() {
    _clearNode();
}

void SVParseData::generate(s32 resid) {
}

void SVParseData::generate(s32 resid, cptr8 path) {
}


void SVParseData::suspend() {
    SVScenePtr t_scene = mApp->getSceneMgr()->getScene();
    if (t_scene) {
        for (s32 i = 0; i < m_Nodes.size(); i++) {
            t_scene->addNode(m_Nodes[i]);
        }
        m_Nodes.destroy();
    }
}

void SVParseData::_addNode(SVNodePtr _node) {
    if (_node) {
        m_Nodes.append(_node);
    }
}

void SVParseData::_clearNode() {
    for (s32 i = 0; i < m_Nodes.size(); i++) {
        m_Nodes[i] = nullptr;
    }
    m_Nodes.destroy();
}


