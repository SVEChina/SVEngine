//
// SVAnimateSkin.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVAnimateSkin.h"

//清理

SVBone::SVBone() {
    m_name = "";
    m_id = 0;
    m_tran.set(0.0f, 0.0f, 0.0f);
    m_scale.set(1.0f, 1.0f, 1.0f);
    m_rot.set(0.0f, 0.0f, 0.0f,1.0f);
    m_pParent = nullptr;
    m_children.clear();
}

void SVBone::clear() {
    m_pParent = nullptr;
    for(s32 i=0;i<m_children.size();i++) {
        m_children[i]->clear();
    }
    m_children.destroy();
}

void SVBone::update() {
    
}

//
SVSkeleton::SVSkeleton(){
    m_name = "ske";
    m_root = nullptr;
    m_boneArray.clear();
}


void SVSkeleton::addBone(SVBonePtr _bone) {
    m_boneArray.append(_bone);
}

void SVSkeleton::destroy() {
    //
    m_boneArray.clear();
    //
    if(m_root) {
        m_root->clear();
        m_root = nullptr;
    }
}

//
SVAnimateSkin::SVAnimateSkin(SVInst* _app,cptr8 _name)
:SVGBase(_app)
,m_name(_name){
    
}

SVAnimateSkin::~SVAnimateSkin() {
    
}

cptr8 SVAnimateSkin::getName() {
    return m_name.c_str();
}


//
SVSkinMgr::SVSkinMgr(SVInst* _app)
:SVGBase(_app){
    
}

SVSkinMgr::~SVSkinMgr() {
    
}

void SVSkinMgr::addSkin(SVAnimateSkinPtr _skin) {
    //m_skinPool
}

void SVSkinMgr::remvoeSkin(cptr8 _name) {
    
}
