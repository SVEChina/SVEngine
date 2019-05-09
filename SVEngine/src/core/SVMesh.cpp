//
// SVMesh.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMesh.h"
#include "../app/SVInst.h"
#include "../base/SVDataSwap.h"
#include "../base/SVLock.h"
#include "../mtl/SVMtlCore.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRendererBase.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderScene.h"

SVMesh::SVMesh(SVInst* _app)
:SVGBase(_app){
    m_pRenderMesh = nullptr;
    m_pMtl = nullptr;
    m_lock = MakeSharedPtr<SVLock>();
}

SVMesh::~SVMesh() {
    m_pRenderMesh = nullptr;
    m_pMtl = nullptr;
    m_lock = nullptr;
}

void SVMesh::setName(cptr8 _name) {
    m_name = _name;
}

cptr8 SVMesh::getName(){
    return m_name.c_str();
}

//数据操作
void SVMesh::setData(SVDataSwapPtr _data,VFTYPE _vtf,s32 _count,s32 _seqMode) {
    m_pRenderMesh = MakeSharedPtr<SVRenderMesh>(mApp);
    m_pRenderMesh->setVertexType(_vtf);
    m_pRenderMesh->setSeqMode(_seqMode);
    m_pRenderMesh->setVertexDataNum(_count);
    m_pRenderMesh->setVertexData(_data);
    m_pRenderMesh->setDrawMethod(E_DM_LINE_LOOP);
    m_pRenderMesh->createMesh();
}

void SVMesh::setRenderMesh(SVRenderMeshPtr _rmesh) {
    m_pRenderMesh = _rmesh;
}

void SVMesh::setMtl(SVMtlCorePtr _mtl) {
    m_pMtl = _mtl;
}

//子mesh操作
void SVMesh::addMesh(SVMeshPtr _mesh) {
    m_lock->lock();
    if(_mesh) {
        m_meshPool.append(_mesh);
    }
    m_lock->unlock();
}

void SVMesh::removeMesh(cptr8 _name) {
    m_lock->lock();
    for(s32 i=0;i<m_meshPool.size();i++){
        if(strcmp( m_meshPool[i]->getName(), _name) == 0){
            m_meshPool.removeForce(i);
            break;
        }
    }
    m_lock->unlock();
}

void SVMesh::clearMesh() {
    m_lock->lock();
    m_meshPool.destroy();
    m_lock->unlock();
}

void SVMesh::update(f32 _dt) {
    if(m_pMtl) {
        FMat4 tMat;
        tMat.setIdentity();
        tMat.setScale(FVec3(200.0f,200.0f,200.0f));
        m_pMtl->setModelMatrix(tMat.get());
        //m_pMtl->setTexture(0,pMeshData->m_pTex);
        m_pMtl->setBlendEnable(false);
        //m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        //m_pMtl->setBlendMode(SVMtlAni2D::SV_MTL_BLENDMODE_NORMAL);
        m_pMtl->update(_dt);
    }
}

void SVMesh::render() {
    //先渲染自己
     SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if(m_pRenderMesh && m_pMtl && t_rs) {
        SVRenderCmdNorPtr t_cmd = MakeSharedPtr<SVRenderCmdNor>();
        t_cmd->setMaterial(m_pMtl);
        t_cmd->setMesh(m_pRenderMesh);
        t_cmd->setRenderer(mApp->getRenderer());
        t_cmd->mTag = "SVMesh";
        t_rs->pushRenderCmd(RST_MASK2D, t_cmd);
    }
    //DATAMAP m_dataMap;
    m_lock->lock();
    for(s32 i=0;i<m_meshPool.size();i++) {
        m_meshPool[i]->render();
    }
    m_lock->unlock();
}

//Morph动画的mesh
SVMorphMesh::SVMorphMesh(SVInst* _app)
:SVMesh(_app){
    
}

SVMorphMesh::~SVMorphMesh() {
    
}


