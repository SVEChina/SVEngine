//
//  SVTimeLine.cpp
//  SVEngine
//
//  Created by 付一洲 on 2018/9/13.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVKeyFrame.h"

SVKeyFrame::SVKeyFrame(SVInst* _app,f32 _time)
:SVGBase(_app){
    m_uid = mApp->m_IDPool.applyUID();
    m_time = _time;
}

SVKeyFrame::~SVKeyFrame(){
    mApp->m_IDPool.returnUID(m_uid);
}

void SVKeyFrame::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_alloc,
                        RAPIDJSON_NAMESPACE::Value &_obj) {
    RAPIDJSON_NAMESPACE::Value t_obj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    SVKeyFrame::_toJSON(_alloc,t_obj);
    _obj.AddMember("keynull", t_obj, _alloc);
}

void SVKeyFrame::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
}

void SVKeyFrame::_toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_alloc,
                         RAPIDJSON_NAMESPACE::Value &_obj) {
    _obj.AddMember("uid", m_uid, _alloc);
    _obj.AddMember("time", m_time, _alloc);
}

void SVKeyFrame::_fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
//    _obj.AddMember("uid", m_uid, _alloc);
//    _obj.AddMember("time", m_time, _alloc);
}

//形变帧
SVKeyDeform::SVKeyDeform(SVInst* _app,f32 _time)
:SVKeyFrame(_app,_time){
}

SVKeyDeform::~SVKeyDeform(){
}

void SVKeyDeform::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_alloc,
                         RAPIDJSON_NAMESPACE::Value &_obj) {
    RAPIDJSON_NAMESPACE::Value t_obj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    SVKeyFrame::_toJSON(_alloc,t_obj);
    _obj.AddMember("keydeform", t_obj, _alloc);
}

void SVKeyDeform::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    
}

//材质帧
SVKeyMtl::SVKeyMtl(SVInst* _app,f32 _time)
:SVKeyFrame(_app,_time){
    m_mtl = nullptr;
}

SVKeyMtl::~SVKeyMtl() {
    m_mtl = nullptr;
}

void SVKeyMtl::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_alloc,
                      RAPIDJSON_NAMESPACE::Value &_obj) {
    RAPIDJSON_NAMESPACE::Value t_obj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    SVKeyFrame::_toJSON(_alloc,t_obj);
    _obj.AddMember("keymtl", t_obj, _alloc);
}

void SVKeyMtl::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    
}

//事件Key
SVKeyEvent::SVKeyEvent(SVInst* _app,f32 _time)
:SVKeyFrame(_app,_time){
}

SVKeyEvent::~SVKeyEvent() {
}

void SVKeyEvent::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_alloc,
                        RAPIDJSON_NAMESPACE::Value &_obj) {
    RAPIDJSON_NAMESPACE::Value t_obj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    SVKeyFrame::_toJSON(_alloc,t_obj);
    _obj.AddMember("keyevent", t_obj, _alloc);
}

void SVKeyEvent::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    
}


