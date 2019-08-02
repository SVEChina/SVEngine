//
// SVSkinNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVSkinNode.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../event/SVEventMgr.h"
#include "../basesys/SVConfig.h"
#include "../core/SVModel.h"
#include "../core/SVAnimateSkin.h"

SVSkinNode::SVSkinNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVSkinNode";
    m_pModel = nullptr;
    m_pSke = nullptr;
    m_aniPool.clear();
}

SVSkinNode::~SVSkinNode() {
    m_pModel = nullptr;
}

void SVSkinNode::update(f32 dt) {
    SVNode::update(dt);
    if(m_pModel) {
        m_pModel->update(dt,m_absolutMat);
    }
}

void SVSkinNode::render() {
    SVNode::render();
    if(m_pModel) {
        m_pModel->render();
    }
}

SVModelPtr SVSkinNode::getModel() {
    return m_pModel;
}

void SVSkinNode::setModel(SVModelPtr _model) {
    m_pModel = _model;
    if(m_pModel) {
        m_aabbBox = m_pModel->getBox();
    }
}

void SVSkinNode::clearModel() {
    m_pModel = nullptr;
}

//
void SVSkinNode::setSke(SVSkeletonPtr _ske) {
    m_pSke = _ske;
}

void SVSkinNode::clearSke() {
    m_pSke = nullptr;
}

//动画操作
void SVSkinNode::addAni(SVAnimateSkinPtr _ani) {
    m_aniPool.append(_ani->getName(),_ani);
}

void SVSkinNode::delAni(cptr8 _name) {
}

void SVSkinNode::clearAni() {
    m_aniPool.clear();
}
