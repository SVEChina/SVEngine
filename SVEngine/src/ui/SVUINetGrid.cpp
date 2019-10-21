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

SVUINetElem::SVUINetElem(SVInst* _app)
:SVGBase(_app){
    m_pTex = nullptr;
    m_pMesh = nullptr;
    m_pRObj = MakeSharedPtr<SVRenderObject>();
    m_tag = -1;
    m_elemPool.clear();
    m_dirty = true;
}

SVUINetElem::~SVUINetElem() {
    m_pTex = nullptr;
    m_pMesh = nullptr;
    m_pRObj = nullptr;
    m_tag = -1;
    m_elemPool.clear();
}

bool SVUINetElem::hasData(s32 _row,s32 _col) {
    for(s32 i=0;i<m_elemPool.size();i++) {
        if( (m_elemPool[i].m_row == _row) &&
            (m_elemPool[i].m_col == _col) ) {
            return true;
        }
    }
    return false;
}

void SVUINetElem::pushData(s32 _row,s32 _col) {
    if( hasData(_row,_col) ) {
        return ;
    }
    ElemCoord t_coord;
    t_coord.m_row = _row;
    t_coord.m_col = _col;
    m_elemPool.append(t_coord);
    m_dirty = true;
}

void SVUINetElem::popData() {
    s32 t_len = m_elemPool.size();
    m_elemPool.remove(t_len-1);
    m_dirty = true;
}

void SVUINetElem::clearData() {
    m_dirty = true;
    m_elemPool.clear();
}

bool SVUINetElem::delData(s32 _row,s32 _col) {
    for(s32 i=0;i<m_elemPool.size();i++) {
        if( (m_elemPool[i].m_row == _row) &&
            (m_elemPool[i].m_col == _col) ) {
            m_elemPool.remove(i);
            m_dirty = true;
            return true;
        }
    }
    return false;
}

void SVUINetElem::refreshData(s32 _unit) {
    if(!m_dirty)
        return ;
    m_dirty = false;
    //构建数据
    s32 t_len = m_elemPool.size();
    if(t_len ==0 )
        return ;
    s32 t_index_num = t_len*6;
    SVDataSwapPtr t_index_data = MakeSharedPtr<SVDataSwap>();
    SVDataSwapPtr t_data = MakeSharedPtr<SVDataSwap>();
    for(s32 i=0;i<t_len;i++) {
        s32 t_x = m_elemPool[i].m_col;
        s32 t_y = m_elemPool[i].m_row;
        V2_T0 verts[6];
        //0
        verts[0].x = t_x*_unit;
        verts[0].y = (t_y+1.0f)*-_unit;
        verts[0].t0x = 0.0f;
        verts[0].t0y = 0.0f;
        //1
        verts[1].x = (t_x+1.0f)*_unit;
        verts[1].y = (t_y+1.0f)*-_unit;
        verts[1].t0x = 1.0f;
        verts[1].t0y = 0.0f;
        //2
        verts[2].x = t_x*_unit;
        verts[2].y = t_y*-_unit;
        verts[2].t0x = 0.0f;
        verts[2].t0y = 1.0f;
        //2
        verts[3].x = verts[2].x;
        verts[3].y = verts[2].y;
        verts[3].t0x = verts[2].t0x;
        verts[3].t0y = verts[2].t0y;
        //1
        verts[4].x = verts[1].x;
        verts[4].y = verts[1].y;
        verts[4].t0x = verts[1].t0x;
        verts[4].t0y = verts[1].t0y;
        //3
        verts[5].x = (t_x+1.0f)*_unit;
        verts[5].y = t_y*-_unit;
        verts[5].t0x = 1.0f;
        verts[5].t0y = 1.0f;
        t_data->appendData(verts, sizeof(V2_T0) * 6);
    }
    //
    if(!m_pMesh) {
        m_pMesh = mApp->getRenderMgr()->createMeshRObj();
        m_pMesh->createMesh();
    }
    m_pMesh->setVertexDataNum(t_len*6);
    m_pMesh->setVertexData(t_data);
    m_pMesh->setVertexType(E_VF_V2_T0);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
}

void SVUINetElem::update(f32 _dt,f32 *_mat) {
    if(m_elemPool.size() == 0)
        return ;//没有数据 不渲染
    if(m_pRObj && m_pMesh) {
        m_pRObj->setMesh(m_pMesh);
        SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
        t_mtl->update(_dt);
        t_mtl->setTexture(0, m_pTex);
        t_mtl->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
        t_mtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
        t_mtl->setModelMatrix(_mat);
        t_mtl->setTexcoordFlip(1.0, -1.0f);
        t_mtl->setDepthEnable(false);
        t_mtl->setZOffEnable(false);
        t_mtl->setZOffParam(-1.0f, -1.0f);
        t_mtl->setBlendEnable(true);
        t_mtl->setBlendState(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        m_pRObj->setMtl(t_mtl);
    }
}

void SVUINetElem::render(RENDERSTREAMTYPE _rsType) {
    if(m_elemPool.size() == 0)
        return ;//没有数据 不渲染
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (m_pRObj) {
        m_pRObj->pushCmd(t_rs, _rsType, "SVUINetElem");
    }
}

//
SVUINetGrid::SVUINetGrid(SVInst *_app)
:SVUIBase(_app){
    m_rsType = RST_UI;
    m_canSelect = true;
    m_visible = true;
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

s32 SVUINetGrid::getMaxGridX() {
    return m_grid_x;
}

s32 SVUINetGrid::getMaxGridY() {
    return m_grid_y;
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
        //包围盒更新
        m_aabbBox.clear();
        SVRect t_rc = SVUIBase::getRect(m_archo,t_w,t_h);
        FVec3 t_min(t_rc.m_lb_pt);
        FVec3 t_max(t_rc.m_rt_pt);
        m_aabbBox.expand(t_min);
        m_aabbBox.expand(t_max);
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
    //绘制元素
    for(s32 i=0;i<m_elemTbl.size();i++) {
        m_elemTbl[i]->refreshData(m_grid_unit);
        m_elemTbl[i]->update(dt,m_absolutMat.get());
    }
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
        for(s32 i=0;i<m_elemTbl.size();i++) {
            m_elemTbl[i]->render(m_rsType);
        }
    }
    SVNode::render();
}

//增加一种元素
bool SVUINetGrid::addElem(s32 _type,cptr8 _texname) {
    if(hasElem(_type)) {
        return false;
    }
    SVUINetElemPtr t_elem = MakeSharedPtr<SVUINetElem>(mApp);
    t_elem->m_tag = _type;
    t_elem->m_pTex = mApp->getTexMgr()->getTexture(_texname,true);
    m_elemTbl.append(t_elem);
    return true;
}

//删除一种元素
bool SVUINetGrid::delElem(s32 _type) {
    for(s32 i=0;i<m_elemTbl.size();i++) {
        if( m_elemTbl[i]->m_tag == _type ) {
            m_elemTbl.removeForce(i);
            return true;
        }
    }
    return false;
}

//查找一种元素
bool SVUINetGrid::hasElem(s32 _type) {
    for(s32 i=0;i<m_elemTbl.size();i++) {
        if( m_elemTbl[i]->m_tag == _type ) {
            return true;
        }
    }
    return false;
}

SVUINetElemPtr SVUINetGrid::getElem(s32 _type) {
    for(s32 i=0;i<m_elemTbl.size();i++) {
        if( m_elemTbl[i]->m_tag == _type ) {
            return m_elemTbl[i];
        }
    }
    return nullptr;
}

//增加某一元素的数据
bool SVUINetGrid::addElemData(s32 _type,s32 _row,s32 _col) {
    SVUINetElemPtr t_elem = getElem(_type);
    if(t_elem){
        t_elem->pushData(_row, _col);
    }
    return true;
}

//删除某一元素的数据
void SVUINetGrid::delElemData(s32 _type,s32 _row,s32 _col) {
    SVUINetElemPtr t_elem = getElem(_type);
    if(t_elem) {
        t_elem->delData(_row, _col);
    }
}

//清楚某一元素的数据
void SVUINetGrid::clearElemData(s32 _type) {
    SVUINetElemPtr t_elem = getElem(_type);
    if(t_elem){
        t_elem->clearData();
    }
}

//弹出某一元素的数据
void SVUINetGrid::popElemData(s32 _type) {
    SVUINetElemPtr t_elem = getElem(_type);
    if(t_elem){
        t_elem->popData();
    }
}

//刷新某一元素的数据
void SVUINetGrid::_updateElemShow(s32 _type) {
    SVUINetElemPtr t_elem = getElem(_type);
    if(t_elem){
        _updateElemShow(t_elem);
    }
}

//刷新所有元素的数据
void SVUINetGrid::_updateAllElemShow() {
    for(s32 i=0;i<m_elemTbl.size();i++) {
        _updateElemShow(m_elemTbl[i]);
    }
}

//
void SVUINetGrid::_updateElemShow(SVUINetElemPtr _elem) {
    if(_elem) {
        _elem->refreshData(m_grid_unit);
    }
}
