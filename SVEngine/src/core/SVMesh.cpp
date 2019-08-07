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
#include "../mtl/SVMtlGLTF.h"
#include "../mtl/SVMtlLib.h"
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

void SVMesh::setBox(SVBoundBox& _box) {
    m_box = _box;
}

SVBoundBox SVMesh::getBox() {
    return m_box;
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

void SVMesh::update(f32 _dt,FMat4& _mat) {
    if(m_pMtl) {
//        FMat4 tMat_rotx;
//        tMat_rotx.setIdentity();
//        tMat_rotx.setRotateX(40.0f);
//
//        FMat4 tMat_roty;
//        tMat_roty.setIdentity();
//        tMat_roty.setRotateY(40.0f);
//
//        FMat4 tMat_sc;
//        tMat_sc.setIdentity();
//        tMat_sc.setScale(FVec3(0.001f,0.001f,0.001f));
//        //
//        FMat4 tMat = tMat_sc * tMat_roty * tMat_rotx;
        m_pMtl->setModelMatrix(_mat.get());
        m_pMtl->setBlendEnable(false);
        m_pMtl->setDepthEnable(true);
        //m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        //m_pMtl->setBlendMode(SVMtlAni2D::SV_MTL_BLENDMODE_NORMAL);
        m_pMtl->update(_dt);
    }
}

void SVMesh::render() {
    //先渲染自己
     SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if(m_pRenderMesh && t_rs) {
        SVRenderCmdNorPtr t_cmd = MakeSharedPtr<SVRenderCmdNor>();
        if(m_pMtl) {
            t_cmd->setMaterial(m_pMtl->clone());
        }else{
            //走默认材质
            SVMtlCorePtr t_mtl_default = SVMtlLib::getSkinMtl(mApp);
            t_cmd->setMaterial(t_mtl_default);
        }
        t_cmd->setMesh(m_pRenderMesh);
        t_cmd->setRenderer(mApp->getRenderer());
        t_cmd->mTag = "SVMesh";
        t_rs->pushRenderCmd(RST_MASK2D, t_cmd);
    }
}

//Morph动画的mesh
SVMorphMesh::SVMorphMesh(SVInst* _app)
:SVMesh(_app){
    
}

SVMorphMesh::~SVMorphMesh() {
    
}


