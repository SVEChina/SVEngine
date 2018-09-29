//
// Created by lidm on 17/9/5.
//

#include "SVPipleGridMgr.h"
#include "SVPipleMain.h"
#include "SVPipleGrid.h"
#include "SVQClickEvent.h"
#include "SVQClickMain.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../event/SVOpEvent.h"

using namespace sv;

SVPipleGridMgr::SVPipleGridMgr(SVInst*_app,SVPipleMain* _piplemain)
:SVEventProc(_app)
,m_pPipleMain(_piplemain){
    m_netWidth = 0;
    m_netHeight = 0;
}

SVPipleGridMgr::~SVPipleGridMgr() {
    m_pPipleMain = nullptr;
}

void SVPipleGridMgr::init(){
    enterex();
}

void SVPipleGridMgr::destroy(){
    enterex();
    for(s32 i=0;i<m_gridPool.size();i++){
        SAFE_DELETE(m_gridPool[i]);
    }
    m_gridPool.clear();
    m_pPipleMain = nullptr;
}

void SVPipleGridMgr::update(f32 _dt,f32 _gametime){
//    //检测死亡
//    QPOOL::Iterator it = m_usePool.begin();
//    while( it!=m_usePool.end() ){
//        SVQClickUnit* t_unit = (*it);
//        //0生存 1非生命周期死亡 死亡动画状态 2 真正死亡状态
//        if( t_unit->getState() == 0 ){
//            t_unit->update(_dt, _gametime);
//            it++;
//        }else if( t_unit->getState() == 1){
//            t_unit->update(_dt, _gametime);
//            it++;
//        }else if( t_unit->getState() == 2){
//            _dead(t_unit);
//            it = m_usePool.erase(it);
//        }else{
//            it++;
//        }
//    }
}
//生成所有的格子
void SVPipleGridMgr::genNet(s32 _w,s32 _h){
    m_netWidth = _w;
    m_netHeight = _h;
    //
    for(s32 i=0;i<m_netWidth*m_netHeight;i++){
        SVPipleGrid* t_grid = new SVPipleGrid(mApp,this);
        s32 t_x = i%m_netWidth;
        s32 t_y = i/m_netWidth;
        E_PIPLEGRID_TYPE t_type = E_PIPLEGRID_TYPE( rand()%2 );
        E_PIPLEGRID_STATE t_state = E_PIPLEGRID_STATE( rand()%4 );
        t_grid->gen(t_type,t_state,t_x, t_y);
        m_gridPool.push_back(t_grid);
    }
}
//检测网格的合理性
void SVPipleGridMgr::checkNet(){
    
}
//检测闭环
bool SVPipleGridMgr::checkClosed(SVPipleGrid* _srcGrid,SVPipleGrid* _fromGrid,SVPipleGridSet* _gridSet){
    if(_fromGrid == nullptr){
        _gridSet->pushGrid(_srcGrid);   //第一个格子 需要推送进来
    }
    //获取目标格子(与进入的格子不一致的格子)
    SVPipleGrid* t_grid = getGrid(_srcGrid->m_side0);
    if( t_grid == _fromGrid)
        t_grid = getGrid(_srcGrid->m_side1);
    //检测目标格子是否关联
    if(t_grid && t_grid->isLink(_srcGrid) ){
        //判断是否闭环
        if( _gridSet->isClose(t_grid) ){
            return true;
        }else{
            //继续检测
            _gridSet->pushGrid(t_grid);
            return checkClosed(t_grid,_srcGrid,_gridSet);
        }
    }
    return false;
}

SVPipleGrid* SVPipleGridMgr::getGrid(vector2di& _pos){
    s32 t_index = _pos.X + _pos.Y*m_netWidth;
    return m_gridPool[t_index];
}
//数字回滚
s32 SVPipleGridMgr::roollingX(s32 _value){
    if(_value <0 ){
        _value += m_netWidth;
    }else if(_value>=m_netWidth ){
        _value -= m_netWidth;
    }
    return _value;
}

//数字回滚
s32 SVPipleGridMgr::roollingY(s32 _value){
    if(_value <0 ){
        _value += m_netHeight;
    }else if(_value>=m_netHeight){
        _value -= m_netHeight;
    }
    return _value;
}
//设置spine
void SVPipleGridMgr::setSpineRes(cptr8 _wave,cptr8 _line){
    m_spineWave = _wave;
    m_spineLine = _line;
}
//搜查闭环
void SVPipleGridMgr::_search(f32 _x,f32 _y){
    //在大网格上定位格子
    vector2di t_pos = vector2di(_x,_y);
    SVPipleGrid* t_target_grid = getGrid( t_pos );
    if( t_target_grid ){
        //格子变换
        t_target_grid->trans();
        //
        SVPipleGridSet t_gridset;
        if( checkClosed(t_target_grid, nullptr,&t_gridset) ){
            //闭环计算得分
            _dead(&t_gridset);
        }else{
            //非闭环则标记 变色这种
        }
    }
}
//闭环计算分数 消亡 + 更新
void SVPipleGridMgr::_dead(SVPipleGridSet* _gridSet){
    //计算得分什么的
    
}

bool SVPipleGridMgr::procEvent(SVEvent *_event){
    if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_BEGIN){
//        SVTouchEvent* t_touch = (SVTouchEvent*)_event;
//        f32 t_mod_x = t_touch->x - 0.5f*720.0f;
//        f32 t_mod_y = t_touch->y - 0.5f*1280.0f;
//        _birth(t_mod_x,t_mod_y);
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_END){
        SVTouchEvent* t_touch = (SVTouchEvent*)_event;
        f32 t_mod_x = t_touch->x - 0.5f*720.0f;
        f32 t_mod_y = t_touch->y - 0.5f*1280.0f;
        //屏幕掉和地图 做一个转换
        _search(t_mod_x,t_mod_y);
        
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_MOVE){
        SVTouchEvent* t_touch = (SVTouchEvent*)_event;
    }
    return true;
}
