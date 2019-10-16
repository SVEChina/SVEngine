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

SVUINetElem::SVUINetElem() {
    m_pTex = nullptr;
    m_pMesh = nullptr;
    m_elemPool.clear();
}

SVUINetElem::~SVUINetElem() {
    m_pTex = nullptr;
    m_pMesh = nullptr;
    m_elemPool.clear();
}

void SVUINetElem::pushData(s32 _row,s32 _col) {
    ElemCoord t_coord;
    t_coord.m_row = _row;
    t_coord.m_col = _col;
    m_elemPool.append(t_coord);
}

void SVUINetElem::popData() {
    s32 t_len = m_elemPool.size();
    m_elemPool.remove(t_len-1);
}

void SVUINetElem::clearData() {
    m_elemPool.clear();
}

bool SVUINetElem::delData(s32 _row,s32 _col) {
    for(s32 i=0;i<m_elemPool.size();i++) {
        if( (m_elemPool[i].m_row == _row) &&
            (m_elemPool[i].m_col == _col) ) {
            m_elemPool.remove(i);
            return true;
        }
    }
    return false;
}

void SVUINetElem::refreshData(s32 _unit,SVRenderMgrPtr _renderMgr) {
    if(!_renderMgr)
        return ;
    //构建数据
    s32 t_len = m_elemPool.size();
    s32 t_index_num = t_len*6;
    SVDataSwapPtr t_index_data = MakeSharedPtr<SVDataSwap>();
    SVDataSwapPtr t_data = MakeSharedPtr<SVDataSwap>();
    for(s32 i=0;i<t_len;i++) {
        s32 t_x = m_elemPool[i].m_row-1;
        s32 t_y = m_elemPool[i].m_col-1;
        V2_T0 verts[4];
        verts[0].x = t_x*_unit;
        verts[0].y = (t_y+1.0f)*-_unit;
        verts[0].t0x = 0.0f;
        verts[0].t0y = 0.0f;

        verts[1].x = (t_x+1.0f)*_unit;
        verts[1].y = (t_y+1.0f)*-_unit;
        verts[1].t0x = 1.0f;
        verts[1].t0y = 0.0f;

        verts[2].x = t_x*_unit;
        verts[2].y = t_y*-_unit;
        verts[2].t0x = 0.0f;
        verts[2].t0y = 1.0f;

        verts[3].x = (t_x+1.0f)*_unit;
        verts[3].y = t_y*-_unit;
        verts[3].t0x = 1.0f;
        verts[3].t0y = 1.0f;
        t_data->appendData(verts, sizeof(V2_T0) * 4);
        //
        s16 index[6];
        index[0] = i*4;
        index[1] = i*4+1;
        index[2] = i*4+2;
        index[3] = i*4+2;
        index[4] = i*4+1;
        index[5] = i*4+3;
        t_index_data->appendData(index, sizeof(s16) * 6);
    }
    //
    if(!m_pMesh) {
        m_pMesh = _renderMgr->createMeshRObj();
        m_pMesh->createMesh();
    }
    m_pMesh->setIndexData(t_index_data, t_index_num);
    m_pMesh->setVertexDataNum(t_len*4);
    m_pMesh->setVertexData(t_data);
    m_pMesh->setVertexType(E_VF_V2_T0);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
}

//
SVUINetGrid::SVUINetGrid(SVInst *_app)
:SVNode(_app){
    m_rsType = RST_UI;
    m_archo = E_ARCHO_LT;
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

void SVUINetGrid::setArcho(EUIARCHO _archo) {
    m_archo = _archo;
    m_refresh = true;
}

void SVUINetGrid::update(f32 dt){
    SVNode::update(dt);
    if(m_refresh) {
        m_refresh = false;
        f32 t_w = m_grid_unit*m_grid_x;
        f32 t_h = m_grid_unit*m_grid_y;
        m_pMesh = SVGeoGen::genRectARCHO(mApp,t_w,t_h,m_archo,m_aabbBox);
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
        //绘制背景
        
        //绘制网格
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVUINetGrid");
        }
        //绘制元素
        
        //ELEMTBL m_elemTbl;
    }
    SVNode::render();
}

//增加一种元素
bool SVUINetGrid::addElem(s32 _type,cptr8 _texname) {
    ELEMTBL::Iterator it = m_elemTbl.find(_type);
    if(it != m_elemTbl.end() ) {
        //已经存在，就不要在加
        return false;
    }
    //
    SVUINetElemPtr t_elem = MakeSharedPtr<SVUINetElem>();
    t_elem->m_pTex = mApp->getTexMgr()->getTexture(_texname);
    m_elemTbl.append(_type,t_elem);
    return true;
}

//删除一种元素
bool SVUINetGrid::delElem(s32 _type) {
    ELEMTBL::Iterator it = m_elemTbl.find(_type);
    if(it != m_elemTbl.end() ) {
        //已经存在，才可以删除
        m_elemTbl.remove(it);
        return true;
    }
    return false;
}

//查找一种元素
bool SVUINetGrid::hasElem(s32 _type) {
    ELEMTBL::Iterator it = m_elemTbl.find(_type);
    if(it == m_elemTbl.end() ) {
        return false;
    }
    return true;
}

//增加某一元素的数据
bool SVUINetGrid::addElemData(s32 _type,s32 _row,s32 _col) {
    ELEMTBL::Iterator it = m_elemTbl.find(_type);
    if(it != m_elemTbl.end() ) {
        //已经存在了，还加毛线
        return false;
    }
    SVUINetElemPtr t_elem = it->data;
    t_elem->pushData(_row, _col);
    return true;
}

//删除某一元素的数据
void SVUINetGrid::delElemData(s32 _type,s32 _row,s32 _col) {
    ELEMTBL::Iterator it = m_elemTbl.find(_type);
    if(it == m_elemTbl.end() ) {
        //不存在了，删除毛线
        return ;
    }
    SVUINetElemPtr t_elem = it->data;
    t_elem->delData(_row, _col);
}

//清楚某一元素的数据
void SVUINetGrid::clearElemData(s32 _type) {
    ELEMTBL::Iterator it = m_elemTbl.find(_type);
    if(it != m_elemTbl.end() ) {
        SVUINetElemPtr t_elem = it->data;
        t_elem->clearData();
    }
}

//弹出某一元素的数据
void SVUINetGrid::popElemData(s32 _type) {
    ELEMTBL::Iterator it = m_elemTbl.find(_type);
    if(it == m_elemTbl.end() ) {
        //不存在了，弹出什么数据
        return ;
    }
    SVUINetElemPtr t_elem = it->data;
    t_elem->popData();
}

//刷新某一元素的数据
void SVUINetGrid::_updateElemShow(s32 _type) {
    ELEMTBL::Iterator it = m_elemTbl.find(_type);
    if(it != m_elemTbl.end() ) {
        _updateElemShow(it->data);
    }
}

//刷新所有元素的数据
void SVUINetGrid::_updateAllElemShow() {
    ELEMTBL::Iterator it = m_elemTbl.begin();
    while( it!=m_elemTbl.end() ) {
        _updateElemShow(it->data);
        it++;
    }
}

void SVUINetGrid::_updateElemShow(SVUINetElemPtr _elem) {
    if(_elem) {
        _elem->refreshData(m_grid_unit, mApp->getRenderMgr());
    }
}
