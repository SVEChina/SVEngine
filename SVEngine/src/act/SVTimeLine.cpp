//
//  SVTimeLine.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVTimeLine.h"
#include "SVKeyFrame.h"
#include "../base/SVUtils.h"

u32 SVTimeLine::maxFrame(f32 _time,s32 _rate) {
    u32 t_value = u32(_time*_rate);
//    if(_time*_rate - t_value > 0) {
//        t_value += 1;
//    }
    return t_value;
}

SVTimeLine::SVTimeLine(SVInst* _app,f32 _time,s32 _rate)
:SVGBase(_app){
    m_type = E_TL_T_BASE;
    m_accTime = 0.0f;
    m_totalTime = _time;
    m_startKey = MakeSharedPtr<SVKeyFrame>(mApp,0);
    //
    u32 t_maxFrame = SVTimeLine::maxFrame(_time,_rate);
    m_endKey = MakeSharedPtr<SVKeyFrame>(mApp,t_maxFrame);
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

//key索引排序
void SVTimeLine::_refreshKey() {
    SVArray<SVKeyFramePtr>::Iterator it1 = m_keyPool.begin();
    SVArray<SVKeyFramePtr>::Iterator it2 = m_keyPool.end();
    bool _inc = true;
    if(_inc) {
        struct KeyTimeCompareInc {
            inline s32 operator()(SVKeyFramePtr f1,SVKeyFramePtr f2) const {
                return (f1->getIndex() < f2->getIndex() );
            }
        };
        KeyTimeCompareInc t_compare;
        quickSort(it1,it2,t_compare);
    } else {
        struct KeyTimeCompareDec {
            inline s32 operator()(SVKeyFramePtr f1,SVKeyFramePtr f2) const {
                return (f1->getIndex() > f2->getIndex() );
            }
        };
        KeyTimeCompareDec t_compare;
        quickSort(it1,it2,t_compare);
    }
}

void SVTimeLine::refreshKey() {
    m_keyLock->lock();
    _refreshKey();
    m_keyLock->unlock();
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

SVKeyFramePtr SVTimeLine::getBeginKey() {
    return m_startKey;
}

SVKeyFramePtr SVTimeLine::getEndKey() {
    return m_endKey;
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

