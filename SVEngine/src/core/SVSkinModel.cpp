//
// SVSkinModel.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVSkinModel.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"
#include "../core/SVSkinAnimator.h"

#ifdef CONFIG_IS_LOAD_ASSIMP

#include "assimp/anim.h"

SVSkinModel::SVSkinModel(SVInst *_app)
:SVBasicModel(_app)
,m_NumBones(0)
,m_pAnimator(nullptr) {
}

SVSkinModel::~SVSkinModel() {
    m_texDiffuse.clear();
    delete m_pAnimator;
}

void SVSkinModel::BoneTransform(s32 iMeshIndex , SVArray<aiMatrix4x4> &Transforms) {
    if (iMeshIndex > m_Entries.size() || iMeshIndex < 0 || m_pAnimator == nullptr){
        return;
    }

    BasicMeshEntry meshEntry = m_Entries[iMeshIndex];
    Transforms = m_pAnimator->GetBoneMatrices(meshEntry.pNode , meshEntry.pMesh);
}

void SVSkinModel::UpdateModel(f32 TimeInSeconds){
    if (m_pAnimator) {
        m_pAnimator->UpdateAnimation(TimeInSeconds , 30);
    }
}

void SVSkinModel::getMeshInfo(u32 iMeshIndex, void **pVertexData, u32 &iVertexNum,
                              u32 &iVertexSize, u32 &iMaterialIndex) {
    if (iMeshIndex >= m_Entries.size())
        return;

    BasicMeshEntry baseInfo = m_Entries[iMeshIndex];
    *pVertexData = &m_vecData[baseInfo.BaseVertex];
    iVertexNum = baseInfo.NumVertex;
    iVertexSize = iVertexNum * sizeof(V3_N_T0_BONE4);
    iMaterialIndex = baseInfo.MaterialIndex;
}

void SVSkinModel::getIndexInfo(u32 iMeshIndex, void **pIndexData, u32 &iIndexNum,u32 &iIndexSize) {
    if (iMeshIndex >= m_Entries.size())
        return;

    BasicMeshEntry baseInfo = m_Entries[iMeshIndex];
    *pIndexData = &m_indexData[baseInfo.BaseIndex];
    iIndexNum = baseInfo.NumIndices;
    iIndexSize = iIndexNum * sizeof(u16);
}

void SVSkinModel::getMaterialInfo(u32 iMaterialIndex , SVTexturePtr* pDiffuseTex) {
    if (iMaterialIndex >= m_texDiffuse.size())
        return;
    SVTexturePtr pTex = m_texDiffuse[iMaterialIndex];
    if (pTex) {
        *pDiffuseTex = pTex;
    }
    return;
}

void SVSkinModel::setAnimateIndex(s32 iIndex){
    if (m_pAnimator)
        m_pAnimator->SetAnimationIndex(iIndex);
}

bool SVSkinModel::InitFromScene(const aiScene *pScene, cptr8 Filename) {
    m_GlobalInverseTransform = pScene->mRootNode->mTransformation;
    m_GlobalInverseTransform.Inverse();

    m_Entries.reallocate(pScene->mNumMeshes);
    m_texDiffuse.set_used(pScene->mNumMaterials);
    u32 NumVertices = 0;
    u32 NumIndices = 0;

    //记录单个mesh在总的buffer中的顶点偏移和索引偏移
    for (u32 i = 0; i < pScene->mNumMeshes; i++) {
        BasicMeshEntry eEntry;
        eEntry.MaterialIndex    = pScene->mMeshes[i]->mMaterialIndex;
        eEntry.NumIndices       = pScene->mMeshes[i]->mNumFaces * 3;
        eEntry.NumVertex        = pScene->mMeshes[i]->mNumVertices;
        eEntry.BaseVertex       = NumVertices;
        eEntry.BaseIndex        = NumIndices;
        eEntry.pMesh            = pScene->mMeshes[i];

        NumVertices             += pScene->mMeshes[i]->mNumVertices;
        NumIndices              += eEntry.NumIndices;
        m_Entries.push_back(eEntry);
    }

    _mapNodeMesh(pScene->mRootNode);

    m_vecData.set_used(NumVertices);
    memset(&m_vecData[0], 0, sizeof(V3_N_T0_BONE4) * NumVertices);

    for (uint i = 0; i < pScene->mNumMeshes; i++) {
        const aiMesh *paiMesh = pScene->mMeshes[i];
        _initMesh(i, paiMesh);
    }

    _initMaterial(pScene,Filename);

    m_pAnimator = new SVSkinAnimator(pScene , 0);

    return true;
}

void SVSkinModel::_initMesh(s32 iMeshIndex, const aiMesh *pMeshData) {
    u32 iVertexBias = 0;
    if (iMeshIndex >= 0 && iMeshIndex < m_Entries.size())
        iVertexBias = m_Entries[iMeshIndex].BaseVertex;

    aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    for (uint i = 0; i < pMeshData->mNumVertices; i++) {
        const aiVector3D *pPos = &(pMeshData->mVertices[i]);
        const aiVector3D *pNormal = &(pMeshData->mNormals[i]);
        const aiVector3D *pTexCoord = pMeshData->HasTextureCoords(0)
                                      ? &(pMeshData->mTextureCoords[0][i]) : &Zero3D;

        u32 iRealIndex = iVertexBias + i;
        m_vecData[iRealIndex].x = pPos->x;
        m_vecData[iRealIndex].y = pPos->y;
        m_vecData[iRealIndex].z = pPos->z;
        m_vecData[iRealIndex].nx = pNormal->x;
        m_vecData[iRealIndex].ny = pNormal->y;
        m_vecData[iRealIndex].nz = pNormal->z;
        m_vecData[iRealIndex].t0x = pTexCoord->x;
#ifdef __APPLE__
        m_vecData[iRealIndex].t0y = 1.0f - pTexCoord->y;
#else
        m_vecData[iRealIndex].t0y = pTexCoord->y;
#endif

    }
    _loadBone(iMeshIndex, pMeshData);

    for (uint i = 0; i < pMeshData->mNumFaces; i++) {
        const aiFace &Face = pMeshData->mFaces[i];
        m_indexData.push_back(Face.mIndices[0]);
        m_indexData.push_back(Face.mIndices[1]);
        m_indexData.push_back(Face.mIndices[2]);
    }
}

void SVSkinModel::_loadBone(u32 MeshIndex, const aiMesh *pMesh) {
    for (u32 i = 0; i < pMesh->mNumBones; i++) {
        u32 BoneIndex = 0;
        SVString BoneName(pMesh->mBones[i]->mName.data);

        u32 t_index = m_BoneMapping[BoneName];
        if (!t_index) {
            BoneIndex = m_NumBones;
            m_NumBones++;
            BoneInfo bi;
            m_BoneInfo.push_back(bi);
            m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;
            m_BoneMapping[BoneName] = BoneIndex;
        } else {
            BoneIndex = m_BoneMapping[BoneName];
        }

        for (u32 j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
            u32 VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            f32 Weight = pMesh->mBones[i]->mWeights[j].mWeight;
            _addBoneData(VertexID, BoneIndex, Weight);
        }
    }
}

void SVSkinModel::_initMaterial(const aiScene *pScene,cptr8 rootPath) {
    
    SVString t_rootpath = rootPath;
    s32 t_pos = t_rootpath.findLast('/');
    SVString rootpath = t_rootpath.subString(0, t_pos+1);
    for (uint i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial *pMaterial = pScene->mMaterials[i];

        m_texDiffuse[i] = nullptr;
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, nullptr, nullptr, nullptr, nullptr,
                                      nullptr) == AI_SUCCESS) {
                SVString p = m_strPath + Path.data;
                //p += ".png";
                p.replace("\\" , "/");
                p = rootpath + p;
                SVTexture *pTex = mApp->getTexMgr()->getTexture(p.c_str(), true);
                m_texDiffuse[i] = pTex;
            }
        }
    }
}

void SVSkinModel::_mapNodeMesh(aiNode* pRoot){
    for (s32 i = 0; i < pRoot->mNumMeshes; ++i) {
        u32 iRealMeshId = pRoot->mMeshes[i];
        if (iRealMeshId < m_Entries.size()){
            if (m_Entries[iRealMeshId].pNode == nullptr){
                m_Entries[iRealMeshId].pNode = pRoot;
            }
            else{
                s32 test = 100;
            }
        }
    }

    for (uint i = 0; i < pRoot->mNumChildren; i++) {
        _mapNodeMesh(pRoot->mChildren[i]);
    }
}

void SVSkinModel::_addBoneData(s32 iVertexIndex, s32 iBoneIndex, f32 fBoneWeight) {
    if (iVertexIndex >= m_vecData.size())
        return;

    V3_N_T0_BONE4 *pVertexData = &(m_vecData[iVertexIndex]);
    if (fabs(pVertexData->weight0 - 0.0) < 0.000001) {
        pVertexData->bone0 = iBoneIndex + 0.1;
        pVertexData->weight0 = fBoneWeight;
        return;
    }
    if (fabs(pVertexData->weight1 - 0.0) < 0.000001) {
        pVertexData->bone1 = iBoneIndex + 0.1;
        pVertexData->weight1 = fBoneWeight;
        return;
    }
    if (fabs(pVertexData->weight2 - 0.0) < 0.000001) {
        pVertexData->bone2 = iBoneIndex + 0.1;
        pVertexData->weight2 = fBoneWeight;
        return;
    }
    if (fabs(pVertexData->weight3 - 0.0) < 0.000001) {
        pVertexData->bone3 = iBoneIndex + 0.1;
        pVertexData->weight3 = fBoneWeight;
        return;
    }
    else{
        s32 i = 100;
    }
}
#endif


