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
    m_pActAni = nullptr;
    m_aniPool.clear();
}

SVSkinNode::~SVSkinNode() {
    clearAni();
    m_pModel = nullptr;
    m_pSke = nullptr;
    m_pActAni = nullptr;
}

void SVSkinNode::update(f32 dt) {
    SVNode::update(dt);
    //更新动画
    if(m_pActAni) {
        m_pActAni->update(dt);
    }
    if(m_pSke){
        m_pSke->refresh();
    }
    //
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

//播放控制
void SVSkinNode::play(cptr8 _name) {
    ANIPOOL::Iterator it = m_aniPool.find(_name);
    if(it!=m_aniPool.end()){
        m_pActAni = it->data;
    }
}

void SVSkinNode::pause() {
}

void SVSkinNode::stop() {
}

//

SVModelPtr SVSkinNode::getModel() {
    return m_pModel;
}

void SVSkinNode::setModel(SVModelPtr _model) {
    m_pModel = _model;
    if(m_pModel) {
        m_pModel->bindSke(m_pSke);
        m_aabbBox = m_pModel->getBox();
    }
}

void SVSkinNode::clearModel() {
    if(m_pModel) {
        m_pModel->unbindSke();
        m_pModel = nullptr;
    }
}

//
void SVSkinNode::setSke(SVSkeletonPtr _ske) {
    m_pSke = _ske;
    //
    ANIPOOL::Iterator it = m_aniPool.begin();
    while(it!=m_aniPool.end()) {
        SVAnimateSkinPtr t_ani = it->data;
        t_ani->bind(_ske);
        it++;
    }
    if(m_pModel) {
        m_pModel->bindSke(m_pSke);
    }
}

void SVSkinNode::clearSke() {
    //
    ANIPOOL::Iterator it = m_aniPool.begin();
    while(it!=m_aniPool.end()) {
        SVAnimateSkinPtr t_ani = it->data;
        t_ani->unbind();
        it++;
    }
    //
    if(m_pModel) {
        m_pModel->unbindSke();
    }
    //
    m_pSke = nullptr;
}

//动画操作
void SVSkinNode::addAni(SVAnimateSkinPtr _ani) {
    m_aniPool.append(_ani->getName(),_ani);
    if(m_pSke) {
        _ani->bind(m_pSke);
    }
    //test
    m_pActAni = _ani;
}

void SVSkinNode::delAni(cptr8 _name) {
    ANIPOOL::Iterator it = m_aniPool.find(_name);
    if(it!=m_aniPool.end()){
        SVAnimateSkinPtr t_ani = it->data;
        t_ani->unbind();
        m_aniPool.remove(it);
    }
}

void SVSkinNode::clearAni() {
    ANIPOOL::Iterator it = m_aniPool.begin();
    while(it!=m_aniPool.end()) {
        SVAnimateSkinPtr t_ani = it->data;
        t_ani->unbind();
        it++;
    }
    m_aniPool.clear();
}
