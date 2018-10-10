//
// SVCoordGridNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVCoordGridNode.h"
#include "../core/SVGeoGen.h"
#include "../mtl/SVMtlNocolor.h"
#include "../node/SVScene.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderMgr.h"

SVCoordGridNode::SVCoordGridNode(SVInst *_app)
:SVNode(_app)
,m_unit(50.0f){
    m_rsType = RST_MAGIC_BEGIN;
    m_pMesh = SVGeoGen::createNetGrid(mApp,50,m_unit);
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
}

SVCoordGridNode::~SVCoordGridNode(){
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
}

void SVCoordGridNode::update(f32 dt){
    SVNode::update(dt);
    //
    _refreshUnit();
    //
    SVMtlGeo3dPtr t_mtl_box = MakeSharedPtr<SVMtlGeo3d>(mApp);
    t_mtl_box->setColor(0.9f, 0.9f, 0.9f, 0.5f);
    t_mtl_box->update(dt * 0.001f);
    t_mtl_box->setModelMatrix(m_absolutMat.get());
    t_mtl_box->setTexcoordFlip(1.0, -1.0f);
    t_mtl_box->setBlendEnable(true);
    t_mtl_box->setBlendState(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    m_pRenderObj->setMesh(m_pMesh);
    m_pRenderObj->setMtl(t_mtl_box);
}

void SVCoordGridNode::render(){
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVCoordGridNode");
        }
        //
        FMat4 m_mat_unit = FMat4_identity;
        SVMtlGeo3dPtr t_mtl = MakeSharedPtr<SVMtlGeo3d>(mApp);
        t_mtl->setColor(1.0f, 1.0f, 1.0f, 1.0f);
        t_mtl->setModelMatrix( m_mat_unit.get() );
        t_mtl->setLineSize(5);
        //
        SVRenderObjInst::pushLineCmd(t_rs,
                                     m_rsType,
                                     FVec3(-50.0f*m_unit,0.0f,0.0f),
                                     FVec3(50.0f*m_unit,0.0f,0.0f),
                                     t_mtl,
                                     "sv_coord_axis");
        //
        SVRenderObjInst::pushLineCmd(t_rs,
                                     m_rsType,
                                     FVec3(0.0f,-50.0f*m_unit,0.0f),
                                     FVec3(0.0f,50.0f*m_unit,0.0f),
                                     t_mtl,
                                     "sv_coord_axis");
    }
    SVNode::render();
}

f32 SVCoordGridNode::getUnit() {
    return m_unit;
}

void SVCoordGridNode::_refreshUnit() {
    
}

