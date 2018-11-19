//
//  SVDragma.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVDragma.h"
#include "SVTimeLine.h"
#include "SVTimeLineDeform.h"
#include "SVTimeLineMtl.h"
#include "SVTimeLineEvent.h"

SVDragma::SVDragma(SVInst* _app)
:SVGBase(_app){
    m_lock = MakeSharedPtr<SVLock>();
    m_uid = mApp->m_IDPool.applyUID();
    m_timeLinePool.resize(E_TL_T_MAX);
    for(s32 i=0;i<E_TL_T_MAX;i++) {
        m_timeLinePool[i] = nullptr;
    }
    m_node = nullptr;
}

SVDragma::~SVDragma() {
    for(s32 i=0;i<E_TL_T_MAX;i++) {
        m_timeLinePool[i] = nullptr;
    }
    mApp->m_IDPool.returnUID(m_uid);
    m_node = nullptr;
    m_lock = nullptr;
}

//一个不创建
void SVDragma::create(f32 _time,u32 _rate) {
}

//
void SVDragma::create(TIMELINETYPE _type,f32 _time,u32 _rate) {
    if(_type == E_TL_T_MAX) {
        //全创建
        m_timeLinePool[0] = MakeSharedPtr<SVTimeLine>(mApp,_time,_rate);
        m_timeLinePool[1] = MakeSharedPtr<SVTimeLineDeform>(mApp,_time,_rate);
        m_timeLinePool[2] = MakeSharedPtr<SVTimeLineMtl>(mApp,_time,_rate);
        m_timeLinePool[3] = MakeSharedPtr<SVTimeLineEvent>(mApp,_time,_rate);
        m_timeLinePool[0]->initKey();
        m_timeLinePool[1]->initKey();
        m_timeLinePool[2]->initKey();
        m_timeLinePool[3]->initKey();
    } else {
        if(_type == E_TL_T_BASE) {
            m_timeLinePool[0] = MakeSharedPtr<SVTimeLine>(mApp,_time,_rate);
            m_timeLinePool[0]->initKey();
        }else if(_type == E_TL_T_DEFORM) {
            m_timeLinePool[1] = MakeSharedPtr<SVTimeLineDeform>(mApp,_time,_rate);
            m_timeLinePool[1]->initKey();
        }else if(_type == E_TL_T_MTL) {
            m_timeLinePool[2] = MakeSharedPtr<SVTimeLineMtl>(mApp,_time,_rate);
            m_timeLinePool[2]->initKey();
        }else if(_type == E_TL_T_EVENT) {
            m_timeLinePool[3] = MakeSharedPtr<SVTimeLineEvent>(mApp,_time,_rate);
            m_timeLinePool[3]->initKey();
        }
    }
}

//
void SVDragma::destroy() {
    m_node = nullptr;
    m_timeLinePool.destroy();
}

SVTimeLinePtr SVDragma::getTimeLine(s32 _type) {
    if(_type>=0 && _type<E_TL_T_MAX) {
        return m_timeLinePool[_type];
    }
    return nullptr;
}

SVNodePtr SVDragma::getBindNode() {
    return m_node;
}

void SVDragma::bind(SVNodePtr _node) {
    m_lock->lock();
    m_node = _node;
    m_lock->unlock();
}

void SVDragma::unbind() {
    m_lock->lock();
    m_node = nullptr;
    m_lock->unlock();
}

void SVDragma::enter(){
    m_lock->lock();
    for(s32 i=0;i<m_timeLinePool.size();i++) {
        m_timeLinePool[i]->enter(m_node);
    }
    m_lock->unlock();
}

void SVDragma::exit(){
    m_lock->lock();
    for(s32 i=0;i<m_timeLinePool.size();i++) {
        m_timeLinePool[i]->exit(m_node);
    }
    m_lock->unlock();
}

void SVDragma::update(f32 _dt) {
    m_lock->lock();
    for(s32 i=0;i<m_timeLinePool.size();i++) {
        m_timeLinePool[i]->update(m_node,_dt);
    }
    m_lock->unlock();
}

void SVDragma::setTotalTime(f32 _t) {
    m_lock->lock();
    for(s32 i=0;i<E_TL_T_MAX;i++) {
        if( m_timeLinePool[i]  ) {
            m_timeLinePool[i]->setTotalTime(_t);
        }
    }
    m_lock->unlock();
}

void SVDragma::setCurTime(f32 _t) {
    m_lock->lock();
    for(s32 i=0;i<E_TL_T_MAX;i++) {
        if( m_timeLinePool[i]  ) {
            m_timeLinePool[i]->setCurTime(_t);
        }
    }
    m_lock->unlock();
}

void SVDragma::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
//    _objValue.AddMember("totalTime", m_totalTime, _allocator);
//    _objValue.AddMember("loop",m_loop,_allocator);
    for(s32 i=0;i<m_timeLinePool.size();i++){
        RAPIDJSON_NAMESPACE::Value timelineObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
        m_timeLinePool[i]->toJSON(_allocator, timelineObj);
        _objValue.AddMember("timeline", timelineObj, _allocator);
    }
}

void SVDragma::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
}
