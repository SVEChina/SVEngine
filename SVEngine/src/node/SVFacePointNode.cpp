//
// SVFacePointNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVFacePointNode.h"
#include "SVScene.h"
#include "../app/SVInst.h"
#include "../basesys/SVSceneMgr.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"
#include "../mtl/SVMtlCore.h"
#include "../core/SVGeoGen.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectST.h"
#include "SVFacePointNode.h"
SVFacePointNode::SVFacePointNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVFacePointNode";
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"debugFacePt");
    m_pMesh = mApp->getRenderMgr()->createMeshRObj();
    SVDataSwapPtr t_data = MakeSharedPtr<SVDataSwap>();
    SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
    t_data->writeData(t_person->getFaceData(), 212*sizeof(f32));
    m_pMesh->setVertexDataNum(106);
    m_pMesh->setVertexData(t_data);
    m_pMesh->setVertexType(E_VF_V2);
    m_pMesh->setDrawMethod(E_DM_POINTS);
    m_pMesh->createMesh();
}

SVFacePointNode::~SVFacePointNode(){
    
}

void SVFacePointNode::update(f32 _dt){
    SVNode::update(_dt);
    if(m_pRenderObj && m_pMtl){
        m_pMtl->setModelMatrix(m_absolutMat.get());
        m_pMtl->update(_dt);
        m_pMtl->setBlendEnable(false);
        m_pMtl->setTexcoordFlip(1.0, 1.0);
        SVDataSwapPtr t_data = MakeSharedPtr<SVDataSwap>();
        SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
        if (t_person->getExist()) {
            t_data->writeData(t_person->getFaceData(), 212*sizeof(f32));
            m_pMesh->setVertexDataNum(106);
            m_pMesh->setVertexData(t_data);
            m_pRenderObj->setMtl(m_pMtl);
            m_pRenderObj->setMesh(m_pMesh);
        }
    }
}

void SVFacePointNode::render(){
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
    SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
    if (t_person->getExist()) {
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, RST_DEBUG, "SVFacePointNode");
        }
    }
}
