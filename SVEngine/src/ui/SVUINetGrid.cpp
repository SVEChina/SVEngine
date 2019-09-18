//
// SVUINetGrid.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVUINetGrid.h"
#include "../app/SVInst.h"
#include "../basesys/SVConfig.h"
#include "../core/SVGeoGen.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtl2D.h"
#include "../mtl/SVMtlNocolor.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRendererBase.h"
#include "../basesys/SVStaticData.h"


//
SVUINetGrid::SVUINetGrid(SVInst *_app)
:SVNode(_app){
    m_rsType = RST_UI;
    m_refresh = true;
    m_grid_unit = 16.0f;
    m_grid_x = 2;
    m_grid_y = 2;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMesh = nullptr;
    m_gridTex = nullptr;
}

SVUINetGrid::~SVUINetGrid(){
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
    m_gridTex = nullptr;
}

//
f32 SVUINetGrid::getWidth() {
    return m_grid_unit*m_grid_x;
}

f32 SVUINetGrid::getHeight() {
    return m_grid_unit*m_grid_y;
}

f32 SVUINetGrid::getGridUnit(f32 _h) {
    return m_grid_unit;
}

void SVUINetGrid::setMaxGridX(s32 _max_gx) {
    m_grid_x = _max_gx;
    m_refresh = true;
}

void SVUINetGrid::setMaxGridY(s32 _max_gy) {
    m_grid_y = _max_gy;
    m_refresh = true;
}

void SVUINetGrid::setGridUnit(f32 _unit) {
    m_grid_unit = _unit;
    m_refresh = true;
}

void SVUINetGrid::update(f32 dt){
    SVNode::update(dt);
    if(m_refresh) {
        m_refresh = false;
        f32 t_w = m_grid_unit*m_grid_x;
        f32 t_h = m_grid_unit*m_grid_y;
        m_pMesh = SVGeoGen::genRectARCHO(mApp,t_w,t_h,E_ARCHO_LB,m_aabbBox);
    }
    if(!m_gridTex){
        m_gridTex = mApp->getTexMgr()->getTextureSync("svres/grid6.png",true,true);
    }
    SVMtlNetGridPtr t_mtl_netgrid = MakeSharedPtr<SVMtlNetGrid>(mApp);
    t_mtl_netgrid->update(dt);
    t_mtl_netgrid->setTexture(0, m_gridTex);
    t_mtl_netgrid->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
    t_mtl_netgrid->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
    t_mtl_netgrid->setModelMatrix(m_absolutMat.get());
    t_mtl_netgrid->setTexcoordFlip(1.0, -1.0f);
    t_mtl_netgrid->setDepthEnable(false);
    t_mtl_netgrid->setZOffEnable(false);
    t_mtl_netgrid->setZOffParam(-1.0f, -1.0f);
    t_mtl_netgrid->setBlendEnable(true);
    t_mtl_netgrid->setBlendState(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    t_mtl_netgrid->setGridSize(m_grid_x,m_grid_y);
    m_pRenderObj->setMtl(t_mtl_netgrid);
}

void SVUINetGrid::render(){
    if (m_visible ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVUINetGrid");
        }
    }
    SVNode::render();
}
