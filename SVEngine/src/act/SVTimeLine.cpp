//
//  SVTimeLine.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVTimeLine.h"
#include "SVKeyFrame.h"

SVTimeLine::SVTimeLine(SVInst* _app,f32 _time)
:SVGBase(_app){
    m_type = E_TL_T_BASE;
    m_accTime = 0.0f;
    m_totalTime = _time;
    m_startKey = MakeSharedPtr<SVKeyFrame>(mApp,0.0f);
    m_endKey = MakeSharedPtr<SVKeyFrame>(mApp,_time);
    m_keyLock = MakeSharedPtr<SVLock>();
}

SVTimeLine::~SVTimeLine() {
    m_keyLock = nullptr;
}

TIMELINETYPE SVTimeLine::getType() {
    return m_type;
}

void SVTimeLine::enter(SVNodePtr _nodePtr) {
    m_accTime = 0.0f;
}

void SVTimeLine::exit(SVNodePtr _nodePtr) {
    m_accTime = 0.0f;
}

void SVTimeLine::update(SVNodePtr _nodePtr,f32 _dt) {
    m_accTime += _dt;
}

void SVTimeLine::setTotalTime(f32 _t) {
    m_totalTime = _t;
}

void SVTimeLine::setCurTime(f32 _t) {
    m_accTime = _t;
}

//冒牌排序
void SVTimeLine::_refreshKey() {
//    s32 t_num = m_keyPool.size();
//    s32 t_times = t_num - 1;
//    for(s32 i = 0;i<t_times;i++) {
//        for(s32 j=0;j<t_times;j++) {
//            SVKeyFramePtr t1 = m_keyPool[j];
//            SVKeyFramePtr t2 = m_keyPool[j+1];
//            if(t1->m_time>t2->m_time) {
//                SVKeyFramePtr tmp = m_keyPool[j];
//                m_keyPool[j] = m_keyPool[j+1];
//                m_keyPool[j+1] = tmp;
//            }
//        }
//        t_times--;
//    }
}

void SVTimeLine::addKey(SVKeyFramePtr _key) {
    m_keyLock->lock();
    if(_key){
        m_keyPool.append(_key);
        _refreshKey();
    }
    m_keyLock->unlock();
}

void SVTimeLine::removeKey(s32 _uid) {
    m_keyLock->lock();
    for(s32 i=0;i<m_keyPool.size();i++) {
        if(m_keyPool[i]->getUID() == _uid) {
            m_keyPool.remove(_uid);
            m_keyPool.reserve(m_keyPool.size());
            break;
        }
    }
    m_keyLock->unlock();
}

SVKeyFramePtr SVTimeLine::getKeyFrame(s32 _uid) {
    for(s32 i=0;i<m_keyPool.size();i++) {
        if(m_keyPool[i]->getUID() == _uid) {
            return m_keyPool[i];
        }
    }
    return nullptr;
}

//
void SVTimeLine::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                        RAPIDJSON_NAMESPACE::Value &_objValue){
//    s32 t_type = m_type;
//    _objValue.AddMember("type",t_type,_allocator);
//    for(s32 i=0;i<m_keyPool.size();i++) {
//        RAPIDJSON_NAMESPACE::Value keyObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
//        m_keyPool[i]->toJSON(_allocator, keyObj);
//        _objValue.AddMember("key", keyObj, _allocator);
//    }
}

void SVTimeLine::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
}

