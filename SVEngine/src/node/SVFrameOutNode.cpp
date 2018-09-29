//
// SVFrameOutNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFrameOutNode.h"
#include "SVScene.h"
#include "../basesys/SVStaticData.h"
#include "../basesys/SVSceneMgr.h"
#include "../mtl/SVMtlRGB2YUV.h"
#include "../basesys/SVConfig.h"
#include "../rendercore/SVRenderTarget.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMgr.h"
#include "../mtl/SVTexture.h"


SVFrameOutNode::SVFrameOutNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVFrameOutNode";
    m_pData = nullptr;
}

SVFrameOutNode::~SVFrameOutNode() {
    
}

void SVFrameOutNode::update(float _dt) {
    SVNode::update(_dt);
}

void SVFrameOutNode::render() {
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
}
