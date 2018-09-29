//
// MaTripleDownMgr.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVTripleDownMgr.h"
#include "SVTripleDownUnit.h"
#include "SVTripleDownDef.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include <sys/time.h>

SVTripleDownMgr::SVTripleDownMgr(SVInst *_app)
:SVEventProc(_app),m_maxNum(100),m_donutWidth(0) , m_donutHeight(0) {
}

SVTripleDownMgr::~SVTripleDownMgr() {
}

void SVTripleDownMgr::init(){
    enterex();
    srand((unsigned int)time(0));
    for(s32 i=0;i<m_maxNum;i++){
        SVTripleDownUnit* t_unit = new SVTripleDownUnit(mApp);
        int iSpineName = rand()%TUT_TYPE_MAX;
        t_unit->init(m_scenename.c_str() , m_arrAnimate[iSpineName].c_str());
        t_unit->setHeight(m_donutHeight);
        t_unit->setWidth(m_donutWidth);
        t_unit->setTripleType((TRIPLEUNITTYPE)iSpineName);
        m_obFreePool.push_back(t_unit);
    }
}

void SVTripleDownMgr::destroy(){
    exitex();
    //使用池
    TRIPLEPOOL::Iterator it_use = m_obUsePool.begin();
    while( it_use!=m_obUsePool.end() ){
        SVTripleDownUnit* t_unit = (*it_use);
        t_unit->destroy();
        it_use++;
    }
    m_obUsePool.clear();
    //缓存池
    TRIPLEPOOL::Iterator it_free = m_obFreePool.begin();
    while( it_free!=m_obFreePool.end() ){
        SVTripleDownUnit* t_unit = (*it_free);
        t_unit->destroy();
        it_free++;
    }
    m_obFreePool.clear();
}

void SVTripleDownMgr::stop(){
    TRIPLEPOOL::Iterator it = m_obUsePool.begin();
    while( it!=m_obUsePool.end() ){
        SVTripleDownUnit* t_unit = (*it);
        t_unit->stop();
        it++;
    }
}

void SVTripleDownMgr::update(float _dt,float _gametime){
    TRIPLEPOOL::Iterator it = m_obUsePool.begin();
    while( it!=m_obUsePool.end() ){
        SVTripleDownUnit* t_unit = (*it);
        if(t_unit->getTripleState() < TUS_DEADED ){
            //生成或者暂停状态
            t_unit->update(_dt,_gametime);
            it++;
        }else if(t_unit->getTripleState() == TUS_DEADED){
            //死亡状态
            t_unit->exit();
            m_obFreePool.push_back(t_unit);
            it = m_obUsePool.erase(it);
        }else{
            it++;
        }
    }
}

bool SVTripleDownMgr::procEvent(SVEvent *_event){
    if(!_event)
        return true;
    
    return true;
}

SVTripleDownUnit* SVTripleDownMgr::getFreeUnit(){
    TRIPLEPOOL::Iterator it_free = m_obFreePool.begin();
    if( it_free!=m_obFreePool.end() ){
        SVTripleDownUnit* t_unit = (*it_free);
        m_obFreePool.erase(it_free);
        m_obUsePool.push_back(t_unit);
        return t_unit;
    }
    return NULL;
}

