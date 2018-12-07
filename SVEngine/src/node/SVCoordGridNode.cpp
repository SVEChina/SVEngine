//
// SVCoordGridNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVCoordGridNode.h"
#include "../core/SVGeoGen.h"
#include "../mtl/SVMtlNocolor.h"
#include "../mtl/SVTexMgr.h"
#include "../node/SVScene.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderMgr.h"

#define COORD_SIZE 64*100

SVCoordGridNode::SVCoordGridNode(SVInst *_app)
:SVNode(_app)
,m_unit(50.0f){
    m_rsType = RST_LASTFILTER;
    m_pMeshX = SVGeoGen::createNetGrid(mApp,COORD_SIZE,0);
    m_pMeshY = SVGeoGen::createNetGrid(mApp,COORD_SIZE,1);
    m_pMeshZ = SVGeoGen::createNetGrid(mApp,COORD_SIZE,2);
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    //m_gridTex = mApp->getTexMgr()->getTextureSync("svres/grid6.png",true);
    m_gridTex = nullptr;
}

SVCoordGridNode::~SVCoordGridNode(){
    m_pMeshX = nullptr;
    m_pMeshY = nullptr;
    m_pMeshZ = nullptr;
    m_pRenderObj = nullptr;
    m_gridTex = nullptr;
}

void SVCoordGridNode::update(f32 dt){
    SVNode::update(dt);
    _refreshUnit();
    //
    if(!m_gridTex){
       m_gridTex = mApp->getTexMgr()->getTextureSync("svres/grid2.png",true,true);
    }
    SVMtlCoordPtr t_mtl_coord = MakeSharedPtr<SVMtlCoord>(mApp);
    t_mtl_coord->update(dt * 0.001f);
    t_mtl_coord->setTexture(0, m_gridTex);
    t_mtl_coord->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
    t_mtl_coord->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
    t_mtl_coord->setModelMatrix(m_absolutMat.get());
    t_mtl_coord->setTexcoordFlip(1.0, -1.0f);
    t_mtl_coord->setDepthEnable(true);
    t_mtl_coord->setZOffEnable(true);
    t_mtl_coord->setZOffParam(-1.0f, -1.0f);
    t_mtl_coord->setBlendEnable(true);
    t_mtl_coord->setBlendState(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    m_pRenderObj->setMtl(t_mtl_coord);
}

void SVCoordGridNode::render(){
//    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
//        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
//        if (m_pRenderObj) {
//            m_pRenderObj->setMesh(m_pMeshX);
//            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVCoordGridNode");
//            m_pRenderObj->setMesh(m_pMeshY);
//            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVCoordGridNode");
//            m_pRenderObj->setMesh(m_pMeshZ);
//            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVCoordGridNode");
     //   }
//        //
//        FMat4 m_mat_unit = FMat4_identity;
//        SVMtlGeo3dPtr t_mtl = MakeSharedPtr<SVMtlGeo3d>(mApp);
//        t_mtl->setColor(1.0f, 1.0f, 1.0f, 1.0f);
//        t_mtl->setModelMatrix( m_mat_unit.get() );
//        t_mtl->setLineSize(5);
//        //
//        SVRenderObjInst::pushLineCmd(t_rs,
//                                     m_rsType,
//                                     FVec3(-50.0f*m_unit,0.0f,0.0f),
//                                     FVec3(50.0f*m_unit,0.0f,0.0f),
//                                     t_mtl,
//                                     "sv_coord_axis");
//        //
//        SVRenderObjInst::pushLineCmd(t_rs,
//                                     m_rsType,
//                                     FVec3(0.0f,0.0f,-50.0f*m_unit),
//                                     FVec3(0.0f,0.0f,50.0f*m_unit),
//                                     t_mtl,
//                                     "sv_coord_axis");
 //   }
    SVNode::render();
}

f32 SVCoordGridNode::getUnit() {
    return m_unit;
}

void SVCoordGridNode::_refreshUnit() {
    
}

