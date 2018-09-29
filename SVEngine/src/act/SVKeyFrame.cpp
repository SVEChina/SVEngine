//
//  SVTimeLine.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVKeyFrame.h"

SVKeyFrame::SVKeyFrame(SVInst* _app)
:SVGBase(_app){
    m_uid = mApp->m_IDPool.applyUID();
    m_time = 0.0f;
}

SVKeyFrame::~SVKeyFrame(){
    mApp->m_IDPool.returnUID(m_uid);
}

void SVKeyFrame::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue) {
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    locationObj.AddMember("time", m_time, _allocator);
    _objValue.AddMember("keyNormal", locationObj, _allocator);
}

void SVKeyFrame::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
}

//
SVKeyDeform::SVKeyDeform(SVInst* _app)
:SVKeyFrame(_app){
}

SVKeyDeform::~SVKeyDeform(){
}

//
SVKeyMtl::SVKeyMtl(SVInst* _app)
:SVKeyFrame(_app){
    m_mtl = nullptr;
}

SVKeyMtl::~SVKeyMtl() {
    m_mtl = nullptr;
}

//事件Key
SVKeyEvent::SVKeyEvent(SVInst* _app)
:SVKeyFrame(_app){
}

SVKeyEvent::~SVKeyEvent() {
}



