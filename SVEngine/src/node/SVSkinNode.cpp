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

SVSkinNode::SVSkinNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVSkinNode";
    m_pModel = nullptr;
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

void SVSkinNode::clear() {
    m_pModel = nullptr;
}

//
//void SVSkinNode::loadModel(cptr8 pPath ,cptr8 pDir) {
////    if (m_pModel)
////        return;
////    //
////    m_pModel = new SVSkinModel(mApp);
////    bool bSuccess = m_pModel->LoadModel(pPath , pDir);
////    if (!bSuccess){
////        return;
////    }
////    //
////    SVMultMeshMtlRenderObject *t_multmesh_obj = (SVMultMeshMtlRenderObject *)m_renderObject;
////    for (s32 i = 0, n = m_pModel->getModelData()->mNumMeshes; i < n; i++) {
////        u32 iVertexNum, iVertexSize, iMaterialIndex;
////        s64 iVertexPoint , iIndexPoint;
////
////        SVTexture* pTexDiffuse = nullptr;
////        m_pModel->getMeshInfo(i,(void **)(&iVertexPoint),iVertexNum,iVertexSize,iMaterialIndex);
////        m_pModel->getMaterialInfo(iMaterialIndex, &pTexDiffuse);
////
////        SVRenderMeshPtr rmesh = MakeSharedPtr<SVRenderMesh>(mApp);
////        rmesh->setVertexPoolType(GL_DYNAMIC_DRAW);
////        rmesh->setIndexPoolType(GL_DYNAMIC_DRAW);
////        rmesh->setVertexType(E_VF_V3_N_T0_BONE_W);
////        rmesh->setVertexNum(iVertexNum);
////        //rmesh->setVertexData((void *) iVertexPoint, iVertexSize);
////        //rmesh->setDrawMethod(E_DM_LINES);
////
////        u32 iIndexNum, iIndexSize;
////        m_pModel->getIndexInfo(i, (void **) (&iIndexPoint), iIndexNum, iIndexSize);
////        //rmesh->setIndexData((void *) iIndexPoint, iIndexSize, iIndexNum);
////
////        SVMtlSkinModel *pMaterialSkin = new SVMtlSkinModel(mApp);
////        pMaterialSkin->init();
////        pMaterialSkin->setTexture(0,pTexDiffuse);
////        //
////        SVMatSkinModelSelect *pMaterialSkinSelect = new SVMatSkinModelSelect(mApp);
////        pMaterialSkinSelect->init();
////        pMaterialSkinSelect->setColor(uid);
////        //
////        t_multmesh_obj->addRenderObj(rmesh, pMaterialSkin);
////        //加入本地map
////        m_matMap.insert(i, pMaterialSkin);
////        m_meshMap.insert(i, rmesh);        //外部的逻辑结构
////        m_matSelectMap.insert(i, pMaterialSkinSelect);
////    }
//}
//
//void SVSkinNode::addAnimate(cptr8 pName , s32 iIndex ,s32 iStart,s32 iEnd){
////    SKINANIMATE::Node* pSkinAni = m_skinAniMap.find(pName);
////    if (!pSkinAni){
////        stuAnimate ani;
////        ani.m_iIndex = iIndex;
////        ani.m_iEnd   = iEnd;
////        ani.m_iStart = iStart;
////        m_skinAniMap.insert(pName , ani);
////    }
//}
//
//void SVSkinNode::setstate(E_ANISTATE _state){
//    m_state = _state;
//}
//
//E_ANISTATE SVSkinNode::getstate(){
//    return m_state;
//}
//
////
//void SVSkinNode::setModel(SVSkinModelPtr _model) {
//    m_pModel = _model;
//}
//
//SVSkinModel *SVSkinNode::getModel() {
//    return m_pModel;
//}
//
//void SVSkinNode::play() {
//}
//
//void SVSkinNode::play(cptr8 actname) {
////    stuAnimate t_ani = m_skinAniMap[actname];
////    if (t_ani == nullptr)
////        return;
////    m_iCurrentAnimateIndex = t_ani.m_iIndex;
////    if (m_pModel){
////        m_pModel->setAnimateIndex(m_iCurrentAnimateIndex);
////    }
//}
//
//void SVSkinNode::pause() {
//}
//
//void SVSkinNode::stop() {
//}
//
//void SVSkinNode::complete() {
//}
//
//void SVSkinNode::reset() {
//}
//
//void SVSkinNode::_updateRenderMesh(f32 dt) {
//    m_fAniPlayedTime += dt;
//    //m_pModel->BoneTransform(m_fAniPlayedTime / 1000.0f, Transforms);
//    m_pModel->UpdateModel(m_fAniPlayedTime);
//    for (s32 i = 0, n = m_matMap.size(); i < n; ++i) {
//        SVArray<aiMatrix4x4> Transforms;
//        m_pModel->BoneTransform(i , Transforms);
//        SVMtlSkinModelPtr pMat = m_matMap[i];
//        if (i == 0){
//            pMat->m_LogicParamDepth.clear = true;
//        }
//        pMat->setModelMatrix(m_absolutMat.get());
//        pMat->setVPMatrix(mApp->m_pGlobalParam->m_mat_vp.get());
//        pMat->m_LogicParamBlend.enable = true;
//        pMat->m_LogicParamBlend.srcParam = GL_ONE;
//        pMat->m_LogicParamBlend.dstParam = GL_ONE_MINUS_SRC_ALPHA;
//
//        for (u32 j = 0; j< Transforms.size(); j++) {
//            pMat->setBoneTransform(j, Transforms[j]);
//        }
//        //select
//        SVMtlSkinModelPtr pMatSelect = m_matSelectMap[i];
//        pMatSelect->setModelMatrix(m_absolutMat.get());
//        pMatSelect->setVPMatrix(mApp->m_pGlobalParam->m_mat_vp.get());
//        pMatSelect->m_LogicParamBlend.enable = false;
//        pMatSelect->m_LogicParamBlend.srcParam = GL_ONE;
//        pMatSelect->m_LogicParamBlend.dstParam = GL_ONE_MINUS_SRC_ALPHA;
//        for (u32 j = 0; j < Transforms.size(); j++) {
//            pMatSelect->setBoneTransform(j, Transforms[j]);
//        }
//    }
//}
