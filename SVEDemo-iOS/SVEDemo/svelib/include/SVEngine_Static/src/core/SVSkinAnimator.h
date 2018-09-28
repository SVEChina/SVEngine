//
// SVSkinAnimator.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SKINANIMATOR_H
#define SV_SKINANIMATOR_H

#ifdef CONFIG_IS_LOAD_ASSIMP

#include <vector>
#include <map>
#include <assimp/FMat4x4.h>
#include "../base/SVObject.h"
#include "../base/SVVec3.h"

using  namespace sv;
using  namespace util;

class aiScene;
class aiNode;
class aiAnimation;
class aiMesh;
struct AnimationNode;

class SVSkinAnimator{
public:
  SVSkinAnimator(const aiScene* pScene, u32 pAnimIndex);
    
  ~SVSkinAnimator();

  void SetAnimationIndex(u32 uAnimationIndex);

  void UpdateAnimation(long lElapsedTime, double dTicksPerSecond);

  const SVArray<aiMatrix4x4>& GetBoneMatrices(const aiNode* pNode, u32 pMeshIndex = 0);
    
  const SVArray<aiMatrix4x4>& GetBoneMatrices(const aiNode* pNode, const aiMesh* pMesh);

  const aiMatrix4x4& GetGlobalTransform(const aiNode* node) const;

private:
  AnimationNode* CreateNodeTree(aiNode* pNode, AnimationNode* pParent);

  /** Calculates the global transformation matrix for the given internal node */
  void CalculateGlobalTransform(AnimationNode* pInternalNode);

  void UpdateTransforms(AnimationNode* pNode, const SVArray<aiMatrix4x4>& vTransforms);

  /** Name to node map to quickly find nodes by their name */
  typedef map<const aiNode*, AnimationNode*> NodeMap;
  NodeMap m_mapNodesByName;

  /** Name to node map to quickly find nodes for given bones by their name */
  typedef map<const char*, const aiNode*> BoneMap;
  BoneMap m_mapBoneNodesByName;

  /** At which frame the last evaluation happened for each channel. 
  * Useful to quickly find the corresponding frame for slightly increased time stamps
  */
  double m_dLastTime;
  FVec3* m_pLastFramePosition;
  /** Array to return transformations results inside. */
  SVArray<aiMatrix4x4> m_vTransforms;

  /** Identity matrix to return a reference to in case of error */
  aiMatrix4x4 m_mIdentityMatrix;

  const aiScene* m_pScene;
  AnimationNode* m_pRootNode;
  u32 m_uCurrentAnimationIndex;
  const aiAnimation* m_pCurrentAnimation;
};

#endif


#endif //SV_SKINANIMATOR_H
