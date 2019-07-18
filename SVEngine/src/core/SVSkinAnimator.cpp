//
// SVSkinAnimator.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVSkinAnimator.h"

////---------------------------------------------------------------------------------
////A little tree structure to match the scene's node structure, but holding additional data.
//struct AnimationNode {
//  SVString sName;
//  AnimationNode* pParent;
//  SVArray<AnimationNode*> vChildren;
//
//  //most recently calculated local transform
//  aiMatrix4x4 mLocalTransform;
//
//  //same, but in world space
//  aiMatrix4x4 mGlobalTransform;
//
//  //index in the current animation's channel array.
//  u32 uChannelIndex;
//
//  //default construction
//  AnimationNode() {
//    uChannelIndex = -1;
//    pParent = NULL;
//  }
//
//  //construction from a given name
//  AnimationNode(const SVString& pName): sName(pName) {
//    uChannelIndex = -1;
//    pParent = NULL;
//  }
//
//  ~AnimationNode() {
//    for (s32 i = 0 ; i < vChildren.size() ; ++i){
//        delete vChildren[i];
//    }
//  }
//};

SVSkinAnimator::SVSkinAnimator() {
//  m_pScene = pScene;
//  m_dLastTime = 0.0;
//  m_pRootNode = NULL;
//  m_uCurrentAnimationIndex = -1;
//  m_pCurrentAnimation = NULL;
//  m_pLastFramePosition = NULL;
//  m_mIdentityMatrix = aiMatrix4x4();
//
//  //build the nodes-for-bones table
//  for (u32 i = 0; i < m_pScene->mNumMeshes; i++) {
//    const aiMesh* pCurrentMesh = m_pScene->mMeshes[i];
//
//    for (u32 j = 0; j < pCurrentMesh->mNumBones; j++) {
//      const aiBone* pCurrentBone = pCurrentMesh->mBones[j];
//
//      m_mapBoneNodesByName[pCurrentBone->mName.data] = m_pScene->mRootNode->FindNode(pCurrentBone->mName);
//    }
//  }
//
//  //changing the current animation also creates the node tree for this animation
//  SetAnimationIndex(uAnimationIndex);
}

SVSkinAnimator::~SVSkinAnimator() {
//  if (m_pRootNode != NULL) {
//    delete m_pRootNode;
//  }
//
//  if (m_pLastFramePosition != NULL) {
//    delete[] m_pLastFramePosition;
//  }
}

//各个动画通道做差值
void SVSkinAnimator::update(f32 _dt) {
    
}

//播放指定动画名称
bool SVSkinAnimator::play(cptr8 _name) {
    return true;
}

//继续播放
bool SVSkinAnimator::play() {
    return true;
}

//暂停
bool SVSkinAnimator::pause() {
    return true;
}

//停止动画
bool SVSkinAnimator::stop() {
    return true;
}

//增加一套动画数据
void SVSkinAnimator::addAnimate(cptr8 _name) {
    
}

//删除一套动画数据
void SVSkinAnimator::delAnimate(cptr8 _name) {
    
}

//清理动画
void SVSkinAnimator::clearAnimates() {
    
}
