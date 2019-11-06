//
// SVPatchNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPatchNode.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../app/SVInst.h"
#include "../basesys/SVStaticData.h"
#include "../mtl/SVMtlNocolor.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderMgr.h"
SVPatchNode::SVPatchNode(SVInst *_app)
: SVNode(_app) {
    ntype = "SVPatchNode";
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_patchMtl = MakeSharedPtr<SVMtlGeo3d>(mApp);
    FVec3 t_corner00 = FVec3(-100.0, 0.0f, 0.0f);
    FVec3 t_corner10 = FVec3(100.0, 0.0f, 0.0f);
    FVec3 t_corner01 = FVec3(-100.0, 100.0f, 0.0f);
    FVec3 t_corner11 = FVec3(100.0, 100.0f, 0.0f);
    m_pMesh = mApp->getDataMgr()->generatePatchMesh(t_corner00, t_corner10, t_corner01, t_corner11, 20, 20);
    m_drawBox = true;
    m_visible = true;
}

SVPatchNode::~SVPatchNode() {
    m_pRenderObj = nullptr;
    m_pMesh = nullptr;
    m_patchMtl = nullptr;
}

void SVPatchNode::setPatchVertexData(SVDataSwapPtr _vertexData, s32 _vertexCount){
    if (m_patchMtl && m_pMesh && _vertexData) {
        m_pMesh->setVertexDataNum(_vertexCount);
        m_pMesh->setVertexData(_vertexData);
    }
}

void SVPatchNode::setPatchIndexData(SVDataSwapPtr _indicesData, s32 _indicesCount){
    if (m_patchMtl && m_pMesh && _indicesData) {
        m_pMesh->setIndexData(_indicesData, _indicesCount);
    }
}

void SVPatchNode::update(f32 _dt) {
    SVNode::update(_dt);
    if (m_pRenderObj && m_pMesh) {
        m_patchMtl->update(_dt);
        m_patchMtl->setModelMatrix(m_absolutMat.get());
        m_patchMtl->setColor(1.0f, 0.0f, 0.0f, 1.0f);
        m_patchMtl->setBlendEnable(false);
        m_patchMtl->setTexcoordFlip(1.0, 1.0);
        m_patchMtl->setLineSize(2.0f);
        m_patchMtl->setBlendState(MTL_BLEND_SRC_ALPHA,MTL_BLEND_ONE_MINUS_SRC_ALPHA);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(m_patchMtl);
    }
}

void SVPatchNode::render() {
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, RST_SOLID_3D, "SVPatchNode");
        }
    }
    SVNode::render();
}










