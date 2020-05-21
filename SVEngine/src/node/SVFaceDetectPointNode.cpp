//
// SVFaceDetectPointNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVFaceDetectPointNode.h"
#include "SVScene.h"
#include "SVCameraNode.h"
#include "../mtl/SVMtlCore.h"
#include "../rendercore/SVRenderMesh.h"
#include "../core/SVGeoGen.h"
#include "../basesys/SVConfig.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderer.h"
#include "../basesys/SVStaticData.h"

SVFaceDetectPointNode::SVFaceDetectPointNode(SVInst *_app):SVNode(_app) {
    ntype = "SVFaceDetectPointNode";
    m_rsType = RST_DEBUG;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_canSelect = false;
    m_pTex = mApp->getTexMgr()->getTextureSync("svres/point.png",true);;
    m_pMesh = nullptr;
}

SVFaceDetectPointNode::~SVFaceDetectPointNode(){
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
    m_pMtl = nullptr;
    m_pTex =nullptr;
}

void SVFaceDetectPointNode::setTexture(SVTexturePtr _tex){
    m_pTex=_tex;
}

void SVFaceDetectPointNode::createMesh( V2* _facepoint){
    m_aabbBox.clear();
    V2_T0 verts[636];
    for(s32 i=0;i<106;i++){
        f32 _inx=_facepoint[i].x;
        f32 _iny=m_height-_facepoint[i].y;
        f32 _x=_inx-m_width/2;
        f32 _y=_iny-m_height/2;
        //   V2_T0 verts[4];
        verts[i*6].x = -0.5f * m_inw+_x;
        verts[i*6].y = -0.5f * m_inh+_y;
        verts[i*6].t0x = 0.0;
        verts[i*6].t0y = 0.0;
        
        verts[i*6+1].x = 0.5f * m_inw+_x;
        verts[i*6+1].y = -0.5f * m_inh+_y;
        verts[i*6+1].t0x = 1.0;
        verts[i*6+1].t0y = 0.0;
        
        verts[i*6+2].x = -0.5f * m_inw+_x;
        verts[i*6+2].y = 0.5f * m_inh+_y;
        verts[i*6+2].t0x = 0.0;
        verts[i*6+2].t0y = 1.0;
        
        verts[i*6+3].x = -0.5f * m_inw+_x;
        verts[i*6+3].y = 0.5f * m_inh+_y;
        verts[i*6+3].t0x = 0.0;
        verts[i*6+3].t0y = 1.0;
        
        verts[i*6+4].x = 0.5f * m_inw+_x;
        verts[i*6+4].y = 0.5f * m_inh+_y;
        verts[i*6+4].t0x = 1.0;
        verts[i*6+4].t0y = 1.0;
        
        verts[i*6+5].x = 0.5f * m_inw+_x;
        verts[i*6+5].y = -0.5f * m_inh+_y;
        verts[i*6+5].t0x = 1.0;
        verts[i*6+5].t0y = 0.0;
        //设置包围盒
        for(s32 j=0;j<6;j++){
            if (j == 0) {
                m_aabbBox.expand(FVec3(verts[i*4+j].x, verts[i*4+j].y, 0.0f));
            }else{
                m_aabbBox.expand(FVec3(verts[i*4+j].x, verts[i*4+j].y, 0.0f));
            }
        }
    }
    //
    if(!m_pMesh){
        m_pMesh = mApp->getRenderMgr()->createMeshRObj();
    }
    SVDataSwapPtr t_data = MakeSharedPtr<SVDataSwap>();
    t_data->writeData(&verts[0], sizeof(V2_T0) * 636);
    m_pMesh->setVertexDataNum(636);
    m_pMesh->setVertexData(t_data);
    m_pMesh->setVertexType(E_VF_V2_T0);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
    m_pMesh->createMesh();
}

void SVFaceDetectPointNode::update(f32 _dt){
    SVNode::update(_dt);
    //创建新的材质
    if (m_pRenderObj && m_pMesh) {
        SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
        t_mtl->setBlendEnable(false);
        t_mtl->setBlendState(MTL_BLEND_ONE, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
        t_mtl->setModelMatrix(m_absolutMat.get());
        t_mtl->setTexcoordFlip(1.0, -1.0);
        t_mtl->setTexture(0,m_pTex);
        t_mtl->setBlendEnable(true);
        t_mtl->setBlendState(MTL_BLEND_SRC_ALPHA,MTL_BLEND_ONE_MINUS_SRC_ALPHA);
        t_mtl->update(_dt);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(t_mtl);
    }
}

void SVFaceDetectPointNode::render(){
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVFaceDetectPointNode");
        }
    }
    SVNode::render();
}

