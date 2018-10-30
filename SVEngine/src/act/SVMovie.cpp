//
//  SVMovie.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVMovie.h"
#include "SVDragma.h"
#include "SVTimeLine.h"
#include "SVTimeLineDeform.h"
#include "SVTimeLineMtl.h"

SVMovie::SVMovie(SVInst* _app)
:SVGBase(_app)
,m_loop(false)
,m_accTime(0)
,m_totalTime(30.0f){
    m_name = "sv_movie";
    m_state = E_MV_ST_NULL;
    m_dragmaLock = MakeSharedPtr<SVLock>();
}

SVMovie::~SVMovie() {
    m_dragmaLock = nullptr;
}

void SVMovie::play() {
    m_state = E_MV_ST_PLAY;
}

void SVMovie::pause() {
    m_state = E_MV_ST_PAUSE;
}

void SVMovie::stop() {
    if(m_state == E_MV_ST_PLAY || m_state == E_MV_ST_PAUSE){
        m_state = E_MV_ST_READY;
    }
}

bool SVMovie::isLoop() {
    return m_loop;
}

void SVMovie::setLoop(bool _loop) {
    m_loop = _loop;
}

void SVMovie::setTotalTime(f32 _time) {
    m_dragmaLock->lock();
    m_totalTime = _time;
    for(s32 i=0;i<m_dragmaPool.size();i++){
        m_dragmaPool[i]->setTotalTime(m_totalTime);
    }
    m_dragmaLock->unlock();
}

f32 SVMovie::getTotalTime(){
    return m_totalTime;
}

void SVMovie::setCurTime(f32 _time){
    m_dragmaLock->lock();
    m_accTime = _time;
    for(s32 i=0;i<m_dragmaPool.size();i++){
        m_dragmaPool[i]->setCurTime(m_totalTime);
    }
    m_dragmaLock->unlock();
}

f32 SVMovie::getCurTime(){
    return m_accTime;
}

//
void SVMovie::addDragma(SVDragmaPtr _dragma) {
    m_dragmaLock->lock();
    if(_dragma) {
        _dragma->setTotalTime(m_totalTime);
        _dragma->setCurTime(m_accTime);
        m_dragmaPool.append(_dragma);
    }
    m_dragmaLock->unlock();
}

void SVMovie::delDragma(SVDragmaPtr _dragma) {
    m_dragmaLock->lock();
    if(_dragma) {
        for(s32 i=0;i<m_dragmaPool.size();i++){
            if( m_dragmaPool[i] == _dragma ) {
                m_dragmaPool.removeForce(i);
                return ;
            }
        }
    }
    m_dragmaLock->unlock();
}

void SVMovie::delDragmaByUID(u32 _uid) {
    m_dragmaLock->lock();
    for(s32 i=0;i<m_dragmaPool.size();i++){
        if( m_dragmaPool[i]->getUID() == _uid ) {
            m_dragmaPool.removeForce(i);
            return ;
        }
    }
    m_dragmaLock->unlock();
}

void SVMovie::delDragmaByIndex(u32 _index) {
    m_dragmaLock->lock();
    if(_index<0 || _index>=m_dragmaPool.size())
        return ;
    m_dragmaPool.removeForce(_index);
    m_dragmaLock->unlock();
}

SVDragmaPtr SVMovie::getDragmaByUID(u32 _uid) {
    for(s32 i=0;i<m_dragmaPool.size();i++){
        if( m_dragmaPool[i]->getUID() == _uid ) {
            return m_dragmaPool[i];
        }
    }
    return nullptr;
}

SVDragmaPtr SVMovie::getDragmaByIndex(u32 _index) {
    if(_index<0 || _index>=m_dragmaPool.size())
        return nullptr;
    return m_dragmaPool[_index];
}

s32 SVMovie::getDragmaNum() {
    return m_dragmaPool.size();
}

void SVMovie::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue) {
    //构建一个Movie
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);
    locationObj.AddMember("name",  RAPIDJSON_NAMESPACE::StringRef(m_name.c_str()), _allocator);
    locationObj.AddMember("totalTime", m_totalTime, _allocator);
    locationObj.AddMember("loop",m_loop,_allocator);
//    for(s32 i=0;i<m_dragmaPool.size();i++){
//        RAPIDJSON_NAMESPACE::Value dragmaObj(RAPIDJSON_NAMESPACE::kObjectType);
//        m_dragmaPool[i]->toJSON(_allocator, dragmaObj);
//        locationObj.AddMember("dragma",dragmaObj,_allocator);
//    }
    _objValue.AddMember("movie", locationObj, _allocator);
}

void SVMovie::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
    m_name = item["name"].GetString();
    m_totalTime = item["totalTime"].GetFloat();
    m_loop = item["loop"].GetBool();
    //
}
