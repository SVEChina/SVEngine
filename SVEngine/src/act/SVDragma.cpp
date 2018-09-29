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

SVDragma::SVDragma(SVInst* _app)
:SVActBase(_app){
    m_uid = mApp->m_IDPool.applyUID();
}

SVDragma::~SVDragma() {
    mApp->m_IDPool.returnUID(m_uid);
}

void SVDragma::enter(SVNodePtr _nodePtr){
//    m_accTime = 0.0f;
//    m_linkNode = _nodePtr;
//    for(s32 i=0;i<m_timeLinePool.size();i++) {
//        m_timeLinePool[i]->enter(_nodePtr);
//    }
}

void SVDragma::exit(SVNodePtr _nodePtr){
//    for(s32 i=0;i<m_timeLinePool.size();i++) {
//        m_timeLinePool[i]->exit(_nodePtr);
//    }
//    //去掉关联node
//    m_linkNode = nullptr;
}

void SVDragma::run(SVNodePtr _nodePtr, f32 dt) {
//    if(m_state == E_MV_ST_PLAY) {
//        for(s32 i=0;i<m_timeLinePool.size();i++) {
//            m_timeLinePool[i]->update(_nodePtr,dt);
//        }
//        m_accTime += dt;
//    }else if(m_state == E_MV_ST_PAUSE) {
//    }
}

bool SVDragma::isEnd() {
//    if(m_loop){
//        if(m_accTime>m_totalTime) {
//            enter(m_linkNode);
//        }
//        return false;
//    }else{
//        if(m_accTime>m_totalTime) {
//            return true;
//        }
//    }
    return false;
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
