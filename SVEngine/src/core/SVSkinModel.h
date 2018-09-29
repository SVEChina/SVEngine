//
// SVSkinModel.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SKINMODEL_H
#define SV_SKINMODEL_H

#include "SVBasicModel.h"

#ifdef CONFIG_IS_LOAD_ASSIMP

#include "../base/SVMap.h"

#include <assert.h>

class SVMesh;
class SVSkinAnimator;

class SVSkinModel : public SVBasicModel {
public:
    SVSkinModel(SVInst *_app);

    ~SVSkinModel();

    void BoneTransform(s32 iMeshIndex, SVArray<aiMatrix4x4> &Transforms);
    void UpdateModel(f32 TimeInSeconds);
    virtual void
    getMeshInfo(u32 iMeshIndex, void **pVertexData, u32 &iVertexNum, u32 &iVertexSize,
                u32 &iMaterialIndex);

    virtual void
    getIndexInfo(u32 iMeshIndex, void **pIndexData, u32 &iIndexNum, u32 &iIndexSize);

    virtual void getMaterialInfo(u32 iMaterialIndex, SVTexturePtr* pDiffuseTex);

    void setAnimateIndex(s32 iIndex);
private:
#define NUM_BONES_PER_VEREX 4

    struct BoneInfo {
        aiMatrix4x4 BoneOffset;
        aiMatrix4x4 FinalTransformation;

        BoneInfo() {
        }
    };

    //加载模型相关
    void _initMesh(s32 iMeshIndex, const aiMesh *pMeshData);

    void _addBoneData(s32 iVertexIndex, s32 iBoneIndex, f32 fBoneWeight);

    void _loadBone(uint MeshIndex, const aiMesh *paiMesh);

    void _initMaterial(const aiScene *pScene,const char *rootPath);

    void _mapNodeMesh(aiNode* pRoot);

#define INVALID_MATERIAL 0xFFFFFFFF

    SVArray<BoneInfo> m_BoneInfo;
    SVMap<SVString, uint> m_BoneMapping;
    aiMatrix4x4 m_GlobalInverseTransform;
    u32 m_NumBones;

    SVArray<SVTexturePtr> m_texDiffuse;

    SVArray<V3_N_T0_BONE4> m_vecData;
    SVArray<u16> m_indexData;

    SVSkinAnimator* m_pAnimator;
protected:
    virtual bool InitFromScene(const aiScene *pScene, const char* Filename);
};

#endif

#endif //SV_SKINMODEL_H
