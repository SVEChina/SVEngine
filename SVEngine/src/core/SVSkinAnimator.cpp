//
// SVSkinAnimator.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVSkinAnimator.h"
#ifdef CONFIG_IS_LOAD_ASSIMP
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"


//---------------------------------------------------------------------------------
//A little tree structure to match the scene's node structure, but holding additional data.
struct AnimationNode {
  SVString sName;
  AnimationNode* pParent;
  SVArray<AnimationNode*> vChildren;

  //most recently calculated local transform
  aiMatrix4x4 mLocalTransform; 

  //same, but in world space
  aiMatrix4x4 mGlobalTransform;

  //index in the current animation's channel array.
  u32 uChannelIndex;

  //default construction
  AnimationNode() {
    uChannelIndex = -1;
    pParent = NULL; 
  }

  //construction from a given name
  AnimationNode(const SVString& pName): sName(pName) {
    uChannelIndex = -1;
    pParent = NULL;
  }

  ~AnimationNode() {
    for (s32 i = 0 ; i < vChildren.size() ; ++i){
        delete vChildren[i];
    }
  }
};
SVSkinAnimator::SVSkinAnimator(const aiScene* pScene, u32 uAnimationIndex) {
  m_pScene = pScene;
  m_dLastTime = 0.0;
  m_pRootNode = NULL;
  m_uCurrentAnimationIndex = -1;
  m_pCurrentAnimation = NULL;
  m_pLastFramePosition = NULL;
  m_mIdentityMatrix = aiMatrix4x4();

  //build the nodes-for-bones table
  for (u32 i = 0; i < m_pScene->mNumMeshes; i++) {
    const aiMesh* pCurrentMesh = m_pScene->mMeshes[i];

    for (u32 j = 0; j < pCurrentMesh->mNumBones; j++) {
      const aiBone* pCurrentBone = pCurrentMesh->mBones[j];

      m_mapBoneNodesByName[pCurrentBone->mName.data] = m_pScene->mRootNode->FindNode(pCurrentBone->mName);
    }
  }

  //changing the current animation also creates the node tree for this animation
  SetAnimationIndex(uAnimationIndex);
}

SVSkinAnimator::~SVSkinAnimator() {
  if (m_pRootNode != NULL) {
    delete m_pRootNode;
  }

  if (m_pLastFramePosition != NULL) {
    delete[] m_pLastFramePosition;
  }
}

//------------------------------------------------------------------------------------------------
//Sets the animation to use for playback. 
void SVSkinAnimator::SetAnimationIndex(u32 uAnimationIndex) {
  if (uAnimationIndex == m_uCurrentAnimationIndex) {
    return;
  }

  //kill data of the previous anim
  if (m_pRootNode != NULL) {
    delete m_pRootNode;
    m_pRootNode = NULL;
  }

  if (m_pLastFramePosition != NULL) {
    delete[] m_pLastFramePosition;
  }

  m_mapNodesByName.clear();
  m_pCurrentAnimation = NULL;

  m_uCurrentAnimationIndex = uAnimationIndex;
  m_pCurrentAnimation = m_pScene->mAnimations[m_uCurrentAnimationIndex];

  //create the internal node tree. Do this even in case of invalid animation index
  //so that the transformation matrices are properly set up to mimic the current scene
  m_pRootNode = CreateNodeTree(m_pScene->mRootNode, NULL);

  //invalid anim index
  if (uAnimationIndex >= m_pScene->mNumAnimations) {
    m_uCurrentAnimationIndex = 0;
    m_pCurrentAnimation = m_pScene->mAnimations[m_uCurrentAnimationIndex];
  }

  m_pLastFramePosition = new FVec3[m_pCurrentAnimation->mNumChannels];
}

//------------------------------------------------------------------------------------------------
//Recursively creates an internal node structure matching the current scene and animation.
AnimationNode* SVSkinAnimator::CreateNodeTree(aiNode* pNode, AnimationNode * pParent) {
  //create a node
  AnimationNode* pInternalNode = new AnimationNode(pNode->mName.data);
  pInternalNode->pParent = pParent;
  m_mapNodesByName[pNode] = pInternalNode;

  //copy its transformation
  pInternalNode->mLocalTransform = pNode->mTransformation;
  CalculateGlobalTransform(pInternalNode);

  //find the index of the animation track affecting this node, if any
  if (m_uCurrentAnimationIndex < m_pScene->mNumAnimations) {
    pInternalNode->uChannelIndex = -1;

    for (u32 a = 0; a < m_pCurrentAnimation->mNumChannels; a++) {
      if (strcmp(m_pCurrentAnimation->mChannels[a]->mNodeName.data,pInternalNode->sName.c_str()) == 0) {
        pInternalNode->uChannelIndex = a;

        break;
      }
    }
  }

  //continue for all child nodes and assign the created internal nodes as our children
  for (u32 a = 0; a < pNode->mNumChildren; a++) {
    AnimationNode* childNode = CreateNodeTree(pNode->mChildren[a], pInternalNode);
    pInternalNode->vChildren.push_back(childNode);
  }

  return pInternalNode;
}

//------------------------------------------------------------------------------------------------
//Calculates the bone matrices for the given mesh. 
const SVArray<aiMatrix4x4>& SVSkinAnimator::GetBoneMatrices(const aiNode* pNode, u32 uNodeMeshIndex) {
  if (uNodeMeshIndex < pNode->mNumMeshes) {
    u32 uMeshIndex = pNode->mMeshes[uNodeMeshIndex];

    if ((m_pScene != NULL) && (uMeshIndex < m_pScene->mNumMeshes)) {
      const aiMesh* pMesh = m_pScene->mMeshes[uMeshIndex];

      for (u32 i = 0; i < m_vTransforms.size(); i++) {
        m_vTransforms[i] = aiMatrix4x4();
      }

      //resize array and initialise it with identity matrices
      m_vTransforms.set_used(pMesh->mNumBones);

      //calculate the mesh's inverse global transform
      aiMatrix4x4 mGlobalInverseMeshTransform = GetGlobalTransform(pNode);
      mGlobalInverseMeshTransform.Inverse();

      //Bone matrices transform from mesh coordinates in bind pose to mesh coordinates in skinned pose
      //Therefore the formula is offsetMatrix * currentGlobalTransform * inverseCurrentMeshTransform
      for (u32 i = 0; i < pMesh->mNumBones; i++) {
        const aiBone* pBone = pMesh->mBones[i];
        const aiMatrix4x4& mCurrentGlobalTransform = GetGlobalTransform(m_mapBoneNodesByName[pBone->mName.data]);
        m_vTransforms[i] = mGlobalInverseMeshTransform * mCurrentGlobalTransform * pBone->mOffsetMatrix;
      }
    }
  }

  //and return the result
  return m_vTransforms;
}

const SVArray<aiMatrix4x4>& SVSkinAnimator::GetBoneMatrices(const aiNode* pNode, const aiMesh* pMesh){
    for (u32 i = 0; i < m_vTransforms.size(); i++) {
        m_vTransforms[i] = aiMatrix4x4();
    }

    if (pNode == NULL || pMesh == NULL)
        return m_vTransforms;

    //resize array and initialise it with identity matrices
    m_vTransforms.set_used(pMesh->mNumBones);

    //calculate the mesh's inverse global transform
    aiMatrix4x4 mGlobalInverseMeshTransform = GetGlobalTransform(pNode);
    mGlobalInverseMeshTransform.Inverse();

    //Bone matrices transform from mesh coordinates in bind pose to mesh coordinates in skinned pose
    //Therefore the formula is offsetMatrix * currentGlobalTransform * inverseCurrentMeshTransform
    for (u32 i = 0; i < pMesh->mNumBones; i++) {
        const aiBone* pBone = pMesh->mBones[i];
        const aiMatrix4x4& mCurrentGlobalTransform = GetGlobalTransform(m_mapBoneNodesByName[pBone->mName.data]);
        m_vTransforms[i] = mGlobalInverseMeshTransform * mCurrentGlobalTransform * pBone->mOffsetMatrix;
    }

    return m_vTransforms;
}
//------------------------------------------------------------------------------------------------
//Calculates the global transformation matrix for the given internal node.
void SVSkinAnimator::CalculateGlobalTransform(AnimationNode* pInternalNode) {
  //concatenate all parent transforms to get the global transform for this node
  pInternalNode->mGlobalTransform = pInternalNode->mLocalTransform;
  AnimationNode* pNode = pInternalNode->pParent;

  while (pNode) {
    pInternalNode->mGlobalTransform = pNode->mLocalTransform * pInternalNode->mGlobalTransform;
    pNode = pNode->pParent;
  }
}

//------------------------------------------------------------------------------------------------
//Retrieves the most recent global transformation matrix for the given node. 
const aiMatrix4x4& SVSkinAnimator::GetGlobalTransform(const aiNode * node) const {
  NodeMap::Node* pName = m_mapNodesByName.find(node);
  if(pName == NULL){
     return m_mIdentityMatrix;
  }

  return pName->getValue()->mGlobalTransform;
}

//------------------------------------------------------------------------------------------------
//Calculates the node transformations for the scene. 
void SVSkinAnimator::UpdateAnimation(long lElapsedTime, double dTicksPerSecond) {
  if ((m_pCurrentAnimation) && (m_pCurrentAnimation->mDuration > 0.0)) {
    double dTime = ((double) lElapsedTime) / 1000.0;

    //calculate current local transformations
    //extract ticks per second. Assume default value if not given
    double dTicksPerSecondCorrected = dTicksPerSecond != 0.0 ? dTicksPerSecond : 20;

    //map into anim's duration
    double dTimeInTicks = 0.0f;

    if (m_pCurrentAnimation->mDuration > 0.0) {
      dTimeInTicks = fmod(dTime * dTicksPerSecondCorrected, m_pCurrentAnimation->mDuration);
    }

    if (m_vTransforms.size() != m_pCurrentAnimation->mNumChannels) {
      m_vTransforms.set_used(m_pCurrentAnimation->mNumChannels);
    }

    //calculate the transformations for each animation channel
    for (u32 i = 0; i < m_pCurrentAnimation->mNumChannels; i++) {
      const aiNodeAnim* pChannel = m_pCurrentAnimation->mChannels[i];

      //******** Position *****
      aiVector3D vPresentPosition(0, 0, 0);

      if (pChannel->mNumPositionKeys > 0) {
        //Look for present frame number. Search from last position if time is after the last time, else from beginning
        //Should be much quicker than always looking from start for the average use case.
        u32 uFrame = (dTimeInTicks >= m_dLastTime) ? m_pLastFramePosition[i].x : 0;

        while (uFrame < pChannel->mNumPositionKeys - 1) {
          if (dTimeInTicks < pChannel->mPositionKeys[uFrame + 1].mTime) {
            break;
          }

          uFrame++;
        }

        //interpolate between this frame's value and next frame's value
        u32 uNextFrame = (uFrame + 1) % pChannel->mNumPositionKeys;
        const aiVectorKey& pKey = pChannel->mPositionKeys[uFrame];
        const aiVectorKey& pNextKey = pChannel->mPositionKeys[uNextFrame];
        double dTimeDifference = pNextKey.mTime - pKey.mTime;

        if (dTimeDifference < 0.0) {
          dTimeDifference += m_pCurrentAnimation->mDuration;
        }

        if (dTimeDifference > 0) {
          float fInterpolationFactor = (float) ((dTimeInTicks - pKey.mTime) / dTimeDifference);
          vPresentPosition = pKey.mValue + (pNextKey.mValue - pKey.mValue) * fInterpolationFactor;
        }
        else {
          vPresentPosition = pKey.mValue;
        }

        m_pLastFramePosition[i].x = uFrame;
      }

      //******** Rotation *********
      aiQuaternion qPresentRotation(1, 0, 0, 0);

      if (pChannel->mNumRotationKeys > 0) {
        u32 uFrame = (dTimeInTicks >= m_dLastTime) ? m_pLastFramePosition[i].y : 0;

        while (uFrame < pChannel->mNumRotationKeys - 1) {
          if (dTimeInTicks < pChannel->mRotationKeys[uFrame + 1].mTime) {
            break;
          }

          uFrame++;
        }

        //interpolate between this frame's value and next frame's value
        u32 uNextFrame = (uFrame + 1) % pChannel->mNumRotationKeys;
        const aiQuatKey& pKey = pChannel->mRotationKeys[uFrame];
        const aiQuatKey& pNextKey = pChannel->mRotationKeys[uNextFrame];
        double dTimeDifference = pNextKey.mTime - pKey.mTime;

        if (dTimeDifference < 0.0) {
          dTimeDifference += m_pCurrentAnimation->mDuration;
        }

        if (dTimeDifference > 0) {
          float fInterpolationFactor = (float) ((dTimeInTicks - pKey.mTime) / dTimeDifference);
          aiQuaternion::Interpolate(qPresentRotation, pKey.mValue, pNextKey.mValue, fInterpolationFactor);
        }
        else {
          qPresentRotation = pKey.mValue;
        }

        m_pLastFramePosition[i].y = uFrame;
      }

      //******** Scaling **********
      aiVector3D vPresentScaling(1, 1, 1);

      if (pChannel->mNumScalingKeys > 0) {
        u32 uFrame = (dTimeInTicks >= m_dLastTime) ? m_pLastFramePosition[i].z : 0;

        while (uFrame < pChannel->mNumScalingKeys - 1) {
          if (dTimeInTicks < pChannel->mScalingKeys[uFrame + 1].mTime) {
            break;
          }

          uFrame++;
        }

        vPresentScaling = pChannel->mScalingKeys[uFrame].mValue;
        m_pLastFramePosition[i].z = uFrame;
      }

      //build a transformation matrix from it
      aiMatrix4x4& mTransformation = m_vTransforms[i];
      mTransformation = aiMatrix4x4(qPresentRotation.GetMatrix());
      mTransformation.a1 *= vPresentScaling.x; mTransformation.b1 *= vPresentScaling.x; mTransformation.c1 *= vPresentScaling.x;
      mTransformation.a2 *= vPresentScaling.y; mTransformation.b2 *= vPresentScaling.y; mTransformation.c2 *= vPresentScaling.y;
      mTransformation.a3 *= vPresentScaling.z; mTransformation.b3 *= vPresentScaling.z; mTransformation.c3 *= vPresentScaling.z;
      mTransformation.a4 = vPresentPosition.x; mTransformation.b4 = vPresentPosition.y; mTransformation.c4 = vPresentPosition.z;
    }

    m_dLastTime = dTimeInTicks;

    //and update all node transformations with the results
    UpdateTransforms(m_pRootNode, m_vTransforms);
  }
}

//------------------------------------------------------------------------------------------------
//Recursively updates the internal node transformations from the given matrix array
void SVSkinAnimator::UpdateTransforms(AnimationNode* pNode, const SVArray<aiMatrix4x4>& vTransforms){
  //update node local transform
  if (pNode->uChannelIndex != -1) {
    if (pNode->uChannelIndex >= vTransforms.size()) {
      return;
    }

    pNode->mLocalTransform = vTransforms[pNode->uChannelIndex];
  }

  //concatenate all parent transforms to get the global transform for this node
  pNode->mGlobalTransform = pNode->mLocalTransform;
  AnimationNode* pParentNode = pNode->pParent;

  while (pParentNode) {
    pNode->mGlobalTransform = pParentNode->mLocalTransform * pNode->mGlobalTransform;
    pParentNode = pParentNode->pParent;
  }
  for (s32 i = 0; i < pNode->vChildren.size(); ++i) {
      UpdateTransforms(pNode->vChildren[i] , vTransforms);
  }
}
#endif
