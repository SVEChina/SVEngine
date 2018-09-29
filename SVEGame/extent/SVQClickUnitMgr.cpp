//
// Created by lidm on 17/9/5.
//

#include "SVQClickUnitMgr.h"
#include "SVQClickUnit.h"
#include "SVQClickEvent.h"
#include "SVQClickMain.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../event/SVOpEvent.h"

SVQClickUnitMgr::SVQClickUnitMgr(SVInst*_app,SVQClickMain* _gamemain)
:SVEventProc(_app)
,m_pGameMain(_gamemain){
}

SVQClickUnitMgr::~SVQClickUnitMgr() {
    m_pGameMain = nullptr;
}

void SVQClickUnitMgr::init(){
    mApp->m_pGlobalMgr->m_pEventMgr->registProcer(this);
}

void SVQClickUnitMgr::destroy(){
    m_pGameMain = nullptr;
    mApp->m_pGlobalMgr->m_pEventMgr->unregistProcer(this);
    //使用
    QPOOL::Iterator it_use = m_usePool.begin();
    while( it_use!=m_usePool.end() ){
        SVQClickUnit* t_unit = (*it_use);
        t_unit->exit();
        t_unit->destroy();
        t_unit->release();
        it_use++;
    }
    m_usePool.clear();
    //自由
    QPOOL::Iterator it_free = m_freePool.begin();
    while( it_free!=m_usePool.end() ){
        SVQClickUnit* t_unit = (*it_free);
        t_unit->exit();
        t_unit->destroy();
        t_unit->release();
        it_free++;
    }
    m_freePool.clear();
}

void SVQClickUnitMgr::update(f32 _dt,f32 _gametime){
    //检测死亡
    QPOOL::Iterator it = m_usePool.begin();
    while( it!=m_usePool.end() ){
        SVQClickUnit* t_unit = (*it);
        //0生存 1非生命周期死亡 死亡动画状态 2 真正死亡状态
        if( t_unit->getState() == 0 ){
            t_unit->update(_dt, _gametime);
            it++;
        }else if( t_unit->getState() == 1){
            t_unit->update(_dt, _gametime);
            it++;
        }else if( t_unit->getState() == 2){
            _dead(t_unit);
            it = m_usePool.erase(it);
        }else{
            it++;
        }
    }
}

void SVQClickUnitMgr::setSpineName(cptr8 _name){
    m_spinename = _name;
}

//出生
void SVQClickUnitMgr::_birth(f32 _x,f32 _y){
    //0开始状态 1进行状态 2暂停状态 3结束状态
    if( m_pGameMain->getState() == 3 )
        return;
    
    SVQClickUnit* t_unit = nullptr;
    if(m_freePool.size() == 0){
        t_unit = new SVQClickUnit(mApp);
        t_unit->init( m_spinename.c_str() );
//        t_unit->setani("mouth_open");
    }else{
        QPOOL::Iterator it_free = m_freePool.begin();
        t_unit = (*it_free);
        m_freePool.erase(it_free);
    }
    //
    if(t_unit){
        t_unit->setInitPos(_x, _y, 0.0f);
        t_unit->setLife(2.0);
        t_unit->setInitSpeed(800.0f);   //速度
        t_unit->enter();
        m_usePool.push_back(t_unit);
        //发送一个加分的消息
        if( m_pGameMain->getState() == 1 ){
            SVEventAddScore* t_event = new SVEventAddScore();
            t_event->m_score = 10;
            t_event->m_playerCode = m_pGameMain->getPlayerCode();
            mApp->m_pGlobalMgr->m_pEventMgr->pushEvent(t_event);
        }
    }
}

//消亡
void SVQClickUnitMgr::_dead(SVQClickUnit* _unit){
    if(!_unit)
        return ;
    _unit->exit();
    m_freePool.push_back(_unit);
}

bool SVQClickUnitMgr::procEvent(SVEvent *_event){
    if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_BEGIN){
        SVTouchEvent* t_touch = (SVTouchEvent*)_event;
        f32 t_mod_x = t_touch->x - 0.5f*720.0f;
        f32 t_mod_y = t_touch->y - 0.5f*1280.0f;
        _birth(t_mod_x,t_mod_y);
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_END){
        SVTouchEvent* t_touch = (SVTouchEvent*)_event;
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_MOVE){
        SVTouchEvent* t_touch = (SVTouchEvent*)_event;
    }
    return true;
}
