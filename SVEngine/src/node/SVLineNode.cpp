//
// SVLineNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVLineNode.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../app/SVInst.h"
#include "../mtl/SVMtlNocolor.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderMgr.h"

SVLineNode::SVLineNode(SVInst *_app)
: SVNode(_app) {
    ntype = "SVLineNode";
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_lineMtl = MakeSharedPtr<SVMtlGeo3d>(mApp);
    m_pMesh = mApp->getRenderMgr()->createMeshRObj();
    m_pMesh->setVertexType(E_VF_V3);
    m_pMesh->setDrawMethod(E_DM_LINES);
    m_pMesh->createMesh();
    m_drawBox = true;
    m_visible = true;
}

SVLineNode::~SVLineNode() {
    m_pRenderObj = nullptr;
    m_pMesh = nullptr;
    m_lineMtl = nullptr;
}

void SVLineNode::setLineData(f32 *_data, s32 _size){
    if (m_lineMtl && _data) {
        SVDataSwapPtr t_data = MakeSharedPtr<SVDataSwap>();
        t_data->writeData(_data, _size*sizeof(f32));
        s32 t_count = _size/3;
        m_pMesh->setVertexDataNum(t_count);
        m_pMesh->setVertexData(t_data);
    }
}

void SVLineNode::update(f32 _dt) {
    SVNode::update(_dt);
    if (m_pRenderObj && m_pMesh) {
        m_lineMtl->update(_dt);
        m_lineMtl->setModelMatrix(m_absolutMat.get());
        m_lineMtl->setLineSize(4.0f);
        m_lineMtl->setColor(1.0f, 0.0f, 0.0f, 1.0f);
        m_lineMtl->setBlendEnable(false);
        m_lineMtl->setTexcoordFlip(1.0, 1.0);
        m_lineMtl->setBlendState(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(m_lineMtl);
    }
}

void SVLineNode::render() {
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, RST_SOLID_3D, "SVLineNode");
        }
    }
    SVNode::render();
}










