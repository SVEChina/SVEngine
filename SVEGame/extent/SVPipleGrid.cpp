//
// SVPipleGrid.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//


#include "SVPipleGrid.h"
#include "SVPipleGridMgr.h"
#include "SVGameCommonEvent.h"
#include "SVGameDefine.h"
#include "../app/SVInst.h"
#include "../act/SVActDeform.h"
#include "../node/SVSpineNode.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVScene.h"
#include "../event/SVEvent.h"
#include "../event/SVEventMgr.h"
#include "../core/SVSpine.h"
#include "../logicsys/SVSceneMgr.h"

using namespace sv;

SVPipleGrid::SVPipleGrid(SVInst*_app,SVPipleGridMgr* _gridmgr)
:SVEventProc(_app)
,m_pGridMgr(_gridmgr){
    m_gridType = E_PIP_GDT_WAVE;
    m_gridState = E_PIP_GDS_0;
    m_light = false;
    m_pAni = nullptr;
}

SVPipleGrid::~SVPipleGrid() {
    m_pGridMgr = nullptr;
//    if(m_pAni){
//        m_pAni->removeFromParent();
//        m_pAni->svdestroy();
//        SAFE_DELETE(m_pAni);
//    }
}

void SVPipleGrid::init(cptr8 _spinename){
    //SVSpineNode* m_pAni;
//    m_spinename = _spinename;
//    m_state = 0;
//    //
//    stringc skins[] = {"flyheart1","flyheart2","flyheart3","flyheart4","flyheart5"};
//    s32 t_skinid = arc4random() % 5;
//    //
//    SVSpineNodeParam spineparam;
//    spineparam.m_spinename = m_spinename;
//    spineparam.m_aniname = skins[t_skinid];
//    spineparam.m_changeAni = true;
//    spineparam.m_loop = false;
//    m_aniname = skins[t_skinid];
//    m_AniSpineNode = (SVSpineNode*)spineparam.genNode(mApp);
}

void SVPipleGrid::destroy(){
}

void SVPipleGrid::enter(){
}

void SVPipleGrid::exit(){
}

void SVPipleGrid::update(f32 _dt,f32 _gametime){
}

void SVPipleGrid::gen(E_PIPLEGRID_TYPE _type,E_PIPLEGRID_STATE _state,s32 _px,s32 _py){
    m_gridType = _type;
    m_gridState = _state;
    locate(_px,_py);
    //做动作切换哈 表现
}

//默认  wave: L  line: I
void SVPipleGrid::locate(s32 _px,s32 _py){
    m_local.X = _px;
    m_local.Y = _py;
    if( m_gridType == E_PIP_GDT_WAVE ){
        if(m_gridState == E_PIP_GDS_0){
            m_side0.X = m_pGridMgr->roollingX(m_local.X);
            m_side0.Y = m_pGridMgr->roollingY(m_local.Y+1);
            m_side1.X = m_pGridMgr->roollingX(m_local.X+1);
            m_side1.Y = m_pGridMgr->roollingY(m_local.Y);
        }else if(m_gridState == E_PIP_GDS_1){
            m_side0.X = m_pGridMgr->roollingX(m_local.X+1);
            m_side0.Y = m_pGridMgr->roollingY(m_local.Y);
            m_side1.X = m_pGridMgr->roollingX(m_local.X);
            m_side1.Y = m_pGridMgr->roollingY(m_local.Y-1);
        }else if(m_gridState == E_PIP_GDS_2){
            m_side0.X = m_pGridMgr->roollingX(m_local.X);
            m_side0.Y = m_pGridMgr->roollingY(m_local.Y-1);
            m_side1.X = m_pGridMgr->roollingX(m_local.X-1);
            m_side1.Y = m_pGridMgr->roollingY(m_local.Y);
        }else if(m_gridState == E_PIP_GDS_3){
            m_side0.X = m_pGridMgr->roollingX(m_local.X-1);
            m_side0.Y = m_pGridMgr->roollingY(m_local.Y);
            m_side1.X = m_pGridMgr->roollingX(m_local.X);
            m_side1.Y = m_pGridMgr->roollingY(m_local.Y+1);
        }
    }else if( m_gridType == E_PIP_GDT_LINE ){
        if(m_gridState == E_PIP_GDS_0 || m_gridState == E_PIP_GDS_2){
            m_side0.X = m_pGridMgr->roollingX(m_local.X);
            m_side0.Y = m_pGridMgr->roollingY(m_local.Y+1);
            m_side1.X = m_pGridMgr->roollingX(m_local.X);
            m_side1.Y = m_pGridMgr->roollingY(m_local.Y-1);
        }else{
            m_side0.X = m_pGridMgr->roollingX(m_local.X-1);
            m_side0.Y = m_pGridMgr->roollingY(m_local.Y);
            m_side1.X = m_pGridMgr->roollingX(m_local.X+1);
            m_side1.Y = m_pGridMgr->roollingY(m_local.Y);
        }
    }
}

bool SVPipleGrid::isLink(SVPipleGrid* _grid){
    if(!_grid)
        return false;
    if( _grid->m_side0 == m_local && (_grid->m_local == m_side0 || _grid->m_local == m_side1) ){
        //目标0指向我，并且我也有一端指向目标 就相连
        return true;
    }
    if( _grid->m_side1 == m_local && (_grid->m_local == m_side0 || _grid->m_local == m_side1) ){
        //目标1指向我，并且我也有一端指向目标 就相连
        return true;
    }
    return false;
}
//
void SVPipleGrid::setLight(bool _light){
    m_light = _light;
}
//格子发生变换
void SVPipleGrid::trans(){
    s32 t_state_num = (int)m_gridState;
    t_state_num += 1;
    t_state_num = t_state_num%((int)E_PIP_GDS_NUM);
    //
    m_gridState = (E_PIPLEGRID_STATE)t_state_num;
    //重新计算一下关系
    locate(m_local.X,m_local.Y);
}
//
bool SVPipleGrid::procEvent(SVEvent *_event){
    return true;
}

//格子集合
SVPipleGridSet::SVPipleGridSet(){
}

SVPipleGridSet::~SVPipleGridSet(){
    m_grids.clear();
}

bool SVPipleGridSet::isClose(SVPipleGrid* _grid){
    if(m_grids.size() == 0)
        return false;
    if(m_grids[0] == _grid)
        return true;
    return false;
}

void SVPipleGridSet::pushGrid(SVPipleGrid* _grid){
    if(_grid){
        m_grids.push_back(_grid);
    }
}




